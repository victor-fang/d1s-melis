/*
* Copyright (c) 2019-2025 Allwinner Technology Co., Ltd. ALL rights reserved.
*
* Allwinner is a trademark of Allwinner Technology Co.,Ltd., registered in
* the the People's Republic of China and other countries.
* All Allwinner Technology Co.,Ltd. trademarks are used with permission.
*
* DISCLAIMER
* THIRD PARTY LICENCES MAY BE REQUIRED TO IMPLEMENT THE SOLUTION/PRODUCT.
* IF YOU NEED TO INTEGRATE THIRD PARTY’S TECHNOLOGY (SONY, DTS, DOLBY, AVS OR MPEGLA, ETC.)
* IN ALLWINNERS’SDK OR PRODUCTS, YOU SHALL BE SOLELY RESPONSIBLE TO OBTAIN
* ALL APPROPRIATELY REQUIRED THIRD PARTY LICENCES.
* ALLWINNER SHALL HAVE NO WARRANTY, INDEMNITY OR OTHER OBLIGATIONS WITH RESPECT TO MATTERS
* COVERED UNDER ANY REQUIRED THIRD PARTY LICENSE.
* YOU ARE SOLELY RESPONSIBLE FOR YOUR USAGE OF THIRD PARTY’S TECHNOLOGY.
*
*
* THIS SOFTWARE IS PROVIDED BY ALLWINNER"AS IS" AND TO THE MAXIMUM EXTENT
* PERMITTED BY LAW, ALLWINNER EXPRESSLY DISCLAIMS ALL WARRANTIES OF ANY KIND,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING WITHOUT LIMITATION REGARDING
* THE TITLE, NON-INFRINGEMENT, ACCURACY, CONDITION, COMPLETENESS, PERFORMANCE
* OR MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL ALLWINNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "fs.h"
#include "fatfs.h"
#include "endians.h"
#include "page_pool.h"
#include "errno.h"
#include "fsys_debug.h"

struct fatent_operations
{
    void (*ent_blocknr)(struct super_block *, int, int *, int *);
    void (*ent_set_ptr)(struct fat_entry *, int);
    int (*ent_bread)(struct super_block *, struct fat_entry *,
                     int, int);
    int (*ent_get)(struct fat_entry *);
    void (*ent_put)(struct fat_entry *, int);
    int (*ent_next)(struct fat_entry *);
};

static void fat12_ent_blocknr(struct super_block *sb, int entry,
                              int *offset, int *blocknr)
{
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    int bytes = entry + (entry >> 1);
    WARN_ON(entry < FAT_START_ENT || sbi->max_cluster <= entry);
    *offset = bytes & (sb->s_blocksize - 1);
    *blocknr = sbi->fat_start + (bytes >> sb->s_blocksize_bits);
}

static void fat_ent_blocknr(struct super_block *sb, int entry,
                            int *offset, int *blocknr)
{
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    int bytes = (entry << sbi->fatent_shift);
    WARN_ON(entry < FAT_START_ENT || sbi->max_cluster <= entry);
    *offset = bytes & (sb->s_blocksize - 1);
    *blocknr = sbi->fat_start + (bytes >> sb->s_blocksize_bits);
}

static void fat12_ent_set_ptr(struct fat_entry *fatent, int offset)
{
    struct buffer_head **bhs = fatent->bhs;
    if (fatent->nr_bhs == 1)
    {
        WARN_ON(offset >= (bhs[0]->b_size - 1));
        fatent->u.ent12_p[0] = bhs[0]->b_data + offset;
        fatent->u.ent12_p[1] = bhs[0]->b_data + (offset + 1);
    }
    else
    {
        WARN_ON(offset != (bhs[0]->b_size - 1));
        fatent->u.ent12_p[0] = bhs[0]->b_data + offset;
        fatent->u.ent12_p[1] = bhs[1]->b_data;
    }
}

static void fat16_ent_set_ptr(struct fat_entry *fatent, int offset)
{
    WARN_ON(offset & (2 - 1));
    fatent->u.ent16_p = (__s16 *)(fatent->bhs[0]->b_data + offset);
}

static void fat32_ent_set_ptr(struct fat_entry *fatent, int offset)
{
    WARN_ON(offset & (4 - 1));
    fatent->u.ent32_p = (__s32 *)(fatent->bhs[0]->b_data + offset);
}

static int fat12_ent_bread(struct super_block *sb, struct fat_entry *fatent,
                           int offset, int blocknr)
{
    struct buffer_head **bhs = fatent->bhs;

    WARN_ON(blocknr < MSDOS_SB(sb)->fat_start);
    /* pre-read fat table for enhance sequence access performance */
    if (!(blocknr & 0x0f))
    {
        int i;
        struct buffer_head *bh;

        for (i = 0; i < 16; i++)
        {
            bh = sb_bread(sb, blocknr + i);
            brelse(bh);
        }
    }

    bhs[0] = sb_bread(sb, blocknr);
    if (!bhs[0])
    {
        goto err;
    }

    if ((offset + 1) < sb->s_blocksize)
    {
        fatent->nr_bhs = 1;
    }
    else
    {
        /* This entry is block boundary, it needs the next block */
        blocknr++;
        bhs[1] = sb_bread(sb, blocknr);
        if (!bhs[1])
        {
            goto err_brelse;
        }
        fatent->nr_bhs = 2;
    }
    fat12_ent_set_ptr(fatent, offset);
    return 0;

err_brelse:
    brelse(bhs[0]);
err:
    fs_log_error("FAT: FAT read failed (blocknr %u)\n",
                 (unsigned long)blocknr);
    return -EIO;
}

static int fat_ent_bread(struct super_block *sb, struct fat_entry *fatent,
                         int offset, int blocknr)
{
    struct fatent_operations *ops = MSDOS_SB(sb)->fatent_ops;

    WARN_ON(blocknr < MSDOS_SB(sb)->fat_start);
    /* pre-read fat table for enhance sequence access performance */
    if (!(blocknr & 0x0f))
    {
        int i;
        struct buffer_head *bh;

        for (i = 0; i < 16; i++)
        {
            bh = sb_bread(sb, blocknr + i);
            brelse(bh);
        }
    }

    fatent->bhs[0] = sb_bread(sb, blocknr);
    if (!fatent->bhs[0])
    {
        fs_log_error("FAT: FAT read failed (blocknr %u)\n",
                     (unsigned long)blocknr);
        return -EIO;
    }
    fatent->nr_bhs = 1;
    ops->ent_set_ptr(fatent, offset);
    return 0;
}

static int fat12_ent_get(struct fat_entry *fatent)
{
    __u8 **ent12_p = fatent->u.ent12_p;
    int next;

    if (fatent->entry & 1)
    {
        next = (*ent12_p[0] >> 4) | (*ent12_p[1] << 4);
    }
    else
    {
        next = (*ent12_p[1] << 8) | *ent12_p[0];
    }
    next &= 0x0fff;
    if (next >= BAD_FAT12)
    {
        next = FAT_ENT_EOF;
    }
    return next;
}

static int fat16_ent_get(struct fat_entry *fatent)
{
    int next = (__u16) * fatent->u.ent16_p;
    WARN_ON((unsigned long)fatent->u.ent16_p & (2 - 1));
    if (next >= BAD_FAT16)
    {
        next = FAT_ENT_EOF;
    }
    return next;
}

static int fat32_ent_get(struct fat_entry *fatent)
{
    int next = (__u32)(*fatent->u.ent32_p) & 0x0fffffff;
    WARN_ON((unsigned long)fatent->u.ent32_p & (4 - 1));
    if (next >= BAD_FAT32)
    {
        next = FAT_ENT_EOF;
    }
    return next;
}

static void fat12_ent_put(struct fat_entry *fatent, int new)
{
    __u8 **ent12_p = fatent->u.ent12_p;

    if (new == FAT_ENT_EOF)
    {
        new = EOF_FAT12;
    }

    if (fatent->entry & 1)
    {
        *ent12_p[0] = (new << 4) | (*ent12_p[0] & 0x0f);
        *ent12_p[1] = new >> 4;
    }
    else
    {
        *ent12_p[0] = new & 0xff;
        *ent12_p[1] = (*ent12_p[1] & 0xf0) | (new >> 8);
    }

    mark_buffer_dirty(fatent->bhs[0]);
    if (fatent->nr_bhs == 2)
    {
        mark_buffer_dirty(fatent->bhs[1]);
    }
}

static void fat16_ent_put(struct fat_entry *fatent, int new)
{
    if (new == FAT_ENT_EOF)
    {
        new = EOF_FAT16;
    }

    *fatent->u.ent16_p = (__u16)new;
    mark_buffer_dirty(fatent->bhs[0]);
}

static void fat32_ent_put(struct fat_entry *fatent, int new)
{
    if (new == FAT_ENT_EOF)
    {
        new = EOF_FAT32;
    }

    WARN_ON(new & 0xf0000000);
    new |= (__u32)(*fatent->u.ent32_p) & ~0x0fffffff;
    *fatent->u.ent32_p = new;
    mark_buffer_dirty(fatent->bhs[0]);
}

static int fat12_ent_next(struct fat_entry *fatent)
{
    __u8 **ent12_p = fatent->u.ent12_p;
    struct buffer_head **bhs = fatent->bhs;
    __u8 *nextp = ent12_p[1] + 1 + (fatent->entry & 1);

    fatent->entry++;
    if (fatent->nr_bhs == 1)
    {
        WARN_ON(ent12_p[0] > (__u8 *)(bhs[0]->b_data + (bhs[0]->b_size - 2)));
        WARN_ON(ent12_p[1] > (__u8 *)(bhs[0]->b_data + (bhs[0]->b_size - 1)));
        if (nextp < (__u8 *)(bhs[0]->b_data + (bhs[0]->b_size - 1)))
        {
            ent12_p[0] = nextp - 1;
            ent12_p[1] = nextp;
            return 1;
        }
    }
    else
    {
        WARN_ON(ent12_p[0] != (__u8 *)(bhs[0]->b_data + (bhs[0]->b_size - 1)));
        WARN_ON(ent12_p[1] != (__u8 *)bhs[1]->b_data);
        ent12_p[0] = nextp - 1;
        ent12_p[1] = nextp;
        brelse(bhs[0]);
        bhs[0] = bhs[1];
        fatent->nr_bhs = 1;
        return 1;
    }
    ent12_p[0] = NULL;
    ent12_p[1] = NULL;
    return 0;
}

static int fat16_ent_next(struct fat_entry *fatent)
{
    const struct buffer_head *bh = fatent->bhs[0];
    fatent->entry++;
    if (fatent->u.ent16_p < (__s16 *)(bh->b_data + (bh->b_size - 2)))
    {
        fatent->u.ent16_p++;
        return 1;
    }
    fatent->u.ent16_p = NULL;
    return 0;
}

static int fat32_ent_next(struct fat_entry *fatent)
{
    const struct buffer_head *bh = fatent->bhs[0];
    fatent->entry++;
    if (fatent->u.ent32_p < (__s32 *)(bh->b_data + (bh->b_size - 4)))
    {
        fatent->u.ent32_p++;
        return 1;
    }
    fatent->u.ent32_p = NULL;
    return 0;
}

static struct fatent_operations fat12_ops =
{
    .ent_blocknr    = fat12_ent_blocknr,
    .ent_set_ptr    = fat12_ent_set_ptr,
    .ent_bread  = fat12_ent_bread,
    .ent_get    = fat12_ent_get,
    .ent_put    = fat12_ent_put,
    .ent_next   = fat12_ent_next,
};

static struct fatent_operations fat16_ops =
{
    .ent_blocknr    = fat_ent_blocknr,
    .ent_set_ptr    = fat16_ent_set_ptr,
    .ent_bread  = fat_ent_bread,
    .ent_get    = fat16_ent_get,
    .ent_put    = fat16_ent_put,
    .ent_next   = fat16_ent_next,
};

static struct fatent_operations fat32_ops =
{
    .ent_blocknr    = fat_ent_blocknr,
    .ent_set_ptr    = fat32_ent_set_ptr,
    .ent_bread  = fat_ent_bread,
    .ent_get    = fat32_ent_get,
    .ent_put    = fat32_ent_put,
    .ent_next   = fat32_ent_next,
};

void fat_ent_access_init(struct super_block *sb)
{
    struct msdos_sb_info *sbi = MSDOS_SB(sb);

    switch (sbi->fat_bits)
    {
        case 32:
            sbi->fatent_shift = 2;
            sbi->fatent_ops = &fat32_ops;
            break;
        case 16:
            sbi->fatent_shift = 1;
            sbi->fatent_ops = &fat16_ops;
            break;
        case 12:
            sbi->fatent_shift = -1;
            sbi->fatent_ops = &fat12_ops;
            break;
    }
}

static   int fat_ent_update_ptr(struct super_block *sb,
                                struct fat_entry *fatent,
                                int offset, int blocknr)
{
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    struct fatent_operations *ops = sbi->fatent_ops;
    struct buffer_head **bhs = fatent->bhs;

    /* Is this fatent's blocks including this entry? */
    if (!fatent->nr_bhs || bhs[0]->b_blocknr != blocknr)
    {
        return 0;
    }
    /* Does this entry need the next block? */
    if (sbi->fat_bits == 12 && (offset + 1) >= sb->s_blocksize)
    {
        if (fatent->nr_bhs != 2 || bhs[1]->b_blocknr != (blocknr + 1))
        {
            return 0;
        }
    }
    ops->ent_set_ptr(fatent, offset);
    return 1;
}

int fat_ent_read(struct inode *inode, struct fat_entry *fatent, int entry)
{
    struct super_block *sb = inode->i_sb;
    struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);
    struct fatent_operations *ops = sbi->fatent_ops;
    int err, offset;
    int blocknr;

    if (entry < FAT_START_ENT || sbi->max_cluster <= entry)
    {
        fatent_brelse(fatent);
        fat_fs_panic(sb, "%s: invalid access to FAT (entry %x)", __FUNCTION__, entry);
        return -EIO;
    }

    fatent_set_entry(fatent, entry);
    ops->ent_blocknr(sb, entry, &offset, &blocknr);

    if (!fat_ent_update_ptr(sb, fatent, offset, blocknr))
    {
        fatent_brelse(fatent);
        err = ops->ent_bread(sb, fatent, offset, blocknr);
        if (err)
        {
            return err;
        }
    }
    return ops->ent_get(fatent);
}

int fat_ent_read_inbh(struct inode *inode, struct fat_entry *fatent, int entry)
{
    struct super_block *sb = inode->i_sb;
    struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);
    struct fatent_operations *ops = sbi->fatent_ops;
    int err, offset;
    int blocknr;

    if (entry < FAT_START_ENT || sbi->max_cluster <= entry)
    {
        fatent_brelse(fatent);
        fat_fs_panic(sb, "%s: invalid access to FAT (entry %x)", __FUNCTION__, entry);
        return -EIO;
    }

    fatent_set_entry(fatent, entry);
    ops->ent_blocknr(sb, entry, &offset, &blocknr);

    if (fat_ent_update_ptr(sb, fatent, offset, blocknr))
    {
        return ops->ent_get(fatent);
    }
    return -ENOSPC;
}

/* FIXME: We can write the blocks as more big chunk. */
static int fat_mirror_bhs(struct super_block *sb, struct buffer_head **bhs,
                          int nr_bhs)
{
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    struct buffer_head *c_bh;
    int err, n, copy;

    err = 0;
    for (copy = 1; copy < sbi->fats; copy++)
    {
        int backup_fat = sbi->fat_length * copy;

        for (n = 0; n < nr_bhs; n++)
        {
            c_bh = sb_getblk(sb, backup_fat + bhs[n]->b_blocknr);
            if (!c_bh)
            {
                err = -ENOMEM;
                goto error;
            }
            memcpy(c_bh->b_data, bhs[n]->b_data, sb->s_blocksize);
            set_buffer_uptodate(c_bh);
            mark_buffer_dirty(c_bh);
            if (sb->s_flags & MS_SYNCHRONOUS)
            {
                err = sync_dirty_buffer(c_bh);
            }
            brelse(c_bh);
            if (err)
            {
                goto error;
            }
        }
    }
error:
    return err;
}

static   int fat_ent_next(struct msdos_sb_info *sbi,
                          struct fat_entry *fatent)
{
    if (sbi->fatent_ops->ent_next(fatent))
    {
        if (fatent->entry < sbi->max_cluster)
        {
            return 1;
        }
    }
    return 0;
}

static   int fat_ent_read_block(struct super_block *sb,
                                struct fat_entry *fatent)
{
    struct fatent_operations *ops = MSDOS_SB(sb)->fatent_ops;
    int blocknr;
    int offset;

    fatent_brelse(fatent);
    ops->ent_blocknr(sb, fatent->entry, &offset, &blocknr);
    return ops->ent_bread(sb, fatent, offset, blocknr);
}

static void fat_collect_bhs(struct buffer_head **bhs, int *nr_bhs,
                            struct fat_entry *fatent)
{
    int n, i;

    for (n = 0; n < fatent->nr_bhs; n++)
    {
        for (i = 0; i < *nr_bhs; i++)
        {
            if (fatent->bhs[n] == bhs[i])
            {
                break;
            }
        }
        if (i == *nr_bhs)
        {
            get_bh(fatent->bhs[n]);
            bhs[i] = fatent->bhs[n];
            (*nr_bhs)++;
        }
    }
}

#if defined CONFIG_FSYS_FAT_RW
int fat_ent_write(struct inode *inode, struct fat_entry *fatent,
                  int new, int wait)
{
    struct super_block *sb = inode->i_sb;
    struct fatent_operations *ops = MSDOS_SB(sb)->fatent_ops;
    int err;

    ops->ent_put(fatent, new);
    if (wait)
    {
        err = fat_sync_bhs(fatent->bhs, fatent->nr_bhs);
        if (err)
        {
            return err;
        }
    }
    return fat_mirror_bhs(sb, fatent->bhs, fatent->nr_bhs);
}

int fat_alloc_clusters(struct inode *inode, int *cluster, int nr_cluster)
{
    struct super_block *sb = inode->i_sb;
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    struct fatent_operations *ops = sbi->fatent_ops;
    struct fat_entry fatent, prev_ent;

    //  struct buffer_head *bhs[MAX_BUF_PER_PAGE];
    struct buffer_head **bhs = NULL;
    int i, count, err, nr_bhs, idx_clus;

    //  BUG_ON(nr_cluster > (MAX_BUF_PER_PAGE / 2));    /* fixed limit */

    if ((int)sbi->free_clusters != -1 && sbi->free_clusters < nr_cluster)
    {
        return -ENOSPC;
    }

    /* the most unfortunate status :
     * a FAT block just have one free cluster,
     * in this condition we must allocate nr_cluster
     * buffer heads to cache FAT blocks.
     * By Sunny, 2010.1.30.
     */
    bhs = (struct buffer_head **)malloc(nr_cluster * sizeof(struct buffer_head *));
    if (bhs == NULL)
    {
        fs_log_error("system allocate failed\n");
        return -ENOMEM;
    }
    err = nr_bhs = idx_clus = 0;
    count = FAT_START_ENT;
    fatent_init(&prev_ent);
    fatent_init(&fatent);
    fatent_set_entry(&fatent, sbi->prev_free + 1);
    while (count < sbi->max_cluster)
    {
        if (fatent.entry >= sbi->max_cluster)
        {
            fatent.entry = FAT_START_ENT;
        }
        fatent_set_entry(&fatent, fatent.entry);
        err = fat_ent_read_block(sb, &fatent);
        if (err)
        {
            goto out;
        }

        /* Find the free entries in a block */
        do
        {
            if (ops->ent_get(&fatent) == FAT_ENT_FREE)
            {
                int entry = fatent.entry;

                /* make the cluster chain */
                ops->ent_put(&fatent, FAT_ENT_EOF);
                if (prev_ent.nr_bhs)
                {
                    ops->ent_put(&prev_ent, entry);
                }

                fat_collect_bhs(bhs, &nr_bhs, &fatent);

                sbi->prev_free = entry;
                if ((int)sbi->free_clusters != -1)
                {
                    sbi->free_clusters--;
                }
                sb->s_dirt = 1;

                cluster[idx_clus] = entry;
                idx_clus++;
                if (idx_clus == nr_cluster)
                {
                    goto out;
                }

                /*
                 * fat_collect_bhs() gets ref-count of bhs,
                 * so we can still use the prev_ent.
                 */
                prev_ent = fatent;
            }
            count++;
            if (count == sbi->max_cluster)
            {
                break;
            }
        } while (fat_ent_next(sbi, &fatent));
    }

    /* Couldn't allocate the free entries */
    sbi->free_clusters = 0;
    sb->s_dirt = 1;
    err = -ENOSPC;

out:
    fatent_brelse(&fatent);
    if (!err)
    {
        if (inode_needs_sync(inode))
        {
            err = fat_sync_bhs(bhs, nr_bhs);
        }
        if (!err)
        {
            err = fat_mirror_bhs(sb, bhs, nr_bhs);
        }
    }
    for (i = 0; i < nr_bhs; i++)
    {
        brelse(bhs[i]);
    }

    if (err && idx_clus)
    {
        fat_free_clusters(inode, cluster[0]);
    }

    free(bhs);
    return err;
}

int fat_free_clusters(struct inode *inode, int cluster)
{
    struct super_block *sb = inode->i_sb;
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    struct fatent_operations *ops = sbi->fatent_ops;
    struct fat_entry fatent;
    struct buffer_head *bhs[MAX_BUF_PER_PAGE];
    int i, err, nr_bhs;

    nr_bhs = 0;
    fatent_init(&fatent);
    do
    {
        cluster = fat_ent_read(inode, &fatent, cluster);
        if (cluster < 0)
        {
            err = cluster;
            goto error;
        }
        else if (cluster == FAT_ENT_FREE)
        {
            fat_fs_panic(sb, "%s: deleting FAT entry beyond EOF",
                         __FUNCTION__);
            err = -EIO;
            goto error;
        }

        ops->ent_put(&fatent, FAT_ENT_FREE);
        if ((int)sbi->free_clusters != -1)
        {
            sbi->free_clusters++;
            sb->s_dirt = 1;
        }

        if (nr_bhs + fatent.nr_bhs > MAX_BUF_PER_PAGE)
        {
            if (sb->s_flags & MS_SYNCHRONOUS)
            {
                err = fat_sync_bhs(bhs, nr_bhs);
                if (err)
                {
                    goto error;
                }
            }
            err = fat_mirror_bhs(sb, bhs, nr_bhs);
            if (err)
            {
                goto error;
            }
            for (i = 0; i < nr_bhs; i++)
            {
                brelse(bhs[i]);
            }
            nr_bhs = 0;
        }
        fat_collect_bhs(bhs, &nr_bhs, &fatent);
    } while (cluster != FAT_ENT_EOF);

    if (sb->s_flags & MS_SYNCHRONOUS)
    {
        err = fat_sync_bhs(bhs, nr_bhs);
        if (err)
        {
            goto error;
        }
    }
    err = fat_mirror_bhs(sb, bhs, nr_bhs);
error:
    fatent_brelse(&fatent);
    for (i = 0; i < nr_bhs; i++)
    {
        brelse(bhs[i]);
    }

    fat_clusters_flush(sb);

    return err;
}
#endif

int fat_count_free_clusters(struct super_block *sb)
{
    struct msdos_sb_info *sbi = MSDOS_SB(sb);
    struct fatent_operations *ops = sbi->fatent_ops;
    struct fat_entry fatent;
    int err = 0, free;

    if ((int)sbi->free_clusters != -1)
    {
        goto out;
    }

    free = 0;

    fatent_init(&fatent);
    fatent_set_entry(&fatent, FAT_START_ENT);
    while (fatent.entry < sbi->max_cluster)
    {
        err = fat_ent_read_block(sb, &fatent);
        if (err)
        {
            goto out;
        }

        do
        {
            if (ops->ent_get(&fatent) == FAT_ENT_FREE)
            {
                free++;
            }
        } while (fat_ent_next(sbi, &fatent));
    }
    sbi->free_clusters = free;
    sb->s_dirt = 1;
    fatent_brelse(&fatent);
out:
    return err;
}
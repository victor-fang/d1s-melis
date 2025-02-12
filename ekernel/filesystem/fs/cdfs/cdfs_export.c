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
#ifdef  CONFIG_EXPORT

#include "cdfs.h"

static struct dentry *isofs_export_iget(struct super_block *sb,
                                        unsigned long block,
                                        unsigned long offset,
                                        __u32 generation)
{
    struct inode *inode;
    struct dentry *result;
    if (block == 0)
    {
        return ERR_PTR(-ESTALE);
    }
    inode = isofs_iget(sb, block, offset);
    if (inode == NULL)
    {
        return ERR_PTR(-ENOMEM);
    }
    if (is_bad_inode(inode)
        || (generation && inode->i_generation != generation))
    {
        iput(inode);
        return ERR_PTR(-ESTALE);
    }
    result = d_alloc_anon(inode);
    if (!result)
    {
        iput(inode);
        return ERR_PTR(-ENOMEM);
    }
    return result;
}

/* This function is surprisingly simple.  The trick is understanding
 * that "child" is always a directory. So, to find its parent, you
 * simply need to find its ".." entry, normalize its block and offset,
 * and return the underlying inode.  See the comments for
 * isofs_normalize_block_and_offset(). */
static struct dentry *isofs_export_get_parent(struct dentry *child)
{
    unsigned long parent_block = 0;
    unsigned long parent_offset = 0;
    struct inode *child_inode = child->d_inode;
    struct iso_inode_info *e_child_inode = ISOFS_I(child_inode);
    struct inode *parent_inode = NULL;
    struct iso_directory_record *de = NULL;
    struct buffer_head *bh = NULL;
    struct dentry *rv = NULL;

    /* "child" must always be a directory. */
    if (!S_ISDIR(child_inode->i_mode))
    {
        __log(KERN_ERR "isofs: isofs_export_get_parent(): "
              "child is not a directory!");
        rv = ERR_PTR(-EACCES);
        goto out;
    }

    /* It is an invariant that the directory offset is zero.  If
     * it is not zero, it means the directory failed to be
     * normalized for some reason. */
    if (e_child_inode->i_iget5_offset != 0)
    {
        __log(KERN_ERR "isofs: isofs_export_get_parent(): "
              "child directory not normalized!");
        rv = ERR_PTR(-EACCES);
        goto out;
    }

    /* The child inode has been normalized such that its
     * i_iget5_block value points to the "." entry.  Fortunately,
     * the ".." entry is located in the same block. */
    parent_block = e_child_inode->i_iget5_block;

    /* Get the block in question. */
    bh = sb_bread(child_inode->i_sb, parent_block);
    if (bh == NULL)
    {
        rv = ERR_PTR(-EACCES);
        goto out;
    }

    /* This is the "." entry. */
    de = (struct iso_directory_record *)bh->b_data;

    /* The ".." entry is always the second entry. */
    parent_offset = (unsigned long)isonum_711(de->length);
    de = (struct iso_directory_record *)(bh->b_data + parent_offset);

    /* Verify it is in fact the ".." entry. */
    if ((isonum_711(de->name_len) != 1) || (de->name[0] != 1))
    {
        __log(KERN_ERR "isofs: Unable to find the \"..\" "
              "directory for NFS.");
        rv = ERR_PTR(-EACCES);
        goto out;
    }

    /* Normalize */
    isofs_normalize_block_and_offset(de, &parent_block, &parent_offset);

    /* Get the inode. */
    parent_inode = isofs_iget(child_inode->i_sb,
                              parent_block,
                              parent_offset);
    if (parent_inode == NULL)
    {
        rv = ERR_PTR(-EACCES);
        goto out;
    }

    /* Allocate the dentry. */
    rv = d_alloc_anon(parent_inode);
    if (rv == NULL)
    {
        rv = ERR_PTR(-ENOMEM);
        goto out;
    }

out:
    if (bh)
    {
        brelse(bh);
    }
    return rv;
}

static int isofs_export_encode_fh(struct dentry *dentry,
                                  __u32 *fh32,
                                  int *max_len,
                                  int connectable)
{
    struct inode *inode = dentry->d_inode;
    struct iso_inode_info *ei = ISOFS_I(inode);
    int len = *max_len;
    int type = 1;
    __u16 *fh16 = (__u16 *)fh32;

    /*
     * WARNING: max_len is 5 for NFSv2.  Because of this
     * limitation, we use the lower 16 bits of fh32[1] to hold the
     * offset of the inode and the upper 16 bits of fh32[1] to
     * hold the offset of the parent.
     */

    if (len < 3 || (connectable && len < 5))
    {
        return 255;
    }

    len = 3;
    fh32[0] = ei->i_iget5_block;
    fh16[2] = (__u16)ei->i_iget5_offset;  /* fh16 [sic] */
    fh32[2] = inode->i_generation;
    if (connectable && !S_ISDIR(inode->i_mode))
    {
        struct inode *parent;
        struct iso_inode_info *eparent;
        spin_lock(&dentry->d_lock);
        parent = dentry->d_parent->d_inode;
        eparent = ISOFS_I(parent);
        fh32[3] = eparent->i_iget5_block;
        fh16[3] = (__u16)eparent->i_iget5_offset;  /* fh16 [sic] */
        fh32[4] = parent->i_generation;
        spin_unlock(&dentry->d_lock);
        len = 5;
        type = 2;
    }
    *max_len = len;
    return type;
}

struct isofs_fid
{
    u32 block;
    u16 offset;
    u16 parent_offset;
    u32 generation;
    u32 parent_block;
    u32 parent_generation;
};

static struct dentry *isofs_fh_to_dentry(struct super_block *sb,
        struct fid *fid, int fh_len, int fh_type)
{
    struct isofs_fid *ifid = (struct isofs_fid *)fid;

    if (fh_len < 3 || fh_type > 2)
    {
        return NULL;
    }

    return isofs_export_iget(sb, ifid->block, ifid->offset,
                             ifid->generation);
}

static struct dentry *isofs_fh_to_parent(struct super_block *sb,
        struct fid *fid, int fh_len, int fh_type)
{
    struct isofs_fid *ifid = (struct isofs_fid *)fid;

    if (fh_type != 2)
    {
        return NULL;
    }

    return isofs_export_iget(sb,
                             fh_len > 2 ? ifid->parent_block : 0,
                             ifid->parent_offset,
                             fh_len > 4 ? ifid->parent_generation : 0);
}

const struct export_operations isofs_export_ops =
{
    .encode_fh  = isofs_export_encode_fh,
    .fh_to_dentry   = isofs_fh_to_dentry,
    .fh_to_parent   = isofs_fh_to_parent,
    .get_parent     = isofs_export_get_parent,
};

#endif  /* CONFIG_EXPORT */
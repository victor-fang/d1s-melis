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
/* Current config support decompression or not */
#ifdef  CONFIG_ZISOFS

/*
 * linux/fs/isofs/compress.c
 *
 * Transparent decompression of files on an iso9660 filesystem
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/vmalloc.h>
#include <linux/zlib.h>

#include "isofs.h"
#include "zisofs.h"

/* This should probably be global. */
static char zisofs_sink_page[PAGE_CACHE_SIZE];

/*
 * This contains the zlib memory allocation and the mutex for the
 * allocation; this avoids failures at block-decompression time.
 */
static void *zisofs_zlib_workspace;
static DEFINE_MUTEX(zisofs_zlib_lock);

/*
 * When decompressing, we typically obtain more than one page
 * per reference.  We inject the additional pages into the page
 * cache as a form of readahead.
 */
static int zisofs_readpage(struct file *file, struct page *page)
{
    struct inode *inode = file->f_path.dentry->d_inode;
    struct address_space *mapping = inode->i_mapping;
    unsigned int maxpage, xpage, fpage, blockindex;
    unsigned long offset;
    unsigned long blockptr, blockendptr, cstart, cend, csize;
    struct buffer_head *bh, *ptrbh[2];
    unsigned long bufsize = ISOFS_BUFFER_SIZE(inode);
    unsigned int bufshift = ISOFS_BUFFER_BITS(inode);
    unsigned long bufmask  = bufsize - 1;
    int err = -EIO;
    int i;
    unsigned int header_size = ISOFS_I(inode)->i_format_parm[0];
    unsigned int zisofs_block_shift = ISOFS_I(inode)->i_format_parm[1];
    /* unsigned long zisofs_block_size = 1UL << zisofs_block_shift; */
    unsigned int zisofs_block_page_shift = zisofs_block_shift - PAGE_CACHE_SHIFT;
    unsigned long zisofs_block_pages = 1UL << zisofs_block_page_shift;
    unsigned long zisofs_block_page_mask = zisofs_block_pages - 1;
    struct page *pages[zisofs_block_pages];
    unsigned long index = page->index;
    int indexblocks;

    /* We have already been given one page, this is the one
       we must do. */
    xpage = index & zisofs_block_page_mask;
    pages[xpage] = page;

    /* The remaining pages need to be allocated and inserted */
    offset = index & ~zisofs_block_page_mask;
    blockindex = offset >> zisofs_block_page_shift;
    maxpage = (inode->i_size + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;

    /*
     * If this page is wholly outside i_size we just return zero;
     * do_generic_file_read() will handle this for us
     */
    if (page->index >= maxpage)
    {
        SetPageUptodate(page);
        unlock_page(page);
        return 0;
    }

    maxpage = min(zisofs_block_pages, maxpage - offset);

    for (i = 0 ; i < maxpage ; i++, offset++)
    {
        if (i != xpage)
        {
            pages[i] = grab_cache_page_nowait(mapping, offset);
        }
        page = pages[i];
        if (page)
        {
            ClearPageError(page);
            kmap(page);
        }
    }

    /* This is the last page filled, plus one; used in case of abort. */
    fpage = 0;

    /* Find the pointer to this specific chunk */
    /* Note: we're not using isonum_731() here because the data is known aligned */
    /* Note: header_size is in 32-bit words (4 bytes) */
    blockptr = (header_size + blockindex) << 2;
    blockendptr = blockptr + 4;

    indexblocks = ((blockptr ^ blockendptr) >> bufshift) ? 2 : 1;
    ptrbh[0] = ptrbh[1] = NULL;

    if (isofs_get_blocks(inode, blockptr >> bufshift, ptrbh, indexblocks) != indexblocks)
    {
        if (ptrbh[0])
        {
            brelse(ptrbh[0]);
        }
        __log(KERN_DEBUG "zisofs: Null buffer on reading block table, inode = %lu, block = %lu",
              inode->i_ino, blockptr >> bufshift);
        goto eio;
    }
    ll_rw_block(READ, indexblocks, ptrbh);

    bh = ptrbh[0];
    if (!bh || (wait_on_buffer(bh), !buffer_uptodate(bh)))
    {
        __log(KERN_DEBUG "zisofs: Failed to read block table, inode = %lu, block = %lu",
              inode->i_ino, blockptr >> bufshift);
        if (ptrbh[1])
        {
            brelse(ptrbh[1]);
        }
        goto eio;
    }
    cstart = le32_to_cpu(*(__le32 *)(bh->b_data + (blockptr & bufmask)));

    if (indexblocks == 2)
    {
        /* We just crossed a block boundary.  Switch to the next block */
        brelse(bh);
        bh = ptrbh[1];
        if (!bh || (wait_on_buffer(bh), !buffer_uptodate(bh)))
        {
            __log(KERN_DEBUG "zisofs: Failed to read block table, inode = %lu, block = %lu",
                  inode->i_ino, blockendptr >> bufshift);
            goto eio;
        }
    }
    cend = le32_to_cpu(*(__le32 *)(bh->b_data + (blockendptr & bufmask)));
    brelse(bh);

    if (cstart > cend)
    {
        goto eio;
    }

    csize = cend - cstart;

    if (csize > deflateBound(1UL << zisofs_block_shift))
    {
        goto eio;
    }

    /* Now page[] contains an array of pages, any of which can be NULL,
       and the locks on which we hold.  We should now read the data and
       release the pages.  If the pages are NULL the decompressed data
       for that particular page should be discarded. */

    if (csize == 0)
    {
        /* This data block is empty. */

        for (fpage = 0 ; fpage < maxpage ; fpage++)
        {
            if ((page = pages[fpage]) != NULL)
            {
                memset(page_address(page), 0, PAGE_CACHE_SIZE);

                flush_dcache_page(page);
                SetPageUptodate(page);
                kunmap(page);
                unlock_page(page);
                if (fpage == xpage)
                {
                    err = 0;    /* The critical page */
                }
                else
                {
                    page_cache_release(page);
                }
            }
        }
    }
    else
    {
        /* This data block is compressed. */
        z_stream stream;
        int bail = 0, left_out = -1;
        int zerr;
        int needblocks = (csize + (cstart & bufmask) + bufmask) >> bufshift;
        int haveblocks;
        struct buffer_head *bhs[needblocks + 1];
        struct buffer_head **bhptr;

        /* Because zlib is not thread-safe, do all the I/O at the top. */

        blockptr = cstart >> bufshift;
        memset(bhs, 0, (needblocks + 1)*sizeof(struct buffer_head *));
        haveblocks = isofs_get_blocks(inode, blockptr, bhs, needblocks);
        ll_rw_block(READ, haveblocks, bhs);

        bhptr = &bhs[0];
        bh = *bhptr++;

        /* First block is special since it may be fractional.
           We also wait for it before grabbing the zlib
           mutex; odds are that the subsequent blocks are
           going to come in in short order so we don't hold
           the zlib mutex longer than necessary. */

        if (!bh || (wait_on_buffer(bh), !buffer_uptodate(bh)))
        {
            __log(KERN_DEBUG "zisofs: Hit null buffer, fpage = %d, xpage = %d, csize = %ld",
                  fpage, xpage, csize);
            goto b_eio;
        }
        stream.next_in  = bh->b_data + (cstart & bufmask);
        stream.avail_in = min(bufsize - (cstart & bufmask), csize);
        csize -= stream.avail_in;

        stream.workspace = zisofs_zlib_workspace;
        mutex_lock(&zisofs_zlib_lock);

        zerr = zlib_inflateInit(&stream);
        if (zerr != Z_OK)
        {
            if (err && zerr == Z_MEM_ERROR)
            {
                err = -ENOMEM;
            }
            __log(KERN_DEBUG "zisofs: zisofs_inflateInit returned %d",
                  zerr);
            goto z_eio;
        }

        while (!bail && fpage < maxpage)
        {
            page = pages[fpage];
            if (page)
            {
                stream.next_out = page_address(page);
            }
            else
            {
                stream.next_out = (void *)&zisofs_sink_page;
            }
            stream.avail_out = PAGE_CACHE_SIZE;

            while (stream.avail_out)
            {
                int ao, ai;
                if (stream.avail_in == 0 && left_out)
                {
                    if (!csize)
                    {
                        __log(KERN_WARNING "zisofs: ZF read beyond end of input");
                        bail = 1;
                        break;
                    }
                    else
                    {
                        bh = *bhptr++;
                        if (!bh ||
                            (wait_on_buffer(bh), !buffer_uptodate(bh)))
                        {
                            /* Reached an EIO */
                            __log(KERN_DEBUG "zisofs: Hit null buffer, fpage = %d, xpage = %d, csize = %ld",
                                  fpage, xpage, csize);

                            bail = 1;
                            break;
                        }
                        stream.next_in = bh->b_data;
                        stream.avail_in = min(csize, bufsize);
                        csize -= stream.avail_in;
                    }
                }
                ao = stream.avail_out;
                ai = stream.avail_in;
                zerr = zlib_inflate(&stream, Z_SYNC_FLUSH);
                left_out = stream.avail_out;
                if (zerr == Z_BUF_ERROR && stream.avail_in == 0)
                {
                    continue;
                }
                if (zerr != Z_OK)
                {
                    /* EOF, error, or trying to read beyond end of input */
                    if (err && zerr == Z_MEM_ERROR)
                    {
                        err = -ENOMEM;
                    }
                    if (zerr != Z_STREAM_END)
                        __log(KERN_DEBUG "zisofs: zisofs_inflate returned %d, inode = %lu, index = %lu, fpage = %d, xpage = %d, avail_in = %d, avail_out = %d, ai = %d, ao = %d",
                              zerr, inode->i_ino, index,
                              fpage, xpage,
                              stream.avail_in, stream.avail_out,
                              ai, ao);
                    bail = 1;
                    break;
                }
            }

            if (stream.avail_out && zerr == Z_STREAM_END)
            {
                /* Fractional page written before EOF.  This may
                   be the last page in the file. */
                memset(stream.next_out, 0, stream.avail_out);
                stream.avail_out = 0;
            }

            if (!stream.avail_out)
            {
                /* This page completed */
                if (page)
                {
                    flush_dcache_page(page);
                    SetPageUptodate(page);
                    kunmap(page);
                    unlock_page(page);
                    if (fpage == xpage)
                    {
                        err = 0;    /* The critical page */
                    }
                    else
                    {
                        page_cache_release(page);
                    }
                }
                fpage++;
            }
        }
        zlib_inflateEnd(&stream);

z_eio:
        mutex_unlock(&zisofs_zlib_lock);

b_eio:
        for (i = 0 ; i < haveblocks ; i++)
        {
            if (bhs[i])
            {
                brelse(bhs[i]);
            }
        }
    }

eio:

    /* Release any residual pages, do not SetPageUptodate */
    while (fpage < maxpage)
    {
        page = pages[fpage];
        if (page)
        {
            flush_dcache_page(page);
            if (fpage == xpage)
            {
                SetPageError(page);
            }
            kunmap(page);
            unlock_page(page);
            if (fpage != xpage)
            {
                page_cache_release(page);
            }
        }
        fpage++;
    }

    /* At this point, err contains 0 or -EIO depending on the "critical" page */
    return err;
}

const struct address_space_operations zisofs_aops =
{
    .readpage = zisofs_readpage,
    /* No sync_page operation supported? */
    /* No bmap operation supported */
};

int zisofs_init(void)
{
    zisofs_zlib_workspace = vmalloc(zlib_inflate_workspacesize());
    if (!zisofs_zlib_workspace)
    {
        return -ENOMEM;
    }

    return 0;
}

void zisofs_cleanup(void)
{
    vfree(zisofs_zlib_workspace);
}

#endif      /* CONFIG_ZISOFS */
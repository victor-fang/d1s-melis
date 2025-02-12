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
#include "udf_decl.h"
#include "nls.h"
#include "udf_fs.h"
#include "buffer_head.h"
#include "slab.h"
#include "page_pool.h"
#include "udf_sb.h"

static int udf_translate_to_linux(uint8_t *, uint8_t *, int, uint8_t *, int);

/*
 * udf_build_ustr
 */
int udf_build_ustr(struct ustr *dest, dstring *ptr, int size)
{
    int usesize;

    if (!dest || !ptr || !size)
    {
        return -1;
    }
    BUG_ON(size < 2);

    usesize = min_t(size_t, ptr[size - 1], sizeof(dest->u_name));
    usesize = min(usesize, size - 2);
    dest->u_cmpID = ptr[0];
    dest->u_len = usesize;
    memcpy(dest->u_name, ptr + 1, usesize);
    memset(dest->u_name + usesize, 0, sizeof(dest->u_name) - usesize);

    return 0;
}

/*
 * udf_build_ustr_exact
 */
static int udf_build_ustr_exact(struct ustr *dest, dstring *ptr, int exactsize)
{
    if ((!dest) || (!ptr) || (!exactsize))
    {
        return -1;
    }

    memset(dest, 0, sizeof(struct ustr));
    dest->u_cmpID = ptr[0];
    dest->u_len = exactsize - 1;
    memcpy(dest->u_name, ptr + 1, exactsize - 1);

    return 0;
}

/*
 * udf_ocu_to_utf8
 *
 * PURPOSE
 *  Convert OSTA Compressed Unicode to the UTF-8 equivalent.
 *
 * PRE-CONDITIONS
 *  utf         Pointer to UTF-8 output buffer.
 *  ocu         Pointer to OSTA Compressed Unicode input buffer
 *              of size UDF_NAME_LEN bytes.
 *              both of type "struct ustr *"
 *
 * POST-CONDITIONS
 *  <return>        Zero on success.
 *
 * HISTORY
 *  November 12, 1997 - Andrew E. Mileski
 *  Written, tested, and released.
 */
int udf_CS0toUTF8(struct ustr *utf_o, const struct ustr *ocu_i)
{
    const uint8_t *ocu;
    uint8_t cmp_id, ocu_len;
    int i;

    ocu_len = ocu_i->u_len;
    if (ocu_len == 0)
    {
        memset(utf_o, 0, sizeof(struct ustr));
        return 0;
    }

    cmp_id = ocu_i->u_cmpID;
    if (cmp_id != 8 && cmp_id != 16)
    {
        memset(utf_o, 0, sizeof(struct ustr));
        udf_error("udf: unknown compression code (%d) stri=%s\n",
                  cmp_id, ocu_i->u_name);
        return 0;
    }

    ocu = ocu_i->u_name;
    utf_o->u_len = 0;
    for (i = 0; (i < ocu_len) && (utf_o->u_len <= (UDF_NAME_LEN - 3));)
    {

        /* Expand OSTA compressed Unicode to Unicode */
        uint32_t c = ocu[i++];
        if (cmp_id == 16)
        {
            c = (c << 8) | ocu[i++];
        }

        /* Compress Unicode to UTF-8 */
        if (c < 0x80U)
        {
            utf_o->u_name[utf_o->u_len++] = (uint8_t)c;
        }
        else if (c < 0x800U)
        {
            utf_o->u_name[utf_o->u_len++] =
                (uint8_t)(0xc0 | (c >> 6));
            utf_o->u_name[utf_o->u_len++] =
                (uint8_t)(0x80 | (c & 0x3f));
        }
        else
        {
            utf_o->u_name[utf_o->u_len++] =
                (uint8_t)(0xe0 | (c >> 12));
            utf_o->u_name[utf_o->u_len++] =
                (uint8_t)(0x80 |
                          ((c >> 6) & 0x3f));
            utf_o->u_name[utf_o->u_len++] =
                (uint8_t)(0x80 | (c & 0x3f));
        }
    }
    utf_o->u_cmpID = 8;

    return utf_o->u_len;
}

static int udf_CS0toNLS(struct nls_table *nls, struct ustr *utf_o,
                        const struct ustr *ocu_i)
{
    const uint8_t *ocu;
    uint8_t cmp_id, ocu_len;
    int i, len;


    ocu_len = ocu_i->u_len;
    if (ocu_len == 0)
    {
        memset(utf_o, 0, sizeof(struct ustr));
        return 0;
    }

    cmp_id = ocu_i->u_cmpID;
    if (cmp_id != 8 && cmp_id != 16)
    {
        memset(utf_o, 0, sizeof(struct ustr));
        udf_error("udf: unknown compression code (%d) stri=%s\n",
                  cmp_id, ocu_i->u_name);
        return 0;
    }

    ocu = ocu_i->u_name;
    utf_o->u_len = 0;
    for (i = 0; (i < ocu_len) && (utf_o->u_len <= (UDF_NAME_LEN - 3));)
    {
        /* Expand OSTA compressed Unicode to Unicode */
        uint32_t c = ocu[i++];
        if (cmp_id == 16)
        {
            c = (c << 8) | ocu[i++];
        }

        len = nls->uni2char(c, &utf_o->u_name[utf_o->u_len],
                            UDF_NAME_LEN - utf_o->u_len);
        /* Valid character? */
        if (len >= 0)
        {
            utf_o->u_len += len;
        }
        else
        {
            utf_o->u_name[utf_o->u_len++] = '?';
        }
    }
    utf_o->u_cmpID = 8;

    return utf_o->u_len;
}

int udf_get_filename(struct super_block *sb, uint8_t *sname, uint8_t *dname,
                     int flen)
{
    struct ustr *filename, *unifilename;
    int len = 0;

    filename = kmalloc(sizeof(struct ustr), GFP_NOFS);
    if (!filename)
    {
        return 0;
    }

    unifilename = kmalloc(sizeof(struct ustr), GFP_NOFS);
    if (!unifilename)
    {
        goto out1;
    }

    if (udf_build_ustr_exact(unifilename, sname, flen))
    {
        goto out2;
    }

    if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8))
    {
        if (!udf_CS0toUTF8(filename, unifilename))
        {
            udf_debug("Failed in udf_get_filename: sname = %s\n",
                      sname);
            goto out2;
        }
    }
    else if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP))
    {
        if (!udf_CS0toNLS(UDF_SB(sb)->s_nls_map, filename,
                          unifilename))
        {
            udf_debug("Failed in udf_get_filename: sname = %s\n",
                      sname);
            goto out2;
        }
    }
    else
    {
        goto out2;
    }

    len = udf_translate_to_linux(dname, filename->u_name, filename->u_len,
                                 unifilename->u_name, unifilename->u_len);
out2:
    kfree(unifilename);
out1:
    kfree(filename);
    return len;
}

#define ILLEGAL_CHAR_MARK   '_'
#define EXT_MARK            '.'
#define CRC_MARK            '#'
#define EXT_SIZE            5

static int udf_translate_to_linux(uint8_t *newName, uint8_t *udfName,
                                  int udfLen, uint8_t *fidName,
                                  int fidNameLen)
{
    int index, newIndex = 0, needsCRC = 0;
    int extIndex = 0, newExtIndex = 0, hasExt = 0;
    unsigned short valueCRC;
    uint8_t curr;
    const uint8_t hexChar[] = "0123456789ABCDEF";

    if (udfName[0] == '.' &&
        (udfLen == 1 || (udfLen == 2 && udfName[1] == '.')))
    {
        needsCRC = 1;
        newIndex = udfLen;
        memcpy(newName, udfName, udfLen);
    }
    else
    {
        for (index = 0; index < udfLen; index++)
        {
            curr = udfName[index];
            if (curr == '/' || curr == 0)
            {
                needsCRC = 1;
                curr = ILLEGAL_CHAR_MARK;
                while (index + 1 < udfLen &&
                       (udfName[index + 1] == '/' ||
                        udfName[index + 1] == 0))
                {
                    index++;
                }
            }
            if (curr == EXT_MARK &&
                (udfLen - index - 1) <= EXT_SIZE)
            {
                if (udfLen == index + 1)
                {
                    hasExt = 0;
                }
                else
                {
                    hasExt = 1;
                    extIndex = index;
                    newExtIndex = newIndex;
                }
            }
            if (newIndex < 256)
            {
                newName[newIndex++] = curr;
            }
            else
            {
                needsCRC = 1;
            }
        }
    }
    if (needsCRC)
    {
        uint8_t ext[EXT_SIZE];
        int localExtIndex = 0;

        if (hasExt)
        {
            int maxFilenameLen;
            for (index = 0;
                 index < EXT_SIZE && extIndex + index + 1 < udfLen;
                 index++)
            {
                curr = udfName[extIndex + index + 1];

                if (curr == '/' || curr == 0)
                {
                    needsCRC = 1;
                    curr = ILLEGAL_CHAR_MARK;
                    while (extIndex + index + 2 < udfLen &&
                           (index + 1 < EXT_SIZE &&
                            (udfName[extIndex + index + 2] == '/' ||
                             udfName[extIndex + index + 2] == 0)))
                    {
                        index++;
                    }
                }
                ext[localExtIndex++] = curr;
            }
            maxFilenameLen = 250 - localExtIndex;
            if (newIndex > maxFilenameLen)
            {
                newIndex = maxFilenameLen;
            }
            else
            {
                newIndex = newExtIndex;
            }
        }
        else if (newIndex > 250)
        {
            newIndex = 250;
        }
        newName[newIndex++] = CRC_MARK;
        valueCRC = crc_itu_t(0, fidName, fidNameLen);
        newName[newIndex++] = hexChar[(valueCRC & 0xf000) >> 12];
        newName[newIndex++] = hexChar[(valueCRC & 0x0f00) >> 8];
        newName[newIndex++] = hexChar[(valueCRC & 0x00f0) >> 4];
        newName[newIndex++] = hexChar[(valueCRC & 0x000f)];

        if (hasExt)
        {
            newName[newIndex++] = EXT_MARK;
            for (index = 0; index < localExtIndex; index++)
            {
                newName[newIndex++] = ext[index];
            }
        }
    }

    return newIndex;
}

#if defined CONFIG_FSYS_UDFFS_RW
static int udf_NLStoCS0(struct nls_table *nls, dstring *ocu, struct ustr *uni,
                        int length)
{
    int len;
    unsigned i, max_val;
    uint16_t uni_char;
    int u_len;

    memset(ocu, 0, sizeof(dstring) * length);
    ocu[0] = 8;
    max_val = 0xffU;

try_again:
    u_len = 0U;
    for (i = 0U; i < uni->u_len; i++)
    {
        len = nls->char2uni(&uni->u_name[i], uni->u_len - i, &uni_char);
        if (!len)
        {
            continue;
        }
        /* Invalid character, deal with it */
        if (len < 0)
        {
            len = 1;
            uni_char = '?';
        }

        if (uni_char > max_val)
        {
            max_val = 0xffffU;
            ocu[0] = (uint8_t)0x10U;
            goto try_again;
        }

        if (max_val == 0xffffU)
        {
            ocu[++u_len] = (uint8_t)(uni_char >> 8);
        }
        ocu[++u_len] = (uint8_t)(uni_char & 0xffU);
        i += len - 1;
    }

    ocu[length - 1] = (uint8_t)u_len + 1;
    return u_len + 1;
}


/*
 *
 * udf_utf8_to_ocu
 *
 * PURPOSE
 *  Convert UTF-8 to the OSTA Compressed Unicode equivalent.
 *
 * DESCRIPTION
 *  This routine is only called by udf_lookup().
 *
 * PRE-CONDITIONS
 *  ocu         Pointer to OSTA Compressed Unicode output
 *              buffer of size UDF_NAME_LEN bytes.
 *  utf         Pointer to UTF-8 input buffer.
 *  utf_len         Length of UTF-8 input buffer in bytes.
 *
 * POST-CONDITIONS
 *  <return>        Zero on success.
 *
 * HISTORY
 *  November 12, 1997 - Andrew E. Mileski
 *  Written, tested, and released.
 */
static int udf_UTF8toCS0(dstring *ocu, struct ustr *utf, int length)
{
    unsigned c, i, max_val, utf_char;
    int utf_cnt, u_len;

    memset(ocu, 0, sizeof(dstring) * length);
    ocu[0] = 8;
    max_val = 0xffU;

try_again:
    u_len = 0U;
    utf_char = 0U;
    utf_cnt = 0U;
    for (i = 0U; i < utf->u_len; i++)
    {
        c = (uint8_t)utf->u_name[i];

        /* Complete a multi-byte UTF-8 character */
        if (utf_cnt)
        {
            utf_char = (utf_char << 6) | (c & 0x3fU);
            if (--utf_cnt)
            {
                continue;
            }
        }
        else
        {
            /* Check for a multi-byte UTF-8 character */
            if (c & 0x80U)
            {
                /* Start a multi-byte UTF-8 character */
                if ((c & 0xe0U) == 0xc0U)
                {
                    utf_char = c & 0x1fU;
                    utf_cnt = 1;
                }
                else if ((c & 0xf0U) == 0xe0U)
                {
                    utf_char = c & 0x0fU;
                    utf_cnt = 2;
                }
                else if ((c & 0xf8U) == 0xf0U)
                {
                    utf_char = c & 0x07U;
                    utf_cnt = 3;
                }
                else if ((c & 0xfcU) == 0xf8U)
                {
                    utf_char = c & 0x03U;
                    utf_cnt = 4;
                }
                else if ((c & 0xfeU) == 0xfcU)
                {
                    utf_char = c & 0x01U;
                    utf_cnt = 5;
                }
                else
                {
                    goto error_out;
                }
                continue;
            }
            else
            {
                /* Single byte UTF-8 character (most common) */
                utf_char = c;
            }
        }

        /* Choose no compression if necessary */
        if (utf_char > max_val)
        {
            if (max_val == 0xffU)
            {
                max_val = 0xffffU;
                ocu[0] = (uint8_t)0x10U;
                goto try_again;
            }
            goto error_out;
        }

        if (max_val == 0xffffU)
        {
            ocu[++u_len] = (uint8_t)(utf_char >> 8);
        }
        ocu[++u_len] = (uint8_t)(utf_char & 0xffU);
    }

    if (utf_cnt)
    {
error_out:
        ocu[++u_len] = '?';
        udf_info("udf: bad UTF-8 character\n");
    }

    ocu[length - 1] = (uint8_t)u_len + 1;

    return u_len + 1;
}

static int udf_char_to_ustr(struct ustr *dest, const uint8_t *src, int strlen)
{
    if ((!dest) || (!src) || (!strlen) || (strlen > UDF_NAME_LEN - 2))
    {
        return 0;
    }

    memset(dest, 0, sizeof(struct ustr));
    memcpy(dest->u_name, src, strlen);
    dest->u_cmpID = 0x08;
    dest->u_len = strlen;

    return strlen;
}

int udf_put_filename(struct super_block *sb, const uint8_t *sname,
                     uint8_t *dname, int flen)
{
    struct ustr unifilename;
    int namelen;

    if (!udf_char_to_ustr(&unifilename, sname, flen))
    {
        return 0;
    }

    if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8))
    {
        namelen = udf_UTF8toCS0(dname, &unifilename, UDF_NAME_LEN);
        if (!namelen)
        {
            return 0;
        }
    }
    else if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP))
    {
        namelen = udf_NLStoCS0(UDF_SB(sb)->s_nls_map, dname,
                               &unifilename, UDF_NAME_LEN);
        if (!namelen)
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    return namelen;
}
#endif  /* FSYS_UDF_RW */
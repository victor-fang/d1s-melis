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
#include "fsys_debug.h"
#include "nls.h"
#include "errno.h"
#include <mod_charset.h>
#include <sys_charset.h>

static struct nls_table default_table;
static int charset_type = 0;

/*
 * Sample implementation from Unicode home page.
 * http://www.stonehand.com/unicode/standard/fss-utf.html
 */
struct utf8_table
{
    int     cmask;
    int     cval;
    int     shift;
    long    lmask;
    long    lval;
};

static struct utf8_table utf8_table[] =
{
    {0x80,  0x00,   0 * 6,    0x7F,           0,         /* 1 byte sequence */},
    {0xE0,  0xC0,   1 * 6,    0x7FF,          0x80,      /* 2 byte sequence */},
    {0xF0,  0xE0,   2 * 6,    0xFFFF,         0x800,     /* 3 byte sequence */},
    {0xF8,  0xF0,   3 * 6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
    {0xFC,  0xF8,   4 * 6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
    {0xFE,  0xFC,   5 * 6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
    {0,                            /* end of table    */}
};

int utf8_mbtowc(__wchar_t *p, const __u8 *s, int n)
{
    long l;
    int c0, c, nc;
    struct utf8_table *t;

    nc = 0;
    c0 = *s;
    l = c0;
    for (t = utf8_table; t->cmask; t++)
    {
        nc++;
        if ((c0 & t->cmask) == t->cval)
        {
            l &= t->lmask;
            if (l < t->lval)
            {
                return -1;
            }
            *p = l;
            return nc;
        }
        if (n <= nc)
        {
            return -1;
        }
        s++;
        c = (*s ^ 0x80) & 0xFF;
        if (c & 0xC0)
        {
            return -1;
        }
        l = (l << 6) | c;
    }
    return -1;
}

int utf8_mbstowcs(__wchar_t *pwcs, const __u8 *s, int n)
{
    __u16 *op;
    const __u8 *ip;
    int size;

    op = pwcs;
    ip = s;
    while (*ip && n > 0)
    {
        if (*ip & 0x80)
        {
            size = utf8_mbtowc(op, ip, n);
            if (size == -1)
            {
                /* Ignore character and move on */
                ip++;
                n--;
            }
            else
            {
                op++;
                ip += size;
                n -= size;
            }
        }
        else
        {
            *op++ = *ip++;
            n--;
        }
    }
    return (op - pwcs);
}

int utf8_wctomb(__u8 *s, __wchar_t wc, int maxlen)
{
    long l;
    int c, nc;
    struct utf8_table *t;

    if (s == 0)
    {
        return 0;
    }

    l = wc;
    nc = 0;
    for (t = utf8_table; t->cmask && maxlen; t++, maxlen--)
    {
        nc++;
        if (l <= t->lmask)
        {
            c = t->shift;
            *s = t->cval | (l >> c);
            while (c > 0)
            {
                c -= 6;
                if(c)
                {
                    s++;
                    *s = 0x80 | ((l >> c) & 0x3F);
                }
            }
            return nc;
        }
    }
    return -1;
}

int utf8_wcstombs(__u8 *s, const __wchar_t *pwcs, int maxlen)
{
    const __u16 *ip;
    __u8 *op;
    int size;

    op = s;
    ip = pwcs;
    while (*ip && maxlen > 0)
    {
        if (*ip > 0x7f)
        {
            size = utf8_wctomb(op, *ip, maxlen);
            if (size == -1)
            {
                /* Ignore character and move on */
                maxlen--;
            }
            else
            {
                op += size;
                maxlen -= size;
            }
        }
        else
        {
            *op++ = (__u8) * ip;
        }
        ip++;
    }
    return (op - s);
}

#define UL_BUF_LEN 256
static unsigned char charset2lower_tbl[UL_BUF_LEN];
static unsigned char charset2upper_tbl[UL_BUF_LEN];

struct nls_table *load_nls(int type)
{
    struct nls_table *nls = &default_table;

    charset_type = type;

    esCHS_GetChUpperTbl(charset_type, nls->charset2upper, UL_BUF_LEN);
    esCHS_GetChLowerTbl(charset_type, nls->charset2lower, UL_BUF_LEN);

    return nls;
}

void unload_nls(struct nls_table *nls)
{
}

int get_current_charset(void)
{
    return charset_type;
}

static int uni2char(__wchar_t uni, unsigned char *out, int boundlen)
{
    int len;

    if (boundlen <= 0)
    {
        return -ENAMETOOLONG;
    }

    len = esCHS_Uni2Char(charset_type, uni, out, boundlen);
    if (len <= 0)
    {
        return -EINVAL;
    }
    return len;
}

static int char2uni(const unsigned char *rawstring, int boundlen, __wchar_t *uni)
{
    int len;

    len = esCHS_Char2Uni(charset_type, rawstring, boundlen, uni);
    if (len <= 0)
    {
        return -EINVAL;
    }
    return len;
}

static struct nls_table default_table =
{
    .uni2char   = uni2char,
    .char2uni   = char2uni,
    .charset2lower  = charset2lower_tbl,
    .charset2upper  = charset2upper_tbl,
};
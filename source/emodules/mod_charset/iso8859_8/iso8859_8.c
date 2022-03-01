/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              Apps Sub-System
*
*                                   (c) Copyright 2007-2009, john.fu <john.fu@allwinnerme.com>
*                                             All Rights Reserved
*
* Moudle  : Charset
* File    : iso8859_8.c
*
* By      : Gary.Wang
* Version : v1.0
* Date    : 2010-11-05
**********************************************************************************************************************
*/
#include "mod_charset_i.h"
#include <libc.h>

static const unsigned short iso8859_8_2uni[96] =
{
    /* 0xa0 */
    0x00a0, 0xfffd, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
    0x00a8, 0x00a9, 0x00d7, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    /* 0xb0 */
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
    0x00b8, 0x00b9, 0x00f7, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0xfffd,
    /* 0xc0 */
    0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
    0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
    /* 0xd0 */
    0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
    0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0x2017,
    /* 0xe0 */
    0x05d0, 0x05d1, 0x05d2, 0x05d3, 0x05d4, 0x05d5, 0x05d6, 0x05d7,
    0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df,
    /* 0xf0 */
    0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7,
    0x05e8, 0x05e9, 0x05ea, 0xfffd, 0xfffd, 0x200e, 0x200f, 0xfffd,
};

static int iso8859_8_mbtowc(__u16 *pwc, const unsigned char *s, int n)
{
    unsigned char c;

    if (s == NULL || n <= 0)
    {
        return -1;
    }

    c = *s;

    if (c >= 0xa0)
    {
        unsigned short wc = iso8859_8_2uni[c - 0xa0];

        if (wc != 0xfffd)
        {
            *pwc = (__u16) wc;
            return 1;
        }
    }
    else
    {
        *pwc = (__u16) c;
        return 1;
    }

    return -1;
}

static const unsigned char iso8859_8_page00[88] =
{
    0xa0, 0x00, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, /* 0xa0-0xa7 */
    0xa8, 0xa9, 0x00, 0xab, 0xac, 0xad, 0xae, 0xaf, /* 0xa8-0xaf */
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, /* 0xb0-0xb7 */
    0xb8, 0xb9, 0x00, 0xbb, 0xbc, 0xbd, 0xbe, 0x00, /* 0xb8-0xbf */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc0-0xc7 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc8-0xcf */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, /* 0xd0-0xd7 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xd8-0xdf */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xe0-0xe7 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xe8-0xef */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xba, /* 0xf0-0xf7 */
};
static const unsigned char iso8859_8_page05[32] =
{
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0xd0-0xd7 */
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0xd8-0xdf */
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 0xe0-0xe7 */
    0xf8, 0xf9, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xe8-0xef */
};
static const unsigned char iso8859_8_page20[16] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfe, /* 0x08-0x0f */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, /* 0x10-0x17 */
};

static int iso8859_8_wctomb(unsigned char *r, __u16 wc, int n)
{
    unsigned char c = 0;

    if (r == NULL || n == 0)
    {
        return -1;
    }

    if (wc < 0x00a0)
    {
        *r = wc;
        return 1;
    }
    else if (wc >= 0x00a0 && wc < 0x00f8)
    {
        c = iso8859_8_page00[wc - 0x00a0];
    }
    else if (wc >= 0x05d0 && wc < 0x05f0)
    {
        c = iso8859_8_page05[wc - 0x05d0];
    }
    else if (wc >= 0x2008 && wc < 0x2018)
    {
        c = iso8859_8_page20[wc - 0x2008];
    }

    if (c != 0)
    {
        *r = c;
        return 1;
    }

    return -1;
}


__s32 ISO8859_8_uni2char(const __u16 uni, __u8 *out, __u32 boundlen)
{
    __s32 n;
    n = iso8859_8_wctomb(out, uni, boundlen);

    if (n <= 0)
    {
        *out = '?';
        return -1;
    }
    else
    {
        return n;
    }
}


__s32 ISO8859_8_char2uni(const __u8 *rawstring, __u32 boundlen, __u16 *uni)
{
    __s32 n;
    n = iso8859_8_mbtowc(uni, rawstring, boundlen);

    if (n <= 0)
    {
        *uni = 0x003f;  /* ? */
        return -1;
    }
    else
    {
        return n;
    }
}


static const __u8 charset2lower[256] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, /* 0x00-0x07 */
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, /* 0x08-0x0f */
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, /* 0x10-0x17 */
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, /* 0x18-0x1f */
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, /* 0x20-0x27 */
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, /* 0x28-0x2f */
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, /* 0x30-0x37 */
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, /* 0x38-0x3f */
    0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, /* 0x40-0x47 */
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, /* 0x48-0x4f */
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, /* 0x50-0x57 */
    0x78, 0x79, 0x7a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, /* 0x58-0x5f */
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, /* 0x60-0x67 */
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, /* 0x68-0x6f */
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, /* 0x70-0x77 */
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, /* 0x78-0x7f */

    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, /* 0x80-0x87 */
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, /* 0x88-0x8f */
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, /* 0x90-0x97 */
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, /* 0x98-0x9f */
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, /* 0xa0-0xa7 */
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, /* 0xa8-0xaf */
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, /* 0xb0-0xb7 */
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, /* 0xb8-0xbf */
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0xc0-0xc7 */
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, /* 0xc8-0xcf */
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, /* 0xd0-0xd7 */
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, /* 0xd8-0xdf */
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0xe0-0xe7 */
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0xe8-0xef */
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 0xf0-0xf7 */
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, /* 0xf8-0xff */
};

static const __u8 charset2upper[256] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, /* 0x00-0x07 */
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, /* 0x08-0x0f */
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, /* 0x10-0x17 */
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, /* 0x18-0x1f */
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, /* 0x20-0x27 */
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, /* 0x28-0x2f */
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, /* 0x30-0x37 */
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, /* 0x38-0x3f */
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, /* 0x40-0x47 */
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, /* 0x48-0x4f */
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, /* 0x50-0x57 */
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, /* 0x58-0x5f */
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, /* 0x60-0x67 */
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, /* 0x68-0x6f */
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, /* 0x70-0x77 */
    0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, /* 0x78-0x7f */

    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, /* 0x80-0x87 */
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, /* 0x88-0x8f */
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, /* 0x90-0x97 */
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, /* 0x98-0x9f */
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, /* 0xa0-0xa7 */
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, /* 0xa8-0xaf */
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, /* 0xb0-0xb7 */
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, /* 0xb8-0xbf */
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0xc0-0xc7 */
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, /* 0xc8-0xcf */
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, /* 0xd0-0xd7 */
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, /* 0xd8-0xdf */
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0xe0-0xe7 */
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0xe8-0xef */
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 0xf0-0xf7 */
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, /* 0xf8-0xff */
};

__u32 ISO8859_8_get_charset2upper_table(void *buf, __u32 size)
{
    __u32  len;

    if (buf == NULL || size == 0)
    {
        return 0;
    }

    len = MIN(size, sizeof(charset2upper));
    eLIBs_memcpy(buf, charset2upper, len);
    return len;
}


__u32 ISO8859_8_get_charset2lower_table(void *buf, __u32 size)
{
    __u32  len;

    if (buf == NULL || size == 0)
    {
        return 0;
    }

    len = MIN(size, sizeof(charset2lower));
    eLIBs_memcpy(buf, charset2lower, len);
    return len;
}

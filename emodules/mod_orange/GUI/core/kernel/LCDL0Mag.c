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
#include <stddef.h>             /* needed for definition of NULL */
#include "gui_core_kernel_private.h"

#if LCD_XMAG > 2
#error Not yet supported
#endif

#if LCD_XMAG == 2

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const U8 abData12[16] =
{
    0x0, 0x3, 0xc, 0xf, 0x30, 0x33, 0x3c, 0x3f, 0xC0, 0xC3, 0xCc, 0xcf, 0xf0, 0xf3, 0xfc, 0xff
};

static const U8 abData22[16] =
{
    0x0,   0x5, 0x5 * 2, 0x5 * 3,
    0x50 * 1 + 0x0, 0x50 * 1 + 0x5, 0x50 * 1 + 0x5 * 2, 0x50 * 1 + 0x5 * 3,
    0x50 * 2 + 0x0, 0x50 * 2 + 0x5, 0x50 * 2 + 0x5 * 2, 0x50 * 2 + 0x5 * 3,
    0x50 * 3 + 0x0, 0x50 * 3 + 0x5, 0x50 * 3 + 0x5 * 2, 0x50 * 3 + 0x5 * 3
};

static const U8 abData32[16] =
{
    0x0, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawBitline
*/
static void _DrawBitline(int x0, int y0, int xsize, int BPP, const U8 *pData, int Diff, const LCD_PIXELINDEX *pTrans)
{
    union
    {
        U8  ab[100];     /* Byte Buffer */
        U16 au16[50];    /* Word Buffer */
    } Buffer;
    int i;
    int NumPixels;
    int NumPixelsMax = sizeof(Buffer.ab) * 8 / BPP / LCD_XMAG;

    while (xsize > 0)
    {
        int iOff = 0;
        NumPixels = (xsize < NumPixelsMax) ? xsize : NumPixelsMax;

        switch (BPP)
        {
            case 1:
                for (i = 0; i < NumPixels; i += 8)
                {
                    /* load the byte */
                    U16 Data;

                    if (Diff)
                    {
                        Data = ((*pData) << 8) | (*(pData + 1));
                        Data >>= 8 - Diff;
                        Data &= 0xff;
                    }
                    else
                    {
                        Data = *pData;
                    }

                    pData++;
                    Buffer.ab[iOff++] = abData12[Data >> 4];
                    Buffer.ab[iOff++] = abData12[Data & 15];
                }

                break;

            case 2:
                for (i = 0; i < NumPixels; i += 4)
                {
                    /* load the byte */
                    U16 Data;

                    if (Diff)
                    {
                        Data = ((*pData) << 8) | (*(pData + 1));
                        Data >>= 8 - (Diff << 1);
                        Data &= 0xff;
                    }
                    else
                    {
                        Data = *pData;
                    }

                    pData++;
                    Buffer.ab[iOff++] = abData22[Data >> 4];
                    Buffer.ab[iOff++] = abData22[Data & 15];
                }

                break;

            case 4:
                for (i = 0; i < NumPixels; i += 2)
                {
                    /* load the byte */
                    U16 Data;

                    if (Diff)
                    {
                        Data = ((*pData) << 8) | (*(pData + 1));
                        Data >>= 8 - (Diff << 2);
                        Data &= 0xff;
                    }
                    else
                    {
                        Data = *pData;
                    }

                    pData++;
                    Buffer.ab[iOff++] = abData32[Data >> 4];
                    Buffer.ab[iOff++] = abData32[Data & 15];
                }

                break;

            case 8:
                for (i = 0; i < NumPixels; i++)
                {
#if LCD_XMAG > 2
                    GUI_MEMSET(&Buffer.ab[i * LCD_XMAG], *(pData + i), LCD_XMAG);
#else
                    Buffer.ab[i * LCD_XMAG] = Buffer.ab[i * LCD_XMAG + 1] = *pData++;
#endif
                }

                break;

            case 16:
                for (i = 0; i < NumPixels; i++)
                {
#if LCD_XMAG > 2
                    GUI_MEMSET(&Buffer.ab[i * LCD_XMAG], *(pData + i), LCD_XMAG);
#else
                    Buffer.au16[i * LCD_XMAG] = Buffer.au16[i * LCD_XMAG + 1] = *(U16 *)pData;
                    pData += 2; /* Move to next word */
#endif
                }

                break;
        }

        LCD_L0_DrawBitmap(x0 + Diff * LCD_XMAG, y0, NumPixels * LCD_XMAG, LCD_YMAG, BPP, 0, Buffer.ab, 0, pTrans);
        x0 += NumPixels * LCD_XMAG;
        xsize -= NumPixels;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_L0_MAG_DrawBitmap
*/
void LCD_L0_MAG_DrawBitmap(int x0, int y0, int xsize, int ysize,
                           int BPP, int BytesPerLine, const U8 *pData, int Diff,
                           const LCD_PIXELINDEX *pTrans)
{
    int iLine;
    y0 *= LCD_YMAG;
    x0 *= LCD_XMAG;

    for (iLine = 0; iLine < ysize; iLine++)
    {
#if LCD_XMAG == 1
        LCD_L0_DrawBitmap(x0, y0, xsize, LCD_YMAG, BPP, 0, pData, Diff, pTrans);
#else
        _DrawBitline(x0, y0, xsize, BPP, pData, Diff, pTrans);
#endif
        y0 += LCD_YMAG;
        pData += BytesPerLine;
    }
}

/*********************************************************************
*
*       LCD_L0_MAG_DrawHLine
*/
void LCD_L0_MAG_DrawHLine(int x0, int y0,  int x1)
{
    x0 *= LCD_XMAG;
    x1 = x1 * (LCD_XMAG) + LCD_XMAG - 1;
    y0 *= LCD_YMAG;
    LCD_L0_FillRect(x0, y0, x1, y0 + LCD_YMAG - 1);
}

/*********************************************************************
*
*       LCD_L0_MAG_DrawVLine
*/
void LCD_L0_MAG_DrawVLine(int x0, int y0,  int y1)
{
    y0 *= LCD_YMAG;
    y1 = y1 * (LCD_YMAG) + LCD_YMAG - 1;
    x0 *= LCD_XMAG;
    LCD_L0_FillRect(x0, y0, x0 + LCD_XMAG - 1, y1);
}

/*********************************************************************
*
*       LCD_L0_MAG_FillRect
*/
void LCD_L0_MAG_FillRect(int x0, int y0, int x1, int y1)
{
    y0 *= LCD_YMAG;
    y1 = y1 * (LCD_YMAG) + LCD_YMAG - 1;
    x0 *= LCD_XMAG;
    x1 = x1 * (LCD_XMAG) + LCD_XMAG - 1;
    LCD_L0_FillRect(x0, y0, x1, y1);
}

/*********************************************************************
*
*       LCD_L0_MAG_GetPixelIndex
*/
unsigned int LCD_L0_MAG_GetPixelIndex(int x, int y)
{
    return LCD_L0_GetPixelIndex(x * LCD_XMAG, y * LCD_YMAG);
}

/*********************************************************************
*
*       LCD_L0_MAG_SetPixelIndex
*/
void LCD_L0_MAG_SetPixelIndex(int x, int y, int ColorIndex)
{
    int ix, iy;
    y *= LCD_YMAG;
    x *= LCD_XMAG;

    for (iy = 0; iy < LCD_YMAG; iy++)
    {
        for (ix = 0; ix < LCD_XMAG; ix++)
        {
            LCD_L0_SetPixelIndex(x + ix, y + iy, ColorIndex);
        }
    }
}

/*********************************************************************
*
*       LCD_L0_MAG_XorPixel
*/
void LCD_L0_MAG_XorPixel(int x, int y)
{
    int ix, iy;
    y *= LCD_YMAG;
    x *= LCD_XMAG;

    for (iy = 0; iy < LCD_YMAG; iy++)
    {
        for (ix = 0; ix < LCD_XMAG; ix++)
        {
            LCD_L0_XorPixel(x + ix, y + iy);
        }
    }
}

#else

void LCDL0Mag_c(void);
void LCDL0Mag_c(void) { } /* avoid empty object files */

#endif

/*************************** End of file ****************************/

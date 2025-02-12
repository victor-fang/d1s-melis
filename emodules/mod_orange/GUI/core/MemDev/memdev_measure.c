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
#include "gui_core_memdev_private.h"


#if GUI_SUPPORT_MEMDEV

/*********************************************************************
*
*       typedefs
*
**********************************************************************
*/

typedef struct
{
    GUI_RECT rUsed;
    tLCDDEV_Color2Index  *pfColor2Index;
    tLCDDEV_Index2Color  *pfIndex2Color;
    tLCDDEV_GetIndexMask *pfGetIndexMask;
} GUI_MEASDEV;

/*********************************************************************
*
*       defines
*
**********************************************************************
*/

#define GUI_MEASDEV_H2P(h) ((GUI_MEASDEV*)GUI_ALLOC_h2p(h))

/*********************************************************************
*
*       static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _MarkPixel
*/
static void _MarkPixel(int x, int y)
{
    GUI_MEASDEV *pDev = (GUI_MEASDEV *)(GUI_MEMDEV_H2P(GUI_Context.hDevData));

    if (x < pDev->rUsed.x0)
    {
        pDev->rUsed.x0 = x;
    }

    if (x > pDev->rUsed.x1)
    {
        pDev->rUsed.x1 = x;
    }

    if (y < pDev->rUsed.y0)
    {
        pDev->rUsed.y0 = y;
    }

    if (y > pDev->rUsed.y1)
    {
        pDev->rUsed.y1 = y;
    }
}

/*********************************************************************
*
*       _MarkRect
*/
static void _MarkRect(int x0, int y0, int x1, int y1)
{
    GUI_MEASDEV *pDev = (GUI_MEASDEV *)(GUI_MEMDEV_H2P(GUI_Context.hDevData));

    if (x0 < pDev->rUsed.x0)
    {
        pDev->rUsed.x0 = x0;
    }

    if (x1 > pDev->rUsed.x1)
    {
        pDev->rUsed.x1 = x1;
    }

    if (y1 < pDev->rUsed.y0)
    {
        pDev->rUsed.y0 = y0;
    }

    if (y1 > pDev->rUsed.y1)
    {
        pDev->rUsed.y1 = y1;
    }
}

/*********************************************************************
*
*       _DrawBitmap
*/
static void _DrawBitmap(int x0, int y0, int xsize, int ysize,
                        int BitsPerPixel, int BytesPerLine,
                        const U8 GUI_UNI_PTR *pData, int Diff,
                        const LCD_PIXELINDEX *pTrans)
{
    GUI_USE_PARA(BitsPerPixel);
    GUI_USE_PARA(BytesPerLine);
    GUI_USE_PARA(pData);
    GUI_USE_PARA(Diff);
    GUI_USE_PARA(pTrans);
    _MarkRect(x0, y0, x0 + xsize - 1, y0 + ysize - 1);
}

/*********************************************************************
*
*       _DrawHLine
*/
static void _DrawHLine(int x0, int y,  int x1)
{
    _MarkRect(x0, y, x1, y);
}

/*********************************************************************
*
*       _DrawVLine
*/
static void _DrawVLine(int x, int y0,  int y1)
{
    _MarkRect(x, y0, x, y1);
}

/*********************************************************************
*
*       _SetPixelIndex
*/
static void _SetPixelIndex(int x, int y, int Index)
{
    GUI_USE_PARA(Index);
    _MarkPixel(x, y);
}

/*********************************************************************
*
*       _XorPixel
*/
static void _XorPixel(int x, int y)
{
    _MarkPixel(x, y);
}

/*********************************************************************
*
*       _GetPixelIndex
*/
static unsigned int _GetPixelIndex(int x, int y)
{
    GUI_USE_PARA(x);
    GUI_USE_PARA(y);
    return 0;
}

/*********************************************************************
*
*       _FillRect
*/
static void _FillRect(int x0, int y0, int x1, int y1)
{
    _MarkRect(x0, y0, x1, y1);
}

/*********************************************************************
*
*       _GetRect
*/
static void _GetRect(LCD_RECT *pRect)
{
    pRect->x0 = pRect->y0 = -4095;
    pRect->x1 = pRect->y1 =  4095;
}

/*********************************************************************
*
*       _Color2Index
*/
static unsigned int _Color2Index(LCD_COLOR Color)
{
    GUI_MEASDEV *pDev = GUI_MEASDEV_H2P(GUI_Context.hDevData);
    return pDev->pfColor2Index(Color);
}

/*********************************************************************
*
*       _Index2Color
*/
static LCD_COLOR _Index2Color(int Index)
{
    GUI_MEASDEV *pDev = GUI_MEASDEV_H2P(GUI_Context.hDevData);
    return pDev->pfIndex2Color(Index);
}

/*********************************************************************
*
*       _GetIndexMask
*/
static unsigned int _GetIndexMask(void)
{
    GUI_MEASDEV *pDev = GUI_MEASDEV_H2P(GUI_Context.hDevData);
    return pDev->pfGetIndexMask();
}

/*********************************************************************
*
*       _CalcPolyRect
*/
static void _CalcPolyRect(GUI_RECT *pr, const GUI_POINT *paPoint, int NumPoints)
{
    int i;
    int xMin, xMax, yMin, yMax;
    xMin = GUI_XMAX;
    yMin = GUI_YMAX;
    xMax = GUI_XMIN;
    yMax = GUI_YMIN;

    for (i = 0; i  < NumPoints; i++)
    {
        int x = paPoint->x;
        int y = paPoint->y;

        if (xMin > x)
        {
            xMin = x;
        }

        if (xMax < x)
        {
            xMax = x;
        }

        if (yMin > y)
        {
            yMin = y;
        }

        if (yMax < y)
        {
            yMax = y;
        }

        paPoint++;
    }

    pr->x0 = xMin;
    pr->x1 = xMax;
    pr->y0 = yMin;
    pr->y1 = yMax;
}

/*********************************************************************
*
*       _FillPolygon
*/
static void _FillPolygon(const GUI_POINT *paPoint, int NumPoints, int x0, int y0)
{
    GUI_RECT r;
    _CalcPolyRect(&r, paPoint, NumPoints);
    GUI_MoveRect(&r, x0, y0);
    _MarkRect(r.x0, r.y0, r.x1, r.y1);
}

/*********************************************************************
*
*       _FillPolygonAA
*/
static void _FillPolygonAA(const GUI_POINT *paPoint, int NumPoints, int x0, int y0)
{
#if GUI_SUPPORT_AA
    GUI_RECT r;
    _CalcPolyRect(&r, paPoint, NumPoints);
    GUI_MoveRect(&r, x0, y0);

    if (GUI_Context.AA_HiResEnable)
    {
        r.x0 /= GUI_Context.AA_Factor;
        r.y0 /= GUI_Context.AA_Factor;
        r.x1 = (r.x1 + GUI_Context.AA_Factor - 1) / GUI_Context.AA_Factor;
        r.y1 = (r.y1 + GUI_Context.AA_Factor - 1) / GUI_Context.AA_Factor;
    }

    _MarkRect(r.x0, r.y0, r.x1, r.y1);
#else
    GUI_USE_PARA(paPoint);
    GUI_USE_PARA(NumPoints);
    GUI_USE_PARA(x0);
    GUI_USE_PARA(y0);
#endif
}

/*********************************************************************
*
*             Device structure
*/
static const tLCDDEV_APIList _APIList =
{
    _Color2Index,
    _Index2Color,
    _GetIndexMask,
    (tLCDDEV_DrawBitmap *)_DrawBitmap,
    _DrawHLine,
    _DrawVLine,
    _FillRect,
    _GetPixelIndex,
    _GetRect,
    _SetPixelIndex,
    _XorPixel,
    NULL,
    _FillPolygon,
    _FillPolygonAA
};

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_MEASDEV_Delete
*/
void GUI_MEASDEV_Delete(GUI_MEASDEV_Handle hMemDev)
{
    /* Make sure memory device is not used */
    if ((GUI_Context.hDevData = hMemDev) != 0)
    {
        GUI_SelectLCD();
    }

    GUI_ALLOC_Free(hMemDev);
}

/*********************************************************************
*
*       GUI_MEASDEV_ClearRect
*/
void GUI_MEASDEV_ClearRect(GUI_MEASDEV_Handle hMemDev)
{
    if (hMemDev)
    {
        GUI_MEASDEV *pDevData;
        GUI_LOCK();
        pDevData = (GUI_MEASDEV *)GUI_ALLOC_h2p(hMemDev);
        pDevData->rUsed.x0 = GUI_XMAX;
        pDevData->rUsed.y0 = GUI_YMAX;
        pDevData->rUsed.x1 = GUI_XMIN;
        pDevData->rUsed.y1 = GUI_YMIN;
        GUI_UNLOCK();
    }
}

/*********************************************************************
*
*       GUI_MEASDEV_Create
*/
GUI_MEASDEV_Handle GUI_MEASDEV_Create(void)
{
    int MemSize;
    GUI_MEASDEV_Handle hMemDev;
    MemSize = sizeof(GUI_MEASDEV);
    hMemDev = GUI_ALLOC_AllocZero(MemSize);

    if (hMemDev)
    {
        GUI_MEASDEV *pDevData;
        GUI_LOCK();
        pDevData = (GUI_MEASDEV *)GUI_ALLOC_h2p(hMemDev);
        pDevData->pfColor2Index = GUI_Context.pDeviceAPI->pfColor2Index;
        pDevData->pfIndex2Color = GUI_Context.pDeviceAPI->pfIndex2Color;
        GUI_MEASDEV_ClearRect(hMemDev);
        GUI_UNLOCK();
    }
    else
    {
        GUI_DEBUG_WARN("GUI_MEASDEV_Create: Alloc failed");
    }

    return hMemDev;
}

/*********************************************************************
*
*       GUI_MEASDEV_Select
*/
void GUI_MEASDEV_Select(GUI_MEASDEV_Handle hMem)
{
    if (hMem == 0)
    {
        GUI_SelectLCD();
    }
    else
    {
#if GUI_WINSUPPORT
        WM_Deactivate();
#endif
        GUI_Context.hDevData     = hMem;
        GUI_Context.pDeviceAPI   = &_APIList;
        GUI_Context.pClipRect_HL[GUI_Context.SelLayer] = NULL;
        LCD_SetClipRectMax();
    }
}

/*********************************************************************
*
*       GUI_MEASDEV_GetRect
*/
void GUI_MEASDEV_GetRect(GUI_MEASDEV_Handle hMem, GUI_RECT *pRect)
{
    if (hMem)
    {
        GUI_MEASDEV *pDev;
        GUI_LOCK();
        pDev = (GUI_MEASDEV *)GUI_ALLOC_h2p(hMem);

        if (pRect)
        {
            pRect->x0 = pDev->rUsed.x0;
            pRect->y0 = pDev->rUsed.y0;
            pRect->x1 = pDev->rUsed.x1;
            pRect->y1 = pDev->rUsed.y1;
        }

        GUI_UNLOCK();
    }
}

#else

void GUIDEV_Measure(void) {} /* avoid empty object files */

#endif /* GUI_MEMDEV_SUPPORT */

/*************************** end of file ****************************/

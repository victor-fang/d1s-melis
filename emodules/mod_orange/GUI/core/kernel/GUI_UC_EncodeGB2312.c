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
#include "gui_core_kernel_private.h"
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetCharCode
*
*
*
*/
static U16 _GetCharCode(const char GUI_UNI_PTR *s)
{
    U16 r;

    if (esCHS_Char2Uni(EPDK_CHARSET_ENM_GBK, (__u8 *)s, 8, &r) <= 0)
    {
        r = 1;
    }

    return r;
}

/*********************************************************************
*
*       _GetCharSize
*
*
*
*/
static int _GetCharSize(const char GUI_UNI_PTR *s)
{
    U8 ch;
    ch = *s;

    if (ch > 0x80)
    {
        return 2;
    }
    else if (ch < 0x80)
    {
        return 1;
    }
    else
    {
        return 1;
    }
}
/*********************************************************************
*
*       _CalcSizeOfChar
*
*
*
*/
static int _CalcSizeOfChar(U16 Char)
{
    int r = 1;

    if ((Char >= 0x2e00) && (Char <= 0x9fff)) /* Single byte (ASCII)  */
    {
        r = 2;
    }
    else if (Char < 0x80)                /* Double byte sequence */
    {
        r = 1;
    }

    return r;
}

/*********************************************************************
*
*       _Encode
*
*
*/
static int _Encode(char *s, U16 Char)
{
    int r;
    r = esCHS_Uni2Char(EPDK_CHARSET_ENM_GBK, Char, (__u8 *)s, 3);

    if (r <= 0)
    {
        return 1;
    }

    return r;
}
/*********************************************************************
*
*       _API_Table
*/
static const GUI_UC_ENC_APILIST _API_Table =
{
    _GetCharCode,     /*  return character code as ASCII */
    _GetCharSize,     /*  return size of character        */
    _CalcSizeOfChar,  /*  return size of character       */
    _Encode           /*  Encode character into 1 or 2     */
};

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_UC_SetEncodeGB2312
*/
void GUI_UC_SetEncodeGB2312(void)
{
    GUI_LOCK();
    GUI_Context.pUC_API = &_API_Table;
    GUI_Context.charset_enum = EPDK_CHARSET_ENM_GB2312;
    GUI_UNLOCK();
}

/*************************** End of file ****************************/

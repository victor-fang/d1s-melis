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
#ifndef __SSET_MENU_TBAR_H__
#define __SSET_MENU_TBAR_H__


/*
*************************************************************************************
*                                       SSET_MenuTbar_Create
*
*Description: SSET子菜单创建
*
*Arguments  : hparent
*             pitem
*             lyrsta
*
*Return     : NULL
*
*************************************************************************************
*/
H_WIN SSET_MenuTbar_Create(H_WIN hparent, H_WIN hlayer, __sset_item_t *pitem);
/*
*************************************************************************************
*                                       SSET_MenuTbar_Destroy
*
*Description: SSET子菜单销毁
*
*Arguments  : h_win  : 要销毁的窗口句柄
*
*Return     : NULL
*
*************************************************************************************
*/
__s32 SSET_MenuTbar_Destroy(H_WIN h_win);
/*
************************************************************************************************************************
*                                       SSET_MenuTbar_SetFocus
*
*Description: 设置MENU的焦点
*
*Arguments  :
*
*Return     :
*
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_SetFocus(H_WIN h_win, __u32 fcs_id);
/*
************************************************************************************************************************
*                                       SSET_MenuTbar_UpdateItem
*
*Description:
*
*Arguments  :
*
*Return     :
*
************************************************************************************************************************
*/
__s32 SSET_MenuTbar_UpdateItem(H_WIN h_win, __u32 item_no, __u32 index, __u32 fcs_sta);  //..


#endif  /* __SSET_MENU_TBAR_H__ */

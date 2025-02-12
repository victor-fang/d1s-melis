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
#ifndef __APP_dtv_SETTINGS_I_H_
#define  __APP_dtv_SETTINGS_I_H_

#include "beetles_app.h"
#include "..//dtv_i.h"

#define dtv_SETTING_FOCUS_COLOR   GUI_BLUE
#define dtv_SETTING_UNFOCUS_COLOR  GUI_BLACK

//#define DTV_AUTO_SEARCH_ENABLE

enum
{
    //1、设置
    DTV_SETTING_ALL_FREQ_SEARCH_ID = 0,     //自动搜索,全频点
#ifdef DTV_AUTO_SEARCH_ENABLE
    DTV_SETTING_AUTO_SEARCH_ID,
#endif
    DTV_SETTING_MANU_SEARCH_ID,     //手动搜索,指定频搜台
    DTV_SETTING_DISPLAY_ID,    //显示设置
    DTV_SETTING_CA_ID,       //CA序列号
    DTV_SETTING_EPG,
    DTV_SETTING_WIRELESS_ID,    //天线选择
    DTV_SETTING_MAX,
    DTV_SETTING_TIME_ZONE_ID,    //时区设置
    DTV_SETTING_BACK2LIST,//从子项返回list
    DTV_SETTING_REPLAY,
    DTV_SETTING_2_MANU_SEARCH,   //手动搜台
    DTV_SETTING_SWITCH_PLAY
};
#endif

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
#ifndef  __DSK_WKM_FF_RR_SPEED_C__
#define  __DSK_WKM_FF_RR_SPEED_C__


#include "dsk_walkman_i.h"


/*
****************************************************************************************************
*Name        : dsk_wkm_set_ff_rr_speed
*Prototype   : __s32  dsk_wkm_set_ff_rr_speed( __u32 ff_rr_speed )
*Arguments   : ff_rr_speed   input. the speed of ff or rr. the speed rang is CEDAR_SLOWEST_FRSPEED~~
*                            ~~CEDAR_FASTEST_FRSPEED.
*Return      : ==0     succeed
*              !=0     fail
*Description : set the speed of ff or rr.
*Other       :
****************************************************************************************************
*/
__s32  dsk_wkm_set_ff_rr_speed(__u32 ff_rr_speed)
{
    __u8   err;
    __s32  ret;

    if (__dsk_wkm_get_open_state() == EPDK_FALSE)
    {
        __wrn("Warning! the walkman is NOT open.\n");
        return -1;
    }

    if (__dsk_wkm_get_playing_state() == EPDK_FALSE)
    {
        __wrn("Warning! the walkman is NOT playing.\n");
        return -1;
    }

    g_pend_mutex(dsk_wkm_res_mutex, &err);
    /* set speed */
    ret = __dsk_wkm_set_ff_rr_speed(ff_rr_speed);
    g_post_mutex(dsk_wkm_res_mutex);
    return ret;
}



/*
****************************************************************************************************
*Name        : dsk_wkm_get_ff_rr_speed
*Prototype   : __s32  dsk_wkm_get_ff_rr_speed( void )
*Arguments   : void
*Return      : the speed of ff or rr. -1 means error.
*Description : get the speed of ff or rr.
*Other       :
****************************************************************************************************
*/
__s32  dsk_wkm_get_ff_rr_speed(void)
{
    __s32  speed;
    __u8   err;

    if (__dsk_wkm_get_open_state() == EPDK_FALSE)
    {
        __wrn("Warning! the walkman is NOT open.\n");
        return -1;
    }

    if (__dsk_wkm_get_playing_state() == EPDK_FALSE)
    {
        __wrn("Warning! the walkman is NOT playing.\n");
        return -1;
    }

    g_pend_mutex(dsk_wkm_res_mutex, &err);
    /* get speed */
    speed = __dsk_wkm_get_ff_rr_speed();
    g_post_mutex(dsk_wkm_res_mutex);
    return speed;
}



#endif     //  ifndef __DSK_WKM_FF_RR_SPEED_C__

/* end of dsk_wkm_ff_rr_speed.c */

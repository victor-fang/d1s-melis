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

#ifndef __PLATFORM_CCMU_H__
#define __PLATFORM_CCMU_H__

#if defined(CONFIG_DRIVERS_SUNXI_CLK)
#include "./sunxi/platform_clk.h"
#if defined(CONFIG_SOC_SUN20IW3) || defined(CONFIG_ARCH_SUN8IW21)
#include "./sunxi/sun8iw21p1/clk_sun8iw21.h"
#endif
#endif

#if defined(CONFIG_DRIVERS_SUNXI_CCU)

#if defined(CONFIG_SOC_SUN20IW1) || defined(CONFIG_ARCH_SUN8IW20)
#include "./sunxi-ng/clk-fixed-rate.h"
#include "./sunxi-ng/ccu-sun8iw20.h"
#include "./sunxi-ng/ccu-sun8iw20-r.h"
#include "./sunxi-ng/ccu-sun8iw20-rtc.h"
#endif

#if defined(CONFIG_ARCH_SUN20IW2)
#include "./sunxi-ng/clk-fixed-rate.h"
#include "./sunxi-ng/ccu-sun20iw2.h"
#include "./sunxi-ng/ccu-sun20iw2-aon.h"
#include "./sunxi-ng/ccu-sun20iw2-r.h"
#endif

#if defined(CONFIG_ARCH_SUN55IW3)
#include "./sunxi-ng/clk-fixed-rate.h"
#include "./sunxi-ng/ccu-sun55iw3.h"
#include "./sunxi-ng/ccu-sun55iw3-dsp.h"
#include "./sunxi-ng/ccu-sun55iw3-r.h"
#endif

#endif
#endif /* __PLATFORM_CCMU_H__ */

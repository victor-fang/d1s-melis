/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/platform/platform_cfg.h
 *
 * Copyright (c) 2007-2017 Allwinnertech Co., Ltd.
 *
 * Authors:  Zhao Wei <zhaowei@allwinnertech.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __PLATFORM_CFG__H__
#define __PLATFORM_CFG__H__

/*#define FPGA_VER*/
#define SUNXI_MEM

#if defined (CONFIG_ARCH_SUN8IW15P1) || defined (CONFIG_ARCH_SUN8IW17P1) || defined (CONFIG_ARCH_SUN50IW3P1) || defined (CONFIG_ARCH_SUN50IW6P1)
#define NO_SUPPROT_CCU_PLATDORM
#define NO_SUPPROT_ISP_BRIDGE_PLATDORM
#define NO_SUPPROT_HARDWARE_CALCULATE
#else

#if defined CONFIG_ARCH_SUN8IW16P1 || defined CONFIG_SOC_SUN8IW19P1
#define ISP0_BRIDGE_VALID
#endif

#if defined(CONFIG_BUF_AUTO_UPDATE)
#define BUF_AUTO_UPDATE
#endif

#endif

#if defined (CONFIG_ARCH_SUN50IW10P1)
#if defined (CONFIG_SUPPORT_ISP_TDM)
#define SUPPORT_ISP_TDM
#endif
#endif

#if defined CONFIG_SENSOR_POWER  || defined CONFIG_SENSOR_POWER_MODULE
#define SENSOR_POER_BEFORE_VIN
#endif

#include "../utility/vin_os.h"
#include "../vin-mipi/combo_common.h"

#ifdef FPGA_VER
#define DPHY_CLK (48*1000*1000)
#else
#define DPHY_CLK (150*1000*1000)
#endif

enum isp_platform {
	ISP500 =  0,
	ISP520,
	ISP521,
	ISP_VERSION_NUM,
};

#if defined CONFIG_SOC_SUN8IW19P1
#include "sun8iw19p1_vin_cfg.h"
#elif defined CONFIG_SOC_SUN20IW1P1
#include "sun20iw1p1_vin_cfg.h"
#endif

#define MOV_ROUND_UP(x, n)	(((x) + (1 << (n)) - 1) >> (n))

struct mbus_framefmt_res {
	u32 res_pix_fmt;    //V4L2_PIX_FMT_NV21
	u32 res_mipi_bps;
	u8 res_combo_mode;
	u8 res_wdr_mode;    //enum isp_wdr_mode, ISP_NORMAL_MODE
	u8 res_time_hs;
	u8 res_lp_mode;
};

enum steam_on_seq {
	SENSOR_BEFORE_MIPI = 0,
	MIPI_BEFORE_SENSOR,
};

#define CSI_CH_0	(1 << 20)
#define CSI_CH_1	(1 << 21)
#define CSI_CH_2	(1 << 22)
#define CSI_CH_3	(1 << 23)

#define MAX_DETECT_NUM	3

/*
 * The subdevices' group IDs.
 */
#define VIN_GRP_ID_SENSOR	(1 << 8)
#define VIN_GRP_ID_MIPI		(1 << 9)
#define VIN_GRP_ID_CSI		(1 << 10)
#define VIN_GRP_ID_ISP		(1 << 11)
#define VIN_GRP_ID_SCALER	(1 << 12)
#define VIN_GRP_ID_CAPTURE	(1 << 13)
#define VIN_GRP_ID_STAT		(1 << 14)
#define VIN_GRP_ID_TDM_RX	(1 << 15)

#define VIN_ALIGN_WIDTH 16
#define VIN_ALIGN_HEIGHT 16

void *sunxi_vin_get_csi_base(int id);
void *sunxi_vin_get_scaler_base(int id);
void *sunxi_vin_get_isp_base(int id);
int sunxi_vin_get_isp_irq(int id);
void *sunxi_vin_get_mipi_base(int id);
void *sunxi_vin_get_csi_top_base(void);
void *sunxi_vin_get_csi_ccu_base(void);
int sunxi_vin_get_csi_top_irq(void);
void *sunxi_vin_get_dma_base(int id);
int sunxi_vin_get_dma_irq(int id);
int *sunxi_vin_get_csi_pins(int id);
int sunxi_vin_get_csi_pins_num(void);

#endif /*__PLATFORM_CFG__H__*/

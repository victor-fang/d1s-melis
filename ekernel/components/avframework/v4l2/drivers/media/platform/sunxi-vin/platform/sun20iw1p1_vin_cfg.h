 /*
  * Hawkview ISP - sun20iw1p1_vin_cfg.h module
  *
  * Copyright (c) 2020 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
  *
  * Authors:  Cao Fuming <caofuming@allwinnertech.com>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation.
  */

#ifndef _SUN20IW1P1_VIN_CFG_H_
#define _SUN20IW1P1_VIN_CFG_H_

#define HAL_CLK_UNINITIALIZED		-1

#define CSI_CCU_REGS_BASE			0x05800000
#define CSI_TOP_REGS_BASE			0x05800800

#define CSI0_REGS_BASE				0x05801000

#define ISP0_REGS_BASE				0x05809410
#define ISP1_REGS_BASE				0x05809420

#define VIPP0_REGS_BASE				0x05809430
#define VIPP1_REGS_BASE				0x05809440

#define CSI_DMA0_REG_BASE			0x05809000
#define CSI_DMA1_REG_BASE			0x05809200

#define GPIO_REGS_VBASE				0x02000000

#define SUNXI_IRQ_CSIC_DMA0	111
#define SUNXI_IRQ_CSIC_DMA1	112
#define SUNXI_IRQ_CSI_TOP_PKT 122

#define VIN_MAX_DEV			2
#define VIN_MAX_CSI			1
#define VIN_MAX_TDM			0
#define VIN_MAX_CCI			1
#define VIN_MAX_MIPI		0
#define VIN_MAX_ISP			2
#define VIN_MAX_SCALER		2

#define MAX_CH_NUM			4



#endif /*_SUN20IW1P1_VIN_CFG_H_*/


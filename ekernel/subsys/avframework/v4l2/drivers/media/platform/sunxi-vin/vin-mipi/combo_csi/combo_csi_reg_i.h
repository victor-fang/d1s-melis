/*
 * combo csi module
 *
 * Copyright (c) 2019 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Zheng Zequn <zequnzheng@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __COMBO_CSI_REG_I_H__
#define __COMBO_CSI_REG_I_H__

/*
 * Detail information of registers----PHY
 */
#define CMB_PHY_TOP_REG_OFF			0x000
#define CMB_PHY_PWDNZ				0
#define CMB_PHY_PWDNZ_MASK			(0x1 << CMB_PHY_PWDNZ)
#define CMB_PHY_RSTN				1
#define CMB_PHY_RSTN_MASK			(0x1 << CMB_PHY_RSTN)
#define CMB_PHY_VREF_EN				2
#define CMB_PHY_VREF_EN_MASK			(0x1 << CMB_PHY_VREF_EN)
#define CMB_PHY_LVLDO_EN			3
#define CMB_PHY_LVLDO_EN_MASK			(0x1 << CMB_PHY_LVLDO_EN)
#define CMB_PHY_VREF_OP2			8
#define CMB_PHY_VREF_OP2_MASK			(0x3 << CMB_PHY_VREF_OP2)
#define CMB_PHY_VREF_OP9			10
#define CMB_PHY_VREF_OP9_MASK			(0x3 << CMB_PHY_VREF_OP9)
#define CMB_PRBS_SEL				24
#define CMB_PRBS_SEL_MASK			(0x3 << CMB_PRBS_SEL)

/*
 * Detail information of registers----PHYA/B
 */
#define CMB_PHY_CTL_REG_OFF			0x100
#define CMB_PHY0_EN				0
#define CMB_PHY0_EN_MASK			(0x1 << CMB_PHY0_EN)
#define CMB_PHY1_EN				1
#define CMB_PHY1_EN_MASK			(0x1 << CMB_PHY1_EN)
#define CMB_PHY_LINK_MODE			2
#define CMB_PHY_LINK_MODE_MASK			(0x1 << CMB_PHY_LINK_MODE)
#define CMB_PHY_LANEDT_EN			4
#define CMB_PHY_LANEDT_EN_MASK			(0xf << CMB_PHY_LANEDT_EN)
#define CMB_PHY_LANECK_EN			8
#define CMB_PHY_LANECK_EN_MASK			(0x3 << CMB_PHY_LANECK_EN)
#define CMB_PHY0_CK_SEL				10
#define CMB_PHY0_CK_SEL_MASK			(0x1 << CMB_PHY0_CK_SEL)
#define CMB_PHY1_CK_SEL				11
#define CMB_PHY1_CK_SEL_MASK			(0x1 << CMB_PHY1_CK_SEL)
#define CMB_PHY0_LP_PEFI			12
#define CMB_PHY0_LP_PEFI_MASK			(0x1 << CMB_PHY0_LP_PEFI)
#define CMB_PHY1_LP_PEFI			13
#define CMB_PHY1_LP_PEFI_MASK			(0x1 << CMB_PHY1_LP_PEFI)
#define CMB_PHY0_HS_PEFI			16
#define CMB_PHY0_HS_PEFI_MASK			(0x7 << CMB_PHY0_HS_PEFI)
#define CMB_PHY0_VCM				19
#define CMB_PHY0_VCM_MASK			(0x1 << CMB_PHY0_VCM)
#define CMB_PHY1_HS_PEFI			20
#define CMB_PHY1_HS_PEFI_MASK			(0x7 << CMB_PHY1_HS_PEFI)
#define CMB_PHY1_VCM				23
#define CMB_PHY1_VCM_MASK			(0x1 << CMB_PHY1_VCM)
#define CMB_PHY0_IBIAS_EN			26
#define CMB_PHY0_IBIAS_EN_MASK			(0x1 << CMB_PHY0_IBIAS_EN)
#define CMB_PHY1_IBIAS_EN			27
#define CMB_PHY1_IBIAS_EN_MASK			(0x1 << CMB_PHY1_IBIAS_EN)
#define CMB_PHY0_WORK_MODE			28
#define CMB_PHY0_WORK_MODE_MASK			(0x3 << CMB_PHY0_WORK_MODE)
#define CMB_PHY1_WORK_MODE			30
#define CMB_PHY1_WORK_MODE_MASK			(0x3 << CMB_PHY1_WORK_MODE)

#define CMB_PHY_TERM_CTL_REG_OFF		0x120
#define CMB_PHY_TERMDT_EN			0
#define CMB_PHY_TERMDT_EN_MASK			(0xf << CMB_PHY_TERMDT_EN)
#define CMB_PHY_TERMCK_EN			4
#define CMB_PHY_TERMCK_EN_MASK			(0x3 << CMB_PHY_TERMCK_EN)
#define CMB_PHY0_TERM_EN_DLY			16
#define CMB_PHY0_TERM_EN_DLY_MASK		(0xff << CMB_PHY0_TERM_EN_DLY)
#define CMB_PHY1_TERM_EN_DLY			24
#define CMB_PHY1_TERM_EN_DLY_MASK		(0xff << CMB_PHY1_TERM_EN_DLY)

#define CMB_PHY_HS_CTL_REG_OFF			0x124
#define CMB_PHY_HSDT_EN				0
#define CMB_PHY_HSDT_EN_MASK			(0xf << CMB_PHY_HSDT_EN)
#define CMB_PHY_HSCK_EN				4
#define CMB_PHY_HSCK_EN_MASK			(0x3 << CMB_PHY_HSCK_EN)
#define CMB_PHY_HSDT_POLAR			8
#define CMB_PHY_HSDT_POLAR_MASK			(0xf << CMB_PHY_HSDT_POLAR)
#define CMB_PHY_HSCK_POLAR			12
#define CMB_PHY_HSCK_POLAR_MASK			(0xf << CMB_PHY_HSCK_POLAR)
#define CMB_PHY0_HS_DLY				16
#define CMB_PHY0_HS_DLY_MASK			(0xff << CMB_PHY0_HS_DLY)
#define CMB_PHY1_HS_DLY				24
#define CMB_PHY1_HS_DLY_MASK			(0xff << CMB_PHY1_HS_DLY)

#define CMB_PHY_S2P_CTL_REG_OFF			0x128
#define CMB_PHY_S2P_EN				0
#define CMB_PHY_S2P_EN_MASK			(0xf << CMB_PHY_S2P_EN)
#define CMB_PHY0_S2P_WIDTH			8
#define CMB_PHY0_S2P_WIDTH_MASK			(0x3 << CMB_PHY0_S2P_WIDTH)
#define CMB_PHY1_S2P_WIDTH			10
#define CMB_PHY1_S2P_WIDTH_MASK			(0x3 << CMB_PHY1_S2P_WIDTH)
#define CMB_PHY0_S2P_DLY			16
#define CMB_PHY0_S2P_DLY_MASK			(0xff << CMB_PHY0_S2P_DLY)
#define CMB_PHY1_S2P_DLY			24
#define CMB_PHY1_S2P_DLY_MASK			(0xff << CMB_PHY1_S2P_DLY)

#define CMB_PHY_MIPIRX_CTL_REG_OFF		0x12C
#define CMB_PHY0_MIPIHS_ENDLAN			0
#define CMB_PHY0_MIPIHS_ENDLAN_MASK		(0x1 << CMB_PHY0_MIPIHS_ENDLAN)
#define CMB_PHY1_MIPIHS_ENDLAN			1
#define CMB_PHY1_MIPIHS_ENDLAN_MASK		(0x1 << CMB_PHY1_MIPIHS_ENDLAN)
#define CMB_PHY0_MIPIHS_SYNC_MODE		2
#define CMB_PHY0_MIPIHS_SYNC_MODE_MASK		(0x1 << CMB_PHY0_MIPIHS_SYNC_MODE)
#define CMB_PHY1_MIPIHS_SYNC_MODE		3
#define CMB_PHY1_MIPIHS_SYNC_MODE_MASK		(0x1 << CMB_PHY1_MIPIHS_SYNC_MODE)
#define CMB_PHY0_MIPIHS_8B9B			4
#define CMB_PHY0_MIPIHS_8B9B_MASK		(0x1 << CMB_PHY0_MIPIHS_8B9B)
#define CMB_PHY1_MIPIHS_8B9B			5
#define CMB_PHY1_MIPIHS_8B9B_MASK		(0x1 << CMB_PHY1_MIPIHS_8B9B)
#define CMB_PHY_MIPI_MPDT_EN			8
#define CMB_PHY_MIPI_MPDT_EN_MASK		(0xf << CMB_PHY_MIPI_MPDT_EN)
#define CMB_PHY_MIPI_LPCK_EN			12
#define CMB_PHY_MIPI_LPCK_EN_MASK		(0x3 << CMB_PHY_MIPI_LPCK_EN)
#define CMB_PHY0_MIPILP_DBC_EN			16
#define CMB_PHY0_MIPILP_DBC_EN_MASK		(0x1 << CMB_PHY0_MIPILP_DBC_EN)
#define CMB_PHY1_MIPILP_DBC_EN			17
#define CMB_PHY1_MIPILP_DBC_EN_MASK		(0x1 << CMB_PHY1_MIPILP_DBC_EN)

/*
 * Detail information of registers----PORT0/1
 */
#define CMB_PORT_CTL_REG_OFF			0x1000
#define CMB_PORT_EN				0
#define CMB_PORT_EN_MASK			(0x1 << CMB_PORT_EN)
#define CMB_PORT_WORK_MODE			4
#define CMB_PORT_WORK_MODE_MASK			(0x3 << CMB_PORT_WORK_MODE)
#define CMB_PORT_LANE_NUM			8
#define CMB_PORT_LANE_NUM_MASK			(0xf << CMB_PORT_LANE_NUM)
#define CMB_PORT_CHANNEL_NUM			16
#define CMB_PORT_CHANNEL_NUM_MASK		(0x3 << CMB_PORT_CHANNEL_NUM)
#define CMB_PORT_OUT_NUM			31
#define CMB_PORT_OUT_NUM_MASK			(0x1 << CMB_PORT_OUT_NUM)

#define CMB_PORT_LANE_MAP_REG0_OFF		0x1004
#define CMB_PORT_LANE0_ID			0
#define CMB_PORT_LANE0_ID_MASK			(0xf << CMB_PORT_LANE0_ID)
#define CMB_PORT_LANE1_ID			4
#define CMB_PORT_LANE1_ID_MASK			(0xf << CMB_PORT_LANE1_ID)
#define CMB_PORT_LANE2_ID			8
#define CMB_PORT_LANE2_ID_MASK			(0xf << CMB_PORT_LANE2_ID)
#define CMB_PORT_LANE3_ID			12
#define CMB_PORT_LANE3_ID_MASK			(0xf << CMB_PORT_LANE3_ID)
#define CMB_PORT_LANE4_ID			16
#define CMB_PORT_LANE4_ID_MASK			(0xf << CMB_PORT_LANE4_ID)
#define CMB_PORT_LANE5_ID			20
#define CMB_PORT_LANE5_ID_MASK			(0xf << CMB_PORT_LANE5_ID)
#define CMB_PORT_LANE6_ID			24
#define CMB_PORT_LANE6_ID_MASK			(0xf << CMB_PORT_LANE6_ID)
#define CMB_PORT_LANE7_ID			28
#define CMB_PORT_LANE7_ID_MASK			(0xf << CMB_PORT_LANE7_ID)

#define CMB_PORT_LANE_MAP_REG1_OFF		0x1008
#define CMB_PORT_LANE8_ID			0
#define CMB_PORT_LANE8_ID_MASK			(0xf << CMB_PORT_LANE8_ID)
#define CMB_PORT_LANE9_ID			4
#define CMB_PORT_LANE9_ID_MASK			(0xf << CMB_PORT_LANE9_ID)
#define CMB_PORT_LANE10_ID			8
#define CMB_PORT_LANE10_ID_MASK			(0xf << CMB_PORT_LANE10_ID)
#define CMB_PORT_LANE11_ID			12
#define CMB_PORT_LANE11_ID_MASK			(0xf << CMB_PORT_LANE11_ID)

#define CMB_PORT_MIPI_CFG_REG_OFF		0x1100
#define CMB_MIPI_UNPACK_EN			0
#define CMB_MIPI_UNPACK_EN_MASK			(0x1 << CMB_MIPI_UNPACK_EN)
#define CMB_MIPI_NO_UNPACK_ALL			1
#define CMB_MIPI_NO_UNPACK_ALL_MASK		(0x1 << CMB_MIPI_NO_UNPACK_ALL)
#define CMB_EMBED_EN				2
#define CMB_EMBED_EN_MASK			(0x1 << CMB_EMBED_EN)
#define CMB_MIPI_USER_DEF_EN			3
#define CMB_MIPI_USER_DEF_EN_MASK		(0x1 << CMB_MIPI_USER_DEF_EN)
#define CMB_MIPI_PH_BYTEORD			4
#define CMB_MIPI_PH_BYTEORD_MASK		(0x3 << CMB_MIPI_PH_BYTEORD)
#define CMB_MIPI_PH_BITOED			6
#define CMB_MIPI_PH_BITOED_MASK			(0x1 << CMB_MIPI_PH_BITOED)
#define CMB_MIPI_PL_BITORD			7
#define CMB_MIPI_PL_BITORD_MASK			(0x1 << CMB_MIPI_PL_BITORD)
#define CMB_MIPI_LINE_SYNC_EN			8
#define CMB_MIPI_LINE_SYNC_EN_MASK		(0x1 << CMB_MIPI_LINE_SYNC_EN)
#define CMB_MIPI_YUV_SEQ			16
#define CMB_MIPI_YUV_SEQ_MASK			(0x3 << CMB_MIPI_YUV_SEQ)

#define CMB_PORT_MIPI_DI_REG_OFF		0x1108
#define CMB_MIPI_CH0_DT				0
#define CMB_MIPI_CH0_DT_MASK			(0x3f << CMB_MIPI_CH0_DT)
#define CMB_MIPI_CH0_VC				6
#define CMB_MIPI_CH0_VC_MASK			(0x3 << CMB_MIPI_CH0_VC)
#define CMB_MIPI_CH1_DT				8
#define CMB_MIPI_CH1_DT_MASK			(0x3f << CMB_MIPI_CH1_DT)
#define CMB_MIPI_CH1_VC				14
#define CMB_MIPI_CH1_VC_MASK			(0x3 << CMB_MIPI_CH1_VC)
#define CMB_MIPI_CH2_DT				16
#define CMB_MIPI_CH2_DT_MASK			(0x3f << CMB_MIPI_CH2_DT)
#define CMB_MIPI_CH2_VC				22
#define CMB_MIPI_CH2_VC_MASK			(0x3 << CMB_MIPI_CH2_VC)
#define CMB_MIPI_CH3_DT				24
#define CMB_MIPI_CH3_DT_MASK			(0x3f << CMB_MIPI_CH3_DT)
#define CMB_MIPI_CH3_VC				30
#define CMB_MIPI_CH3_VC_MASK			(0x3 << CMB_MIPI_CH3_VC)

#define CMB_PORT_MIPI_DI_TRIG_REG_OFF		0x1110
#define CMB_MIPI_FS				0
#define CMB_MIPI_FS_MASK			(0x1 << CMB_MIPI_FS)
#define CMB_MIPI_FE				1
#define CMB_MIPI_FE_MASK			(0x1 << CMB_MIPI_FE)
#define CMB_MIPI_LS				2
#define CMB_MIPI_LS_MASK			(0x1 << CMB_MIPI_LS)
#define CMB_MIPI_LE				3
#define CMB_MIPI_LE_MASK			(0x1 << CMB_MIPI_LE)
#define CMB_MIPI_GS0				8
#define CMB_MIPI_GS0_MASK			(0x1 << CMB_MIPI_GS0)
#define CMB_MIPI_GS1				9
#define CMB_MIPI_GS1_MASK			(0x1 << CMB_MIPI_GS1)
#define CMB_MIPI_GS2				10
#define CMB_MIPI_GS2_MASK			(0x1 << CMB_MIPI_GS2)
#define CMB_MIPI_GS3				11
#define CMB_MIPI_GS3_MASK			(0x1 << CMB_MIPI_GS3)
#define CMB_MIPI_GS4				12
#define CMB_MIPI_GS4_MASK			(0x1 << CMB_MIPI_GS4)
#define CMB_MIPI_GS5				13
#define CMB_MIPI_GS5_MASK			(0x1 << CMB_MIPI_GS5)
#define CMB_MIPI_GS6				14
#define CMB_MIPI_GS6_MASK			(0x1 << CMB_MIPI_GS6)
#define CMB_MIPI_GS7				15
#define CMB_MIPI_GS7_MASK			(0x1 << CMB_MIPI_GS7)
#define CMB_MIPI_GL				16
#define CMB_MIPI_GL_MASK			(0x1 << CMB_MIPI_GL)
#define CMB_MIPI_YUV				17
#define CMB_MIPI_YUV_MASK			(0x1 << CMB_MIPI_YUV)
#define CMB_MIPI_RGB				18
#define CMB_MIPI_RGB_MASK			(0x1 << CMB_MIPI_RGB)
#define CMB_MIPI_RAW				19
#define CMB_MIPI_RAW_MASK			(0x1 << CMB_MIPI_RAW)
#define CMB_MIPI_SRC_IS_FIELD			30
#define CMB_MIPI_SRC_IS_FIELD_MASK		(0x1 << CMB_MIPI_SRC_IS_FIELD)
#define CMB_MIPI_FIELD_REV			31
#define CMB_MIPI_FIELD_REV_MASK			(0x1 << CMB_MIPI_FIELD_REV)

#endif
/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-cci/csi_cci_reg_i.h
 *
 * Copyright (c) 2007-2017 Allwinnertech Co., Ltd.
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


/*
 ******************************************************************************
 *
 * csi_cci_reg_i.h
 *
 * Hawkview ISP - csi_cci_reg_i.h module
 *
 * Copyright (c) 2014 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version         Author         Date        Description
 *
 *   2.0        Yang Feng     2014/07/15    Second Version
 *
 ******************************************************************************
 */

#ifndef _CSI_CCI_REG_I_H_
#define _CSI_CCI_REG_I_H_
/*
 * Detail information of registers
 */
#define CCI_CTRL_OFF              0x0000

#define CCI_CTRL_CCI_EN           0
#define CCI_CTRL_SOFT_RESET       1
#define CCI_CTRL_CCI_STA          16
#define CCI_CTRL_TRAN_RESULT      24
#define CCI_CTRL_READ_TRAN_MODE   28
#define CCI_CTRL_RESTART_MODE     29
#define CCI_CTRL_REPEAT_TRAN      30
#define CCI_CTRL_SINGLE_TRAN      31

#define CCI_CFG_OFF               0x0004
#define CCI_CFG_CSI_TRIG          0
#define CCI_CFG_CSI_TRIG_MASK    (0xf << CCI_CFG_CSI_TRIG)
#define CCI_CFG_TRIG_MODE         4
#define CCI_CFG_TRIG_MODE_MASK   (0x7 << CCI_CFG_TRIG_MODE)
#define CCI_CFG_SRC_SEL           7
#define CCI_CFG_SRC_SEL_MASK     (0x1 << CCI_CFG_SRC_SEL)
#define CCI_CFG_PACKET_MODE       15
#define CCI_CFG_PACKET_MODE_MASK (0x1 << CCI_CFG_PACKET_MODE)
#define CCI_CFG_INTERVAL          16
#define CCI_CFG_INTERVAL_MASK    (0xff << CCI_CFG_INTERVAL)
#define CCI_CFG_TIMEOUT_N         24
#define CCI_CFG_TIMEOUT_N_MASK   (0xff << CCI_CFG_TIMEOUT_N)

#define CCI_FMT_OFF               0x0008
#define CCI_FMT_PACKET_CNT        0
#define CCI_FMT_PACKET_CNT_MASK   (0xffff <<  CCI_FMT_PACKET_CNT)
#define CCI_FMT_DATA_BYTE         16
#define CCI_FMT_DATA_BYTE_MASK   (0xf << CCI_FMT_DATA_BYTE)
#define CCI_FMT_ADDR_BYTE         20
#define CCI_FMT_ADDR_BYTE_MASK   (0xf << CCI_FMT_ADDR_BYTE)
#define CCI_FMT_CMD               24
#define CCI_FMT_CMD_MASK         (0x1 << CCI_FMT_CMD)
#define CCI_FMT_SLV_ID            25
#define CCI_FMT_SLV_ID_MASK      (0x7f << CCI_FMT_SLV_ID)

#define CCI_BUS_CTRL_OFF           0x000C
#define CCI_BUS_CTRL_SDA_MOE       0
#define CCI_BUS_CTRL_SCL_MOE       1
#define CCI_BUS_CTRL_SDA_MOV       2
#define CCI_BUS_CTRL_SCL_MOV       3
#define CCI_BUS_CTRL_SDA_PEN       4
#define CCI_BUS_CTRL_SCL_PEN       5
#define CCI_BUS_CTRL_SDA_STA       6
#define CCI_BUS_CTRL_SCL_STA       7
#define CCI_BUS_CTRL_CLK_M         8
#define CCI_BUS_CTRL_CLK_M_MASK   (0xF << CCI_BUS_CTRL_CLK_M)
#define CCI_BUS_CTRL_CLK_N         12
#define CCI_BUS_CTRL_CLK_N_MASK   (0x7 << CCI_BUS_CTRL_CLK_N)
#define CCI_BUS_CTRL_DLY_TRIG      15
#define CCI_BUS_CTRL_DLY_CYC       16
#define CCI_BUS_CTRL_DLY_CYC_MASK (0xffff << CCI_BUS_CTRL_DLY_CYC)

#define CCI_INT_CTRL_OFF           0x0014
#define CCI_INT_CTRL_S_TRAN_COM_PD 0
#define CCI_INT_CTRL_S_TRAN_ERR_PD 1
#define CCI_INT_CTRL_RES0          2
#define CCI_INT_CTRL_S_TRAN_COM_INT_EN  16
#define CCI_INT_CTRL_S_TRAN_ERR_INT_EN  17
#define CCI_INT_CTRL_RES1               18

#define CCI_LC_TRIG_OFF                 0x0018
#define CCI_LC_TRIG_LN_CNT              0
#define CCI_LC_TRIG_LN_CNT_MASK        (0x1fff << CCI_LC_TRIG_LN_CNT)
#define CCI_LC_TRIG_RES0                13

#define CCI_FIFO_ACC_OFF                0x0100
#define CCI_FIFO_ACC_DATA_FIFO          0

#define CCI_RSV_REG_OFF                 0x0200

#endif /*_CSI_CCI_REG_I_H_*/

/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-csi/parser_reg_i.h
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

#ifndef __CSIC__PARSER__REG__I__H__
#define __CSIC__PARSER__REG__I__H__

/*
 * Detail information of registers
 */
#define PRS_EN_REG_OFF              0X000
#define PRS_EN_REG_PRS_EN           0
#define PRS_EN_REG_PRS_EN_MASK     (0X1 << PRS_EN_REG_PRS_EN)
#define PRS_EN_REG_PRS_MODE         1
#define PRS_EN_REG_PRS_MODE_MASK   (0X1 << PRS_EN_REG_PRS_MODE)
#define PRS_CH_PRS_MODE       	    2
#define PRS_CH_PRS_MODE_MASK       (0X1 << PRS_CH_PRS_MODE)
#define PRS_EN_REG_PCLK_EN          15
#define PRS_EN_REG_PCLK_EN_MASK    (0X1 << PRS_EN_REG_PCLK_EN)
#define PRS_EN_REG_NCSIC_EN         16
#define PRS_EN_REG_NCSIC_EN_MASK   (0X1 << PRS_EN_REG_NCSIC_EN)
#define PRS_EN_REG_MCSIC_EN         31
#define PRS_EN_REG_MCSIC_EN_MASK   (0X1 << PRS_EN_REG_MCSIC_EN)


#define PRS_NCSIC_IF_CFG_REG_OFF       0X004
#define PRS_NCSIC_IF_CSI_IF            0
#define PRS_NCSIC_IF_CSI_IF_MASK      (0X1F << PRS_NCSIC_IF_CSI_IF)
#define PRS_NCSIC_IF_OUTPUT_MODE       5
#define PRS_NCSIC_IF_OUTPUT_MODE_MASK (0X1 << PRS_NCSIC_IF_OUTPUT_MODE)
#define PRS_NCSIC_IF_INPUT_SEQ         6
#define PRS_NCSIC_IF_INPUT_SEQ_MASK   (0X3 << PRS_NCSIC_IF_INPUT_SEQ)
#define PRS_NCSIC_IF_DATA_WIDTH        8
#define PRS_NCSIC_IF_DATA_WIDTH_MASK  (0X7 << PRS_NCSIC_IF_DATA_WIDTH)
#define PRS_NCSIC_IF_SEQ_8PLUS2        11
#define PRS_NCSIC_IF_SEQ_8PLUS2_MASK  (0X3 << PRS_NCSIC_IF_SEQ_8PLUS2)
#define PRS_NCSIC_IF_DDR_SAMPLE_MODE   13
#define PRS_NCSIC_IF_DDR_SAMPLE_MODE_MASK (0X1 << PRS_NCSIC_IF_DDR_SAMPLE_MODE)
#define PRS_NCSIC_IF_FIELD_DT          14
#define PRS_NCSIC_IF_FIELD_DT_MASK    (0X3 << PRS_NCSIC_IF_FIELD_DT)
#define PRS_NCSIC_IF_CLK_POL           16
#define PRS_NCSIC_IF_CLK_POL_MASK     (0X1 << PRS_NCSIC_IF_CLK_POL)
#define PRS_NCSIC_IF_HREF_POL          17
#define PRS_NCSIC_IF_HREF_POL_MASK    (0X1 << PRS_NCSIC_IF_HREF_POL)
#define PRS_NCSIC_IF_VREF_POL          18
#define PRS_NCSIC_IF_VREF_POL_MASK    (0X1 << PRS_NCSIC_IF_VREF_POL)
#define PRS_NCSIC_IF_FIELD_POL         19
#define PRS_NCSIC_IF_FIELD_POL_MASK   (0X1 << PRS_NCSIC_IF_FIELD_POL)
#define PRS_NCSIC_IF_RES0              20
#define PRS_NCSIC_IF_SRC_TYPE_BASE     20
#define PRS_NCSIC_IF_SRC_TYPE_CH0      20
#define PRS_NCSIC_IF_SRC_TYPE_CH1      21
#define PRS_NCSIC_IF_SRC_TYPE_CH2      22
#define PRS_NCSIC_IF_SRC_TYPE_CH3      23
#define PRS_NCSIC_IF_SRC_TYPE_MASK    ((0X1 << PRS_NCSIC_IF_SRC_TYPE_CH0) || \
									(0X1 << PRS_NCSIC_IF_SRC_TYPE_CH1) || \
									(0X1 << PRS_NCSIC_IF_SRC_TYPE_CH2) || \
									(0X1 << PRS_NCSIC_IF_SRC_TYPE_CH3))
#define PRS_NCSIC_IF_PCLK_SHIFT        24
#define PRS_NCSIC_IF_PCLK_SHIFT_MASK  (0XF << PRS_NCSIC_IF_PCLK_SHIFT)

#define PRS_MCSIC_IF_CFG_REG_OFF        0X008
#define PRS_MCSIC_IF_OUTPUT_MODE        5
#define PRS_MCSIC_IF_OUTPUT_MODE_MASK  (0X1 << PRS_MCSIC_IF_OUTPUT_MODE)
#define PRS_MCSIC_IF_INPUT_SEQ          6
#define PRS_MCSIC_IF_INPUT_SEQ_MASK    (0X3 << PRS_MCSIC_IF_INPUT_SEQ)

#define PRS_CAP_REG_OFF         0X00C
#define CH0_SCAP_ON             0
#define CH0_VCAP_ON             1
#define CH0_SV_ON_MASK         (0X3 << CH0_SCAP_ON)
#define CH0_FPS_DS              2
#define CH0_FPS_DS_MASK        (0XF << CH0_FPS_DS)
#define CH1_SCAP_ON             8
#define CH1_VCAP_ON             9
#define CH1_SV_ON_MASK         (0X3 << CH1_SCAP_ON)
#define CH1_FPS_DS              10
#define CH1_FPS_DS_MASK        (0XF << CH1_FPS_DS)
#define CH2_SCAP_ON             16
#define CH2_VCAP_ON             17
#define CH2_SV_ON_MASK         (0X3 << CH2_SCAP_ON)
#define CH2_FPS_DS              18
#define CH2_FPS_DS_MASK        (0XF << CH2_FPS_DS)
#define CH3_SCAP_ON             24
#define CH3_VCAP_ON             25
#define CH3_SV_ON_MASK         (0X3 << CH3_SCAP_ON)
#define CH3_FPS_DS             26
#define CH3_FPS_DS_MASK        (0XF << CH3_FPS_DS)

#define PRS_SIGNAL_STA_REG_OFF   0X010
#define DATA_STA                 0
#define DATA_STA_MASK           (0XFFFFFF << DATA_STA)
#define PCLK_STA                 24
#define PCLK_STA_MASK           (0XF << PCLK_STA)

#define PRS_NCSIC_BT656_HEAD_CFG_REG_OFF   0X014
#define CH0_ID                  0
#define CH0_ID_MASK            (0XF << CH0_ID)
#define CH1_ID                  8
#define CH1_ID_MASK            (0XF << CH1_ID)
#define CH2_ID                  16
#define CH2_ID_MASK            (0XF << CH2_ID)
#define CH3_ID                  24
#define CH3_ID_MASK            (0XF << CH3_ID)

/***channel 0***/
#define PARSER_CH_OFF           0x100

#define PRS_CH0_INFMT_REG_OFF   0X024
#define PRS_CH0_INPUT_FMT       0
#define PRS_CH0_INPUT_FMT_MASK (0XF << PRS_CH0_INPUT_FMT)

#define PRS_CH0_OUTPUT_HSIZE_REG_OFF   0X028
#define PRS_CH0_HOR_START              0
#define PRS_CH0_HOR_START_MASK        (0X1FFF << PRS_CH0_HOR_START)
#define PRS_CH0_HOR_LEN                16
#define PRS_CH0_HOR_LEN_MASK          (0X1FFF << PRS_CH0_HOR_LEN)

#define PRS_CH0_OUTPUT_VSIZE_REG_OFF   0X02C
#define PRS_CH0_VER_START              0
#define PRS_CH0_VER_START_MASK        (0X1FFF << PRS_CH0_VER_START)
#define PRS_CH0_VER_LEN                16
#define PRS_CH0_VER_LEN_MASK          (0X1FFF << PRS_CH0_VER_LEN)

#define PRS_CH0_INPUT_PARA0_REG_OFF    0X030
#define PRS_CH0_INPUT_SRC_TYPE         0
#define PRS_CH0_INPUT_SRC_TYPE_MASK   (0X1 << PRS_CH0_INPUT_SRC_TYPE)

#define PRS_CH0_INPUT_PARA1_REG_OFF    0X034
#define PRS_CH0_INPUT_HT               0
#define PRS_CH0_INPUT_HT_MASK         (0X3FFF << PRS_CH0_INPUT_HT)
#define PRS_CH0_INPUT_VT               16
#define PRS_CH0_INPUT_VT_MASK         (0X3FFF << PRS_CH0_INPUT_VT)

#define PRS_CH0_INPUT_PARA2_REG_OFF    0X038
#define PRS_CH0_INPUT_HB               0
#define PRS_CH0_INPUT_HB_MASK         (0X3FFF << PRS_CH0_INPUT_HB)
#define PRS_CH0_INPUT_VB               16
#define PRS_CH0_INPUT_VB_MASK         (0X3FFF << PRS_CH0_INPUT_VB)

#define PRS_CH0_INPUT_PARA3_REG_OFF   0X03C
#define PRS_CH0_INPUT_X               0
#define PRS_CH0_INPUT_X_MASK         (0X3FFF << PRS_CH0_INPUT_X)
#define PRS_CH0_INPUT_Y               16
#define PRS_CH0_INPUT_Y_MASK         (0X3FFF << PRS_CH0_INPUT_Y)

#define PRS_CH0_INT_EN_REG_OFF           0X040
#define PRS_CH0_INPUT_PARA0_INT_EN       0
#define PRS_CH0_INPUT_PARA0_INT_EN_MASK (0X1 << PRS_CH0_INPUT_PARA0_INT_EN)

#define PRS_CH0_INPUT_PARA1_INT_EN       1
#define PRS_CH0_INPUT_PARA1_INT_EN_MASK (0X1 << PRS_CH0_INPUT_PARA1_INT_EN)

#define PRS_CH0_MUL_ERR_INT_EN           2
#define PRS_CH0_MUL_ERR_INT_EN_MASK     (0X1 << PRS_CH0_MUL_ERR_INT_EN)

#define PRS_CH0_INT_STA_REG_OFF          0X044
#define PRS_CH0_INPUT_SRC_PD0            0
#define PRS_CH0_INPUT_SRC_PD0_MASK      (0X1 << PRS_CH0_INPUT_SRC_PD0)

#define PRS_CH0_INPUT_SRC_PD1            1
#define PRS_CH0_INPUT_SRC_PD1_MASK      (0X1 << PRS_CH0_INPUT_SRC_PD1)

#define PRS_CH0_MUL_ERR_PD               2
#define PRS_CH0_MUL_ERR_PD_MASK         (0X1 << PRS_CH0_MUL_ERR_PD)

#define PRS_CH0_LINE_TIME_REG_OFF	0X048
#define PRS_CH0_INPUT_HSYN		0
#define PRS_CH0_INPUT_HSYN_MASK 	(0XFFFF << PRS_CH0_INPUT_HSYN)
#define PRS_CH0_INPUT_HBLK		16
#define PRS_CH0_INPUT_HBLK_MASK 	(0XFFFF << PRS_CH0_INPUT_HBLK)

#define PRS_NCSIC_RX_SIGNAL0_DLY_ADJ_REG_OFF  0X500
#define PRS_PCLK_DLY                0
#define PRS_PCLK_DLY_MASK          (0X1F << PRS_PCLK_DLY)
#define PRS_HSYNC_DLY               8
#define PRS_HSYNC_DLY_MASK         (0X1F << PRS_HSYNC_DLY)
#define PRS_VSYNC_DLY               16
#define PRS_VSYNC_DLY_MASK         (0X1F << PRS_VSYNC_DLY)
#define PRS_FILED_DLY               24
#define PRS_FILED_DLY_MASK         (0X1F << PRS_FILED_DLY)

#define PRS_NCSIC_RX_SIGNAL1_DLY_ADJ_REG_OFF  0X504
#define PRS_D20_DLY                 0
#define PRS_D20_DLY_MASK           (0X1F << PRS_D20_DLY)
#define PRS_D21_DLY                 8
#define PRS_D21_DLY_MASK           (0X1F << PRS_D21_DLY)
#define PRS_D22_DLY                 16
#define PRS_D22_DLY_MASK           (0X1F << PRS_D22_DLY)
#define PRS_D23_DLY                 24
#define PRS_D23_DLY_MASK           (0X1F << PRS_D23_DLY)

#define PRS_NCSIC_RX_SIGNAL2_DLY_ADJ_REG_OFF  0X508
#define PRS_D16_DLY                 0
#define PRS_D16_DLY_MASK           (0X1F << PRS_D16_DLY)
#define PRS_D17_DLY                 8
#define PRS_D17_DLY_MASK           (0X1F << PRS_D17_DLY)
#define PRS_D18_DLY                 16
#define PRS_D18_DLY_MASK           (0X1F << PRS_D18_DLY)
#define PRS_D19_DLY                 24
#define PRS_D19_DLY_MASK           (0X1F << PRS_D19_DLY)

#define PRS_NCSIC_RX_SIGNAL3_DLY_ADJ_REG_OFF  0X50C
#define PRS_D12_DLY                 0
#define PRS_D12_DLY_MASK           (0X1F << PRS_D12_DLY)
#define PRS_D13_DLY                 8
#define PRS_D13_DLY_MASK           (0X1F << PRS_D13_DLY)
#define PRS_D14_DLY                 16
#define PRS_D14_DLY_MASK           (0X1F << PRS_D14_DLY)
#define PRS_D15_DLY                 24
#define PRS_D15_DLY_MASK           (0X1F << PRS_D15_DLY)

#define PRS_NCSIC_RX_SIGNAL4_DLY_ADJ_REG_OFF  0X510
#define PRS_D8_DLY                  0
#define PRS_D8_DLY_MASK            (0X1F << PRS_D8_DLY)
#define PRS_D9_DLY                  8
#define PRS_D9_DLY_MASK            (0X1F << PRS_D9_DLY)
#define PRS_D10_DLY                 16
#define PRS_D10_DLY_MASK           (0X1F << PRS_D10_DLY)
#define PRS_D11_DLY                 24
#define PRS_D11_DLY_MASK           (0X1F << PRS_D11_DLY)

#define PRS_NCSIC_RX_SIGNAL5_DLY_ADJ_REG_OFF  0X514
#define PRS_D4_DLY                  0
#define PRS_D4_DLY_MASK            (0X1F << PRS_D4_DLY)
#define PRS_D5_DLY                  8
#define PRS_D5_DLY_MASK            (0X1F << PRS_D5_DLY)
#define PRS_D6_DLY                  16
#define PRS_D6_DLY_MASK            (0X1F << PRS_D6_DLY)
#define PRS_D7_DLY                  24
#define PRS_D7_DLY_MASK            (0X1F << PRS_D7_DLY)

#define PRS_NCSIC_RX_SIGNAL6_DLY_ADJ_REG_OFF  0X518
#define PRS_D0_DLY                  0
#define PRS_D0_DLY_MASK            (0X1F << PRS_D0_DLY)
#define PRS_D1_DLY                  8
#define PRS_D1_DLY_MASK            (0X1F << PRS_D1_DLY)
#define PRS_D2_DLY                  16
#define PRS_D2_DLY_MASK            (0X1F << PRS_D2_DLY)
#define PRS_D3_DLY                  24
#define PRS_D3_DLY_MASK            (0X1F << PRS_D3_DLY)

#define PRS_NCSIC_SYNC_EN_OFF                    0x520
#define PRS_NCSIC_SYNC_EN_SENT_SYNC_SINGAL       0
#define PRS_NCSIC_SYNC_EN_SENT_SYNC_SINGAL_MASK (0x1 << PRS_NCSIC_SYNC_EN_SENT_SYNC_SINGAL)
#define PRS_NCSIC_SYNC_SINGAL_SOURCE_SEL         1
#define PRS_NCSIC_SYNC_SINGAL_SOURCE_SEL_MASK   (0x1 << PRS_NCSIC_SYNC_SINGAL_SOURCE_SEL)
#define PRS_NCSIC_SYNC_SINGAL_VIA_BY             2
#define PRS_NCSIC_SYNC_SINGAL_VIA_BY_MASK (0x1 << PRS_NCSIC_SYNC_SINGAL_VIA_BY)
#define PRS_NCSIC_SYNC_INPUT_VSYNC_EN            4
#define PRS_NCSIC_SYNC_INPUT_VSYNC_EN_MASK (0xF << PRS_NCSIC_SYNC_INPUT_VSYNC_EN)
#define PRS_NCSIC_SYNC_BENCH_SEL                 8
#define PRS_NCSIC_SYNC_BENCH_SEL_MASK (0xF << PRS_NCSIC_SYNC_BENCH_SEL)
#define PRS_NCSIC_SYNC_INPUT_VSYNC_SEL           16
#define PRS_NCSIC_SYNC_INPUT_VSYNC_SEL_MASK (0xF << PRS_NCSIC_SYNC_INPUT_VSYNC_SEL)

#define PRS_CSIC_SYNC_CFG_OFF           0x524
#define PRS_CSIC_SYNC_DISTANCE          0
#define PRS_CSIC_SYNC_DISTANCE_MASK    (0xFFFF << PRS_CSIC_SYNC_DISTANCE)
#define PRS_CSIC_SYNC_PULSE_CFG         16
#define PRS_CSIC_SYNC_PULSE_CFG_MASK   (0xFFFF << PRS_CSIC_SYNC_PULSE_CFG)

#define PRS_CSIC_VS_WAIT_N_OFF          0x528
#define PRS_CSIC_SYNC_WAIT_N            0
#define PRS_CSIC_SYNC_WAIT_N_MASK      (0xFFFFFFFF << PRS_CSIC_SYNC_WAIT_N)

#define PRS_CSIC_VS_WAIT_M_OFF          0x52C
#define PRS_CSIC_SYNC_WAIT_M            0
#define PRS_CSIC_SYNC_WAIT_M_MASK      (0xFFFFFFFF << PRS_CSIC_SYNC_WAIT_M)

#define PRS_CSIC_XS_ENABLE_REG_OFF      0x540
#define PRS_CSIC_XS_XHS_OUT_EN          0
#define PRS_CSIC_XS_XHS_OUT_EN_MASK    (0x1 << PRS_CSIC_XS_XHS_OUT_EN)
#define PRS_CSIC_XS_XVS_OUT_EN          1
#define PRS_CSIC_XS_XVS_OUT_EN_MASK    (0x1 << PRS_CSIC_XS_XVS_OUT_EN)

#define PRS_CSIC_XVS_PERIOD_REG_OFF     0x544
#define PRS_CSIC_XVS_T                  0
#define PRS_CSIC_XVS_T_MASK            (0xFFFFFFFF << PRS_CSIC_XVS_T)

#define PRS_CSIC_XHS_PERIOD_REG_OFF  0x548
#define PRS_CSIC_XHS_T               0
#define PRS_CSIC_XHS_T_MASK         (0xFFFFFFFF << PRS_CSIC_XHS_T)

#define PRS_CSIC_XVS_LEN_REG_OFF     0x54C
#define PRS_CSIC_XVS_LEN             0
#define PRS_CSIC_XVS_LEN_MASK       (0xFFFFFFFF << PRS_CSIC_XVS_LEN)

#define PRS_CSIC_XHS_LEN_REG_OFF     0x550
#define PRS_CSIC_XHS_LEN             0
#define PRS_CSIC_XHS_LEN_MASK       (0xFFFFFFFF << PRS_CSIC_XHS_LEN)

#endif /* __CSIC__PARSER__REG__I__H__ */

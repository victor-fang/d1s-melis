/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/top_reg.h
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

#ifndef __CSIC__TOP__REG__H__
#define __CSIC__TOP__REG__H__

#include <stdbool.h>
//#include <v4l2-types.h>

#define MAX_CSIC_TOP_NUM 2

/*register value*/

/*register data struct*/

struct csic_feature_list {
	unsigned int dma_num;
	unsigned int vipp_num;
	unsigned int isp_num;
	unsigned int ncsi_num;
	unsigned int mcsi_num;
	unsigned int parser_num;
};

struct csic_version {
	unsigned int ver_big;
	unsigned int ver_small;
};

enum  csic_mulp_cs {
	CSIC_MULF_DMA0_CS = 0x1,
	CSIC_MULF_DMA1_CS = 0x2,
	CSIC_MULF_DMA2_CS = 0x4,
	CSIC_MULF_DMA3_CS = 0x8,
	CSIC_MULF_DMA4_CS = 0x10,
	CSIC_MULF_DMA5_CS = 0x20,
	CSIC_MULF_DMA6_CS = 0x40,
	CSIC_MULF_DMA7_CS = 0x80,
	CSIC_MULF_ALL_CS = 0xFF,
};

enum csis_mulp_int {
	MULF_DONE = 0X1,
	MULF_ERR = 0x2,
	MULF_ALL = 0x3,
};

struct cisc_mulp_int_status {
	bool mulf_done;
	bool mulf_err;
};

/*
 * functions about top register
 */
int csic_top_set_base_addr(unsigned int sel, unsigned long addr);
void csic_top_enable(unsigned int sel);
void csic_top_disable(unsigned int sel);
void csic_isp_bridge_enable(unsigned int sel);
void csic_isp_bridge_disable(unsigned int sel);
void csic_top_sram_pwdn(unsigned int sel, unsigned int en);
void csic_top_version_read_en(unsigned int sel, unsigned int en);
void csic_isp_input_select(unsigned int sel, unsigned int isp, unsigned int in,
				unsigned int psr, unsigned int ch);
void csic_vipp_input_select(unsigned int sel, unsigned int vipp,
				unsigned int isp, unsigned int ch);
void csic_dma_input_select(unsigned int sel, unsigned int dma,
				unsigned int parser, unsigned int ch);
void csic_feature_list_get(unsigned int sel, struct csic_feature_list *fl);
void csic_version_get(unsigned int sel, struct csic_version *v);
void csic_mbus_req_mex_set(unsigned int sel, unsigned int data);
void csic_mulp_mode_en(unsigned int sel, unsigned int en);
void csic_mulp_dma_cs(unsigned int sel, enum csic_mulp_cs cs);
void csic_mulp_int_enable(unsigned int sel, enum csis_mulp_int interrupt);
void csic_mulp_int_disable(unsigned int sel, enum csis_mulp_int interrupt);
void csic_mulp_int_get_status(unsigned int sel, struct cisc_mulp_int_status *status);
void csic_mulp_int_clear_status(unsigned int sel, enum csis_mulp_int interrupt);
void csic_ptn_generation_en(unsigned int sel, unsigned int en);
void csic_ptn_control(unsigned int sel, int mode, int dw, int port);
void csic_ptn_length(unsigned int sel, unsigned int len);
void csic_ptn_addr(unsigned int sel, unsigned long dma_addr);
void csic_ptn_size(unsigned int sel, unsigned int w, unsigned int h);

/*
 * functions about ccu register
 */
int csic_ccu_set_base_addr(unsigned long addr);
void csic_ccu_clk_gating_enable(void);
void csic_ccu_clk_gating_disable(void);
void csic_ccu_mcsi_clk_mode(unsigned int mode);
void csic_ccu_mcsi_combo_clk_en(unsigned int sel, unsigned int en);
void csic_ccu_mcsi_mipi_clk_en(unsigned int sel, unsigned int en);
void csic_ccu_mcsi_parser_clk_en(unsigned int sel, unsigned int en);
void csic_ccu_misp_isp_clk_en(unsigned int sel, unsigned int en);
void csic_ccu_mcsi_post_clk_enable(unsigned int sel);
void csic_ccu_mcsi_post_clk_disable(unsigned int sel);
void csic_ccu_bk_clk_en(unsigned int sel, unsigned int en);
void csic_ccu_vipp_clk_en(unsigned int sel, unsigned int en);

#endif /* __CSIC__TOP__REG__H__ */

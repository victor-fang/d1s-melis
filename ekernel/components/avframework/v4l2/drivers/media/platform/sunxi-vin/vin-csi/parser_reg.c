/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-csi/parser_reg.c
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

#include "parser_reg_i.h"
#include "parser_reg.h"

#include "../utility/vin_io.h"
#include "../platform/platform_cfg.h"

volatile void __iomem *csic_prs_base[VIN_MAX_CSI];

int csic_prs_set_base_addr(unsigned int sel, unsigned long addr)
{
	if (sel > VIN_MAX_CSI - 1)
		return -1;
	csic_prs_base[sel] = (volatile void __iomem *)addr;

	return 0;
}

void csic_prs_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_EN_REG_PRS_EN_MASK, 1 << PRS_EN_REG_PRS_EN);
}

void csic_prs_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_EN_REG_PRS_EN_MASK, 0 << PRS_EN_REG_PRS_EN);
}

void csic_prs_mode(unsigned int sel, enum prs_mode mode)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_EN_REG_PRS_MODE_MASK, mode << PRS_EN_REG_PRS_MODE);
}

void csic_prs_pclk_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_EN_REG_PCLK_EN_MASK, en << PRS_EN_REG_PCLK_EN);
}

void csic_prs_ncsi_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_EN_REG_NCSIC_EN_MASK, en << PRS_EN_REG_NCSIC_EN);
}

void csic_prs_mcsi_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_EN_REG_MCSIC_EN_MASK, en << PRS_EN_REG_MCSIC_EN);
}

void csic_prs_ch_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_EN_REG_OFF,
			PRS_CH_PRS_MODE_MASK, en << PRS_CH_PRS_MODE);
}

void csic_prs_ncsi_if_cfg(unsigned int sel, struct prs_ncsi_if_cfg *if_cfg)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_CSI_IF_MASK,
		if_cfg->intf << PRS_NCSIC_IF_CSI_IF);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_OUTPUT_MODE_MASK,
		if_cfg->mode << PRS_NCSIC_IF_OUTPUT_MODE);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_INPUT_SEQ_MASK,
		if_cfg->seq << PRS_NCSIC_IF_INPUT_SEQ);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_DATA_WIDTH_MASK,
		if_cfg->dw << PRS_NCSIC_IF_DATA_WIDTH);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_CLK_POL_MASK,
		if_cfg->clk << PRS_NCSIC_IF_CLK_POL);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_HREF_POL_MASK,
		if_cfg->href << PRS_NCSIC_IF_HREF_POL);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_VREF_POL_MASK,
		if_cfg->vref << PRS_NCSIC_IF_VREF_POL);

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_FIELD_POL_MASK,
		if_cfg->field << PRS_NCSIC_IF_FIELD_POL);

	if (if_cfg->type == PROGRESSED) {
		vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
			PRS_NCSIC_IF_SRC_TYPE_MASK,
			0x0 << PRS_NCSIC_IF_SRC_TYPE_BASE);
	} else if (if_cfg->type == INTERLACE) {
		vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
			PRS_NCSIC_IF_SRC_TYPE_MASK,
			0xf << PRS_NCSIC_IF_SRC_TYPE_BASE);
	}

	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_IF_CFG_REG_OFF,
		PRS_NCSIC_IF_DDR_SAMPLE_MODE_MASK,
		if_cfg->ddr_sample << PRS_NCSIC_IF_DDR_SAMPLE_MODE);
}

void csic_prs_mcsi_if_cfg(unsigned int sel, struct prs_mcsi_if_cfg *if_cfg)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_MCSIC_IF_CFG_REG_OFF,
		PRS_MCSIC_IF_INPUT_SEQ_MASK,
		if_cfg->seq << PRS_MCSIC_IF_INPUT_SEQ);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_MCSIC_IF_CFG_REG_OFF,
		PRS_MCSIC_IF_OUTPUT_MODE_MASK,
		if_cfg->mode << PRS_MCSIC_IF_OUTPUT_MODE);
}

void csic_prs_capture_start(unsigned int sel, unsigned int ch_total_num,
			struct prs_cap_mode *mode)
{
	/*u32 reg_val = (((ch_total_num >= 4) ? mode->mode : 0) << 24) +
	    (((ch_total_num >= 3) ? mode->mode : 0) << 16) +
	    (((ch_total_num >= 2) ? mode->mode : 0) << 8) +
	    (((ch_total_num >= 1) ? mode->mode : 0));
	vin_reg_writel(csic_prs_base[sel] + PRS_CAP_REG_OFF, reg_val);*/
	if (ch_total_num >= 4)
		vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
			CH3_SV_ON_MASK, mode->mode << CH3_SCAP_ON);
	if (ch_total_num >= 3)
		vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
			CH2_SV_ON_MASK, mode->mode << CH2_SCAP_ON);
	if (ch_total_num >= 2)
		vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
			CH1_SV_ON_MASK, mode->mode << CH1_SCAP_ON);
	if (ch_total_num >= 1)
		vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
			CH0_SV_ON_MASK, mode->mode << CH0_SCAP_ON);
}

void csic_prs_capture_stop(unsigned int sel)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH3_SV_ON_MASK, 0 << CH3_SCAP_ON);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH2_SV_ON_MASK, 0 << CH2_SCAP_ON);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH1_SV_ON_MASK, 0 << CH1_SCAP_ON);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH0_SV_ON_MASK, 0 << CH0_SCAP_ON);
}

void csic_prs_fps_ds(unsigned int sel, struct prs_fps_ds *prs_fps_ds)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH0_FPS_DS_MASK, prs_fps_ds->ch0_fps_ds << CH0_FPS_DS);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH1_FPS_DS_MASK, prs_fps_ds->ch1_fps_ds << CH1_FPS_DS);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH2_FPS_DS_MASK, prs_fps_ds->ch2_fps_ds << CH2_FPS_DS);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CAP_REG_OFF,
		CH3_FPS_DS_MASK, prs_fps_ds->ch3_fps_ds << CH3_FPS_DS);
}

void csic_prs_signal_status(unsigned int sel, struct prs_signal_status *status)
{
	unsigned int reg_val = vin_reg_readl(csic_prs_base[sel] + PRS_SIGNAL_STA_REG_OFF);

	status->pclk_sta = (reg_val & PCLK_STA_MASK) >> PCLK_STA;
	status->data_sta = (reg_val & DATA_STA_MASK) >> DATA_STA;
}

void csic_prs_ncsi_bt656_header_cfg(unsigned int sel, struct prs_ncsi_bt656_header *header)
{
	unsigned int reg_val = (header->ch0_id << CH0_ID) |
				(header->ch1_id << CH1_ID) |
				(header->ch2_id << CH2_ID) |
				(header->ch3_id << CH3_ID);
	vin_reg_writel(csic_prs_base[sel] + PRS_NCSIC_BT656_HEAD_CFG_REG_OFF,
			reg_val);
}

void csic_prs_input_fmt_cfg(unsigned int sel, unsigned int ch,
				enum prs_input_fmt fmt)
{
	vin_reg_writel(csic_prs_base[sel] + PRS_CH0_INFMT_REG_OFF +
			ch * PARSER_CH_OFF, fmt);
}

void csic_prs_output_size_cfg(unsigned int sel, unsigned int ch,
			struct prs_output_size *size)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CH0_OUTPUT_HSIZE_REG_OFF +
		ch * PARSER_CH_OFF, PRS_CH0_HOR_START_MASK,
		size->hor_start << PRS_CH0_HOR_START);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CH0_OUTPUT_HSIZE_REG_OFF +
		ch * PARSER_CH_OFF, PRS_CH0_HOR_LEN_MASK,
		size->hor_len << PRS_CH0_HOR_LEN);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CH0_OUTPUT_VSIZE_REG_OFF +
		ch * PARSER_CH_OFF, PRS_CH0_VER_START_MASK,
		size->ver_start << PRS_CH0_VER_START);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CH0_OUTPUT_VSIZE_REG_OFF +
		ch * PARSER_CH_OFF, PRS_CH0_VER_LEN_MASK,
		size->ver_len << PRS_CH0_VER_LEN);
}


/* for debug */
void csic_prs_input_para_get(unsigned int sel, unsigned int ch,
				struct prs_input_para *para)
{
	unsigned int reg_val = vin_reg_readl(csic_prs_base[sel] +
		PRS_CH0_INPUT_PARA1_REG_OFF + ch * PARSER_CH_OFF);
	para->input_ht = (reg_val >> PRS_CH0_INPUT_HT) & 0X3FFF;
	para->input_vt = (reg_val >> PRS_CH0_INPUT_VT) & 0X3FFF;
	reg_val = vin_reg_readl(csic_prs_base[sel] +
			PRS_CH0_LINE_TIME_REG_OFF + ch * PARSER_CH_OFF);
	para->input_hs = (reg_val >> PRS_CH0_INPUT_HSYN) & 0XFFFF;
	para->input_hb = (reg_val >> PRS_CH0_INPUT_HBLK) & 0XFFFF;
}

void csic_prs_int_enable(unsigned int sel, unsigned int ch,
				enum prs_int_sel interrupt)
{
	vin_reg_set(csic_prs_base[sel] + PRS_CH0_INT_EN_REG_OFF +
		    ch * PARSER_CH_OFF, interrupt);
}
void csic_prs_int_disable(unsigned int sel, unsigned int ch,
				enum prs_int_sel interrupt)
{
	vin_reg_clr(csic_prs_base[sel] + PRS_CH0_INT_EN_REG_OFF +
		    ch * PARSER_CH_OFF, interrupt);
}
void csic_prs_int_get_status(unsigned int sel, unsigned int ch,
				struct prs_int_status *status)
{
	unsigned int reg_val = vin_reg_readl(csic_prs_base[sel] +
		PRS_CH0_INT_STA_REG_OFF + ch * PARSER_CH_OFF);
	status->input_src_pd0 = (reg_val >> PRS_CH0_INPUT_SRC_PD0) & 0X01;
	status->input_src_pd1 = (reg_val >> PRS_CH0_INPUT_SRC_PD1) & 0X01;
	status->mul_err_pd = (reg_val >> PRS_CH0_MUL_ERR_PD) & 0X01;
}
void csic_prs_int_clear_status(unsigned int sel, unsigned int ch,
				enum prs_int_sel interrupt)
{
	vin_reg_writel(csic_prs_base[sel] + PRS_CH0_INT_STA_REG_OFF +
			ch * PARSER_CH_OFF, interrupt);
}

void csic_prs_set_pclk_dly(unsigned int sel, unsigned int pclk_dly)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_RX_SIGNAL0_DLY_ADJ_REG_OFF,
		PRS_PCLK_DLY_MASK, pclk_dly << PRS_PCLK_DLY);
}

void csic_prs_sync_en_cfg(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_SYNC_EN_OFF,
			PRS_NCSIC_SYNC_INPUT_VSYNC_SEL_MASK, sync->prs_sync_scr_sel << PRS_NCSIC_SYNC_INPUT_VSYNC_SEL);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_SYNC_EN_OFF,
			PRS_NCSIC_SYNC_BENCH_SEL_MASK, sync->prs_sync_bench_sel << PRS_NCSIC_SYNC_BENCH_SEL);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_SYNC_EN_OFF,
			PRS_NCSIC_SYNC_INPUT_VSYNC_EN_MASK, sync->prs_sync_input_vsync_en << PRS_NCSIC_SYNC_INPUT_VSYNC_EN);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_SYNC_EN_OFF,
			PRS_NCSIC_SYNC_SINGAL_VIA_BY_MASK, sync->prs_sync_singal_via_by << PRS_NCSIC_SYNC_SINGAL_VIA_BY);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_SYNC_EN_OFF,
			PRS_NCSIC_SYNC_SINGAL_SOURCE_SEL_MASK, sync->prs_sync_singal_scr_sel << PRS_NCSIC_SYNC_SINGAL_SOURCE_SEL);
}
void csic_prs_sync_en(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_NCSIC_SYNC_EN_OFF,
			PRS_NCSIC_SYNC_EN_SENT_SYNC_SINGAL_MASK, sync->prs_sync_en << PRS_NCSIC_SYNC_EN_SENT_SYNC_SINGAL);
}

void csic_prs_sync_cfg(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_SYNC_CFG_OFF,
			PRS_CSIC_SYNC_PULSE_CFG_MASK, sync->prs_sync_pulse_cfg << PRS_CSIC_SYNC_PULSE_CFG);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_SYNC_CFG_OFF,
			PRS_CSIC_SYNC_DISTANCE_MASK, sync->prs_sync_dist << PRS_CSIC_SYNC_DISTANCE);
}
void csic_prs_sync_wait_N(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_VS_WAIT_N_OFF,
			PRS_CSIC_SYNC_WAIT_N_MASK, sync->prs_sync_wait_n << PRS_CSIC_SYNC_WAIT_N);
}
void csic_prs_sync_wait_M(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_VS_WAIT_M_OFF,
			PRS_CSIC_SYNC_WAIT_M_MASK, sync->prs_sync_wait_m << PRS_CSIC_SYNC_WAIT_M);
}

void csic_prs_xs_en(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_XS_ENABLE_REG_OFF,
			PRS_CSIC_XS_XVS_OUT_EN_MASK, sync->prs_xvs_out_en << PRS_CSIC_XS_XVS_OUT_EN);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_XS_ENABLE_REG_OFF,
			PRS_CSIC_XS_XHS_OUT_EN_MASK, sync->prs_xhs_out_en << PRS_CSIC_XS_XHS_OUT_EN);
}

void csic_prs_xs_period_len_register(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_XVS_PERIOD_REG_OFF,
			PRS_CSIC_XVS_T_MASK, sync->prs_xvs_t << PRS_CSIC_XVS_T);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_XHS_PERIOD_REG_OFF,
			PRS_CSIC_XHS_T_MASK, sync->prs_xhs_t << PRS_CSIC_XHS_T);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_XVS_LEN_REG_OFF,
			PRS_CSIC_XVS_LEN_MASK, sync->prs_xvs_len << PRS_CSIC_XVS_LEN);
	vin_reg_clr_set(csic_prs_base[sel] + PRS_CSIC_XHS_LEN_REG_OFF,
			PRS_CSIC_XHS_LEN_MASK, sync->prs_xhs_len << PRS_CSIC_XHS_LEN);
}

/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-video/dma_reg.c
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

#include "dma_reg_i.h"
#include "dma_reg.h"

#include "../utility/vin_io.h"
#include "../platform/platform_cfg.h"

#define ADDR_BIT_R_SHIFT 2

volatile void __iomem *csic_dma_base[VIN_MAX_SCALER];

int csic_dma_set_base_addr(unsigned int sel, unsigned long addr)
{
	if (sel > VIN_MAX_SCALER - 1)
		return -1;
	csic_dma_base[sel] = (volatile void __iomem *)addr;

	return 0;
}

/* open module */

int csic_dma_get_frame_cnt(unsigned int sel)
{
	unsigned int reg_val = vin_reg_readl(csic_dma_base[sel] + CSIC_DMA_FRM_CNT_REG_OFF);

	return ((reg_val & CSIC_DMA_FRM_CNT_MASK) >> CSIC_DMA_FRM_CNT);
}

void csic_dma_top_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_DMA_TOP_EN_MASK, 1 << CSIC_DMA_TOP_EN);
}

void csic_dma_top_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_DMA_TOP_EN_MASK, 0 << CSIC_DMA_TOP_EN);
}

void csic_dma_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_DMA_EN_MASK, 1 << CSIC_DMA_EN);
}

void csic_dma_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_DMA_EN_MASK, 0 << CSIC_DMA_EN);
}

void csic_fbc_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_FBC_EN_MASK, 1 << CSIC_FBC_EN);
}

void csic_fbc_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_FBC_EN_MASK, 0 << CSIC_FBC_EN);
}

void csic_lbc_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_DMA_EN_MASK, 1 << CSIC_DMA_EN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_LBC_EN_MASK, 1 << CSIC_LBC_EN);
}

void csic_lbc_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_DMA_EN_MASK, 0 << CSIC_DMA_EN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_LBC_EN_MASK, 0 << CSIC_LBC_EN);
}

void csic_frame_cnt_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_FRAME_CNT_EN_MASK, 1 << CSIC_FRAME_CNT_EN);
}

void csic_frame_cnt_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_FRAME_CNT_EN_MASK, 0 << CSIC_FRAME_CNT_EN);
}

void csic_buf_addr_fifo_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_BUF_ADDR_MODE_MASK, en << CSIC_BUF_ADDR_MODE);
}

void csic_dma_clk_cnt_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_CLK_CNT_EN_MASK, 1 << CSIC_CLK_CNT_EN);

}
void csic_dma_clk_cnt_sample(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_CLK_CNT_SPL_MASK, 1 << CSIC_CLK_CNT_SPL);

}

void csic_dma_output_fmt_cfg(unsigned int sel, enum output_fmt fmt)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			OUTPUT_FMT_MASK, fmt << OUTPUT_FMT);

}

void csic_dma_buf_length_software_enable(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
		CSIC_BUF_LENGTH_CFG_MODE_MASK, en << CSIC_BUF_LENGTH_CFG_MODE);
}

void csi_dam_flip_software_enable(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_FLIP_SIZE_CFG_MODE_MASK, en << CSIC_FLIP_SIZE_CFG_MODE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_EN_REG_OFF,
			CSIC_VFLIP_BUF_ADDR_CFG_MODE_MASK, en << CSIC_VFLIP_BUF_ADDR_CFG_MODE);
}

void csic_dma_flip_en(unsigned int sel, struct csic_dma_flip *flip)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			HFLIP_EN_MASK, flip->hflip_en << HFLIP_EN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			VFLIP_EN_MASK, flip->vflip_en << VFLIP_EN);
}
void csic_dma_config(unsigned int sel, struct csic_dma_cfg *cfg)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			MIN_SDR_WR_SIZE_MASK, cfg->block << MIN_SDR_WR_SIZE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			FPS_DS_MASK, cfg->ds << FPS_DS);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			FIELD_SEL_MASK,	cfg->field << FIELD_SEL);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			HFLIP_EN_MASK, cfg->flip.hflip_en << HFLIP_EN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			VFLIP_EN_MASK, cfg->flip.vflip_en << VFLIP_EN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
			OUTPUT_FMT_MASK, cfg->fmt << OUTPUT_FMT);
}

void csic_dma_10bit_cut2_8bit_enable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
				ENABLE_10BIT_CUT2_8BIT_MASK, 1 << ENABLE_10BIT_CUT2_8BIT);
}

void csic_dma_10bit_cut2_8bit_disable(unsigned int sel)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_CFG_REG_OFF,
				ENABLE_10BIT_CUT2_8BIT_MASK, 0 << ENABLE_10BIT_CUT2_8BIT);
}

void csic_set_threshold_for_bufa_mode(unsigned int sel, struct dma_bufa_threshold *threshold)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_BUF_THRESHOLD_REG_OFF,
				DMA_BUFA_FIFO_THRESHOLD_MASK, threshold->bufa_fifo_threshold << DMA_BUFA_FIFO_THRESHOLD);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_BUF_THRESHOLD_REG_OFF,
				DMA_STORED_FRM_THRESHOLD_MASK, threshold->stored_frm_threshold << DMA_STORED_FRM_THRESHOLD);
}

void csic_dma_output_size_cfg(unsigned int sel, struct dma_output_size *size)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_HSIZE_REG_OFF,
			HOR_START_MASK, size->hor_start << HOR_START);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_HSIZE_REG_OFF,
			HOR_LEN_MASK, size->hor_len << HOR_LEN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_VSIZE_REG_OFF,
			VER_START_MASK, size->ver_start << VER_START);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_VSIZE_REG_OFF,
			VER_LEN_MASK, size->ver_len << VER_LEN);
}
void csic_dma_buffer_address(unsigned int sel, enum fifo_buf_sel buf,
				unsigned long addr)
{
#ifndef BUF_AUTO_UPDATE
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_F0_BUFA_REG_OFF + buf * 4,
			F0_BUFA_MASK, addr >> ADDR_BIT_R_SHIFT);
#else
	vin_reg_writel(csic_dma_base[sel] + CSIC_DMA_BUFA_F0_ENTRY_REG_OFF + buf * 2,
			addr >> ADDR_BIT_R_SHIFT);
#endif
}
void csic_dma_buffer_length(unsigned int sel, struct dma_buf_len *buf_len)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_BUF_LEN_REG_OFF,
			BUF_LEN_MASK, buf_len->buf_len_y << BUF_LEN);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_BUF_LEN_REG_OFF,
			BUF_LEN_C_MASK,	buf_len->buf_len_c << BUF_LEN_C);

}
void csic_dma_flip_size(unsigned int sel, struct dma_flip_size *flip_size)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_FLIP_SIZE_REG_OFF,
			VALID_LEN_MASK, flip_size->hor_len << VALID_LEN);
	vin_reg_clr_set(csic_dma_base[sel]+CSIC_DMA_FLIP_SIZE_REG_OFF,
			VER_LEN_MASK, flip_size->ver_len << VER_LEN);
}

void csic_dma_line_cnt(unsigned int sel, int line)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_LINE_CNT_REG_OFF,
			LINE_CNT_NUM_MASK, line);
}
void csic_dma_frm_cnt(unsigned int sel, struct csi_sync_ctrl *sync)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_DMA_FRM_CNT_REG_OFF,
			CSIC_DMA_CLR_DIS_MASK, sync->dma_clr_dist<<CSIC_DMA_CLR_DIS);
}
void csic_dma_cap_status(unsigned int sel, struct dma_capture_status *status)
{
	unsigned int reg_val = vin_reg_readl(csic_dma_base[sel] + CSIC_DMA_CAP_STA_REG_OFF);

	status->scap_sta = (reg_val & SCAP_STA_MASK) >> SCAP_STA;
	status->vcap_sta = (reg_val & VCAP_STA_MASK) >> VCAP_STA;
	status->field_sta = (reg_val & FIELD_STA_MASK) >> FIELD_STA;
}

void csic_dma_int_enable(unsigned int sel,	enum dma_int_sel interrupt)
{
	vin_reg_set(csic_dma_base[sel] + CSIC_DMA_INT_EN_REG_OFF, interrupt);
}
void csic_dma_int_disable(unsigned int sel, enum dma_int_sel interrupt)
{
	vin_reg_clr(csic_dma_base[sel] + CSIC_DMA_INT_EN_REG_OFF, interrupt);
}
void csic_dma_int_get_status(unsigned int sel, struct dma_int_status *status)
{
	unsigned int reg_val = vin_reg_readl(csic_dma_base[sel] + CSIC_DMA_INT_STA_REG_OFF);

	status->capture_done = (reg_val & CD_PD_MASK) >> CD_PD;
	status->frame_done = (reg_val & FD_PD_MASK) >> FD_PD;
	status->buf_0_overflow = (reg_val & FIFO0_OF_PD_MASK) >> FIFO0_OF_PD;
	status->buf_1_overflow = (reg_val & FIFO1_OF_PD_MASK) >> FIFO1_OF_PD;
	status->buf_2_overflow = (reg_val & FIFO2_OF_PD_MASK) >> FIFO2_OF_PD;
	status->line_cnt_flag = (reg_val & LC_INT_EN_MASK) >> LC_INT_EN;
	status->hblank_overflow = (reg_val & HB_OF_INT_EN_MASK) >> HB_OF_INT_EN;
	status->vsync_trig = (reg_val & VS_INT_EN_MASK) >> VS_INT_EN;
	status->fbc_ovhd_wrddr_full = (reg_val & FBC_OVHD_WRDDR_FULL_INT_EN_MASK) >> FBC_OVHD_WRDDR_FULL_INT_EN;
	status->fbc_data_wrddr_full = (reg_val & FBC_DATA_WRDDR_FULL_INT_EN_MASK) >> FBC_DATA_WRDDR_FULL_INT_EN;
	status->buf_addr_fifo = (reg_val & BUF_ADDR_FIFO_INT_PD_MASK) >> BUF_ADDR_FIFO_INT_PD;
	status->stored_frm_cnt = (reg_val & STORED_FRM_CNT_INT_PD_MASK) >> STORED_FRM_CNT_INT_PD;
	status->frm_lost = (reg_val & FRM_LOST_INT_PD_MASK) >> FRM_LOST_INT_PD;
	status->lbc_hb = (reg_val & LBC_HB_INT_PD_MASK) >> LBC_HB_INT_PD;
}
void csic_dma_int_clear_status(unsigned int sel, enum dma_int_sel interrupt)
{
	vin_reg_writel(csic_dma_base[sel] + CSIC_DMA_INT_STA_REG_OFF, interrupt);
}

void csic_lbc_cmp_ratio(unsigned int sel, struct dma_lbc_cmp *lbc_cmp)
{
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				WHETHER_LOSSY_ENABLE_MASK, lbc_cmp->is_lossy << WHETHER_LOSSY_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				GLB_ENABLE_MASK, lbc_cmp->glb_enable << GLB_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				DTS_ENABLE_MASK, lbc_cmp->dts_enable << DTS_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				OTS_ENABLE_MASK, lbc_cmp->ots_enable << OTS_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				MSQ_ENABLE_MASK, lbc_cmp->msq_enable << MSQ_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				UPDATE_ADVANTURE_ENABLE_MASK, lbc_cmp->updata_adv_en << UPDATE_ADVANTURE_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				UPDATE_ADVANTURE_RATIO_MASK, lbc_cmp->updata_adv_ratio << UPDATE_ADVANTURE_RATIO);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				LIMIT_QP_ENABLE_MASK, lbc_cmp->lmtqp_en << LIMIT_QP_ENABLE);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_CONFIGURE_REG_OFF,
				LIMIT_QP_MIM_MASK, lbc_cmp->lmtqp_min << LIMIT_QP_MIM);

	vin_reg_writel(csic_dma_base[sel] + CSIC_LBC_LINE_TARGET_BIT0_REG_OFF, lbc_cmp->line_tar_bits[0]);
	vin_reg_writel(csic_dma_base[sel] + CSIC_LBC_LINE_TARGET_BIT1_REG_OFF, lbc_cmp->line_tar_bits[1]);

	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_RC_ADV_REG_OFF,
				RATE_CONTROL_ADVANTURE_0_MASK, lbc_cmp->rc_adv[0] << RATE_CONTROL_ADVANTURE_0);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_RC_ADV_REG_OFF,
				RATE_CONTROL_ADVANTURE_1_MASK, lbc_cmp->rc_adv[1] << RATE_CONTROL_ADVANTURE_1);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_RC_ADV_REG_OFF,
				RATE_CONTROL_ADVANTURE_2_MASK, lbc_cmp->rc_adv[2] << RATE_CONTROL_ADVANTURE_2);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_RC_ADV_REG_OFF,
				RATE_CONTROL_ADVANTURE_3_MASK, lbc_cmp->rc_adv[3] << RATE_CONTROL_ADVANTURE_3);

	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_MB_MIN_REG_OFF,
				MACROBLOCK_MIN_BITS0_MASK, lbc_cmp->mb_mi_bits[0] << MACROBLOCK_MIN_BITS0);
	vin_reg_clr_set(csic_dma_base[sel] + CSIC_LBC_MB_MIN_REG_OFF,
				MACROBLOCK_MIN_BITS1_MASK, lbc_cmp->mb_mi_bits[1] << MACROBLOCK_MIN_BITS1);
}


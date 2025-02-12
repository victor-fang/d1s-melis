/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-video/dma_reg.h
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

#ifndef __CSIC__DMA__REG__H__
#define __CSIC__DMA__REG__H__

#include <stdbool.h>
//#include <v4l2-types.h>
#include <sunxi_camera_v2.h>

/*register value*/

/*
 * output data format
 */
enum output_fmt {
	/* only when input is raw */
	FIELD_RAW_8 = 0,
	FIELD_RAW_10 = 1,
	FIELD_RAW_12 = 2,
	FIELD_RGB565 = 4,
	FIELD_RGB888 = 5,
	FIELD_PRGB888 = 6,
	FRAME_RAW_8 = 8,
	FRAME_RAW_10 = 9,
	FRAME_RAW_12 = 10,
	FRAME_RGB565 = 12,
	FRAME_RGB888 = 13,
	FRAME_PRGB888 = 14,

	/* only when input is yuv422/yuv420 */
	FIELD_PLANAR_YUV422 = 0, /* yuv422 */
	LBC_MODE_OUTPUT = 0,     /* yuv420 */
	FIELD_PLANAR_YUV420 = 1,
	FRAME_PLANAR_YUV420 = 2,
	FRAME_PLANAR_YUV422 = 3,
	FIELD_UV_CB_YUV422 = 4,
	FIELD_UV_CB_YUV420 = 5,
	FRAME_UV_CB_YUV420 = 6,
	FRAME_UV_CB_YUV422 = 7,
	FIELD_VU_CB_YUV422 = 8,
	FIELD_VU_CB_YUV420 = 9,
	FRAME_VU_CB_YUV420 = 10,
	FRAME_VU_CB_YUV422 = 11,
	FIELD_CB_YUV400 = 13,
	FRAME_CB_YUV400 = 15,
};

/*
 * input field selection, only when input is ccir656
 */
enum field_sel {
	FIELD_1,	/* odd field */
	FIELD_2,	/* even field */
	FIELD_EITHER,	/* either field */
};

/*
 * fps down sample
 */
enum fps_ds {
	FPS_NO_DS,	/* no down sample */
	FPS_2_DS,	/* only receives the first frame every 2 frames */
	FPS_3_DS,	/* only receives the first frame every 3 frames */
	FPS_4_DS,	/* only receives the first frame every 4 frames */
	FPS_5_DS,	/* only receives the first frame every 5 frames */
	FPS_6_DS,	/* only receives the first frame every 6 frames */
	FPS_7_DS,	/* only receives the first frame every 7 frames */
	FPS_8_DS,	/* only receives the first frame every 8 frames */
	FPS_9_DS,	/* only receives the first frame every 9 frames */
	FPS_10_DS,	/* only receives the first frame every 10 frames */
	FPS_11_DS,	/* only receives the first frame every 11 frames */
	FPS_12_DS,	/* only receives the first frame every 12 frames */
	FPS_13_DS,	/* only receives the first frame every 13 frames */
	FPS_14_DS,	/* only receives the first frame every 14 frames */
	FPS_15_DS,	/* only receives the first frame every 15 frames */
	FPS_16_DS,	/* only receives the first frame every 16 frames */
};

/*
 * Minimum size of SDRAM block write
 */
enum min_sdr_wr_size {
	SDR_256B,	/* if hflip is enable, always select 256 bytes */
	SDR_512B,
	SDR_1K,
	SDR_2K,
};

/*
 * dma fifo buffer select
 */
enum fifo_buf_sel {
	FIFO0_BUF_A = 0,
	FIFO1_BUF_A,
	FIFO2_BUF_A,
};

/*
 * dma interrupt select
 */
enum dma_int_sel {
	DMA_INT_CAPTURE_DONE = 0X1,
	DMA_INT_FRAME_DONE = 0X2,
	DMA_INT_BUF_0_OVERFLOW = 0X4,
	DMA_INT_BUF_1_OVERFLOW = 0X8,
	DMA_INT_BUF_2_OVERFLOW = 0X10,
	DMA_INT_LINE_CNT = 0X20,
	DMA_INT_HBLANK_OVERFLOW = 0X40,
	DMA_INT_VSYNC_TRIG = 0X80,
	DMA_INT_FBC_OVHD_WRDDR_FULL = 0X100,
	DMA_INT_FBC_DATA_WRDDR_FULL = 0X200,
	DMA_INT_BUF_ADDR_FIFO = 0x2000,
	DMA_INT_STORED_FRM_CNT = 0x4000,
	DMA_INT_FRM_LOST = 0x8000,
	DMA_INT_LBC_HB = 0x10000,
	DMA_INT_ALL = 0X1E3FF,
};

/*register data struct*/

struct csic_dma_flip {
	unsigned int hflip_en;
	unsigned int vflip_en;
};

struct csic_dma_cfg {
	enum output_fmt fmt;
	struct csic_dma_flip flip;
	enum field_sel field;
	enum fps_ds ds;
	enum min_sdr_wr_size block;
};

struct dma_output_size {
	unsigned int hor_len;
	unsigned int hor_start;
	unsigned int ver_len;
	unsigned int ver_start;
};

struct dma_buf_len {
	unsigned int buf_len_y;
	unsigned int buf_len_c;
};

struct dma_flip_size {
	unsigned int ver_len;
	unsigned int hor_len;
};

struct dma_capture_status {
	bool field_sta;
	bool vcap_sta;
	bool scap_sta;
};

struct dma_int_status {
	bool capture_done;
	bool frame_done;
	bool buf_0_overflow;
	bool buf_1_overflow;
	bool buf_2_overflow;
	bool line_cnt_flag;
	bool hblank_overflow;
	bool vsync_trig;
	bool fbc_ovhd_wrddr_full;
	bool fbc_data_wrddr_full;
	bool buf_addr_fifo;
	bool stored_frm_cnt;
	bool frm_lost;
	bool lbc_hb;
};

struct dma_fifo_threshold {
	unsigned int fifo_nearly_full_th;
	unsigned int fifo_thrs;
};

struct dma_pclk_statistic {
	unsigned int pclk_cnt_line_max;
	unsigned int pclk_cnt_line_min;
};

struct dma_lbc_cmp {
	unsigned char is_lossy;
	unsigned char bit_depth;
	unsigned char glb_enable;
	unsigned char dts_enable;
	unsigned char ots_enable;
	unsigned char msq_enable;
	unsigned int cmp_ratio_even;
	unsigned int cmp_ratio_odd;
	unsigned int mb_mi_bits[2];
	unsigned char rc_adv[4];
	unsigned char lmtqp_en;
	unsigned char lmtqp_min;
	unsigned char updata_adv_en;
	unsigned char updata_adv_ratio;
	unsigned int line_tar_bits[2];
};

struct dma_bufa_threshold {
	unsigned char bufa_fifo_threshold;
	unsigned char stored_frm_threshold;
	unsigned char bufa_fifo_total;
};

int csic_dma_set_base_addr(unsigned int sel, unsigned long addr);
int csic_dma_get_frame_cnt(unsigned int sel);
void csic_dma_top_enable(unsigned int sel);
void csic_dma_top_disable(unsigned int sel);
void csic_dma_enable(unsigned int sel);
void csic_dma_disable(unsigned int sel);
void csic_fbc_enable(unsigned int sel);
void csic_fbc_disable(unsigned int sel);
void csic_lbc_enable(unsigned int sel);
void csic_lbc_disable(unsigned int sel);
void csic_frame_cnt_enable(unsigned int sel);
void csic_frame_cnt_disable(unsigned int sel);
void csic_buf_addr_fifo_en(unsigned int sel, unsigned int en);
void csic_dma_clk_cnt_en(unsigned int sel, unsigned int en);
void csic_dma_clk_cnt_sample(unsigned int sel, unsigned int en);
void csic_dma_output_fmt_cfg(unsigned int sel, enum output_fmt fmt);
void csic_dma_buf_length_software_enable(unsigned int sel, unsigned int en);
void csi_dam_flip_software_enable(unsigned int sel, unsigned int en);
void csic_dma_flip_en(unsigned int sel, struct csic_dma_flip *flip);
void csic_dma_config(unsigned int sel, struct csic_dma_cfg *cfg);
void csic_dma_10bit_cut2_8bit_enable(unsigned int sel);
void csic_dma_10bit_cut2_8bit_disable(unsigned int sel);
void csic_set_threshold_for_bufa_mode(unsigned int sel, struct dma_bufa_threshold *threshold);
void csic_dma_output_size_cfg(unsigned int sel, struct dma_output_size *size);
void csic_dma_buffer_address(unsigned int sel, enum fifo_buf_sel buf,
				unsigned long addr);
void csic_dma_buffer_length(unsigned int sel, struct dma_buf_len *buf_len);
void csic_dma_flip_size(unsigned int sel, struct dma_flip_size *flip_size);
void csic_dma_line_cnt(unsigned int sel, int line);
void csic_dma_frm_cnt(unsigned int sel, struct csi_sync_ctrl *sync);
void csic_dma_cap_status(unsigned int sel, struct dma_capture_status *status);
void csic_dma_int_enable(unsigned int sel,	enum dma_int_sel interrupt);
void csic_dma_int_disable(unsigned int sel, enum dma_int_sel interrupt);
void csic_dma_int_get_status(unsigned int sel, struct dma_int_status *status);
void csic_dma_int_clear_status(unsigned int sel, enum dma_int_sel interrupt);
void csic_lbc_cmp_ratio(unsigned int sel, struct dma_lbc_cmp *lbc_cmp);

/*for debug*/
void csic_dma_line_count(unsigned int sel, unsigned int line_cnt);
void csic_dma_frame_clk_cnt(unsigned int sel, unsigned int clk_cnt);
void csic_dma_acc_itnl_cnt(unsigned int sel, unsigned int acc_clk_cnt,
				unsigned int itnl_clk_cnt);
void csic_dma_fifo_statistic(unsigned int sel, unsigned int fifo_frm_max);
void csic_dma_fifo_threshold(unsigned int sel, struct dma_fifo_threshold *threshold);
void csic_dma_pclk_statistic(unsigned int sel, struct dma_pclk_statistic *pclk_stat);

#endif /* __CSIC__DMA__REG__H__ */

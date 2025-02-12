/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-mipi/bsp_mipi_csi_v1.c
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
 * sunxi mipi bsp interface
 * Author:raymonxiu
 */

#include "dphy/dphy.h"
#include "dphy/dphy_reg.h"
#include "protocol/protocol.h"
#include "protocol/protocol_reg.h"

#include <linux/delay.h>
#include "bsp_mipi_csi.h"

int bsp_mipi_csi_set_base_addr(unsigned int sel, unsigned long addr_base)
{
	return ptcl_reg_map(sel, addr_base);
}

int bsp_mipi_dphy_set_base_addr(unsigned int sel, unsigned long addr_base)
{
	return dphy_reg_map(sel, addr_base);
}

void bsp_mipi_csi_dphy_init(unsigned int sel)
{
	dphy_ana_init(sel);
}

void bsp_mipi_csi_dphy_exit(unsigned int sel)
{
	dphy_ana_exit(sel);
}

void bsp_mipi_csi_dphy_enable(unsigned int sel, unsigned int flags)
{
	dphy_enable(sel);
}

void bsp_mipi_csi_dphy_disable(unsigned int sel, unsigned int flags)
{
	dphy_disable(sel);
}

void bsp_mipi_csi_protocol_enable(unsigned int sel)
{
	ptcl_enable(sel);
}

void bsp_mipi_csi_set_version(unsigned int sel, unsigned int ver)
{

}

void bsp_mipi_csi_protocol_disable(unsigned int sel)
{
	ptcl_disable(sel);
}

void mipi_dphy_cfg_1data(unsigned int sel, unsigned int code, unsigned int data)
{

}


void bsp_mipi_csi_det_mipi_clk(unsigned int sel, unsigned int *mipi_bps,
			       unsigned int dphy_clk)
{
	unsigned int freq_cnt;

	dphy_rx_freq_cnt_enable(sel);
	usleep_range(1000, 2000);
	freq_cnt = dphy_rx_get_freq_cnt(sel);
	if (freq_cnt == 0)
		return;

	*mipi_bps = 1000 * dphy_clk * 8 / freq_cnt;
}

void bsp_mipi_csi_set_rx_dly(unsigned int sel, unsigned int mipi_bps,
			     unsigned int dphy_clk)
{
	unsigned short rx_dly_cnt;

	if (mipi_bps == 0)
		return;

	rx_dly_cnt = (8 * (dphy_clk / (mipi_bps / 8)));

	dphy_rx_set_rx_dly(sel, rx_dly_cnt);
}

void bsp_mipi_csi_set_lprst_dly(unsigned int sel, unsigned int mipi_bps,
				unsigned int dphy_clk)
{
	dphy_rx_set_lprst_dly(sel, (4 * (dphy_clk / (mipi_bps / 2))));
}

void bsp_mipi_csi_set_lp_ulps_wp(unsigned int sel, unsigned int lp_ulps_wp_ms,
				 unsigned int lp_clk)
{
	unsigned int lp_ulps_wp_cnt;

	lp_ulps_wp_cnt = lp_ulps_wp_ms * lp_clk / 1000;
	dphy_rx_set_lp_ulps_wp(sel, lp_ulps_wp_cnt);
}

void bsp_mipi_csi_set_dphy_timing(unsigned int sel, unsigned int *mipi_bps,
				  unsigned int dphy_clk, unsigned int mode)
{
	if (mode == 1)
		bsp_mipi_csi_det_mipi_clk(sel, mipi_bps, dphy_clk);

	dphy_rx_dbc_enable(sel);
	bsp_mipi_csi_set_rx_dly(sel, *mipi_bps, dphy_clk);
	bsp_mipi_csi_set_lprst_dly(sel, *mipi_bps, dphy_clk);
	dphy_rx_hs_clk_miss_cnt_disable(sel);
	dphy_rx_hs_sync_cnt_disable(sel);
	dphy_rx_lp_to_cnt_disable(sel);
	dphy_rx_set_hs_sync_to(sel, 0xff);
	dphy_rx_set_hs_clk_miss(sel, 0xff);
	dphy_rx_set_lp_to(sel, 0xff);
	dphy_rx_set_lp_ulps_wp(sel, 0xff);
	dphy_rx_set_entm_to_enrx_dly(sel, 4);
}

void bsp_mipi_csi_set_lane(unsigned int sel, unsigned char lane_num)
{
	dphy_rx_disable(sel, 4);
	dphy_set_data_lane(sel, lane_num);
	dphy_rx_enable(sel, lane_num);
	ptcl_set_data_lane(sel, lane_num);
}

void bsp_mipi_csi_set_total_ch(unsigned int sel, unsigned char ch_num)
{
	ptcl_set_total_ch(sel, ch_num);
}

void bsp_mipi_csi_set_pkt_header(unsigned int sel, unsigned char ch,
				 unsigned char vc, enum pkt_fmt mipi_pkt_fmt)
{
	ptcl_set_vc(sel, ch, vc);
	ptcl_set_dt(sel, ch, mipi_pkt_fmt);
}

void bsp_mipi_csi_set_src_type(unsigned int sel, unsigned char ch,
			       enum source_type src_type)
{
	ptcl_set_src_type(sel, ch, src_type);
}

void bsp_mipi_csi_set_para(unsigned int sel, struct mipi_para *para)
{
	bsp_mipi_csi_set_dphy_timing(sel, &para->bps, para->dphy_freq,
				     para->auto_check_bps);
	bsp_mipi_csi_set_lane(sel, para->lane_num);
	bsp_mipi_csi_set_total_ch(sel, para->total_rx_ch);
}

void bsp_mipi_csi_set_fmt(unsigned int sel, unsigned int total_rx_ch,
			  struct mipi_fmt_cfg *fmt)
{
	unsigned int i;

	for (i = 0; i < total_rx_ch; i++) {
		bsp_mipi_csi_set_pkt_header(sel, i, fmt->vc[i],
					    fmt->packet_fmt[i]);
		if (fmt->field[i] == V4L2_FIELD_NONE ||
		    fmt->field[i] == V4L2_FIELD_ANY)
			bsp_mipi_csi_set_src_type(sel, i, PROGRESSIVE);
		else
			bsp_mipi_csi_set_src_type(sel, i, INTERLACED);
	}

	bsp_data_formats_enable(sel, fmt->fmt_type);
}

void bsp_mipi_csi_set_dol(unsigned int sel, unsigned int mode, unsigned int ch)
{

}


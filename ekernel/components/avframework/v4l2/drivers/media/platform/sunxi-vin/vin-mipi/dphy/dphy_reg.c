/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-mipi/dphy/dphy_reg.c
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
 * sunxi mipi dphy low-level interface
 * Author:raymonxiu
 */

#include <linux/delay.h>
#include "dphy.h"
#include "dphy_reg_i.h"
#include "dphy_reg.h"

DPHY_CTL_REG_t *dphy_ctl[MAX_MIPI_DPHY];
DPHY_TX_CTL_REG_t *dphy_tx_ctl[MAX_MIPI_DPHY];
DPHY_RX_CTL_REG_t *dphy_rx_ctl[MAX_MIPI_DPHY];
DPHY_TX_TIME0_REG_t *dphy_tx_time0[MAX_MIPI_DPHY];
DPHY_TX_TIME1_REG_t *dphy_tx_time1[MAX_MIPI_DPHY];
DPHY_TX_TIME2_REG_t *dphy_tx_time2[MAX_MIPI_DPHY];
DPHY_TX_TIME3_REG_t *dphy_tx_time3[MAX_MIPI_DPHY];
DPHY_TX_TIME4_REG_t *dphy_tx_time4[MAX_MIPI_DPHY];
DPHY_RX_TIME0_REG_t *dphy_rx_time0[MAX_MIPI_DPHY];
DPHY_RX_TIME1_REG_t *dphy_rx_time1[MAX_MIPI_DPHY];
DPHY_RX_TIME2_REG_t *dphy_rx_time2[MAX_MIPI_DPHY];
DPHY_RX_TIME3_REG_t *dphy_rx_time3[MAX_MIPI_DPHY];
DPHY_ANA0_REG_t *dphy_ana0[MAX_MIPI_DPHY];
DPHY_ANA1_REG_t *dphy_ana1[MAX_MIPI_DPHY];
DPHY_ANA2_REG_t *dphy_ana2[MAX_MIPI_DPHY];
DPHY_ANA3_REG_t *dphy_ana3[MAX_MIPI_DPHY];
DPHY_ANA4_REG_t *dphy_ana4[MAX_MIPI_DPHY];
DPHY_INT_EN0_REG_t *dphy_int_en0[MAX_MIPI_DPHY];
DPHY_INT_EN1_REG_t *dphy_int_en1[MAX_MIPI_DPHY];
DPHY_INT_EN2_REG_t *dphy_int_en2[MAX_MIPI_DPHY];
DPHY_INT_PD0_REG_t *dphy_int_pd0[MAX_MIPI_DPHY];
DPHY_INT_PD1_REG_t *dphy_int_pd1[MAX_MIPI_DPHY];
DPHY_INT_PD2_REG_t *dphy_int_pd2[MAX_MIPI_DPHY];
DPHY_DBG0_REG_t *dphy_dbg0[MAX_MIPI_DPHY];
DPHY_DBG1_REG_t *dphy_dbg1[MAX_MIPI_DPHY];
DPHY_DBG2_REG_t *dphy_dbg2[MAX_MIPI_DPHY];
DPHY_DBG3_REG_t *dphy_dbg3[MAX_MIPI_DPHY];
DPHY_DBG4_REG_t *dphy_dbg4[MAX_MIPI_DPHY];
DPHY_DBG5_REG_t *dphy_dbg5[MAX_MIPI_DPHY];

int dphy_reg_map(unsigned int sel, unsigned long addr_base)
{
	if (sel >= MAX_MIPI_DPHY)
		return -1;

	dphy_ctl[sel] = (DPHY_CTL_REG_t *) (addr_base + DPHY_CTL_REG_OFF);
	dphy_tx_ctl[sel] =
	    (DPHY_TX_CTL_REG_t *) (addr_base + DPHY_TX_CTL_REG_OFF);
	dphy_rx_ctl[sel] =
	    (DPHY_RX_CTL_REG_t *) (addr_base + DPHY_RX_CTL_REG_OFF);
	dphy_tx_time0[sel] =
	    (DPHY_TX_TIME0_REG_t *) (addr_base + DPHY_TX_TIME0_REG_OFF);
	dphy_tx_time1[sel] =
	    (DPHY_TX_TIME1_REG_t *) (addr_base + DPHY_TX_TIME1_REG_OFF);
	dphy_tx_time2[sel] =
	    (DPHY_TX_TIME2_REG_t *) (addr_base + DPHY_TX_TIME2_REG_OFF);
	dphy_tx_time3[sel] =
	    (DPHY_TX_TIME3_REG_t *) (addr_base + DPHY_TX_TIME3_REG_OFF);
	dphy_tx_time4[sel] =
	    (DPHY_TX_TIME4_REG_t *) (addr_base + DPHY_TX_TIME4_REG_OFF);
	dphy_rx_time0[sel] =
	    (DPHY_RX_TIME0_REG_t *) (addr_base + DPHY_RX_TIME0_REG_OFF);
	dphy_rx_time1[sel] =
	    (DPHY_RX_TIME1_REG_t *) (addr_base + DPHY_RX_TIME1_REG_OFF);
	dphy_rx_time2[sel] =
	    (DPHY_RX_TIME2_REG_t *) (addr_base + DPHY_RX_TIME2_REG_OFF);
	dphy_rx_time3[sel] =
	    (DPHY_RX_TIME3_REG_t *) (addr_base + DPHY_RX_TIME3_REG_OFF);
	dphy_ana0[sel] = (DPHY_ANA0_REG_t *) (addr_base + DPHY_ANA0_REG_OFF);
	dphy_ana1[sel] = (DPHY_ANA1_REG_t *) (addr_base + DPHY_ANA1_REG_OFF);
	dphy_ana2[sel] = (DPHY_ANA2_REG_t *) (addr_base + DPHY_ANA2_REG_OFF);
	dphy_ana3[sel] = (DPHY_ANA3_REG_t *) (addr_base + DPHY_ANA3_REG_OFF);
	dphy_ana4[sel] = (DPHY_ANA4_REG_t *) (addr_base + DPHY_ANA4_REG_OFF);
	dphy_int_en0[sel] =
	    (DPHY_INT_EN0_REG_t *) (addr_base + DPHY_INT_EN0_REG_OFF);
	dphy_int_en1[sel] =
	    (DPHY_INT_EN1_REG_t *) (addr_base + DPHY_INT_EN1_REG_OFF);
	dphy_int_en2[sel] =
	    (DPHY_INT_EN2_REG_t *) (addr_base + DPHY_INT_EN2_REG_OFF);
	dphy_int_pd0[sel] =
	    (DPHY_INT_PD0_REG_t *) (addr_base + DPHY_INT_PD0_REG_OFF);
	dphy_int_pd1[sel] =
	    (DPHY_INT_PD1_REG_t *) (addr_base + DPHY_INT_PD1_REG_OFF);
	dphy_int_pd2[sel] =
	    (DPHY_INT_PD2_REG_t *) (addr_base + DPHY_INT_PD2_REG_OFF);
	dphy_dbg0[sel] = (DPHY_DBG0_REG_t *) (addr_base + DPHY_DBG0_REG_OFF);
	dphy_dbg1[sel] = (DPHY_DBG1_REG_t *) (addr_base + DPHY_DBG1_REG_OFF);
	dphy_dbg2[sel] = (DPHY_DBG2_REG_t *) (addr_base + DPHY_DBG2_REG_OFF);
	dphy_dbg3[sel] = (DPHY_DBG3_REG_t *) (addr_base + DPHY_DBG3_REG_OFF);
	dphy_dbg4[sel] = (DPHY_DBG4_REG_t *) (addr_base + DPHY_DBG4_REG_OFF);
	dphy_dbg5[sel] = (DPHY_DBG5_REG_t *) (addr_base + DPHY_DBG5_REG_OFF);

	return 0;
}

void dphy_enable(unsigned int sel)
{
	dphy_ctl[sel]->bits.module_en = 1;
}

void dphy_disable(unsigned int sel)
{
	dphy_ctl[sel]->bits.module_en = 0;
}

void dphy_set_data_lane(unsigned int sel, unsigned char lane_num)
{
	dphy_ctl[sel]->bits.lane_num = lane_num - 1;
}

unsigned char dphy_get_data_lane(unsigned int sel)
{
	return dphy_ctl[sel]->bits.lane_num + 1;
}

void dphy_rx_enable(unsigned int sel, unsigned char lane_num)
{
	dphy_rx_ctl[sel]->bits.rx_clk_force = 1;
	switch (lane_num - 1) {
	case 3:
		dphy_rx_ctl[sel]->bits.rx_d3_force = 1;
	case 2:
		dphy_rx_ctl[sel]->bits.rx_d2_force = 1;
	case 1:
		dphy_rx_ctl[sel]->bits.rx_d1_force = 1;
	case 0:
		dphy_rx_ctl[sel]->bits.rx_d0_force = 1;
		break;
	default:
		dphy_rx_ctl[sel]->bits.rx_d3_force = 0;
		dphy_rx_ctl[sel]->bits.rx_d2_force = 0;
		dphy_rx_ctl[sel]->bits.rx_d1_force = 0;
		dphy_rx_ctl[sel]->bits.rx_d0_force = 0;
		break;
	}
}

void dphy_rx_disable(unsigned int sel, unsigned char lane_num)
{
	dphy_rx_ctl[sel]->bits.rx_clk_force = 0;
	switch (lane_num - 1) {
	case 3:
		dphy_rx_ctl[sel]->bits.rx_d3_force = 0;
	case 2:
		dphy_rx_ctl[sel]->bits.rx_d2_force = 0;
	case 1:
		dphy_rx_ctl[sel]->bits.rx_d1_force = 0;
	case 0:
		dphy_rx_ctl[sel]->bits.rx_d0_force = 0;
		break;
	default:
		dphy_rx_ctl[sel]->bits.rx_d3_force = 0;
		dphy_rx_ctl[sel]->bits.rx_d2_force = 0;
		dphy_rx_ctl[sel]->bits.rx_d1_force = 0;
		dphy_rx_ctl[sel]->bits.rx_d0_force = 0;
		break;
	}
}

void dphy_rx_dbc_enable(unsigned int sel)
{
	dphy_rx_ctl[sel]->bits.dbc_en = 1;
}

void dphy_rx_dbc_disable(unsigned int sel)
{
	dphy_rx_ctl[sel]->bits.dbc_en = 0;
}

void dphy_rx_hs_clk_miss_cnt_enable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.hsrx_clk_miss_en = 1;
}

void dphy_rx_hs_clk_miss_cnt_disable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.hsrx_clk_miss_en = 0;
}

void dphy_rx_hs_sync_cnt_enable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.hsrx_sync_err_to_en = 1;
}

void dphy_rx_hs_sync_cnt_disable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.hsrx_sync_err_to_en = 0;
}

void dphy_rx_lp_to_cnt_enable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.lprx_to_en = 1;
}

void dphy_rx_lp_to_cnt_disable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.lprx_to_en = 0;
}

void dphy_rx_freq_cnt_enable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.freq_cnt_en = 1;
}

void dphy_rx_freq_cnt_disable(unsigned int sel)
{
	dphy_rx_time0[sel]->bits.freq_cnt_en = 0;
}

void dphy_rx_set_hs_clk_miss(unsigned int sel, unsigned char cnt)
{
	dphy_rx_time0[sel]->bits.hsrx_clk_miss = cnt;
}

unsigned char dphy_rx_get_hs_clk_miss(unsigned int sel)
{
	return dphy_rx_time0[sel]->bits.hsrx_clk_miss;
}

void dphy_rx_set_hs_sync_to(unsigned int sel, unsigned char cnt)
{
	dphy_rx_time0[sel]->bits.hsrx_sync_err_to = cnt;
}

unsigned char dphy_rx_get_hs_sync_to(unsigned int sel)
{
	return dphy_rx_time0[sel]->bits.hsrx_sync_err_to;
}

void dphy_rx_set_lp_to(unsigned int sel, unsigned char cnt)
{
	dphy_rx_time0[sel]->bits.lprx_to = cnt;
}

unsigned char dphy_rx_get_lp_to(unsigned int sel)
{
	return dphy_rx_time0[sel]->bits.lprx_to;
}

void dphy_rx_set_rx_dly(unsigned int sel, unsigned short cnt)
{
	dphy_rx_time1[sel]->bits.rx_dly = cnt;
}

unsigned short dphy_rx_get_rx_dly(unsigned int sel)
{
	return dphy_rx_time1[sel]->bits.rx_dly;
}

void dphy_rx_set_lprst_dly(unsigned int sel, unsigned char cnt)
{
	dphy_rx_time3[sel]->bits.lprst_dly = cnt;
}

unsigned char dphy_rx_get_lprst_dly(unsigned int sel)
{
	return dphy_rx_time3[sel]->bits.lprst_dly;
}

void dphy_rx_set_entm_to_enrx_dly(unsigned int sel, unsigned char cnt)
{
	dphy_rx_time2[sel]->bits.hsrx_ana0_set = cnt;
}

unsigned char dphy_rx_get_entm_to_enrx_dly(unsigned int sel)
{
	return dphy_rx_time2[sel]->bits.hsrx_ana0_set;
}

void dphy_rx_set_lp_ulps_wp(unsigned int sel, unsigned int cnt)
{
	dphy_rx_time1[sel]->bits.lprx_ulps_wp = cnt;
}

unsigned int dphy_rx_get_lp_ulps_wp(unsigned int sel)
{
	return dphy_rx_time1[sel]->bits.lprx_ulps_wp;
}

unsigned short dphy_rx_get_freq_cnt(unsigned int sel)
{
	return dphy_rx_time3[sel]->bits.freq_cnt;
}

void dphy_int_enable(unsigned int sel, enum dphy_int dphy_int)
{
	switch (dphy_int) {
	case SOT_D0:
		dphy_int_en0[sel]->bits.sot_d0_int = 1;
		break;
	case SOT_D1:
		dphy_int_en0[sel]->bits.sot_d1_int = 1;
		break;
	case SOT_D2:
		dphy_int_en0[sel]->bits.sot_d2_int = 1;
		break;
	case SOT_D3:
		dphy_int_en0[sel]->bits.sot_d3_int = 1;
		break;
	case SOT_ERR_D0:
		dphy_int_en0[sel]->bits.sot_err_d0_int = 1;
		break;
	case SOT_ERR_D1:
		dphy_int_en0[sel]->bits.sot_err_d1_int = 1;
		break;
	case SOT_ERR_D2:
		dphy_int_en0[sel]->bits.sot_err_d2_int = 1;
		break;
	case SOT_ERR_D3:
		dphy_int_en0[sel]->bits.sot_err_d3_int = 1;
		break;
	case SOT_SYNC_ERR_D0:
		dphy_int_en0[sel]->bits.sot_sync_err_d0_int = 1;
		break;
	case SOT_SYNC_ERR_D1:
		dphy_int_en0[sel]->bits.sot_sync_err_d1_int = 1;
		break;
	case SOT_SYNC_ERR_D2:
		dphy_int_en0[sel]->bits.sot_sync_err_d2_int = 1;
		break;
	case SOT_SYNC_ERR_D3:
		dphy_int_en0[sel]->bits.sot_sync_err_d3_int = 1;
		break;
	case RX_ALG_ERR_D0:
		dphy_int_en0[sel]->bits.rx_alg_err_d0_int = 1;
		break;
	case RX_ALG_ERR_D1:
		dphy_int_en0[sel]->bits.rx_alg_err_d1_int = 1;
		break;
	case RX_ALG_ERR_D2:
		dphy_int_en0[sel]->bits.rx_alg_err_d2_int = 1;
		break;
	case RX_ALG_ERR_D3:
		dphy_int_en0[sel]->bits.rx_alg_err_d3_int = 1;
		break;
	case CD_LP0_ERR_CLK:
		dphy_int_en0[sel]->bits.cd_lp0_err_clk_int = 1;
		break;
	case CD_LP1_ERR_CLK:
		dphy_int_en0[sel]->bits.cd_lp1_err_clk_int = 1;
		break;
	case CD_LP0_ERR_D0:
		dphy_int_en0[sel]->bits.cd_lp0_err_d0_int = 1;
		break;
	case CD_LP1_ERR_D0:
		dphy_int_en0[sel]->bits.cd_lp1_err_d0_int = 1;
		break;
	case CD_LP0_ERR_D1:
		dphy_int_en0[sel]->bits.cd_lp0_err_d1_int = 1;
		break;
	case CD_LP1_ERR_D1:
		dphy_int_en0[sel]->bits.cd_lp1_err_d1_int = 1;
		break;
	case CD_LP0_ERR_D2:
		dphy_int_en0[sel]->bits.cd_lp0_err_d2_int = 1;
		break;
	case CD_LP1_ERR_D2:
		dphy_int_en0[sel]->bits.cd_lp1_err_d2_int = 1;
		break;
	case CD_LP0_ERR_D3:
		dphy_int_en0[sel]->bits.cd_lp0_err_d3_int = 1;
		break;
	case CD_LP1_ERR_D3:
		dphy_int_en0[sel]->bits.cd_lp1_err_d3_int = 1;
		break;
	case ULPS_D0:
		dphy_int_en1[sel]->bits.ulps_d0_int = 1;
		break;
	case ULPS_D1:
		dphy_int_en1[sel]->bits.ulps_d1_int = 1;
		break;
	case ULPS_D2:
		dphy_int_en1[sel]->bits.ulps_d2_int = 1;
		break;
	case ULPS_D3:
		dphy_int_en1[sel]->bits.ulps_d3_int = 1;
		break;
	case ULPS_WP_D0:
		dphy_int_en1[sel]->bits.ulps_wp_d0_int = 1;
		break;
	case ULPS_WP_D1:
		dphy_int_en1[sel]->bits.ulps_wp_d1_int = 1;
		break;
	case ULPS_WP_D2:
		dphy_int_en1[sel]->bits.ulps_wp_d2_int = 1;
		break;
	case ULPS_WP_D3:
		dphy_int_en1[sel]->bits.ulps_wp_d3_int = 1;
		break;
	case ULPS_CLK:
		dphy_int_en1[sel]->bits.ulps_clk_int = 1;
		break;
	case ULPS_WP_CLK:
		dphy_int_en1[sel]->bits.ulps_wp_clk_int = 1;
		break;
	case LPDT_D0:
		dphy_int_en1[sel]->bits.lpdt_d0_int = 1;
		break;
	case RX_TRND_D0:
		dphy_int_en1[sel]->bits.rx_trnd_d0_int = 1;
		break;
	case TX_TRND_ERR_D0:
		dphy_int_en1[sel]->bits.tx_trnd_err_d0_int = 1;
		break;
	case UNDEF1_D0:
		dphy_int_en1[sel]->bits.undef1_d0_int = 1;
		break;
	case UNDEF2_D0:
		dphy_int_en1[sel]->bits.undef2_d0_int = 1;
		break;
	case UNDEF3_D0:
		dphy_int_en1[sel]->bits.undef3_d0_int = 1;
		break;
	case UNDEF4_D0:
		dphy_int_en1[sel]->bits.undef4_d0_int = 1;
		break;
	case UNDEF5_D0:
		dphy_int_en1[sel]->bits.undef5_d0_int = 1;
		break;
	case RST_D0:
		dphy_int_en1[sel]->bits.rst_d0_int = 1;
		break;
	case RST_D1:
		dphy_int_en1[sel]->bits.rst_d1_int = 1;
		break;
	case RST_D2:
		dphy_int_en1[sel]->bits.rst_d2_int = 1;
		break;
	case RST_D3:
		dphy_int_en1[sel]->bits.rst_d3_int = 1;
		break;
	case ESC_CMD_ERR_D0:
		dphy_int_en1[sel]->bits.esc_cmd_err_d0_int = 1;
		break;
	case ESC_CMD_ERR_D1:
		dphy_int_en1[sel]->bits.esc_cmd_err_d1_int = 1;
		break;
	case ESC_CMD_ERR_D2:
		dphy_int_en1[sel]->bits.esc_cmd_err_d2_int = 1;
		break;
	case ESC_CMD_ERR_D3:
		dphy_int_en1[sel]->bits.esc_cmd_err_d3_int = 1;
		break;
	case FALSE_CTL_D0:
		dphy_int_en1[sel]->bits.false_ctl_d0_int = 1;
		break;
	case FALSE_CTL_D1:
		dphy_int_en1[sel]->bits.false_ctl_d1_int = 1;
		break;
	case FALSE_CTL_D2:
		dphy_int_en1[sel]->bits.false_ctl_d2_int = 1;
		break;
	case FALSE_CTL_D3:
		dphy_int_en1[sel]->bits.false_ctl_d3_int = 1;
		break;
	default:
		break;
	}
}

void dphy_int_disable(unsigned int sel, enum dphy_int dphy_int)
{
	switch (dphy_int) {
	case SOT_D0:
		dphy_int_en0[sel]->bits.sot_d0_int = 0;
		break;
	case SOT_D1:
		dphy_int_en0[sel]->bits.sot_d1_int = 0;
		break;
	case SOT_D2:
		dphy_int_en0[sel]->bits.sot_d2_int = 0;
		break;
	case SOT_D3:
		dphy_int_en0[sel]->bits.sot_d3_int = 0;
		break;
	case SOT_ERR_D0:
		dphy_int_en0[sel]->bits.sot_err_d0_int = 0;
		break;
	case SOT_ERR_D1:
		dphy_int_en0[sel]->bits.sot_err_d1_int = 0;
		break;
	case SOT_ERR_D2:
		dphy_int_en0[sel]->bits.sot_err_d2_int = 0;
		break;
	case SOT_ERR_D3:
		dphy_int_en0[sel]->bits.sot_err_d3_int = 0;
		break;
	case SOT_SYNC_ERR_D0:
		dphy_int_en0[sel]->bits.sot_sync_err_d0_int = 0;
		break;
	case SOT_SYNC_ERR_D1:
		dphy_int_en0[sel]->bits.sot_sync_err_d1_int = 0;
		break;
	case SOT_SYNC_ERR_D2:
		dphy_int_en0[sel]->bits.sot_sync_err_d2_int = 0;
		break;
	case SOT_SYNC_ERR_D3:
		dphy_int_en0[sel]->bits.sot_sync_err_d3_int = 0;
		break;
	case RX_ALG_ERR_D0:
		dphy_int_en0[sel]->bits.rx_alg_err_d0_int = 0;
		break;
	case RX_ALG_ERR_D1:
		dphy_int_en0[sel]->bits.rx_alg_err_d1_int = 0;
		break;
	case RX_ALG_ERR_D2:
		dphy_int_en0[sel]->bits.rx_alg_err_d2_int = 0;
		break;
	case RX_ALG_ERR_D3:
		dphy_int_en0[sel]->bits.rx_alg_err_d3_int = 0;
		break;
	case CD_LP0_ERR_CLK:
		dphy_int_en0[sel]->bits.cd_lp0_err_clk_int = 0;
		break;
	case CD_LP1_ERR_CLK:
		dphy_int_en0[sel]->bits.cd_lp1_err_clk_int = 0;
		break;
	case CD_LP0_ERR_D0:
		dphy_int_en0[sel]->bits.cd_lp0_err_d0_int = 0;
		break;
	case CD_LP1_ERR_D0:
		dphy_int_en0[sel]->bits.cd_lp1_err_d0_int = 0;
		break;
	case CD_LP0_ERR_D1:
		dphy_int_en0[sel]->bits.cd_lp0_err_d1_int = 0;
		break;
	case CD_LP1_ERR_D1:
		dphy_int_en0[sel]->bits.cd_lp1_err_d1_int = 0;
		break;
	case CD_LP0_ERR_D2:
		dphy_int_en0[sel]->bits.cd_lp0_err_d2_int = 0;
		break;
	case CD_LP1_ERR_D2:
		dphy_int_en0[sel]->bits.cd_lp1_err_d2_int = 0;
		break;
	case CD_LP0_ERR_D3:
		dphy_int_en0[sel]->bits.cd_lp0_err_d3_int = 0;
		break;
	case CD_LP1_ERR_D3:
		dphy_int_en0[sel]->bits.cd_lp1_err_d3_int = 0;
		break;
	case ULPS_D0:
		dphy_int_en1[sel]->bits.ulps_d0_int = 0;
		break;
	case ULPS_D1:
		dphy_int_en1[sel]->bits.ulps_d1_int = 0;
		break;
	case ULPS_D2:
		dphy_int_en1[sel]->bits.ulps_d2_int = 0;
		break;
	case ULPS_D3:
		dphy_int_en1[sel]->bits.ulps_d3_int = 0;
		break;
	case ULPS_WP_D0:
		dphy_int_en1[sel]->bits.ulps_wp_d0_int = 0;
		break;
	case ULPS_WP_D1:
		dphy_int_en1[sel]->bits.ulps_wp_d1_int = 0;
		break;
	case ULPS_WP_D2:
		dphy_int_en1[sel]->bits.ulps_wp_d2_int = 0;
		break;
	case ULPS_WP_D3:
		dphy_int_en1[sel]->bits.ulps_wp_d3_int = 0;
		break;
	case ULPS_CLK:
		dphy_int_en1[sel]->bits.ulps_clk_int = 0;
		break;
	case ULPS_WP_CLK:
		dphy_int_en1[sel]->bits.ulps_wp_clk_int = 0;
		break;
	case LPDT_D0:
		dphy_int_en1[sel]->bits.lpdt_d0_int = 0;
		break;
	case RX_TRND_D0:
		dphy_int_en1[sel]->bits.rx_trnd_d0_int = 0;
		break;
	case TX_TRND_ERR_D0:
		dphy_int_en1[sel]->bits.tx_trnd_err_d0_int = 0;
		break;
	case UNDEF1_D0:
		dphy_int_en1[sel]->bits.undef1_d0_int = 0;
		break;
	case UNDEF2_D0:
		dphy_int_en1[sel]->bits.undef2_d0_int = 0;
		break;
	case UNDEF3_D0:
		dphy_int_en1[sel]->bits.undef3_d0_int = 0;
		break;
	case UNDEF4_D0:
		dphy_int_en1[sel]->bits.undef4_d0_int = 0;
		break;
	case UNDEF5_D0:
		dphy_int_en1[sel]->bits.undef5_d0_int = 0;
		break;
	case RST_D0:
		dphy_int_en1[sel]->bits.rst_d0_int = 0;
		break;
	case RST_D1:
		dphy_int_en1[sel]->bits.rst_d1_int = 0;
		break;
	case RST_D2:
		dphy_int_en1[sel]->bits.rst_d2_int = 0;
		break;
	case RST_D3:
		dphy_int_en1[sel]->bits.rst_d3_int = 0;
		break;
	case ESC_CMD_ERR_D0:
		dphy_int_en1[sel]->bits.esc_cmd_err_d0_int = 0;
		break;
	case ESC_CMD_ERR_D1:
		dphy_int_en1[sel]->bits.esc_cmd_err_d1_int = 0;
		break;
	case ESC_CMD_ERR_D2:
		dphy_int_en1[sel]->bits.esc_cmd_err_d2_int = 0;
		break;
	case ESC_CMD_ERR_D3:
		dphy_int_en1[sel]->bits.esc_cmd_err_d3_int = 0;
		break;
	case FALSE_CTL_D0:
		dphy_int_en1[sel]->bits.false_ctl_d0_int = 0;
		break;
	case FALSE_CTL_D1:
		dphy_int_en1[sel]->bits.false_ctl_d1_int = 0;
		break;
	case FALSE_CTL_D2:
		dphy_int_en1[sel]->bits.false_ctl_d2_int = 0;
		break;
	case FALSE_CTL_D3:
		dphy_int_en1[sel]->bits.false_ctl_d3_int = 0;
		break;
	default:
		break;
	}
}

int dphy_get_int_status(unsigned int sel, enum dphy_int dphy_int)
{
	switch (dphy_int) {
	case SOT_D0:
		return dphy_int_pd0[sel]->bits.sot_d0_pd;
	case SOT_D1:
		return dphy_int_pd0[sel]->bits.sot_d1_pd;
	case SOT_D2:
		return dphy_int_pd0[sel]->bits.sot_d2_pd;
	case SOT_D3:
		return dphy_int_pd0[sel]->bits.sot_d3_pd;
	case SOT_ERR_D0:
		return dphy_int_pd0[sel]->bits.sot_err_d0_pd;
	case SOT_ERR_D1:
		return dphy_int_pd0[sel]->bits.sot_err_d1_pd;
	case SOT_ERR_D2:
		return dphy_int_pd0[sel]->bits.sot_err_d2_pd;
	case SOT_ERR_D3:
		return dphy_int_pd0[sel]->bits.sot_err_d3_pd;
	case SOT_SYNC_ERR_D0:
		return dphy_int_pd0[sel]->bits.sot_sync_err_d0_pd;
	case SOT_SYNC_ERR_D1:
		return dphy_int_pd0[sel]->bits.sot_sync_err_d1_pd;
	case SOT_SYNC_ERR_D2:
		return dphy_int_pd0[sel]->bits.sot_sync_err_d2_pd;
	case SOT_SYNC_ERR_D3:
		return dphy_int_pd0[sel]->bits.sot_sync_err_d3_pd;
	case RX_ALG_ERR_D0:
		return dphy_int_pd0[sel]->bits.rx_alg_err_d0_pd;
	case RX_ALG_ERR_D1:
		return dphy_int_pd0[sel]->bits.rx_alg_err_d1_pd;
	case RX_ALG_ERR_D2:
		return dphy_int_pd0[sel]->bits.rx_alg_err_d2_pd;
	case RX_ALG_ERR_D3:
		return dphy_int_pd0[sel]->bits.rx_alg_err_d3_pd;
	case CD_LP0_ERR_CLK:
		return dphy_int_pd0[sel]->bits.cd_lp0_err_clk_pd;
	case CD_LP1_ERR_CLK:
		return dphy_int_pd0[sel]->bits.cd_lp1_err_clk_pd;
	case CD_LP0_ERR_D0:
		return dphy_int_pd0[sel]->bits.cd_lp0_err_d0_pd;
	case CD_LP1_ERR_D0:
		return dphy_int_pd0[sel]->bits.cd_lp1_err_d0_pd;
	case CD_LP0_ERR_D1:
		return dphy_int_pd0[sel]->bits.cd_lp0_err_d1_pd;
	case CD_LP1_ERR_D1:
		return dphy_int_pd0[sel]->bits.cd_lp1_err_d1_pd;
	case CD_LP0_ERR_D2:
		return dphy_int_pd0[sel]->bits.cd_lp0_err_d2_pd;
	case CD_LP1_ERR_D2:
		return dphy_int_pd0[sel]->bits.cd_lp1_err_d2_pd;
	case CD_LP0_ERR_D3:
		return dphy_int_pd0[sel]->bits.cd_lp0_err_d3_pd;
	case CD_LP1_ERR_D3:
		return dphy_int_pd0[sel]->bits.cd_lp1_err_d3_pd;
	case ULPS_D0:
		return dphy_int_pd1[sel]->bits.ulps_d0_pd;
	case ULPS_D1:
		return dphy_int_pd1[sel]->bits.ulps_d1_pd;
	case ULPS_D2:
		return dphy_int_pd1[sel]->bits.ulps_d2_pd;
	case ULPS_D3:
		return dphy_int_pd1[sel]->bits.ulps_d3_pd;
	case ULPS_WP_D0:
		return dphy_int_pd1[sel]->bits.ulps_wp_d0_pd;
	case ULPS_WP_D1:
		return dphy_int_pd1[sel]->bits.ulps_wp_d1_pd;
	case ULPS_WP_D2:
		return dphy_int_pd1[sel]->bits.ulps_wp_d2_pd;
	case ULPS_WP_D3:
		return dphy_int_pd1[sel]->bits.ulps_wp_d3_pd;
	case ULPS_CLK:
		return dphy_int_pd1[sel]->bits.ulps_clk_pd;
	case ULPS_WP_CLK:
		return dphy_int_pd1[sel]->bits.ulps_wp_clk_pd;
	case LPDT_D0:
		return dphy_int_pd1[sel]->bits.lpdt_d0_pd;
	case RX_TRND_D0:
		return dphy_int_pd1[sel]->bits.rx_trnd_d0_pd;
	case TX_TRND_ERR_D0:
		return dphy_int_pd1[sel]->bits.tx_trnd_err_d0_pd;
	case UNDEF1_D0:
		return dphy_int_pd1[sel]->bits.undef1_d0_pd;
	case UNDEF2_D0:
		return dphy_int_pd1[sel]->bits.undef2_d0_pd;
	case UNDEF3_D0:
		return dphy_int_pd1[sel]->bits.undef3_d0_pd;
	case UNDEF4_D0:
		return dphy_int_pd1[sel]->bits.undef4_d0_pd;
	case UNDEF5_D0:
		return dphy_int_pd1[sel]->bits.undef5_d0_pd;
	case RST_D0:
		return dphy_int_pd1[sel]->bits.rst_d0_pd;
	case RST_D1:
		return dphy_int_pd1[sel]->bits.rst_d1_pd;
	case RST_D2:
		return dphy_int_pd1[sel]->bits.rst_d2_pd;
	case RST_D3:
		return dphy_int_pd1[sel]->bits.rst_d3_pd;
	case ESC_CMD_ERR_D0:
		return dphy_int_pd1[sel]->bits.esc_cmd_err_d0_pd;
	case ESC_CMD_ERR_D1:
		return dphy_int_pd1[sel]->bits.esc_cmd_err_d1_pd;
	case ESC_CMD_ERR_D2:
		return dphy_int_pd1[sel]->bits.esc_cmd_err_d2_pd;
	case ESC_CMD_ERR_D3:
		return dphy_int_pd1[sel]->bits.esc_cmd_err_d3_pd;
	case FALSE_CTL_D0:
		return dphy_int_pd1[sel]->bits.false_ctl_d0_pd;
	case FALSE_CTL_D1:
		return dphy_int_pd1[sel]->bits.false_ctl_d1_pd;
	case FALSE_CTL_D2:
		return dphy_int_pd1[sel]->bits.false_ctl_d2_pd;
	case FALSE_CTL_D3:
		return dphy_int_pd1[sel]->bits.false_ctl_d3_pd;
	default:
		return -1;
	}
}

void dphy_clear_int_status(unsigned int sel, enum dphy_int dphy_int)
{
	switch (dphy_int) {
	case SOT_D0:
		dphy_int_pd0[sel]->bits.sot_d0_pd = 1;
		break;
	case SOT_D1:
		dphy_int_pd0[sel]->bits.sot_d1_pd = 1;
		break;
	case SOT_D2:
		dphy_int_pd0[sel]->bits.sot_d2_pd = 1;
		break;
	case SOT_D3:
		dphy_int_pd0[sel]->bits.sot_d3_pd = 1;
		break;
	case SOT_ERR_D0:
		dphy_int_pd0[sel]->bits.sot_err_d0_pd = 1;
		break;
	case SOT_ERR_D1:
		dphy_int_pd0[sel]->bits.sot_err_d1_pd = 1;
		break;
	case SOT_ERR_D2:
		dphy_int_pd0[sel]->bits.sot_err_d2_pd = 1;
		break;
	case SOT_ERR_D3:
		dphy_int_pd0[sel]->bits.sot_err_d3_pd = 1;
		break;
	case SOT_SYNC_ERR_D0:
		dphy_int_pd0[sel]->bits.sot_sync_err_d0_pd = 1;
		break;
	case SOT_SYNC_ERR_D1:
		dphy_int_pd0[sel]->bits.sot_sync_err_d1_pd = 1;
		break;
	case SOT_SYNC_ERR_D2:
		dphy_int_pd0[sel]->bits.sot_sync_err_d2_pd = 1;
		break;
	case SOT_SYNC_ERR_D3:
		dphy_int_pd0[sel]->bits.sot_sync_err_d3_pd = 1;
		break;
	case RX_ALG_ERR_D0:
		dphy_int_pd0[sel]->bits.rx_alg_err_d0_pd = 1;
		break;
	case RX_ALG_ERR_D1:
		dphy_int_pd0[sel]->bits.rx_alg_err_d1_pd = 1;
		break;
	case RX_ALG_ERR_D2:
		dphy_int_pd0[sel]->bits.rx_alg_err_d2_pd = 1;
		break;
	case RX_ALG_ERR_D3:
		dphy_int_pd0[sel]->bits.rx_alg_err_d3_pd = 1;
		break;
	case CD_LP0_ERR_CLK:
		dphy_int_pd0[sel]->bits.cd_lp0_err_clk_pd = 1;
		break;
	case CD_LP1_ERR_CLK:
		dphy_int_pd0[sel]->bits.cd_lp1_err_clk_pd = 1;
		break;
	case CD_LP0_ERR_D0:
		dphy_int_pd0[sel]->bits.cd_lp0_err_d0_pd = 1;
		break;
	case CD_LP1_ERR_D0:
		dphy_int_pd0[sel]->bits.cd_lp1_err_d0_pd = 1;
		break;
	case CD_LP0_ERR_D1:
		dphy_int_pd0[sel]->bits.cd_lp0_err_d1_pd = 1;
		break;
	case CD_LP1_ERR_D1:
		dphy_int_pd0[sel]->bits.cd_lp1_err_d1_pd = 1;
		break;
	case CD_LP0_ERR_D2:
		dphy_int_pd0[sel]->bits.cd_lp0_err_d2_pd = 1;
		break;
	case CD_LP1_ERR_D2:
		dphy_int_pd0[sel]->bits.cd_lp1_err_d2_pd = 1;
		break;
	case CD_LP0_ERR_D3:
		dphy_int_pd0[sel]->bits.cd_lp0_err_d3_pd = 1;
		break;
	case CD_LP1_ERR_D3:
		dphy_int_pd0[sel]->bits.cd_lp1_err_d3_pd = 1;
		break;
	case ULPS_D0:
		dphy_int_pd1[sel]->bits.ulps_d0_pd = 1;
		break;
	case ULPS_D1:
		dphy_int_pd1[sel]->bits.ulps_d1_pd = 1;
		break;
	case ULPS_D2:
		dphy_int_pd1[sel]->bits.ulps_d2_pd = 1;
		break;
	case ULPS_D3:
		dphy_int_pd1[sel]->bits.ulps_d3_pd = 1;
		break;
	case ULPS_WP_D0:
		dphy_int_pd1[sel]->bits.ulps_wp_d0_pd = 1;
		break;
	case ULPS_WP_D1:
		dphy_int_pd1[sel]->bits.ulps_wp_d1_pd = 1;
		break;
	case ULPS_WP_D2:
		dphy_int_pd1[sel]->bits.ulps_wp_d2_pd = 1;
		break;
	case ULPS_WP_D3:
		dphy_int_pd1[sel]->bits.ulps_wp_d3_pd = 1;
		break;
	case ULPS_CLK:
		dphy_int_pd1[sel]->bits.ulps_clk_pd = 1;
		break;
	case ULPS_WP_CLK:
		dphy_int_pd1[sel]->bits.ulps_wp_clk_pd = 1;
		break;
	case LPDT_D0:
		dphy_int_pd1[sel]->bits.lpdt_d0_pd = 1;
		break;
	case RX_TRND_D0:
		dphy_int_pd1[sel]->bits.rx_trnd_d0_pd = 1;
		break;
	case TX_TRND_ERR_D0:
		dphy_int_pd1[sel]->bits.tx_trnd_err_d0_pd = 1;
		break;
	case UNDEF1_D0:
		dphy_int_pd1[sel]->bits.undef1_d0_pd = 1;
		break;
	case UNDEF2_D0:
		dphy_int_pd1[sel]->bits.undef2_d0_pd = 1;
		break;
	case UNDEF3_D0:
		dphy_int_pd1[sel]->bits.undef3_d0_pd = 1;
		break;
	case UNDEF4_D0:
		dphy_int_pd1[sel]->bits.undef4_d0_pd = 1;
		break;
	case UNDEF5_D0:
		dphy_int_pd1[sel]->bits.undef5_d0_pd = 1;
		break;
	case RST_D0:
		dphy_int_pd1[sel]->bits.rst_d0_pd = 1;
		break;
	case RST_D1:
		dphy_int_pd1[sel]->bits.rst_d1_pd = 1;
		break;
	case RST_D2:
		dphy_int_pd1[sel]->bits.rst_d2_pd = 1;
		break;
	case RST_D3:
		dphy_int_pd1[sel]->bits.rst_d3_pd = 1;
		break;
	case ESC_CMD_ERR_D0:
		dphy_int_pd1[sel]->bits.esc_cmd_err_d0_pd = 1;
		break;
	case ESC_CMD_ERR_D1:
		dphy_int_pd1[sel]->bits.esc_cmd_err_d1_pd = 1;
		break;
	case ESC_CMD_ERR_D2:
		dphy_int_pd1[sel]->bits.esc_cmd_err_d2_pd = 1;
		break;
	case ESC_CMD_ERR_D3:
		dphy_int_pd1[sel]->bits.esc_cmd_err_d3_pd = 1;
		break;
	case FALSE_CTL_D0:
		dphy_int_pd1[sel]->bits.false_ctl_d0_pd = 1;
		break;
	case FALSE_CTL_D1:
		dphy_int_pd1[sel]->bits.false_ctl_d1_pd = 1;
		break;
	case FALSE_CTL_D2:
		dphy_int_pd1[sel]->bits.false_ctl_d2_pd = 1;
		break;
	case FALSE_CTL_D3:
		dphy_int_pd1[sel]->bits.false_ctl_d3_pd = 1;
		break;
	default:
		break;
	}
}

enum dphy_lane_state dphy_get_lane_state(unsigned int sel, enum dphy_lane lane)
{
	switch (lane) {
	case LANE_D0:
		return (enum dphy_lane_state)dphy_dbg3[sel]->bits.lprx_sta_d0;
	case LANE_D1:
		return (enum dphy_lane_state)dphy_dbg3[sel]->bits.lprx_sta_d1;
	case LANE_D2:
		return (enum dphy_lane_state)dphy_dbg3[sel]->bits.lprx_sta_d2;
	case LANE_D3:
		return (enum dphy_lane_state)dphy_dbg3[sel]->bits.lprx_sta_d3;
	case LANE_CLK:
		return (enum dphy_lane_state)dphy_dbg3[sel]->bits.lprx_sta_clk;
	default:
		return -1;
	}
}

unsigned char dphy_get_hs_data(unsigned int sel, enum dphy_lane lane)
{
	switch (lane) {
	case LANE_D0:
		return dphy_dbg5[sel]->bits.hsrx_data0;
	case LANE_D1:
		return dphy_dbg5[sel]->bits.hsrx_data1;
	case LANE_D2:
		return dphy_dbg5[sel]->bits.hsrx_data2;
	case LANE_D3:
		return dphy_dbg5[sel]->bits.hsrx_data3;
	default:
		return -1;
	}
}

void dphy_ana_init(unsigned int sel)
{
	dphy_ana0[sel]->bits.reg_pws = 1;
	dphy_ana0[sel]->bits.reg_sfb = 2;
	dphy_ana0[sel]->bits.reg_slv = 7;
	dphy_ana1[sel]->bits.reg_svtt = 4;
	dphy_ana0[sel]->bits.reg_dmpc = 0;
	dphy_ana0[sel]->bits.reg_dmp = 0;
	dphy_ana4[sel]->bits.reg_dmplvc = 1;
	dphy_ana4[sel]->bits.reg_dmplvd = 1;

	dphy_ana2[sel]->bits.enib = 1;
	dphy_ana3[sel]->bits.enldor = 1;
	dphy_ana3[sel]->bits.enldod = 1;
	dphy_ana3[sel]->bits.enldoc = 1;
	udelay(3);
	dphy_ana3[sel]->bits.envttc = 0;

}

void dphy_ana_exit(unsigned int sel)
{
	dphy_ana3[sel]->bits.envttc = 1;
	udelay(3);
	dphy_ana3[sel]->bits.enldoc = 0;
	dphy_ana3[sel]->bits.enldod = 0;
	dphy_ana3[sel]->bits.enldor = 0;
	dphy_ana2[sel]->bits.enib = 0;
}

/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-cci/bsp_cci.c
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

#include "bsp_cci.h"
#include "../platform/platform_cfg.h"

#include <waitqueue.h>
#include <melis/log.h>
//#include <v4l2-types.h>

rt_wqueue_t wait[MAX_CSIC_CCI_NUM];

bool status_err_flag[MAX_CSIC_CCI_NUM] = { 0 };
bool register_flag[MAX_CSIC_CCI_NUM] = { 0 };
struct rt_mutex cci_mutex[MAX_CSIC_CCI_NUM];
struct rt_semaphore cci_sema[MAX_CSIC_CCI_NUM];

int bsp_csi_cci_set_base_addr(unsigned int sel, unsigned long addr)
{
	register_flag[sel] = 1;
	rt_wqueue_init(&wait[sel]);
	rt_mutex_init(&cci_mutex[sel], "ccimtx", RT_IPC_FLAG_FIFO);
	rt_sem_init(&cci_sema[sel], "ccisema", 1, RT_IPC_FLAG_FIFO);
	return csi_cci_set_base_addr(sel, addr);
}

static void cci_cal_div(unsigned int clk, unsigned char *div_coef)
{
	unsigned char clk_m = 0;
	unsigned char clk_n = 0;
	unsigned int src_clk = CCI_HCLK / 10;
	unsigned int sclk_req = clk;
	unsigned char _2_pow_clk_n = 1;
	unsigned int sclk_real = 0;
	unsigned int divider = src_clk / sclk_req;

	if (divider == 0) {
		clk_m = 1;
		goto out;
	}

	while (clk_n < 8) {
		clk_m = (divider / _2_pow_clk_n) - 1;
		while (clk_m < 16) {
			sclk_real = src_clk / (clk_m + 1) / _2_pow_clk_n;
			if (sclk_real <= sclk_req)
				goto out;
			else
				clk_m++;
		}

		clk_n++;
		_2_pow_clk_n *= 2;
	}

out:
	div_coef[0] = clk_m;
	div_coef[1] = clk_n;
}

void bsp_csi_cci_init(unsigned int sel)
{
	unsigned char div_coef[2];

	csi_cci_reset(sel);
	csi_cci_enable(sel);
	cci_cal_div(400 * 1000, div_coef);
	csi_cci_set_clk_div(sel, div_coef);
	csi_cci_set_pkt_interval(sel, 16);
	csi_cci_set_ack_timeout(sel, 16);
	csi_cci_set_trig_dly(sel, 0);
	cci_pad_en(sel);
}

void bsp_csi_cci_exit(unsigned int sel)
{
	cci_int_disable(sel, CCI_INT_ALL);
	csi_cci_reset(sel);
	csi_cci_disable(sel);
}

void bsp_cci_tx_start(unsigned int sel, struct cci_msg *msg)
{
	unsigned int i, j, pkt_len, max_pkt_num, pkt_num;
	unsigned char *buf;

	csi_cci_set_bus_fmt(sel, &msg->bus_fmt);

	buf = msg->pkt_buf;
	pkt_num = msg->pkt_num;
	pkt_len = msg->bus_fmt.addr_len + msg->bus_fmt.data_len;
	if (pkt_len != 0)
		max_pkt_num = FIFO_DEPTH / pkt_len;

	while (pkt_num != 0) {
		if (pkt_num < max_pkt_num || pkt_len == 0) {
			j = pkt_num;
			pkt_num = 0;
		} else {
			j = max_pkt_num;
			pkt_num = pkt_num - max_pkt_num;
		}

		for (i = 0; i < j; i++) {
			csi_cci_wr_tx_buf(sel, buf, pkt_len);
			buf += pkt_len;
		}
		csi_cci_trans_start(sel, SINGLE);
	}
}

void bsp_cci_tx_data_rb(unsigned int sel, struct cci_msg *msg)
{
	unsigned int i, j, pkt_len, max_pkt_num, pkt_num;
	unsigned char *buf;

	csi_cci_set_bus_fmt(sel, &msg->bus_fmt);
	buf = msg->pkt_buf;
	pkt_num = msg->pkt_num;
	pkt_len = msg->bus_fmt.addr_len + msg->bus_fmt.data_len;
	max_pkt_num = FIFO_DEPTH / pkt_len;

	while (pkt_num != 0) {
		if (pkt_num < max_pkt_num) {
			j = pkt_num;
			pkt_num = 0;
		} else {
			j = max_pkt_num;
			pkt_num = pkt_num - max_pkt_num;
		}

		for (i = 0; i < j; i++) {
			csi_cci_fifo_pt_add(sel, msg->bus_fmt.addr_len);
			csi_cci_rd_tx_buf(sel, buf + msg->bus_fmt.addr_len,
					  msg->bus_fmt.data_len);
			buf += pkt_len;
		}
	}
	csi_cci_fifo_pt_reset(sel);
}
static void bsp_cci_error_process(unsigned int sel)
{
	cci_int_clear_status(sel, CCI_INT_ERROR);
	cci_stop(sel);
	cci_sck_cycles(sel, 16);
	bsp_csi_cci_exit(sel);
	bsp_csi_cci_init_helper(sel);
}
static int bsp_cci_tx_start_wait_done_unlocked(unsigned int sel,
					       struct cci_msg *msg)
{
	int ret = 0;
	unsigned long timeout = 0;

	bsp_cci_tx_start(sel, msg);
#ifdef FPGA_VER
	timeout = rt_wqueue_wait(&wait[sel], csi_cci_get_trans_done(sel) == 0, 20);
#else
	timeout = rt_wqueue_wait(&wait[sel], csi_cci_get_trans_done(sel) == 0, 1000);
	if (timeout == 0) {
		pr_err("[VIN CCI_%d ERR] timeout error at addr_8bit = %x, wr_flag = %d, val = %x\n",
		     sel, msg->bus_fmt.saddr_7bit << 1, msg->bus_fmt.wr_flag, *(int *)msg->pkt_buf);
		ret = -1;
	}
#endif
	if (status_err_flag[sel] == 1) {
		pr_err("[VIN CCI_%d ERR] Status error at addr_8bit = %x, wr_flag = %d, val = %x\n",
		     sel, msg->bus_fmt.saddr_7bit << 1, msg->bus_fmt.wr_flag, *(int *)msg->pkt_buf);
		ret = -1;
	}
	if (msg->bus_fmt.wr_flag == 1)
		bsp_cci_tx_data_rb(sel, msg);
	return ret;
}

int bsp_cci_tx_start_wait_done(unsigned int sel, struct cci_msg *msg)
{
	int ret = -1;

	if (!register_flag[sel])
		return ret;

	rt_mutex_take(&cci_mutex[sel], RT_WAITING_FOREVER);
	if (rt_sem_trytake(&cci_sema[sel])) {
		pr_info("down_trylock fail!\n");
		return -1;
	}
	ret = bsp_cci_tx_start_wait_done_unlocked(sel, msg);
	rt_sem_release(&cci_sema[sel]);
	rt_mutex_release(&cci_mutex[sel]);
	return ret;
}

int bsp_cci_irq_process(unsigned int sel)
{
	struct cci_int_status status;
	unsigned int ret = 0;

	cci_int_get_status(sel, &status);
	if (status.error) {
		bsp_cci_error_process(sel);
		status_err_flag[sel] = 1;
		ret = -1;
	}
	if (status.complete) {
		status_err_flag[sel] = 0;
		cci_int_clear_status(sel, CCI_INT_FINISH);
	}
	rt_wqueue_wakeup(&wait[sel], NULL);
	return ret;
}

void bsp_csi_cci_init_helper(unsigned int sel)
{
	struct cci_tx_mode tx_mode;

	bsp_csi_cci_init(sel);
	tx_mode.tx_buf_mode.buf_src = FIFO;
	tx_mode.tx_buf_mode.pkt_mode = COMPACT;
	tx_mode.tx_buf_mode.pkt_cnt = 1;
	tx_mode.tx_trig_mode.trig_src = NO_TRIG;
	tx_mode.tx_trig_mode.trig_con = TRIG_DEFAULT;
	tx_mode.tx_trig_line_cnt = 0;
	csi_cci_set_tx_buf_mode(sel, &tx_mode.tx_buf_mode);
	csi_cci_set_trig_mode(sel, &tx_mode.tx_trig_mode);
	csi_cci_set_trig_line_cnt(sel, tx_mode.tx_trig_line_cnt);
	cci_int_clear_status(sel, CCI_INT_ALL);
	cci_int_enable(sel, CCI_INT_ALL);
}

int cci_wr_8_8(unsigned int sel, unsigned char reg, unsigned char data,
	       unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];

	buf[0] = reg;
	buf[1] = data;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_16_8(unsigned int sel, unsigned short reg, unsigned char data,
		unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];

	buf[0] = (reg & 0xff00) >> 8;
	buf[1] = (reg & 0x00ff);
	buf[2] = data;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_16_16(unsigned int sel, unsigned short reg, unsigned short data,
		 unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[4];

	buf[0] = (reg & 0xff00) >> 8;
	buf[1] = (reg & 0x00ff);
	buf[2] = (data & 0xff00) >> 8;
	buf[3] = (data & 0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_8_16(unsigned int sel, unsigned char reg, unsigned short data,
		unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];

	buf[0] = reg;
	buf[1] = (data & 0xff00) >> 8;
	buf[2] = (data & 0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_0_16(unsigned int sel, unsigned short data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];

	buf[0] = (data & 0xff00) >> 8;
	buf[1] = (data & 0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITHOUT_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 0;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_rd_8_8(unsigned int sel, unsigned char reg, unsigned char *data,
	       unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];
	int ret;

	buf[0] = reg;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[1];
	return ret;
}

int cci_rd_16_8(unsigned int sel, unsigned short reg, unsigned char *data,
		unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];
	int ret;

	buf[0] = (reg & 0xff00) >> 8;
	buf[1] = (reg & 0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[2];
	return ret;
}

int cci_rd_16_16(unsigned int sel, unsigned short reg, unsigned short *data,
		 unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[4];
	int ret;

	buf[0] = (reg & 0xff00) >> 8;
	buf[1] = (reg & 0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[2] * 256 + buf[3];
	return ret;
}

int cci_rd_8_16(unsigned int sel, unsigned char reg, unsigned short *data,
		unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];
	int ret;

	buf[0] = reg;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[1] * 256 + buf[2];
	return ret;
}

int cci_rd_0_16(unsigned int sel, unsigned short *data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];
	int ret;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITHOUT_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 0;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[0] * 256 + buf[1];
	return ret;
}

#define CCI_MAX_MSG_LEN 12

int cci_wr_a16_d8_continuous(unsigned int sel, unsigned short reg,
			     unsigned char *data, unsigned char slv, int size)
{
	struct cci_msg msg;
	unsigned char buf[2 + CCI_MAX_MSG_LEN];
	int ret = 0, i;

	while (size > 0) {
		int len = size > CCI_MAX_MSG_LEN ? CCI_MAX_MSG_LEN : size;

		buf[0] = (reg & 0xff00) >> 8;
		buf[1] = (reg & 0x00ff);
		for (i = 2; i < 2 + len; i++)
			buf[i] = *data++;

		msg.bus_fmt.saddr_7bit = slv;
		msg.bus_fmt.wr_flag = 0;
		msg.bus_fmt.rs_start = START_WITH_ID_W;
		msg.bus_fmt.rs_mode = STOP_START;
		msg.bus_fmt.addr_len = 2;
		msg.bus_fmt.data_len = len;
		msg.pkt_buf = buf;
		msg.pkt_num = 1;
		ret = bsp_cci_tx_start_wait_done(sel, &msg);
		if (ret >= 0) {
			ret = 0;
		} else if (ret < 0) {
			pr_info("[CCI] sensor_write error!\n");
			break;
		}
		reg += len;
		size -= len;
	}
	return ret;
}


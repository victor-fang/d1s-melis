/*
 * ekernel/components/avframework/v4l2/platform/sunxi-vin/vin-csi/sunxi_csi.h
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

#ifndef _SUNXI_CSI_H_
#define _SUNXI_CSI_H_

#include <linux/compiler.h>

#include "../platform/platform_cfg.h"
#include "parser_reg.h"

enum csi_pad {
	CSI_PAD_SINK,
	CSI_PAD_SOURCE,
	CSI_PAD_NUM,
};

struct csi_format {
	u32 code;
	enum input_seq seq;
	enum prs_input_fmt infmt;
	unsigned int data_width;
};

struct csi_dev {
	u8 id;
	u8 capture_mode;    //V4L2_MODE_VIDEO
	int irq;
	void __iomem *base;
	struct v4l2_subdev subdev;
	struct media_pad csi_pads[CSI_PAD_NUM];
	struct rt_mutex subdev_lock;
	struct bus_info bus_info;
	struct frame_info frame_info;
	struct frame_arrange arrange;
	struct pinctrl *pctrl;
	struct v4l2_mbus_framefmt mf;
	struct prs_output_size out_size;
	struct csi_format *csi_fmt;
	struct prs_ncsi_if_cfg ncsi_if;
	struct rt_mutex reset_lock;
	struct prs_fps_ds prs_fps_ds;
	unsigned int reset_time;
};

struct v4l2_subdev *sunxi_csi_get_subdev(int id);
int sunxi_csi_platform_register(void);
void sunxi_csi_platform_unregister(void);

#endif /*_SUNXI_CSI_H_*/

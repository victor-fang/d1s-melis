/*
  * sensor_helper.c: helper function for sensors.
  *
  * Copyright (c) 2017 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
  *
  * Authors:  Zhao Wei <zhaowei@allwinnertech.com>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation.
  */

#include <linux/videodev2.h>
#include <v4l2-subdev.h>
#include <v4l2-dev.h>

#include "camera.h"
#include "sensor_helper.h"
#include "../../vin-cci/sunxi_cci.h"

#if defined CONFIG_SOC_SUN8IW19P1
#include "../../utility/vin_config_sun8iw19p1_real.h"
#elif defined CONFIG_SOC_SUN20IW1P1
#include "../../utility/vin_config_sun20iw1p1_real.h"
#endif

char helper_ccm0[I2C_NAME_SIZE] = "";
char helper_ccm1[I2C_NAME_SIZE] = "";

#define SENSOR_MODULE_NAME "vin_sensor_helper"

struct sensor_helper_dev *glb_sensor_helper[VIN_MAX_CSI];


struct sensor_info *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct sensor_info, sd);
}

void sensor_cfg_req(struct v4l2_subdev *sd, struct sensor_config *cfg)
{
	struct sensor_info *info = to_state(sd);

	if (info == NULL) {
		pr_err("%s is not initialized\n", sd->name);
		return;
	}
	if (info->current_wins == NULL) {
		pr_err("%s format is not initialized\n", sd->name);
		return;
	}

	cfg->width = info->current_wins->width_input;
	cfg->height = info->current_wins->height_input;
	cfg->hoffset = info->current_wins->hoffset;
	cfg->voffset = info->current_wins->voffset;
	cfg->hts = info->current_wins->hts;
	cfg->vts = info->current_wins->vts;
	cfg->pclk = info->current_wins->pclk;
	cfg->fps_fixed = info->current_wins->fps_fixed;
	cfg->bin_factor = info->current_wins->bin_factor;
	cfg->intg_min = info->current_wins->intg_min;
	cfg->intg_max = info->current_wins->intg_max;
	cfg->gain_min = info->current_wins->gain_min;
	cfg->gain_max = info->current_wins->gain_max;
	cfg->mbus_code = info->fmt->mbus_code;
	cfg->wdr_mode = info->current_wins->wdr_mode;
}

void sensor_isp_input(struct v4l2_subdev *sd, struct v4l2_mbus_framefmt *mf)
{
	struct sensor_info *info = to_state(sd);

	if (info == NULL) {
		pr_err("%s is not initialized\n", sd->name);
		return;
	}
	if (info->current_wins == NULL) {
		pr_err("%s format is not initialized\n", sd->name);
		return;
	}

	info->current_wins->width_input = mf->width;
	info->current_wins->height_input = mf->height;
}

unsigned int sensor_get_exp(struct v4l2_subdev *sd)
{
	struct sensor_info *info = to_state(sd);
	unsigned int exp_us = 0;
	if (info == NULL) {
		pr_err("%s is not initialized\n", sd->name);
		return -1;
	}
	if (info->exp == 0) {
		return -1;
	}
	exp_us = info->exp/16*info->current_wins->hts/(info->current_wins->pclk/1000000);
	return exp_us;
}

#if defined CONFIG_ARCH_SUN8IW16P1
static unsigned int __sensor_get_parameter(struct v4l2_subdev *sd, struct v4l2_mbus_config *cfg)
{
	struct sensor_info *info = to_state(sd);
	unsigned flags = cfg->flags & 0xf;

	switch (flags) {
	case V4L2_MBUS_CSI2_1_LANE:
		info->lane_num = 1;
		break;
	case V4L2_MBUS_CSI2_2_LANE:
		info->lane_num = 2;
		break;
	case V4L2_MBUS_CSI2_3_LANE:
		info->lane_num = 3;
		break;
	case V4L2_MBUS_CSI2_4_LANE:
		info->lane_num = 4;
		break;
	default:
		return -1;
	}

	switch (info->fmt->mbus_code) {
	case MEDIA_BUS_FMT_SBGGR8_1X8:
	case MEDIA_BUS_FMT_SGBRG8_1X8:
	case MEDIA_BUS_FMT_SGRBG8_1X8:
	case MEDIA_BUS_FMT_SRGGB8_1X8:
		info->bit_width = 8;
		break;
	case MEDIA_BUS_FMT_SBGGR10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
	case MEDIA_BUS_FMT_SGRBG10_1X10:
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		info->bit_width = 10;
		break;
	case MEDIA_BUS_FMT_SBGGR12_1X12:
	case MEDIA_BUS_FMT_SGBRG12_1X12:
	case MEDIA_BUS_FMT_SGRBG12_1X12:
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		info->bit_width = 12;
		break;
	default:
		return -1;
	}

	return 0;
}

unsigned int sensor_get_clk(struct v4l2_subdev *sd, struct v4l2_mbus_config *cfg,
				unsigned long *top_clk, unsigned long *isp_clk)
{
	struct sensor_info *info = to_state(sd);
	unsigned long topclk_theory, ispclk_theory;
	unsigned long topclk_max = 432*1000*1000;
	unsigned long ispclk_max = 432*1000*1000;
	unsigned int hblank;
	unsigned int hblank_cmp = 128;
	int ret;

	if (info->current_wins->top_clk && info->current_wins->isp_clk) {
		*top_clk = info->current_wins->top_clk;
		*isp_clk = info->current_wins->isp_clk;
	} else if (info->current_wins->mipi_bps && info->current_wins->hts && info->current_wins->pclk) {
		ret = __sensor_get_parameter(sd, cfg);
		if (ret) {
			vin_warn("__sensor_get_parameter warning! sensor is not mipi or raw sensor!\n");
			return -1;
		}

		if (info->current_wins->hts <= info->current_wins->width) {
			vin_warn("%s warning! hts must be greater than width!\n", __func__);
			return -1;
		}
		hblank = info->current_wins->hts - info->current_wins->width;
		topclk_theory = info->current_wins->mipi_bps * info->lane_num / info->bit_width;
		ispclk_theory = max(topclk_theory, (unsigned long)info->current_wins->pclk) /
				info->current_wins->hts *
				(info->current_wins->width + max(hblank_cmp, hblank/10));

		*top_clk = min(topclk_max, roundup(topclk_theory + topclk_theory*5/100, 1000000));
		*isp_clk = min(ispclk_max, roundup(ispclk_theory + ispclk_theory*5/100, 1000000));
		if (!(*top_clk) || !(*isp_clk)) {
			vin_warn("%s warning! top_clk = %ld, isp_clk = %ld\n", __func__, *top_clk, *isp_clk);
			return -1;
		}
	}  else
		return -1;

	return 0;
}
#endif

int sensor_enum_mbus_code(struct v4l2_subdev *sd,
				struct v4l2_subdev_pad_config *cfg,
				struct v4l2_subdev_mbus_code_enum *code)
{
	struct sensor_info *info = to_state(sd);

	if (code->index >= info->fmt_num)
		return -EINVAL;
	code->code = info->fmt_pt[code->index].mbus_code;
	return 0;
}

int sensor_enum_frame_size(struct v4l2_subdev *sd,
			struct v4l2_subdev_pad_config *cfg,
			struct v4l2_subdev_frame_size_enum *fse)
{
	struct sensor_info *info = to_state(sd);

	if (fse->index >= info->win_size_num)
		return -EINVAL;
	fse->min_width = info->win_pt[fse->index].width;
	fse->max_width = fse->min_width;
	fse->max_height = info->win_pt[fse->index].height;
	fse->min_height = fse->max_height;
	return 0;
}

static struct sensor_format_struct *sensor_find_mbus_code(struct v4l2_subdev *sd,
			struct v4l2_mbus_framefmt *fmt)
{
	struct sensor_info *info = to_state(sd);
	int i;

	for (i = 0; i < info->fmt_num; ++i) {
		if (info->fmt_pt[i].mbus_code == fmt->code)
			break;
	}
	if (i >= info->fmt_num)
		return info->fmt_pt;
	return &info->fmt_pt[i];
}

static struct sensor_win_size *sensor_find_frame_size(struct v4l2_subdev *sd,
		struct v4l2_mbus_framefmt *fmt)
{
	struct sensor_info *info = to_state(sd);
	struct sensor_win_size *ws = info->win_pt;
	struct sensor_win_size *best_ws = &ws[0];
	int best_dist = INT_MAX;
	int fps_flag = 0;
	int i;

	/*judge if sensor have wdr command win*/
	if (info->isp_wdr_mode == ISP_COMANDING_MODE) {
		for (i = 0; i < info->win_size_num; ++i) {
			if (ws->wdr_mode == ISP_COMANDING_MODE) {
				best_ws = ws;
				break;
			}
			++ws;
		}
		if (i == info->win_size_num)
			info->isp_wdr_mode = 0;
	}

	/*judge if sensor have wdr dol win*/
	ws = info->win_pt;
	if (info->isp_wdr_mode == ISP_DOL_WDR_MODE) {
		for (i = 0; i < info->win_size_num; ++i) {
			if (ws->wdr_mode == ISP_DOL_WDR_MODE) {
				best_ws = ws;
				break;
			}
			++ws;
		}
		if (i == info->win_size_num)
			info->isp_wdr_mode = 0;
	}

	/*judge if sensor have the right fps win*/
	ws = info->win_pt;
	if (info->isp_wdr_mode == ISP_COMANDING_MODE) {
		for (i = 0; i < info->win_size_num; ++i) {
			if ((ws->fps_fixed == info->tpf.denominator) &&
			   (ws->wdr_mode == ISP_COMANDING_MODE)) {
				best_ws = ws;
				fps_flag = 1;
				break;
			}
			++ws;
		}
	} else if (info->isp_wdr_mode == ISP_DOL_WDR_MODE) {
		for (i = 0; i < info->win_size_num; ++i) {
			if ((ws->fps_fixed == info->tpf.denominator) &&
			   (ws->wdr_mode == ISP_DOL_WDR_MODE)) {
				best_ws = ws;
				fps_flag = 1;
				break;
			}
			++ws;
		}
	} else {
		for (i = 0; i < info->win_size_num; ++i) {
			if ((ws->fps_fixed == info->tpf.denominator) &&
			   (ws->wdr_mode == ISP_NORMAL_MODE)) {
				best_ws = ws;
				fps_flag = 1;
				break;
			}
			++ws;
		}
	}

	/*judge if sensor have the right resoulution win*/
	ws = info->win_pt;
	if (info->isp_wdr_mode == ISP_COMANDING_MODE) {
		if (fps_flag) {
			for (i = 0; i < info->win_size_num; ++i) {
				int dist = abs((int)ws->width - (int)fmt->width) +
				    abs((int)ws->height - (int)fmt->height);

				if ((dist < best_dist) &&
				    (ws->width >= fmt->width) &&
				    (ws->height >= fmt->height) &&
				    (ws->wdr_mode == ISP_COMANDING_MODE) &&
				    (ws->fps_fixed == info->tpf.denominator)) {
					best_dist = dist;
					best_ws = ws;
				}
				++ws;
			}
		} else {
			for (i = 0; i < info->win_size_num; ++i) {
				int dist = abs((int)ws->width - (int)fmt->width) +
				    abs((int)ws->height - (int)fmt->height);

				if ((dist < best_dist) &&
				    (ws->width >= fmt->width) &&
				    (ws->height >= fmt->height) &&
				    (ws->wdr_mode == ISP_COMANDING_MODE)) {
					best_dist = dist;
					best_ws = ws;
				}
				++ws;
			}
		}
	} else if (info->isp_wdr_mode == ISP_DOL_WDR_MODE) {
		if (fps_flag) {
			for (i = 0; i < info->win_size_num; ++i) {
				int dist = abs((int)ws->width - (int)fmt->width) +
				    abs((int)ws->height - (int)fmt->height);

				if ((dist < best_dist) &&
				    (ws->width >= fmt->width) &&
				    (ws->height >= fmt->height) &&
				    (ws->wdr_mode == ISP_DOL_WDR_MODE) &&
				    (ws->fps_fixed == info->tpf.denominator)) {
					best_dist = dist;
					best_ws = ws;
				}
				++ws;
			}
		} else {
			for (i = 0; i < info->win_size_num; ++i) {
				int dist = abs((int)ws->width - (int)fmt->width) +
				    abs((int)ws->height - (int)fmt->height);

				if ((dist < best_dist) &&
				    (ws->width >= fmt->width) &&
				    (ws->height >= fmt->height) &&
				    (ws->wdr_mode == ISP_DOL_WDR_MODE)) {
					best_dist = dist;
					best_ws = ws;
				}
				++ws;
			}
		}
	} else {
		if (fps_flag) {
			for (i = 0; i < info->win_size_num; ++i) {
				int dist = abs((int)ws->width - (int)fmt->width) +
				    abs((int)ws->height - (int)fmt->height);

				if ((dist < best_dist) &&
				    (ws->width >= fmt->width) &&
				    (ws->height >= fmt->height) &&
				    (ws->wdr_mode == ISP_NORMAL_MODE) &&
				    (ws->fps_fixed == info->tpf.denominator)) {
					best_dist = dist;
					best_ws = ws;
				}
				++ws;
			}
		} else {
			for (i = 0; i < info->win_size_num; ++i) {
				int dist = abs((int)ws->width - (int)fmt->width) +
				    abs((int)ws->height - (int)fmt->height);

				if ((dist < best_dist) &&
				    (ws->width >= fmt->width) &&
				    (ws->height >= fmt->height) &&
				    (ws->wdr_mode == ISP_NORMAL_MODE)) {
					best_dist = dist;
					best_ws = ws;
				}
				++ws;
			}
		}
	}

	info->isp_wdr_mode = best_ws->wdr_mode;

	return best_ws;
}

static void sensor_fill_mbus_fmt(struct v4l2_subdev *sd,
				struct v4l2_mbus_framefmt *mf,
				const struct sensor_win_size *ws, u32 code)
{
	struct sensor_info *info = to_state(sd);
	struct mbus_framefmt_res *res = (void *)mf->reserved;

	mf->width = ws->width;
	mf->height = ws->height;
	mf->code = code;
	mf->field = info->sensor_field;
	res->res_mipi_bps = ws->mipi_bps;
	res->res_combo_mode = info->combo_mode | ws->if_mode;
	res->res_wdr_mode = ws->wdr_mode;
	res->res_lp_mode = ws->lp_mode;
	res->res_time_hs = info->time_hs;
}

static void sensor_try_format(struct v4l2_subdev *sd,
				struct v4l2_subdev_pad_config *cfg,
				struct v4l2_subdev_format *fmt,
				struct sensor_win_size **ws,
				struct sensor_format_struct **sf)
{
	struct sensor_info *info = to_state(sd);
	u32 code = MEDIA_BUS_FMT_YUYV8_2X8;

	if (fmt->pad == SENSOR_PAD_SOURCE) {
		if ((sd->entity.stream_count > 0 || info->use_current_win) &&
		    info->current_wins != NULL) {
			code = info->fmt->mbus_code;
			*ws = info->current_wins;
			*sf = info->fmt;
			info->isp_wdr_mode = info->current_wins->wdr_mode;
		} else {
			*ws = sensor_find_frame_size(sd, &fmt->format);
			*sf = sensor_find_mbus_code(sd, &fmt->format);
			code = (*sf)->mbus_code;
		}
	}
	sensor_fill_mbus_fmt(sd, &fmt->format, *ws, code);
}

int sensor_get_fmt(struct v4l2_subdev *sd,
			struct v4l2_subdev_pad_config *cfg,
			struct v4l2_subdev_format *fmt)
{
	struct sensor_info *info = to_state(sd);
	const struct sensor_win_size *ws;
	u32 code;

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		if (cfg == NULL) {
			pr_err("%s cfg is NULL!\n", sd->name);
			rt_mutex_release(&info->lock);
			return -EINVAL;
		}
		fmt->format = cfg->try_fmt;
		return 0;
	}
	rt_mutex_take(&info->lock, RT_WAITING_FOREVER);
	switch (fmt->pad) {
	case SENSOR_PAD_SOURCE:
		code = info->fmt->mbus_code;
		ws = info->current_wins;
		break;
	default:
		rt_mutex_release(&info->lock);
		return -EINVAL;
	}
	sensor_fill_mbus_fmt(sd, &fmt->format, ws, code);
	rt_mutex_release(&info->lock);
	return 0;
}

int sensor_set_fmt(struct v4l2_subdev *sd,
			struct v4l2_subdev_pad_config *cfg,
			struct v4l2_subdev_format *fmt)
{
	struct sensor_info *info = to_state(sd);
	struct sensor_win_size *ws = NULL;
	struct sensor_format_struct *sf = NULL;
	struct v4l2_mbus_framefmt *mf;
	int ret = 0;

	rt_mutex_take(&info->lock, RT_WAITING_FOREVER);
	vin_log(VIN_LOG_FMT, "%s %s %d*%d 0x%x 0x%x\n", sd->name, __func__,
		fmt->format.width, fmt->format.height,
		fmt->format.code, fmt->format.field);
	sensor_try_format(sd, cfg, fmt, &ws, &sf);
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		if (cfg == NULL) {
			pr_err("%s cfg is NULL!\n", sd->name);
			rt_mutex_release(&info->lock);
			return -EINVAL;
		}
		mf = &cfg->try_fmt;
		*mf = fmt->format;
	} else {
		switch (fmt->pad) {
		case SENSOR_PAD_SOURCE:
			info->current_wins = ws;
			info->fmt = sf;
			break;
		default:
			ret = -EBUSY;
		}
	}
	rt_mutex_release(&info->lock);
	return ret;
}

int sensor_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;
	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->capturemode = info->capture_mode;
	if (info->current_wins && info->current_wins->fps_fixed)
		cp->timeperframe.denominator = info->current_wins->fps_fixed;
	else
		cp->timeperframe.denominator = info->tpf.denominator;
	cp->timeperframe.numerator = 1;
	return 0;
}

int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
		return -EINVAL;
	if (info->tpf.numerator == 0)
		return -EINVAL;
	info->capture_mode = cp->capturemode;
	info->tpf = cp->timeperframe;
	info->use_current_win = cp->reserved[0];
	info->isp_wdr_mode = cp->reserved[1];
	return 0;
}

int sensor_try_ctrl(struct v4l2_ctrl *ctrl)
{
	/*
	 * to cheat control framework, because of  when ctrl->cur.val == ctrl->val
	 * s_ctrl would not be called
	 */
	if ((ctrl->minimum == 0) && (ctrl->maximum == 1)) {
		if (ctrl->val)
			ctrl->cur.val = 0;
		else
			ctrl->cur.val = 1;
	} else {
		if (ctrl->val == ctrl->maximum)
			ctrl->cur.val = ctrl->val - 1;
		else
			ctrl->cur.val = ctrl->val + 1;
	}

	/*
	 * to cheat control framework, because of  when ctrl->flags is
	 * V4L2_CTRL_FLAG_VOLATILE, s_ctrl would not be called
	 */
	switch (ctrl->id) {
	case V4L2_CID_EXPOSURE:
	case V4L2_CID_EXPOSURE_ABSOLUTE:
	case V4L2_CID_GAIN:
		if (ctrl->val != ctrl->cur.val)
			ctrl->flags &= ~V4L2_CTRL_FLAG_VOLATILE;
		break;
	default:
		break;
	}
	return 0;
}

int actuator_init(struct v4l2_subdev *sd, struct actuator_para *range)
{
#ifdef CONFIG_ACTUATOR_MODULE
	struct modules_config *modules = sd_to_modules(sd);
	struct v4l2_subdev *act_sd = NULL;
	struct actuator_para_t vcm_para;

	if (modules == NULL)
		return -EINVAL;

	act_sd = modules->modules.act[modules->sensors->valid_idx].sd;
	if (act_sd == NULL)
		return 0;

	if (act_sd->entity.use_count > 0)
		return 0;

	vcm_para.active_min = range->code_min;
	vcm_para.active_max = range->code_max;
	return v4l2_subdev_call(act_sd, core, ioctl, ACT_INIT, &vcm_para);
#else
	return 0;
#endif
}

int actuator_set_code(struct v4l2_subdev *sd, struct actuator_ctrl *pos)
{
#ifdef CONFIG_ACTUATOR_MODULE
	struct modules_config *modules = sd_to_modules(sd);
	struct v4l2_subdev *act_sd = NULL;
	struct actuator_ctrl_word_t vcm_ctrl;

	if (modules == NULL)
		return -EINVAL;

	act_sd = modules->modules.act[modules->sensors->valid_idx].sd;
	if (act_sd == NULL)
		return 0;

	vcm_ctrl.code = pos->code;
	vcm_ctrl.sr = 0x0;
	return v4l2_subdev_call(act_sd, core, ioctl, ACT_SET_CODE, &vcm_ctrl);
#else
	return 0;
#endif
}

#if defined CONFIG_SUNXI_REGULATOR_DT && !defined SENSOR_POER_BEFORE_VIN
static int sensor_helper_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device *dev = &pdev->dev;
	struct sensor_helper_dev *sensor_helper = NULL;
	char power_supply[20] = {0};
	char property_name[16] = {0};
	const char *ccm_name;
	int ret;

	if (np == NULL) {
		vin_err("sensor_helper failed to get of node\n");
		return -ENODEV;
	}
	sensor_helper = kzalloc(sizeof(struct sensor_helper_dev), GFP_KERNEL);
	if (!sensor_helper) {
		ret = -ENOMEM;
		goto ekzalloc;
	}

	of_property_read_u32(np, "device_id", &pdev->id);
	if (pdev->id < 0) {
		vin_err("sensor_helper failed to get device id\n");
		ret = -EINVAL;
		goto freedev;
	}
	sensor_helper->id = pdev->id;

	if (sensor_helper->id == 0)
		strcpy(sensor_helper->name, helper_ccm0);
	else if (sensor_helper->id == 1)
		strcpy(sensor_helper->name, helper_ccm1);

	if (!strcmp(sensor_helper->name, "")) {
		snprintf(property_name, sizeof(property_name), "sensor%d_mname", pdev->id);
		ret = of_property_read_string(np, property_name, &ccm_name);
		if (ret) {
			vin_warn("fetch %s from device_tree failed\n", property_name);
			return -EINVAL;
		} else
			strcpy(sensor_helper->name, ccm_name);
	}
	vin_log(VIN_LOG_POWER, "sensor_helper get name is %s\n", sensor_helper->name);

	snprintf(power_supply, sizeof(power_supply), "sensor%d_iovdd", pdev->id);
	sensor_helper->pmic[IOVDD] = regulator_get_optional(dev, power_supply);
	if (IS_ERR(sensor_helper->pmic[IOVDD])) {
		vin_warn("%s: cannot get %s supply, setting it to NULL!\n", __func__, power_supply);
		sensor_helper->pmic[IOVDD] = NULL;
	}

	snprintf(power_supply, sizeof(power_supply), "sensor%d_avdd", pdev->id);
	sensor_helper->pmic[AVDD] = regulator_get_optional(dev, power_supply);
	if (IS_ERR(sensor_helper->pmic[AVDD])) {
		vin_warn("%s: cannot get %s supply, setting it to NULL!\n", __func__, power_supply);
		sensor_helper->pmic[AVDD] = NULL;
	}

	snprintf(power_supply, sizeof(power_supply), "sensor%d_dvdd", pdev->id);
	sensor_helper->pmic[DVDD] = regulator_get_optional(dev, power_supply);
	if (IS_ERR(sensor_helper->pmic[DVDD])) {
		vin_warn("%s: cannot get %s supply, setting it to NULL!\n", __func__, power_supply);
		sensor_helper->pmic[DVDD] = NULL;
	}

	glb_sensor_helper[pdev->id] = sensor_helper;

	platform_set_drvdata(pdev, sensor_helper);

	vin_log(VIN_LOG_POWER, "sensor_helper probe end sensor_sel = %d!\n", sensor_helper->id);

	return 0;
freedev:
	kfree(sensor_helper);
ekzalloc:
	vin_err("sensor_helper probe err!\n");
	return ret;
}

static int sensor_helper_remove(struct platform_device *pdev)
{
	struct sensor_helper_dev *sensor_helper = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	regulator_put(sensor_helper->pmic[IOVDD]);
	sensor_helper->pmic[IOVDD] = NULL;
	regulator_put(sensor_helper->pmic[AVDD]);
	sensor_helper->pmic[AVDD] = NULL;
	regulator_put(sensor_helper->pmic[DVDD]);
	sensor_helper->pmic[DVDD] = NULL;

	kfree(sensor_helper);
	return 0;
}

static const struct of_device_id sunxi_sensor_helper_match[] = {
	{.compatible = "allwinner,sunxi-sensor",},
	{},
};

static struct platform_driver sensor_helper_platform_driver = {
	.probe = sensor_helper_probe,
	.remove = sensor_helper_remove,
	.driver = {
		   .name = SENSOR_MODULE_NAME,
		   .owner = THIS_MODULE,
		   .of_match_table = sunxi_sensor_helper_match,
		   },
};
#endif

static int vin_io_init(void)
{
	int ret;

	ret = sunxi_cci_platform_register();
	if (ret)
		return ret;

#if defined CONFIG_SUNXI_REGULATOR_DT && !defined SENSOR_POER_BEFORE_VIN
	ret = platform_driver_register(&sensor_helper_platform_driver);
	if (ret) {
		vin_err("sensor helper platform driver register failed\n");
		return ret;
	}
	vin_log(VIN_LOG_POWER, "sensor_helper_init end\n");
#endif

	return 0;
}

static void vin_io_exit(void)
{
	sunxi_cci_platform_unregister();
#if defined CONFIG_SUNXI_REGULATOR_DT && !defined SENSOR_POER_BEFORE_VIN
	platform_driver_unregister(&sensor_helper_platform_driver);
	vin_log(VIN_LOG_POWER, "sensor_helper_exit end\n");
#endif
}



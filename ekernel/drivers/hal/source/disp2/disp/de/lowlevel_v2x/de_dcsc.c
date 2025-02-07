/*
 * Allwinner SoCs display driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

/*****************************************************************************
 *All Winner Tech, All Right Reserved. 2014-2015 Copyright (c)
 *
 *File name   :       de_ccsc.c
 *
 *Description :       display engine 2.0 device csc basic function definition
 *
 *History     :       2014/05/19  vito cheng  v0.1  Initial version
 *****************************************************************************/

#include "de_rtmx.h"
#include "de_csc_type.h"
#include "de_csc.h"

#define DCSC_OFST	0xB0000
#define CSC_ENHANCE_MODE_NUM 3
/* must equal to ENHANCE_MODE_NUM */
#ifdef CONFIG_COMMAND_PQD
void pq_get_enhance(struct disp_csc_config *conig);
#endif

static volatile struct __csc_reg_t *dcsc_dev[DE_NUM];
static volatile struct __csc2_reg_t *dcsc2_dev[DE_NUM];
static volatile struct __gamma_reg_t *gamma_dev[DE_NUM];
static struct de_reg_blocks dcsc_coeff_block[DE_NUM];
static struct de_reg_blocks dcsc_enable_block[DE_NUM];
static struct de_reg_blocks dcsc_gamma_block[DE_NUM];
static struct de_reg_blocks dcsc_ctl_block[DE_NUM];
static struct disp_csc_config g_dcsc_config[DE_NUM];

static unsigned int is_in_smbl[DE_NUM];
/* device csc and smbl in the same module or not */

static int de_dcsc_set_reg_base(unsigned int sel, void *base)
{
	DE_INF("sel=%d, base=0x%p\n", sel, base);
	if (is_in_smbl[sel])
		dcsc2_dev[sel] = (struct __csc2_reg_t *) base;
	else
		dcsc_dev[sel] = (struct __csc_reg_t *) base;

	return 0;
}

static int de_gamma_set_reg_base(unsigned int sel, void *base)
{
	gamma_dev[sel] = (struct __gamma_reg_t *) base;
	return 0;
}

int _csc_enhance_setting[CSC_ENHANCE_MODE_NUM][4] = {
    {50, 50, 50, 50},
    /* normal */
    {50, 50, 50, 50},
    /* vivid */
    {50, 40, 50, 50},
    /* soft */
};

int de_dcsc_apply(unsigned int sel, struct disp_csc_config *config)
{
	int csc_coeff[12];
	unsigned int enhance_mode;

	config->enhance_mode =
	    (config->enhance_mode > CSC_ENHANCE_MODE_NUM - 1)
	     ? g_dcsc_config[sel].enhance_mode : config->enhance_mode;
	enhance_mode = config->enhance_mode;
	config->brightness = _csc_enhance_setting[enhance_mode][0];
	config->contrast = _csc_enhance_setting[enhance_mode][1];
	config->saturation = _csc_enhance_setting[enhance_mode][2];
	config->hue = _csc_enhance_setting[enhance_mode][3];

	DE_INF("sel=%d, in_fmt=%d, mode=%d, out_fmt=%d, mode=%d, range=%d\n",
	      sel, config->in_fmt, config->in_mode, config->out_fmt,
	      config->out_mode, config->out_color_range);

	memcpy(&g_dcsc_config[sel], config, sizeof(struct disp_csc_config));
	de_csc_coeff_calc(config->in_fmt, config->in_mode, config->out_fmt,
			  config->out_mode, config->brightness,
			  config->contrast, config->saturation, config->hue,
			  config->out_color_range, csc_coeff);

	if (is_in_smbl[sel]) {
		dcsc2_dev[sel]->c00.dwval = *(csc_coeff);
		dcsc2_dev[sel]->c01.dwval = *(csc_coeff + 1);
		dcsc2_dev[sel]->c02.dwval = *(csc_coeff + 2);
		dcsc2_dev[sel]->c03.dwval = *(csc_coeff + 3) >> 6;
		dcsc2_dev[sel]->c10.dwval = *(csc_coeff + 4);
		dcsc2_dev[sel]->c11.dwval = *(csc_coeff + 5);
		dcsc2_dev[sel]->c12.dwval = *(csc_coeff + 6);
		dcsc2_dev[sel]->c13.dwval = *(csc_coeff + 7) >> 6;
		dcsc2_dev[sel]->c20.dwval = *(csc_coeff + 8);
		dcsc2_dev[sel]->c21.dwval = *(csc_coeff + 9);
		dcsc2_dev[sel]->c22.dwval = *(csc_coeff + 10);
		dcsc2_dev[sel]->c23.dwval = *(csc_coeff + 11) >> 6;
		dcsc2_dev[sel]->bypass.bits.enable = 1;
		/* always enable csc */
	} else {
#ifdef DE_GAMMA
		dcsc_dev[sel]->c00.dwval = (*(csc_coeff)) << 7;
		dcsc_dev[sel]->c01.dwval = (*(csc_coeff + 1)) << 7;
		dcsc_dev[sel]->c02.dwval = (*(csc_coeff + 2)) << 7;
		dcsc_dev[sel]->c03.dwval = (*(csc_coeff + 3) + 0x200) << 7;
		dcsc_dev[sel]->c10.dwval = (*(csc_coeff + 4)) << 7;
		dcsc_dev[sel]->c11.dwval = (*(csc_coeff + 5)) << 7;
		dcsc_dev[sel]->c12.dwval = (*(csc_coeff + 6)) << 7;
		dcsc_dev[sel]->c13.dwval = (*(csc_coeff + 7) + 0x200) << 7;
		dcsc_dev[sel]->c20.dwval = (*(csc_coeff + 8)) << 7;
		dcsc_dev[sel]->c21.dwval = (*(csc_coeff + 9)) << 7;
		dcsc_dev[sel]->c22.dwval = (*(csc_coeff + 10)) << 7;
		dcsc_dev[sel]->c23.dwval = (*(csc_coeff + 11) + 0x200) << 7;
#else
		dcsc_dev[sel]->c00.dwval = *(csc_coeff);
		dcsc_dev[sel]->c01.dwval = *(csc_coeff + 1);
		dcsc_dev[sel]->c02.dwval = *(csc_coeff + 2);
		dcsc_dev[sel]->c03.dwval = *(csc_coeff + 3) + 0x200;
		dcsc_dev[sel]->c10.dwval = *(csc_coeff + 4);
		dcsc_dev[sel]->c11.dwval = *(csc_coeff + 5);
		dcsc_dev[sel]->c12.dwval = *(csc_coeff + 6);
		dcsc_dev[sel]->c13.dwval = *(csc_coeff + 7) + 0x200;
		dcsc_dev[sel]->c20.dwval = *(csc_coeff + 8);
		dcsc_dev[sel]->c21.dwval = *(csc_coeff + 9);
		dcsc_dev[sel]->c22.dwval = *(csc_coeff + 10);
		dcsc_dev[sel]->c23.dwval = *(csc_coeff + 11) + 0x200;
#endif
		dcsc_dev[sel]->bypass.bits.enable = 1;
		/* always enable csc */
	}

	dcsc_coeff_block[sel].dirty = 1;
	dcsc_enable_block[sel].dirty = 1;

	return 0;
}

int de_dcsc_get_config(unsigned int sel, struct disp_csc_config *config)
{
	memcpy(config, &g_dcsc_config[sel], sizeof(struct disp_csc_config));

	return 0;
}

s32 de_blue_screen(u32 sel, u32 mode, u32 blue_en, u32 rgb)
{
	gamma_dev[sel]->gamma_ctl.bits.blue_en = blue_en;
	gamma_dev[sel]->gamma_ctl.bits.color_mode = mode;

	gamma_dev[sel]->blue_screen.bits.R = rgb & 0xff0000 << 4;
	gamma_dev[sel]->blue_screen.bits.G = rgb & 0xff00 << 2;
	gamma_dev[sel]->blue_screen.bits.B = rgb & 0xff;
	dcsc_ctl_block[sel].dirty = 1;
	return 0;
}

s32 de_gamma(u32 sel, u32 en, u32 *gamma_tbl)
{
	int i;
	unsigned int *addr = (unsigned int *)(&(gamma_dev[sel]->gamma_tbl[0]));

	for (i = 0; i < 256; i++) {
		*addr = ((gamma_tbl[i] & 0xff0000)<< 4) |
			((gamma_tbl[i] & 0xff00)<< 2) |
			(gamma_tbl[i] & 0xff);
		addr++;
	}

	gamma_dev[sel]->gamma_ctl.bits.gamma_en = en;
//	dcsc_gamma_block[sel].dirty = 1;
//	dcsc_ctl_block[sel].dirty = 1;
	memcpy((void *)dcsc_gamma_block[sel].off,
	   dcsc_gamma_block[sel].val, dcsc_gamma_block[sel].size);

	memcpy((void *)dcsc_ctl_block[sel].off,
	   dcsc_ctl_block[sel].val, dcsc_ctl_block[sel].size);

	return 0;
}

int de_dcsc_update_regs(unsigned int sel)
{
	unsigned int reg_val;

	if (dcsc_enable_block[sel].dirty == 0x1) {
		if (is_in_smbl[sel]) {
			reg_val =
			    readl((void  *)dcsc_enable_block[sel].off);
			reg_val &= 0xfffffffd;
			reg_val |=
			    (*((unsigned int *)dcsc_enable_block[sel].val));
			writel(reg_val,
			       (void  *)dcsc_enable_block[sel].off);
		} else {
			regwrite((void *)dcsc_enable_block[sel].off,
			   dcsc_enable_block[sel].val,
			   dcsc_enable_block[sel].size);
		}
		dcsc_enable_block[sel].dirty = 0x0;
	}

	if (dcsc_coeff_block[sel].dirty == 0x1) {
		regwrite((void *)dcsc_coeff_block[sel].off,
		   dcsc_coeff_block[sel].val, dcsc_coeff_block[sel].size);
		dcsc_coeff_block[sel].dirty = 0x0;
	}
/*
	if (dcsc_gamma_block[sel].dirty == 0x1) {
		memcpy((void *)dcsc_gamma_block[sel].off,
		   dcsc_gamma_block[sel].val, dcsc_gamma_block[sel].size);
		dcsc_gamma_block[sel].dirty = 0x0;
	}

	if (dcsc_ctl_block[sel].dirty == 0x1) {
		memcpy((void *)dcsc_ctl_block[sel].off,
		   dcsc_ctl_block[sel].val, dcsc_ctl_block[sel].size);
		dcsc_ctl_block[sel].dirty = 0x0;
	}
*/
	return 0;
}

int de_dcsc_init(struct disp_bsp_init_para *para)
{
	uintptr_t base;
	void *memory;
	int screen_id, device_num;

	device_num = de_feat_get_num_screens();

	for (screen_id = 0; screen_id < device_num; screen_id++) {
		is_in_smbl[screen_id] = de_feat_is_support_smbl(screen_id);

#if defined(CONFIG_ARCH_SUN50IW10)
		base = para->reg_base[DISP_MOD_DE + screen_id]
		    + (screen_id + 1) * 0x00100000 + DCSC_OFST;
		if (screen_id)
			base = base - 0x00100000;
#else
		base = para->reg_base[DISP_MOD_DE]
		    + (screen_id + 1) * 0x00100000 + DCSC_OFST;
#endif
		DE_INF("sel %d, Dcsc_base=0x%p\n", screen_id, (void *)base);

		if (is_in_smbl[screen_id]) {
			memory = disp_sys_malloc(sizeof(struct __csc2_reg_t));
			if (memory == NULL) {
				DE_WRN("disp_sys_malloc Ccsc[%d] mm fail! size=0x%x\n",
				     screen_id,
				     (unsigned int)sizeof(struct __csc2_reg_t));
				return -1;
			}

			dcsc_enable_block[screen_id].off = base;
			dcsc_enable_block[screen_id].val = memory;
			dcsc_enable_block[screen_id].size = 0x04;
			dcsc_enable_block[screen_id].dirty = 0;

			dcsc_coeff_block[screen_id].off = base + 0x80;
			dcsc_coeff_block[screen_id].val = memory + 0x80;
			dcsc_coeff_block[screen_id].size = 0x30;
			dcsc_coeff_block[screen_id].dirty = 0;

		} else {
			memory =  disp_sys_malloc(sizeof(struct __csc_reg_t) + sizeof(struct __gamma_reg_t));
			if (memory == NULL) {
				DE_WRN("disp_sys_malloc Ccsc[%d] mm fail! size=0x%x\n",
				     screen_id,
				     (unsigned int)sizeof(struct __csc_reg_t));
				return -1;
			}

			dcsc_enable_block[screen_id].off = base;
			dcsc_enable_block[screen_id].val = memory;
			dcsc_enable_block[screen_id].size = 0x04;
			dcsc_enable_block[screen_id].dirty = 0;

			dcsc_coeff_block[screen_id].off = base + 0x10;
			dcsc_coeff_block[screen_id].val = memory + 0x10;
			dcsc_coeff_block[screen_id].size = 0x30;
			dcsc_coeff_block[screen_id].dirty = 0;

			dcsc_ctl_block[screen_id].off = base + 0x40;
			dcsc_ctl_block[screen_id].val = memory + sizeof(struct __csc_reg_t);
			dcsc_ctl_block[screen_id].size = 0x08;
			dcsc_ctl_block[screen_id].dirty = 0;

			dcsc_gamma_block[screen_id].off = base + 0x100;
			dcsc_gamma_block[screen_id].val = memory + sizeof(struct __csc_reg_t) + dcsc_ctl_block[screen_id].size;
			dcsc_gamma_block[screen_id].size = 1024;/*now only 2^8*4*/
			dcsc_gamma_block[screen_id].dirty = 0;

			de_gamma_set_reg_base(screen_id, memory + sizeof(struct __csc_reg_t));
		}
		g_dcsc_config[screen_id].enhance_mode = 0;
		de_dcsc_set_reg_base(screen_id, memory);
	}

	return 0;
}

int de_dcsc_exit(void)
{
	int screen_id, device_num;

	device_num = de_feat_get_num_screens();

	for (screen_id = 0; screen_id < device_num; screen_id++) {
		is_in_smbl[screen_id] = de_feat_is_support_smbl(screen_id);

		if (is_in_smbl[screen_id])
			disp_sys_free(dcsc_enable_block[screen_id].val);
		else
			disp_sys_free(dcsc_enable_block[screen_id].val);
	}

	return 0;
}


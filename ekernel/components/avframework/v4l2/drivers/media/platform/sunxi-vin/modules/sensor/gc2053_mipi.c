/*
 * A V4L2 driver for Raw cameras.
 *
 * Copyright (c) 2017 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Zhao Wei <zhaowei@allwinnertech.com>
 *    Liang WeiJie <liangweijie@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <melis/init.h>
#include <linux/videodev2.h>
#include <hal_timer.h>
#include <v4l2-device.h>
#include <v4l2-mediabus.h>

#include "camera.h"
#include "sensor_helper.h"

#define MCLK              (24*1000*1000)
#define V4L2_IDENT_SENSOR  0x2053

//define the registers
#define EXP_HIGH		0xff
#define EXP_MID			0x03
#define EXP_LOW			0x04
#define GAIN_HIGH		0xff
#define GAIN_LOW		0x24
/*
 * Our nominal (default) frame rate.
 */
#define ID_REG_HIGH		0xf0
#define ID_REG_LOW		0xf1
#define ID_VAL_HIGH		((V4L2_IDENT_SENSOR) >> 8)
#define ID_VAL_LOW		((V4L2_IDENT_SENSOR) & 0xff)
#define SENSOR_FRAME_RATE 20

/*
 * The GC2053 i2c address
 */
#define I2C_ADDR 0x6e

#define SENSOR_NUM 0x2
#define SENSOR_NAME "gc2053_mipi"
#define SENSOR_NAME_2 "gc2053_mipi_2"

/*
 * The default register settings
 */

static struct regval_list sensor_default_regs[] = {

};

/*
 * window_size=1920*1080 mipi@2lane
 * mclk=24mhz, mipi_clk=594Mbps
 * pixel_line_total=2640, line_frame_total=1125
 * row_time=35.55us, frame_rate=25fps
 */
static struct regval_list sensor_1080p20_regs[] = {
	{0xfe, 0xf0},
	{0xfe, 0xf0},
	{0xfe, 0xf0},
	{0xfe, 0x00},
	{0xf2, 0x00}, //[1]I2C_open_ena [0]pwd_dn
	{0xf3, 0x00}, //0f//00[3]Sdata_pad_io [2:0]Ssync_pad_io
	{0xf4, 0x36}, //[6:4]pll_ldo_set
	{0xf5, 0xc0}, //[7]soc_mclk_enable [6]pll_ldo_en [5:4]cp_clk_sel [3:0]cp_clk_div
	{0xf6, 0x84}, //[7:3]wpllclk_div [2:0]refmp_div
	{0xf7, 0x11}, //[7]refdiv2d5_en [6]refdiv1d5_en [5:4]scaler_mode [3]refmp_enb [1]div2en [0]pllmp_en
	{0xf8, 0x33}, //2c//[7:0]pllmp_div
	{0xf9, 0x82}, //[7:3]rpllclk_div [2:1]pllmp_prediv [0]analog_pwc
	{0xfc, 0x8e},
	/**** CISCTL, & ANALOG****/
	{0xfe, 0x00},
	{0x87, 0x18}, //[6]aec_delay_mode
	{0xee, 0x30}, //[5:4]dwen_sramen
	{0xd0, 0xb7}, //ramp_en
	{0x03, 0x04},
	{0x04, 0x60},
	{0x05, 0x04}, //5, //05     //04
	{0x06, 0xd2}, //fa, //2a//60//[11:0]hb         //0x0400 : 37ms
	{0x07, 0x00},
	{0x08, 0x19}, //19
	{0x09, 0x00},
	{0x0a, 0x02}, //cisctl row start
	{0x0b, 0x00},
	{0x0c, 0x02}, //cisctl col start
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x12, 0xe2}, //vsync_ahead_mode
	{0x13, 0x16},
	{0x17, 0x80}, //mirror flip
	{0x19, 0x0a}, //ad_pipe_num
	{0x21, 0x1c}, //eqc1fc_eqc2fc_sw
	{0x28, 0x0a}, //16//eqc2_c2clpen_sw
	{0x29, 0x24}, //eq_post_width
	{0x2b, 0x04}, //c2clpen --eqc2
	{0x32, 0xf8}, //[5]txh_en ->avdd28
	{0x37, 0x03}, //[3:2]eqc2sel=0
	{0x39, 0x15}, //17 //[3:0]rsgl
	{0x43, 0x07}, //vclamp
	{0x44, 0x0b}, //0e//post_tx_width
	{0x46, 0x40}, //txh——3.2v
	{0x4b, 0x20}, //rst_tx_width
	{0x4e, 0x08}, //12//ramp_t1_width
	{0x55, 0x20}, //read_tx_width_pp
	{0x66, 0x05}, //18//stspd_width_r1
	{0x67, 0x05}, //40//5//stspd_width_r
	{0x77, 0x01}, //dacin offset x31
	{0x78, 0x00}, //dacin offset
	{0x7c, 0x93}, //[1:0] co1comp
	{0x8c, 0x12}, //12 ramp_t1_ref
	{0x8d, 0x92}, //90
	{0x90, 0x00},
	{0x9d, 0x10},
	{0xce, 0x7c}, //70//78//[4:2]c1isel
	{0xd2, 0x41}, //[5:3]c2clamp
	{0xd3, 0xdc}, //ec//0x39[7]=0, 0xd3[3]=1 rsgh=vref
	{0xe6, 0x50}, //ramps offset
	/*gai n*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk */
	{0x26, 0x30}, //23 //[4]写0，全n mode
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x40}, //[7:0]WB_offset
	{0xfe, 0x04},
	{0x14, 0x78}, //g1 ratio
	{0x15, 0x78}, //r ratio
	{0x16, 0x78}, //b ratio
	{0x17, 0x78}, //g2 ratio
	/*win dow*/
	{0xfe, 0x01},
	{0x92, 0x00}, //win y1
	{0x94, 0x03}, //win x1
	{0x95, 0x04},
	{0x96, 0x38}, //[10:0]out_height
	{0x97, 0x07},
	{0x98, 0x80}, //[11:0]out_width
	/*ISP */
	{0xfe, 0x01},
	{0x01, 0x05}, //03//[3]dpc blending mode [2]noise_mode [1:0]center_choose 2b'11:median 2b'10:avg 2'b00:near
	{0x02, 0x89}, //[7:0]BFF_sram_mode
	{0x04, 0x01}, //[0]DD_en
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},
	/**** DVP & IPI****/
	{0xfe, 0x01},
	{0x9a, 0x06}, //[5]OUT_gate_mode [4]hsync_delay_half_pclk [3]data_delay_half_pclk [2]vsync_polarity [1]hsync_polarity [0]pclk_out_polarity
	{0xfe, 0x00},
	{0x7b, 0x2a}, //[7:6]updn [5:4]drv_high_data [3:2]drv_low_data [1:0]drv_pclk
	{0x23, 0x2d}, //[3]rst_rc [2:1]drv_sync [0]pwd_rc
	{0xfe, 0x03},
	{0x01, 0x27}, //20//27[6:5]clkctr [2]phy-lane1_en [1]phy-lane0_en [0]phy_clk_en
	{0x02, 0x56}, //[7:6]data1ctr [5:4]data0ctr [3:0]mipi_diff
	{0x03, 0x8e}, //b2//b6[7]clklane_p2s_sel [6:5]data0hs_ph [4]data0_delay1s [3]clkdelay1s [2]mipi_en [1:0]clkhs_ph
	{0x12, 0x80},
	{0x13, 0x07}, //LWC
	{0x15, 0x12}, //[1:0]clk_lane_mode
	{0xfe, 0x00},
	{0x3e, 0x91}, //40//91[7]lane_ena [6]DVPBUF_ena [5]ULPEna [4]MIPI_ena [3]mipi_set_auto_disable [2]RAW8_mode [1]ine_sync_mode [0]double_lane_en
};

/*
 * window_size=1920*1080 mipi@2lane
 * mclk=24mhz, mipi_clk=594Mbps
 * pixel_line_total=2640, line_frame_total=1125
 * row_time=29.629us, frame_rate=30fps
 */
static struct regval_list sensor_1080p30_regs[] = {
#if 0
	/*  1920*1080@30fps  */
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x00},
	{0xf2, 0x00}, //[1]I2C_open_ena [0]pwd_dn
	{0xf3, 0x00}, //0f//00[3]Sdata_pad_io [2:0]Ssync_pad_io
	{0xf4, 0x36}, //[6:4]pll_ldo_set
	{0xf5, 0xc0}, //[7]soc_mclk_enable [6]pll_ldo_en [5:4]cp_clk_sel [3:0]cp_clk_div
	{0xf6, 0x44}, //[7:3]wpllclk_div [2:0]refmp_div
	{0xf7, 0x01}, //[7]refdiv2d5_en [6]refdiv1d5_en [5:4]scaler_mode [3]refmp_enb [1]div2en [0]pllmp_en
	{0xf8, 0x63}, //2c//[7:0]pllmp_div
	{0xf9, 0x40}, //[7:3]rpllclk_div [2:1]pllmp_prediv [0]analog_pwc
	{0xfc, 0x8e},
	/**** CISCTL, & ANALOG****/
	{0xfe, 0x00},
	{0x87, 0x18}, //[6]aec_delay_mode
	{0xee, 0x30}, //[5:4]dwen_sramen
	{0xd0, 0xb7}, //ramp_en
	{0x03, 0x04},
	{0x04, 0x60},
	{0x05, 0x04}, //05 hts
	{0x06, 0x4c}, //2a//60//[11:0]hb
	{0x07, 0x00},
	{0x08, 0x11}, //19
	{0x09, 0x00},
	{0x0a, 0x02}, //cisctl row start
	{0x0b, 0x00},
	{0x0c, 0x02}, //cisctl col start
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x12, 0xe2}, //vsync_ahead_mode
	{0x13, 0x16},
	{0x19, 0x0a}, //ad_pipe_num
	{0x21, 0x1c}, //eqc1fc_eqc2fc_sw
	{0x28, 0x0a}, //16//eqc2_c2clpen_sw
	{0x29, 0x24}, //eq_post_width
	{0x2b, 0x04}, //c2clpen --eqc2
	{0x32, 0xf8}, //[5]txh_en ->avdd28
	{0x37, 0x03}, //[3:2]eqc2sel=0
	{0x39, 0x15}, //17 //[3:0]rsgl
	{0x43, 0x07}, //vclamp
	{0x44, 0x0b}, //0e//post_tx_width
	{0x46, 0x40}, //txh——3.2v
	{0x4b, 0x20}, //rst_tx_width
	{0x4e, 0x08}, //12//ramp_t1_width
	{0x55, 0x20}, //read_tx_width_pp
	{0x66, 0x05}, //18//stspd_width_r1
	{0x67, 0x05}, //40//5//stspd_width_r
	{0x77, 0x01}, //dacin offset x31
	{0x78, 0x00}, //dacin offset
	{0x7c, 0x93}, //[1:0] co1comp
	{0x8c, 0x12}, //12 ramp_t1_ref
	{0x8d, 0x92}, //90
	{0x90, 0x01},
	{0x9d, 0x10},
	{0xce, 0x7c}, //70//78//[4:2]c1isel
	{0xd2, 0x41}, //[5:3]c2clamp
	{0xd3, 0xdc}, //ec//0x39[7]=0, 0xd3[3]=1 rsgh=vref
	{0xe6, 0x50}, //ramps offset
	/*gai n*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk */
	{0x26, 0x30}, //23 //[4]写0，全n mode
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x40}, //[7:0]WB_offset
	{0xfe, 0x04},
	{0x14, 0x78}, //g1 ratio
	{0x15, 0x78}, //r ratio
	{0x16, 0x78}, //b ratio
	{0x17, 0x78}, //g2 ratio
	/*win dow*/
	{0xfe, 0x01},
	{0x92, 0x00}, //win y1
	{0x94, 0x03}, //win x1
	{0x95, 0x04},
	{0x96, 0x38}, //[10:0]out_height
	{0x97, 0x07},
	{0x98, 0x80}, //[11:0]out_width
	/*ISP */
	{0xfe, 0x01},
	{0x01, 0x05}, //03//[3]dpc blending mode [2]noise_mode [1:0]center_choose 2b'11:median 2b'10:avg 2'b00:near
	{0x02, 0x89}, //[7:0]BFF_sram_mode
	{0x04, 0x01}, //[0]DD_en
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},
	/**** DVP & IPI****/
	{0xfe, 0x01},
	{0x9a, 0x06}, //[5]OUT_gate_mode [4]hsync_delay_half_pclk [3]data_delay_half_pclk [2]vsync_polarity [1]hsync_polarity [0]pclk_out_polarity
	{0xfe, 0x00},
	{0x7b, 0x2a}, //[7:6]updn [5:4]drv_high_data [3:2]drv_low_data [1:0]drv_pclk
	{0x23, 0x2d}, //[3]rst_rc [2:1]drv_sync [0]pwd_rc
	{0xfe, 0x03},
	{0x01, 0x27}, //20//27[6:5]clkctr [2]phy-lane1_en [1]phy-lane0_en [0]phy_clk_en
	{0x02, 0x56}, //[7:6]data1ctr [5:4]data0ctr [3:0]mipi_diff
	{0x03, 0x8e}, //b2//b6[7]clklane_p2s_sel [6:5]data0hs_ph [4]data0_delay1s [3]clkdelay1s [2]mipi_en [1:0]clkhs_ph
	{0x12, 0x80},
	{0x13, 0x07}, //LWC
	{0x15, 0x12}, //[1:0]clk_lane_mode
	{0xfe, 0x00},
	{0x3e, 0x91}, //40//91[7]lane_ena [6]DVPBUF_ena [5]ULPEna [4]MIPI_ena [3]mipi_set_auto_disable [2]RAW8_mode [1]ine_sync_mode [0]double_lane_en
#endif

    /*  1928*1088@30fps */
	/****system****/
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x00},
	{0xf2, 0x00},
	{0xf3, 0x00},
	{0xf4, 0x36},
	{0xf5, 0xc0},
	{0xf6, 0x44},
	{0xf7, 0x01},
	{0xf8, 0x63},
	{0xf9, 0x40},
	{0xfc, 0x8e},
/****CISCTL & ANALOG****/
	{0xfe, 0x00},
	{0x87, 0x18},
	{0xee, 0x30},
	{0xd0, 0xb7},
	{0x03, 0x04},
	{0x04, 0x60},
	{0x05, 0x04},
	{0x06, 0x4c},
	{0x07, 0x00},
	{0x08, 0x11},
	{0x09, 0x00},
	{0x0a, 0x02},
	{0x0b, 0x00},
	{0x0c, 0x02},
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x12, 0xe2},
	{0x13, 0x16},
	{0x19, 0x0a},
	{0x21, 0x1c},
	{0x28, 0x0a},
	{0x29, 0x24},
	{0x2b, 0x04},
	{0x32, 0xf8},
	{0x37, 0x03},
	{0x39, 0x15},
	{0x43, 0x07},
	{0x44, 0x40},
	{0x46, 0x0b},
	{0x4b, 0x20},
	{0x4e, 0x08},
	{0x55, 0x20},
	{0x66, 0x05},
	{0x67, 0x05},
	{0x77, 0x01},
	{0x78, 0x00},
	{0x7c, 0x93},
	{0x8c, 0x12},
	{0x8d, 0x92},
	{0x90, 0x01},
	{0x9d, 0x10},
	{0xce, 0x7c},
	{0xd2, 0x41},
	{0xd3, 0xdc},
	{0xe6, 0x50},
	/*gain*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk*/
	{0x26, 0x30},
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x40},
	{0xfe, 0x04},
	{0x14, 0x78},
	{0x15, 0x78},
	{0x16, 0x78},
	{0x17, 0x78},
	/*window*/
	{0xfe, 0x01},
	{0x92, 0x00},
	{0x94, 0x03},
	{0x95, 0x04},
	{0x96, 0x40},
	{0x97, 0x07},
	{0x98, 0x88},
	/*ISP*/
	{0xfe, 0x01},
	{0x01, 0x05},
	{0x02, 0x89},
	{0x04, 0x01},
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},
	/****DVP & MIPI****/
	{0xfe, 0x01},
	{0x9a, 0x06},
	{0xfe, 0x00},
	{0x7b, 0x2a},
	{0x23, 0x2d},
	{0xfe, 0x03},
	{0x01, 0x27},
	{0x02, 0x56},
	{0x03, 0x8e},
	{0x12, 0x80},
	{0x13, 0x07},
	{0x15, 0x12},
	{0xfe, 0x00},
	{0x3e, 0x91},
};

static struct regval_list sensor_1080p27_5_regs[] = {
	/*  1928*1088@27.5fps */
	/****system****/
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x00},
	{0xf2, 0x00},
	{0xf3, 0x00},
	{0xf4, 0x36},
	{0xf5, 0xc0},
	{0xf6, 0x44},
	{0xf7, 0x01},
	{0xf8, 0x63},
	{0xf9, 0x40},
	{0xfc, 0x8e},
/****CISCTL & ANALOG****/
	{0xfe, 0x00},
	{0x87, 0x18},
	{0xee, 0x30},
	{0xd0, 0xb7},
	{0x03, 0x04},
	{0x04, 0x60},
	{0x05, 0x04},
	{0x06, 0x4c},
	{0x07, 0x00},
	{0x08, 0x6c},
	{0x09, 0x00},
	{0x0a, 0x02},
	{0x0b, 0x00},
	{0x0c, 0x02},
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x12, 0xe2},
	{0x13, 0x16},
	{0x19, 0x0a},
	{0x21, 0x1c},
	{0x28, 0x0a},
	{0x29, 0x24},
	{0x2b, 0x04},
	{0x32, 0xf8},
	{0x37, 0x03},
    {0x39, 0x15},
	{0x41, 0x04},
	{0x42, 0xbf},
	{0x43, 0x07},
	{0x44, 0x40},
	{0x46, 0x0b},
	{0x4b, 0x20},
	{0x4e, 0x08},
	{0x55, 0x20},
	{0x66, 0x05},
	{0x67, 0x05},
	{0x77, 0x01},
	{0x78, 0x00},
	{0x7c, 0x93},
	{0x8c, 0x12},
	{0x8d, 0x90},// 0x90
	{0x90, 0x01},
	{0x9d, 0x10},
	{0xce, 0x7c},
	{0xd2, 0x41},
	{0xd3, 0xdc},
	{0xe6, 0x50},
	/*gain*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk*/
	{0x26, 0x30},
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x40},
	{0xfe, 0x04},
	{0x14, 0x78},
	{0x15, 0x78},
	{0x16, 0x78},
	{0x17, 0x78},
	/*window*/
	{0xfe, 0x01},
	{0x92, 0x00},
	{0x94, 0x03},
	{0x95, 0x04},
	{0x96, 0x40},
	{0x97, 0x07},
	{0x98, 0x88},
	/*ISP*/
	{0xfe, 0x01},
	{0x01, 0x05},
	{0x02, 0x89},
	{0x04, 0x01},
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},
	/****DVP & MIPI****/
	{0xfe, 0x01},
	{0x9a, 0x06},
	{0xfe, 0x00},
	{0x7b, 0x2a},
	{0x23, 0x2d},
	{0xfe, 0x03},
	{0x01, 0x27},
	{0x02, 0x56},
	{0x03, 0x8e},
	{0x12, 0x80},
	{0x13, 0x07},
	{0x15, 0x12},
	{0xfe, 0x00},
//	{0x17, 0x83},
	{0x3e, 0x91},
};

/*
 * window_size=1920*1080 mipi@2lane
 * mclk=24mhz, mipi_clk=594Mbps
 * pixel_line_total=2640, line_frame_total=1125
 * row_time=35.55us, frame_rate=25fps
 */
static struct regval_list sensor_1080p25_regs[] = {
#if 0
	/*  1928*1088@25fps vts=1350*/
	/****system****/
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x00},
	{0xf2, 0x00},
	{0xf3, 0x00},
	{0xf4, 0x36},
	{0xf5, 0xc0},
	{0xf6, 0x44},
	{0xf7, 0x01},
	{0xf8, 0x63},
	{0xf9, 0x40},
	{0xfc, 0x8e},
/****CISCTL & ANALOG****/
	{0xfe, 0x00},
	{0x87, 0x18},
	{0xee, 0x30},
	{0xd0, 0xb7},
	{0x03, 0x04},
	{0x04, 0x60},
	{0x05, 0x04},
	{0x06, 0x4c},
	{0x07, 0x00},
	{0x08, 0xfa},//vb
	{0x09, 0x00},
	{0x0a, 0x02},
	{0x0b, 0x00},
	{0x0c, 0x02},
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x12, 0xe2},
	{0x13, 0x16},
	{0x19, 0x0a},
	{0x21, 0x1c},
	{0x28, 0x0a},
	{0x29, 0x24},
	{0x2b, 0x04},
	{0x32, 0xf8},
	{0x37, 0x03},
	{0x39, 0x15},
	{0x41, 0x05},//vts-1350
	{0x42, 0x46},
	{0x43, 0x07},
	{0x44, 0x40},
	{0x46, 0x0b},
	{0x4b, 0x20},
	{0x4e, 0x08},
	{0x55, 0x20},
	{0x66, 0x05},
	{0x67, 0x05},
	{0x77, 0x01},
	{0x78, 0x00},
	{0x7c, 0x93},
	{0x8c, 0x12},
	{0x8d, 0x90},// 0x90
	{0x90, 0x01},
	{0x9d, 0x10},
	{0xce, 0x7c},
	{0xd2, 0x41},
	{0xd3, 0xdc},
	{0xe6, 0x50},
	/*gain*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk*/
	{0x26, 0x30},
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x40},
	{0xfe, 0x04},
	{0x14, 0x78},
	{0x15, 0x78},
	{0x16, 0x78},
	{0x17, 0x78},
	/*window*/
	{0xfe, 0x01},
	{0x92, 0x00},
	{0x94, 0x03},
	{0x95, 0x04},
	{0x96, 0x40},
	{0x97, 0x07},
	{0x98, 0x88},
	/*ISP*/
	{0xfe, 0x01},
	{0x01, 0x05},
	{0x02, 0x89},
	{0x04, 0x01},
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},
	/****DVP & MIPI****/
	{0xfe, 0x01},
	{0x9a, 0x06},
	{0xfe, 0x00},
	{0x7b, 0x2a},
	{0x23, 0x2d},
	{0xfe, 0x03},
	{0x01, 0x27},
	{0x02, 0x56},
	{0x03, 0x8e},
	{0x12, 0x80},
	{0x13, 0x07},
	{0x15, 0x12},
	{0xfe, 0x00},
	{0x17, 0x83},
	{0x3e, 0x91},
#endif

    /*  1928*1088@25fps */
	/****system****/
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x00},
	{0xf2, 0x00},
	{0xf3, 0x00},
	{0xf4, 0x36},
	{0xf5, 0xc0},
	{0xf6, 0x44},
	{0xf7, 0x01},
	{0xf8, 0x63},
	{0xf9, 0x40},
	{0xfc, 0x8e},
/****CISCTL & ANALOG****/
	{0xfe, 0x00},
	{0x87, 0x18},
	{0xee, 0x30},
	{0xd0, 0xb7},
	{0x03, 0x04},
	{0x04, 0x60},
	{0x05, 0x05},//04
	{0x06, 0x28},//4c
	{0x07, 0x00},
	{0x08, 0x11},
	{0x09, 0x00},
	{0x0a, 0x02},
	{0x0b, 0x00},
	{0x0c, 0x02},
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x12, 0xe2},
	{0x13, 0x16},
	{0x19, 0x0a},
	{0x21, 0x1c},
	{0x28, 0x0a},
	{0x29, 0x24},
	{0x2b, 0x04},
	{0x32, 0xf8},
	{0x37, 0x03},
	{0x39, 0x15},
	{0x43, 0x07},
	{0x44, 0x40},
	{0x46, 0x0b},
	{0x4b, 0x20},
	{0x4e, 0x08},
	{0x55, 0x20},
	{0x66, 0x05},
	{0x67, 0x05},
	{0x77, 0x01},
	{0x78, 0x00},
	{0x7c, 0x93},
	{0x8c, 0x12},
	{0x8d, 0x92},
	{0x90, 0x01},
	{0x9d, 0x10},
	{0xce, 0x7c},
	{0xd2, 0x41},
	{0xd3, 0xdc},
	{0xe6, 0x50},
	/*gain*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk*/
	{0x26, 0x30},
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x40},
	{0xfe, 0x04},
	{0x14, 0x78},
	{0x15, 0x78},
	{0x16, 0x78},
	{0x17, 0x78},
	/*window*/
	{0xfe, 0x01},
	{0x92, 0x00},
	{0x94, 0x03},
	{0x95, 0x04},
	{0x96, 0x40},
	{0x97, 0x07},
	{0x98, 0x88},
	/*ISP*/
	{0xfe, 0x01},
	{0x01, 0x05},
	{0x02, 0x89},
	{0x04, 0x01},
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},
	/****DVP & MIPI****/
	{0xfe, 0x01},
	{0x9a, 0x06},
	{0xfe, 0x00},
	{0x7b, 0x2a},
	{0x23, 0x2d},
	{0xfe, 0x03},
	{0x01, 0x27},
	{0x02, 0x56},
	{0x03, 0x8e},
	{0x12, 0x80},
	{0x13, 0x07},
	{0x15, 0x12},
	{0xfe, 0x00},
	{0x17, 0x83},
	{0x3e, 0x91},
};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */

static struct regval_list sensor_fmt_raw[] = {

};


/*
 * Code for dealing with controls.
 * fill with different sensor module
 * different sensor module has different settings here
 * if not support the follow function , retrun -EINVAL
 */

static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	*value = info->exp;
	sensor_dbg("sensor_get_exposure = %d\n", info->exp);
	return 0;
}

static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_val)
{
	struct sensor_info *info = to_state(sd);
	int tmp_exp_val = exp_val / 16;

	sensor_dbg("exp_val:%d\n", exp_val);
	sensor_write(sd, 0x03, (tmp_exp_val >> 8) & 0xFF);
	sensor_write(sd, 0x04, (tmp_exp_val & 0xFF));
	info->exp = exp_val;
	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	*value = info->gain;
	sensor_dbg("sensor_get_gain = %d\n", info->gain);
	return 0;
}
unsigned char regValTable[29][4] = {
	{0x00, 0x00, 0x01, 0x00},
	{0x00, 0x10, 0x01, 0x0c},
	{0x00, 0x20, 0x01, 0x1b},
	{0x00, 0x30, 0x01, 0x2c},
	{0x00, 0x40, 0x01, 0x3f},
	{0x00, 0x50, 0x02, 0x16},
	{0x00, 0x60, 0x02, 0x35},
	{0x00, 0x70, 0x03, 0x16},
	{0x00, 0x80, 0x04, 0x02},
	{0x00, 0x90, 0x04, 0x31},
	{0x00, 0xa0, 0x05, 0x32},
	{0x00, 0xb0, 0x06, 0x35},
	{0x00, 0xc0, 0x08, 0x04},
	{0x00, 0x5a, 0x09, 0x19},
	{0x00, 0x83, 0x0b, 0x0f},
	{0x00, 0x93, 0x0d, 0x12},
	{0x00, 0x84, 0x10, 0x00},
	{0x00, 0x94, 0x12, 0x3a},
	{0x01, 0x2c, 0x1a, 0x02},
	{0x01, 0x3c, 0x1b, 0x20},
	{0x00, 0x8c, 0x20, 0x0f},
	{0x00, 0x9c, 0x26, 0x07},
	{0x02, 0x64, 0x36, 0x21},
	{0x02, 0x74, 0x37, 0x3a},
	{0x00, 0xc6, 0x3d, 0x02},
	{0x00, 0xdc, 0x3f, 0x3f},
	{0x02, 0x85, 0x3f, 0x3f},
	{0x02, 0x95, 0x3f, 0x3f},
	{0x00, 0xce, 0x3f, 0x3f}
};

unsigned int gainLevelTable[29] = {
	64,    74,    89,    102,   127,   147,
	177,   203,   260,   300,   361,   415,
	504,   581,   722,   832,   1027,  1182,
	1408,  1621,  1990,  2291,  2850,  3282,
	4048,  5180,  5500,  6744,  7073,
};

static int setSensorGain(struct v4l2_subdev *sd, int gain)
{
	int i;
	int total;
	unsigned int tol_dig_gain = 0;

	total = sizeof(gainLevelTable) / sizeof(unsigned int);
	for (i = 0; (total > 1) && (i < (total - 1)); i++) {
		if ((gainLevelTable[i] <= gain) && (gain < gainLevelTable[i+1]))
			break;
	}

	tol_dig_gain = gain * 64 / gainLevelTable[i];
	sensor_write(sd, 0xfe, 0x00);
	sensor_write(sd, 0xb4, regValTable[i][0]);
	sensor_write(sd, 0xb3, regValTable[i][1]);
	sensor_write(sd, 0xb8, regValTable[i][2]);
	sensor_write(sd, 0xb9, regValTable[i][3]);
	sensor_write(sd, 0xb1, (tol_dig_gain>>6));
	sensor_write(sd, 0xb2, ((tol_dig_gain&0x3f)<<2));

	return 0;
}
static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	struct sensor_info *info = to_state(sd);

	if (gain_val == info->gain) {
		return 0;
	}

	sensor_dbg("gain_val:%d\n", gain_val);
	setSensorGain(sd, gain_val * 4);

	info->gain = gain_val;

	return 0;
}


static int sensor_s_exp_gain(struct v4l2_subdev *sd,
				 struct sensor_exp_gain *exp_gain)
{
	int exp_val, gain_val;
	struct sensor_info *info = to_state(sd);

	exp_val = exp_gain->exp_val;
	gain_val = exp_gain->gain_val;

	if (gain_val < (1 * 16)) {
		gain_val = 16;
	}

	if (exp_val > 0xfffff)
		exp_val = 0xfffff;

	sensor_s_exp(sd, exp_val);
	sensor_s_gain(sd, gain_val);

	info->exp = exp_val;
	info->gain = gain_val;
	return 0;
}

/*
 *set && get sensor flip
 */

data_type sensor_flip_status;

static int sensor_get_fmt_mbus_core(struct v4l2_subdev *sd, int *code)
{
	struct sensor_info *info = to_state(sd);
	data_type get_value;

	//sensor_read(sd, 0x17, &get_value);
	switch (sensor_flip_status&0x03) {
	case 0x00:
		*code = MEDIA_BUS_FMT_SRGGB10_1X10;//*code = MEDIA_BUS_FMT_SGBRG10_1X10;//*code = MEDIA_BUS_FMT_SBGGR10_1X10;
		break;

	case 0x01:
		*code = MEDIA_BUS_FMT_SRGGB10_1X10;//*code = MEDIA_BUS_FMT_SBGGR10_1X10;//*code = MEDIA_BUS_FMT_SGBRG10_1X10;//*code = MEDIA_BUS_FMT_SGRBG10_1X10;
		break;

	case 0x02:
		*code = MEDIA_BUS_FMT_SBGGR10_1X10;//*code = MEDIA_BUS_FMT_SRGGB10_1X10;//*code = MEDIA_BUS_FMT_SGRBG10_1X10;//*code = MEDIA_BUS_FMT_SGBRG10_1X10;
		break;

	case 0x03:
		*code = MEDIA_BUS_FMT_SBGGR10_1X10;//*code = MEDIA_BUS_FMT_SGRBG10_1X10;//*code = MEDIA_BUS_FMT_SRGGB10_1X10;//*code = MEDIA_BUS_FMT_SBGGR10_1X10; 
		break;

	default:
		*code = info->fmt->mbus_code;
	}
	return 0;
}


static int sensor_s_hflip(struct v4l2_subdev *sd, int enable)
{
	data_type get_value;
	data_type set_value;

	if (!(enable == 0 || enable == 1))
		return -1;

	//sensor_read(sd, 0x17, &get_value);
	get_value = sensor_flip_status;
	if (enable)
		set_value = get_value | 0x01;
	else
		set_value = get_value & 0xFE;

	sensor_flip_status = set_value;
	sensor_write(sd, 0x17, set_value);
	return 0;
}

static int sensor_s_vflip(struct v4l2_subdev *sd, int enable)
{
	data_type get_value;
	data_type set_value;

	if (!(enable == 0 || enable == 1))
		return -1;

	//sensor_read(sd, 0x17, &get_value);
	get_value = sensor_flip_status;
	if (enable)
		set_value = get_value | 0x02;
	else
		set_value = get_value & 0xFD;

	sensor_flip_status = set_value;
	sensor_write(sd, 0x17, set_value);
	return 0;
}


/*
 * Stuff that knows about the sensor.
 */
static int sensor_power(struct v4l2_subdev *sd, int on)
{
	switch (on) {
	case STBY_ON:
		sensor_dbg("STBY_ON!\n");
		cci_lock(sd);
		vin_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		cci_unlock(sd);
		break;

	case STBY_OFF:
		sensor_dbg("STBY_OFF!\n");
		cci_lock(sd);
		vin_set_mclk_freq(sd, MCLK);
		vin_set_mclk(sd, ON);
		usleep(12000);
		vin_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		usleep(12000);
		cci_unlock(sd);
		usleep(12000);
		break;

	case PWR_ON:
		sensor_dbg("PWR_ON!\n");
		cci_lock(sd);
		vin_gpio_set_status(sd, PWDN, 1);
		vin_gpio_set_status(sd, RESET, 1);
		vin_gpio_set_status(sd, POWER_EN, 1);
		vin_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep(1200);
		#if 0//sk
		vin_gpio_write(sd, POWER_EN, CSI_GPIO_HIGH);
		#endif
		//vin_set_pmu_channel(sd, CMBCSI, ON);
		vin_set_pmu_channel(sd, IOVDD, ON);
		usleep(120);
		vin_set_pmu_channel(sd, DVDD, ON);
		usleep(1200);
		vin_set_pmu_channel(sd, AVDD, ON);
		vin_set_mclk_freq(sd, MCLK);
		usleep(1200);
		vin_set_mclk(sd, ON);
		usleep(12000);
		vin_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		usleep(12000);
		vin_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep(31000);
		cci_unlock(sd);
		break;

	case PWR_OFF:
		sensor_dbg("PWR_OFF!do nothing\n");
		cci_lock(sd);
		vin_set_mclk(sd, OFF);
		vin_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		//vin_set_pmu_channel(sd, CMBCSI, OFF);
		vin_set_pmu_channel(sd, AVDD, OFF);
		vin_set_pmu_channel(sd, DVDD, OFF);
		vin_set_pmu_channel(sd, IOVDD, OFF);
		usleep(12000);
		vin_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vin_gpio_set_status(sd, RESET, 0);
		vin_gpio_set_status(sd, PWDN, 0);
		cci_unlock(sd);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{

	sensor_dbg("%s: val=%d\n", __func__);
	switch (val) {
	case 0:
		vin_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep(12000);
		break;

	case 1:
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep(12000);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	data_type rdval;
	int eRet;
	int times_out = 3;
	do {
		eRet = sensor_read(sd, ID_REG_HIGH, &rdval);
		sensor_dbg("eRet:%d, ID_VAL_HIGH:0x%x, times_out:%d\n", eRet, rdval, times_out);
		usleep(220000);
		times_out--;
	} while (eRet < 0  &&  times_out > 0);

	sensor_read(sd, ID_REG_HIGH, &rdval);
	sensor_dbg("ID_VAL_HIGH = %2x, Done!\n", rdval);
	if (rdval != ID_VAL_HIGH)
		return -ENODEV;

	sensor_read(sd, ID_REG_LOW, &rdval);
	sensor_dbg("ID_VAL_LOW = %2x, Done!\n", rdval);
	if (rdval != ID_VAL_LOW)
		return -ENODEV;

	sensor_dbg("Done!\n");
	return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct sensor_info *info = to_state(sd);

	sensor_dbg("sensor_init\n");

	/*Make sure it is a target sensor */
	ret = sensor_detect(sd);
	if (ret) {
		sensor_err("chip found is not an target chip.\n");
		return ret;
	}

	info->focus_status = 0;
	info->low_speed    = 0;
	info->width        = 1920;
	info->height       = 1080;
	info->hflip        = 0;
	info->vflip        = 0;
	info->gain         = 0;
	info->exp          = 0;

	info->tpf.numerator      = 1;
	info->tpf.denominator    = 20;	/* 30fps */
	info->preview_first_flag = 1;
	return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	int ret = 0;
	struct sensor_info *info = to_state(sd);

	switch (cmd) {
	case GET_CURRENT_WIN_CFG:
		if (info->current_wins != NULL) {
			memcpy(arg, info->current_wins,
				sizeof(struct sensor_win_size));
			ret = 0;
		} else {
			sensor_err("empty wins!\n");
			ret = -1;
		}
		break;
	case SET_FPS:
		break;
	case VIDIOC_VIN_SENSOR_EXP_GAIN:
		sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
		break;
	//case VIDIOC_VIN_SENSOR_SET_FPS:
//		ret = sensor_s_fps(sd, (struct sensor_fps *)arg);
		//break;
	case VIDIOC_VIN_SENSOR_CFG_REQ:
		sensor_cfg_req(sd, (struct sensor_config *)arg);
		break;
	default:
		return -EINVAL;
	}
	return ret;
}

/*
 * Store information about the video data format.
 */
static struct sensor_format_struct sensor_formats[] = {
	{
		.desc      = "Raw RGB Bayer",
		.mbus_code = MEDIA_BUS_FMT_SRGGB10_1X10, /*.mbus_code = MEDIA_BUS_FMT_SBGGR10_1X10, */
		.regs      = sensor_fmt_raw,
		.regs_size = ARRAY_SIZE(sensor_fmt_raw),
		.bpp       = 1
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */

static struct sensor_win_size sensor_win_sizes[] = {

	{
		.width      = 1920,
		.height     = 1080,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 3300,
		.vts        = 1125,
		.pclk       = 74250000,
		.mipi_bps   = 297 * 1000 * 1000,
		.fps_fixed  = 20,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = 1125 << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 110 << 4,
		.regs       = sensor_1080p20_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p20_regs),
		.set_size   = NULL,
	},

	{
		.width      = 1928,
		.height     = 1088,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 2200,
		.vts        = 1125,
		.pclk       = 74250000,
		.mipi_bps   = 297 * 1000 * 1000,
		.fps_fixed  = 30,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = 1125 << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 110 << 4,
		.regs       = sensor_1080p30_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p30_regs),
		.set_size   = NULL,
		.vipp_w     = 1920,
		.vipp_h     = 1080,
		.vipp_hoff  = 4,
		.vipp_voff  = 4,
	},

	{
		.width      = 1928,
		.height	    = 1088,
		.hoffset    = 0,
		.voffset    = 0,
		.hts	    = 2200,
		.vts        = 1215,
		.pclk	    = 74250000,
		.mipi_bps   = 297 * 1000 * 1000,
		.fps_fixed  = 27,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = 1125 << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 110 << 4,
		.regs	    = sensor_1080p27_5_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p27_5_regs),
		.set_size   = NULL,
		.vipp_w	    = 1920,
		.vipp_h	    = 1080,
		.vipp_hoff  = 4,
		.vipp_voff  = 4,
    },

    {
		.width      = 1928,
		.height     = 1086,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 2200,
		.vts        = 1125,
		.pclk       = 74250000,
		.mipi_bps   = 297 * 1000 * 1000,
		.fps_fixed  = 25,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = (1125 - 16) << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 110 << 4,
		.regs       = sensor_1080p25_regs,
		.regs_size  = ARRAY_SIZE(sensor_1080p25_regs),
		.set_size   = NULL,
		.vipp_w     = 1920,
		.vipp_h     = 1080,
		.vipp_hoff  = 4,
		.vipp_voff  = 3,
	},
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
				struct v4l2_mbus_config *cfg)
{
	//struct sensor_info *info = to_state(sd);

	cfg->type  = V4L2_MBUS_CSI2;
	cfg->flags = 0 | V4L2_MBUS_CSI2_2_LANE | V4L2_MBUS_CSI2_CHANNEL_0;

	return 0;
}

static int sensor_g_ctrl(struct v4l2_ctrl *ctrl)
{
	struct sensor_info *info =
			container_of(ctrl->handler, struct sensor_info, handler);
	struct v4l2_subdev *sd = &info->sd;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_g_gain(sd, &ctrl->val);
	case V4L2_CID_EXPOSURE:
		return sensor_g_exp(sd, &ctrl->val);
	}
	return -EINVAL;
}

static int sensor_queryctrl(struct v4l2_subdev *sd, struct v4l2_queryctrl *qc)
{
	/* Fill in min, max, step and default value for these controls. */
	/* see include/linux/videodev2.h for details */
	switch (qc->id) {
	case V4L2_CID_GAIN:
		return v4l2_ctrl_query_fill(qc, 1 * 16, 128 * 16 - 1, 1, 16);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, 1, 65536 * 16, 1, 1);
	}
	return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_queryctrl qc;
	int ret;

	struct sensor_info *info =
			container_of(ctrl->handler, struct sensor_info, handler);
	struct v4l2_subdev *sd = &info->sd;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_s_gain(sd, ctrl->val);
	case V4L2_CID_EXPOSURE:
		return sensor_s_exp(sd, ctrl->val);
	case V4L2_CID_HFLIP:
		return sensor_s_hflip(sd, ctrl->val);
	case V4L2_CID_VFLIP:
		return sensor_s_vflip(sd, ctrl->val);
	}
	return -EINVAL;
}

static int sensor_reg_init(struct sensor_info *info)
{
	int ret;
	struct v4l2_subdev *sd = &info->sd;
	struct sensor_format_struct *sensor_fmt = info->fmt;
	struct sensor_win_size *wsize = info->current_wins;

	ret = sensor_write_array(sd, sensor_default_regs,
				 ARRAY_SIZE(sensor_default_regs));
	if (ret < 0) {
		sensor_err("write sensor_default_regs error\n");
		return ret;
	}

	sensor_write_array(sd, sensor_fmt->regs, sensor_fmt->regs_size);

	sensor_flip_status = 0x80;

	if (wsize->regs)
		sensor_write_array(sd, wsize->regs, wsize->regs_size);

	if (wsize->set_size)
		wsize->set_size(sd);

	info->width = wsize->width;
	info->height = wsize->height;

	sensor_dbg("s_fmt set width = %d, height = %d\n", wsize->width,
			 wsize->height);

	return 0;
}

static int sensor_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct sensor_info *info = to_state(sd);

	sensor_dbg("%s on = %d, %d*%d fps: %d code: %x\n", __func__, enable,
			 info->current_wins->width, info->current_wins->height,
			 info->current_wins->fps_fixed, info->fmt->mbus_code);

	if (!enable)
		return 0;

	return sensor_reg_init(info);
}

/* ----------------------------------------------------------------------- */
static const struct v4l2_ctrl_ops sensor_ctrl_ops = {
	.g_volatile_ctrl = sensor_g_ctrl,
	.s_ctrl = sensor_s_ctrl,
	.try_ctrl = sensor_try_ctrl,
	//.queryctrl = sensor_queryctrl,
};

static const struct v4l2_subdev_core_ops sensor_core_ops = {

	.reset = sensor_reset,
	.init = sensor_init,
	.s_power = sensor_power,
	.ioctl = sensor_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = sensor_compat_ioctl32,
#endif
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.s_parm = sensor_s_parm,
	.g_parm = sensor_g_parm,
	.s_stream = sensor_s_stream,
	.g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_pad_ops sensor_pad_ops = {
	.enum_mbus_code = sensor_enum_mbus_code,
	.enum_frame_size = sensor_enum_frame_size,
	.get_fmt = sensor_get_fmt,
	.set_fmt = sensor_set_fmt,
};

static const struct v4l2_subdev_ops sensor_ops = {
	.core = &sensor_core_ops,
	.video = &sensor_video_ops,
	.pad = &sensor_pad_ops,
};

/* ----------------------------------------------------------------------- */
static struct cci_driver cci_drv[] = {
	{
		.name = SENSOR_NAME,
		//.addr_width = CCI_BITS_16,
		.addr_width = CCI_BITS_8,
		.data_width = CCI_BITS_8,
	}, {
		.name = SENSOR_NAME_2,
		//.addr_width = CCI_BITS_16,
		.addr_width = CCI_BITS_8,
		.data_width = CCI_BITS_8,
	}
};

static int sensor_init_controls(struct v4l2_subdev *sd, const struct v4l2_ctrl_ops *ops)
{
	struct sensor_info *info = to_state(sd);
	struct v4l2_ctrl_handler *handler = &info->handler;
	struct v4l2_ctrl *ctrl;
	int ret = 0;

	v4l2_ctrl_handler_init(handler, 2);

	ctrl = v4l2_ctrl_new_std(handler, ops, V4L2_CID_GAIN, 1 * 1600,
				  256 * 1600, 1, 1 * 1600);

	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std(handler, ops, V4L2_CID_EXPOSURE, 1,
			      65536 * 16, 1, 1);
	v4l2_ctrl_new_std(handler, ops, V4L2_CID_HFLIP, 0, 1, 1, 0);
	v4l2_ctrl_new_std(handler, ops, V4L2_CID_VFLIP, 0, 1, 1, 0);

	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	if (handler->error) {
		ret = handler->error;
		v4l2_ctrl_handler_free(handler);
	}

	sd->ctrl_handler = handler;

	return ret;
}

static int sensor_probe(int id)
{
	struct v4l2_subdev *sd;
	struct sensor_info *info;

	info = rt_calloc(1, sizeof(struct sensor_info));
	if (info == NULL)
		return -ENOMEM;

	sd = &info->sd;
	cci_dev_probe_helper(sd, &sensor_ops, &cci_drv[id]);

	sensor_init_controls(sd, &sensor_ctrl_ops);

	rt_mutex_init(&info->lock, "sinfomtx", RT_IPC_FLAG_FIFO);

	info->fmt = &sensor_formats[0];
	info->fmt_pt = &sensor_formats[0];
	info->win_pt = &sensor_win_sizes[0];
	info->fmt_num = N_FMTS;
	info->win_size_num = N_WIN_SIZES;
	info->sensor_field = V4L2_FIELD_NONE;
	// use CMB_PHYA_OFFSET2  also ok
	info->combo_mode = CMB_TERMINAL_RES | CMB_PHYA_OFFSET3 | MIPI_NORMAL_MODE;
	//info->combo_mode = CMB_PHYA_OFFSET2 | MIPI_NORMAL_MODE;
	info->stream_seq = MIPI_BEFORE_SENSOR;
	info->af_first_flag = 1;
	info->exp = 0;
	info->gain = 0;

	return 0;
}

static int sensor_remove(int id)
{
	struct v4l2_subdev *sd;
	int i;

	sd = cci_dev_remove_helper(&cci_drv[id]);

	rt_free(to_state(sd));
	return 0;
}

static int gc2053_mipi_init(void)
{
	sensor_probe(0);

	return 0;
}

static void gc2053_mipi_remove(void)
{
	sensor_remove(0);
}

device_initcall(gc2053_mipi_init);

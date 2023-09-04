/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) Fuzhou Rockchip Electronics Co.Ltd
 * Author:
 *      Sandy Huang <hjc@rock-chips.com>
 *      Mark Yao <mark.yao@rock-chips.com>
 */

#ifndef _ROCKCHIP_LVDS_
#define _ROCKCHIP_LVDS_

#define RK3288_LVDS_CH0_REG0			0x00
#define RK3288_LVDS_CH0_REG0_LVDS_EN		BIT(7)
#define RK3288_LVDS_CH0_REG0_TTL_EN		BIT(6)
#define RK3288_LVDS_CH0_REG0_LANECK_EN		BIT(5)
#define RK3288_LVDS_CH0_REG0_LANE4_EN		BIT(4)
#define RK3288_LVDS_CH0_REG0_LANE3_EN		BIT(3)
#define RK3288_LVDS_CH0_REG0_LANE2_EN		BIT(2)
#define RK3288_LVDS_CH0_REG0_LANE1_EN		BIT(1)
#define RK3288_LVDS_CH0_REG0_LANE0_EN		BIT(0)

#define RK3288_LVDS_CH0_REG1			0x04
#define RK3288_LVDS_CH0_REG1_LANECK_BIAS	BIT(5)
#define RK3288_LVDS_CH0_REG1_LANE4_BIAS		BIT(4)
#define RK3288_LVDS_CH0_REG1_LANE3_BIAS		BIT(3)
#define RK3288_LVDS_CH0_REG1_LANE2_BIAS		BIT(2)
#define RK3288_LVDS_CH0_REG1_LANE1_BIAS		BIT(1)
#define RK3288_LVDS_CH0_REG1_LANE0_BIAS		BIT(0)

#define RK3288_LVDS_CH0_REG2			0x08
#define RK3288_LVDS_CH0_REG2_RESERVE_ON		BIT(7)
#define RK3288_LVDS_CH0_REG2_LANECK_LVDS_MODE	BIT(6)
#define RK3288_LVDS_CH0_REG2_LANE4_LVDS_MODE	BIT(5)
#define RK3288_LVDS_CH0_REG2_LANE3_LVDS_MODE	BIT(4)
#define RK3288_LVDS_CH0_REG2_LANE2_LVDS_MODE	BIT(3)
#define RK3288_LVDS_CH0_REG2_LANE1_LVDS_MODE	BIT(2)
#define RK3288_LVDS_CH0_REG2_LANE0_LVDS_MODE	BIT(1)
#define RK3288_LVDS_CH0_REG2_PLL_FBDIV8		BIT(0)

#define RK3288_LVDS_CH0_REG3			0x0c
#define RK3288_LVDS_CH0_REG3_PLL_FBDIV_MASK	0xff

#define RK3288_LVDS_CH0_REG4			0x10
#define RK3288_LVDS_CH0_REG4_LANECK_TTL_MODE	BIT(5)
#define RK3288_LVDS_CH0_REG4_LANE4_TTL_MODE	BIT(4)
#define RK3288_LVDS_CH0_REG4_LANE3_TTL_MODE	BIT(3)
#define RK3288_LVDS_CH0_REG4_LANE2_TTL_MODE	BIT(2)
#define RK3288_LVDS_CH0_REG4_LANE1_TTL_MODE	BIT(1)
#define RK3288_LVDS_CH0_REG4_LANE0_TTL_MODE	BIT(0)

#define RK3288_LVDS_CH0_REG5			0x14
#define RK3288_LVDS_CH0_REG5_LANECK_TTL_DATA	BIT(5)
#define RK3288_LVDS_CH0_REG5_LANE4_TTL_DATA	BIT(4)
#define RK3288_LVDS_CH0_REG5_LANE3_TTL_DATA	BIT(3)
#define RK3288_LVDS_CH0_REG5_LANE2_TTL_DATA	BIT(2)
#define RK3288_LVDS_CH0_REG5_LANE1_TTL_DATA	BIT(1)
#define RK3288_LVDS_CH0_REG5_LANE0_TTL_DATA	BIT(0)

#define RK3288_LVDS_CFG_REGC			0x30
#define RK3288_LVDS_CFG_REGC_PLL_ENABLE		0x00
#define RK3288_LVDS_CFG_REGC_PLL_DISABLE	0xff

#define RK3288_LVDS_CH0_REGD			0x34
#define RK3288_LVDS_CH0_REGD_PLL_PREDIV_MASK	0x1f

#define RK3288_LVDS_CH0_REG20			0x80
#define RK3288_LVDS_CH0_REG20_MSB		0x45
#define RK3288_LVDS_CH0_REG20_LSB		0x44

#define RK3288_LVDS_CFG_REG21			0x84
#define RK3288_LVDS_CFG_REG21_TX_ENABLE		0x92
#define RK3288_LVDS_CFG_REG21_TX_DISABLE	0x00
#define RK3288_LVDS_CH1_OFFSET                 0x100

/* fbdiv value is split over 2 registers, with bit8 in reg2 */
#define RK3288_LVDS_PLL_FBDIV_REG2(_fbd) \
		(_fbd & BIT(8) ? RK3288_LVDS_CH0_REG2_PLL_FBDIV8 : 0)
#define RK3288_LVDS_PLL_FBDIV_REG3(_fbd) \
		(_fbd & RK3288_LVDS_CH0_REG3_PLL_FBDIV_MASK)
#define RK3288_LVDS_PLL_PREDIV_REGD(_pd) \
		(_pd & RK3288_LVDS_CH0_REGD_PLL_PREDIV_MASK)

#define RK3288_LVDS_SOC_CON6_SEL_VOP_LIT	BIT(3)

#define LVDS_FMT_MASK				(0x07 << 16)
#define LVDS_MSB				BIT(3)
#define LVDS_DUAL				BIT(4)
#define LVDS_FMT_1				BIT(5)
#define LVDS_TTL_EN				BIT(6)
#define LVDS_START_PHASE_RST_1			BIT(7)
#define LVDS_DCLK_INV				BIT(8)
#define LVDS_CH0_EN				BIT(11)
#define LVDS_CH1_EN				BIT(12)
#define LVDS_PWRDN				BIT(15)

#define LVDS_24BIT				(0 << 1)
#define LVDS_18BIT				(1 << 1)
#define LVDS_FORMAT_VESA			(0 << 0)
#define LVDS_FORMAT_JEIDA			(1 << 0)

#define LVDS_VESA_24				0
#define LVDS_JEIDA_24				1
#define LVDS_VESA_18				2
#define LVDS_JEIDA_18				3

#endif /* _ROCKCHIP_LVDS_ */

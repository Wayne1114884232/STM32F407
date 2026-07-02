#ifndef __TFTLCD_FSMC_H
#define __TFTLCD_FSMC_H

#include "sys.h"   // 必须包含stm32f4xx.h，否则编译报错

/* FSMC 配置（与HAL库完全一致） */
#define TFTLCD_FSMC_BANK               FSMC_Bank1_NORSRAM4
#define TFTLCD_FSMC_BANK_ADDR          (0x6C000000)
#define TFTLCD_FSMC_REG_SEL            (6)

/* 时序参数（与HAL库完全一致） */
#define TFTLCD_FSMC_READ_AST           0x0F
#define TFTLCD_FSMC_READ_DST           0x3C
#define TFTLCD_FSMC_WRITE_AST          0x04
#define TFTLCD_FSMC_WRITE_DST          0x04

/* 寄存器/数据地址（与HAL库完全一致） */
#define TFTLCD_FSMC_REG_ADDR  (TFTLCD_FSMC_BANK_ADDR | (((1U << TFTLCD_FSMC_REG_SEL)-1) << 1))
#define TFTLCD_FSMC_DAT_ADDR  (TFTLCD_FSMC_BANK_ADDR | ((1U << TFTLCD_FSMC_REG_SEL) << 1))

/* 操作宏（与HAL库完全一致） */
#define TFTLCD_FSMC_REG    (*(volatile uint16_t *)TFTLCD_FSMC_REG_ADDR)
#define TFTLCD_FSMC_DAT    (*(volatile uint16_t *)TFTLCD_FSMC_DAT_ADDR)

/* 引脚定义（与正点原子硬件完全一致） */
#define TFTLCD_RS_PORT         GPIOF
#define TFTLCD_RS_PIN          GPIO_Pin_12
#define TFTLCD_RS_AF           GPIO_AF_FSMC  // FSMC 复用功能(AF12)
#define TFTLCD_RS_CLK          RCC_AHB1Periph_GPIOF

#define TFTLCD_CS_PORT         GPIOG
#define TFTLCD_CS_PIN          GPIO_Pin_12
#define TFTLCD_CS_AF           GPIO_AF_FSMC
#define TFTLCD_CS_CLK          RCC_AHB1Periph_GPIOG

#define TFTLCD_RD_PORT         GPIOD
#define TFTLCD_RD_PIN          GPIO_Pin_4
#define TFTLCD_RD_AF           GPIO_AF_FSMC
#define TFTLCD_RD_CLK          RCC_AHB1Periph_GPIOD

#define TFTLCD_WR_PORT         GPIOD
#define TFTLCD_WR_PIN          GPIO_Pin_5
#define TFTLCD_WR_AF           GPIO_AF_FSMC
#define TFTLCD_WR_CLK          RCC_AHB1Periph_GPIOD

/* 数据口 D0~D15 */
#define TFTLCD_D0_PIN          GPIO_Pin_14
#define TFTLCD_D1_PIN          GPIO_Pin_15
#define TFTLCD_D2_PIN          GPIO_Pin_0
#define TFTLCD_D3_PIN          GPIO_Pin_1
#define TFTLCD_D4_PIN          GPIO_Pin_7
#define TFTLCD_D5_PIN          GPIO_Pin_8
#define TFTLCD_D6_PIN          GPIO_Pin_9
#define TFTLCD_D7_PIN          GPIO_Pin_10
#define TFTLCD_D8_PIN          GPIO_Pin_11
#define TFTLCD_D9_PIN          GPIO_Pin_12
#define TFTLCD_D10_PIN         GPIO_Pin_13
#define TFTLCD_D11_PIN         GPIO_Pin_14
#define TFTLCD_D12_PIN         GPIO_Pin_15
#define TFTLCD_D13_PIN         GPIO_Pin_8
#define TFTLCD_D14_PIN         GPIO_Pin_9
#define TFTLCD_D15_PIN         GPIO_Pin_10

/* 函数声明 */
void tftlcd_fsmc_init(void);

#endif

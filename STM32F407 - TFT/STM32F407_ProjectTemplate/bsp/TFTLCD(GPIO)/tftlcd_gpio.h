#ifndef __TFTLCD_GPIO_H
#define __TFTLCD_GPIO_H

#include "sys.h"

/* -------------------------- TFTLCD GPIO 引脚定义 -------------------------- */
/* RS -> PA15 */
#define RCC_RS_GPIO         RCC_AHB1Periph_GPIOA    
#define PORT_RS_GPIO        GPIOA
#define GPIO_RS_PIN         GPIO_Pin_15
#define RS_OUT(x)           GPIO_WriteBit(PORT_RS_GPIO, GPIO_RS_PIN, (BitAction)x)

/* CS -> PC6 */
#define RCC_CS_GPIO         RCC_AHB1Periph_GPIOC    
#define PORT_CS_GPIO        GPIOC
#define GPIO_CS_PIN         GPIO_Pin_6
#define CS_OUT(x)           GPIO_WriteBit(PORT_CS_GPIO, GPIO_CS_PIN, (BitAction)x)

/* RD -> PB7 */
#define RCC_RD_GPIO         RCC_AHB1Periph_GPIOB    
#define PORT_RD_GPIO        GPIOB
#define GPIO_RD_PIN         GPIO_Pin_7
#define RD_OUT(x)           GPIO_WriteBit(PORT_RD_GPIO, GPIO_RD_PIN, (BitAction)x)

/* WR -> PB5 */
#define RCC_WR_GPIO         RCC_AHB1Periph_GPIOB   
#define PORT_WR_GPIO        GPIOB
#define GPIO_WR_PIN         GPIO_Pin_5
#define WR_OUT(x)           GPIO_WriteBit(PORT_WR_GPIO, GPIO_WR_PIN, (BitAction)x)

/* DATA -> PD 全引脚 */
#define RCC_DATA_GPIO       RCC_AHB1Periph_GPIOD   
#define PORT_DATA_GPIO      GPIOD
#define GPIO_DATA_PIN       GPIO_Pin_All
#define DATA_OUT(x)         GPIO_Write(PORT_DATA_GPIO, x)
#define DATA_IN()           GPIO_ReadInputData(PORT_DATA_GPIO)

/* -------------------------- 操作函数声明 -------------------------- */
void tftlcd_gpio_init(void);                            /* GPIO初始化 */
void tftlcd_gpio_write_cmd(uint16_t cmd);               /* 写命令 */
void tftlcd_gpio_write_dat(uint16_t dat);               /* 写数据 */
void tftlcd_gpio_write_reg(uint16_t reg, uint16_t dat); /* 写寄存器 */
uint16_t tftlcd_gpio_read_dat(void);                    /* 读数据 */

#endif




#ifndef __TFTLCD_TOUCH_IIC_H
#define __TFTLCD_TOUCH_IIC_H

#include "tftlcd.h"

#if (tftlcd_USING_TOUCH != 0)

/* -------------------------- TFT触摸IIC 引脚定义 -------------------------- */
/* SCL -> PC7 */
#define RCC_TOUCH_IIC_SCL_GPIO     RCC_AHB1Periph_GPIOC   
#define PORT_TOUCH_IIC_SCL_GPIO    GPIOC
#define GPIO_TOUCH_IIC_SCL_PIN     GPIO_Pin_7
#define TOUCH_IIC_SCL(x)           GPIO_WriteBit(PORT_TOUCH_IIC_SCL_GPIO, GPIO_TOUCH_IIC_SCL_PIN, (BitAction)x)

/* SDA -> PB12 */
#define RCC_TOUCH_IIC_SDA_GPIO     RCC_AHB1Periph_GPIOB    
#define PORT_TOUCH_IIC_SDA_GPIO    GPIOB
#define GPIO_TOUCH_IIC_SDA_PIN     GPIO_Pin_12
#define TOUCH_IIC_SDA(x)           GPIO_WriteBit(PORT_TOUCH_IIC_SDA_GPIO, GPIO_TOUCH_IIC_SDA_PIN, (BitAction)x)
#define TOUCH_IIC_READ_SDA()       GPIO_ReadInputDataBit(PORT_TOUCH_IIC_SDA_GPIO, GPIO_TOUCH_IIC_SDA_PIN)

/* 错误代码 */
#define tftlcd_TOUCH_IIC_EOK    0   /* 没有错误 */
#define tftlcd_TOUCH_IIC_ERROR  1   /* 错误 */

/* 操作函数 */
void tftlcd_touch_iic_init(uint8_t iic_addr);
uint8_t tftlcd_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len);
void tftlcd_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len);

#endif /* tftlcd_USING_TOUCH */

#endif




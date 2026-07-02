#ifndef __TFTLCD_TOUCH_IIC_H
#define __TFTLCD_TOUCH_IIC_H

#include "sys.h"

#if (TFTLCD_USING_TOUCH != 0)

/************************** IIC引脚宏定义 **************************/
/* SCL PB0 */
#define TFTLCD_TOUCH_IIC_SCL_PORT        GPIOB
#define TFTLCD_TOUCH_IIC_SCL_PIN         GPIO_Pin_0
#define TFTLCD_TOUCH_IIC_SCL_CLK         RCC_AHB1Periph_GPIOB
#define TFTLCD_TOUCH_IIC_SCL(x)          GPIO_WriteBit(TFTLCD_TOUCH_IIC_SCL_PORT, TFTLCD_TOUCH_IIC_SCL_PIN, (BitAction)x)

/* SDA PF11 */
#define TFTLCD_TOUCH_IIC_SDA_PORT        GPIOF
#define TFTLCD_TOUCH_IIC_SDA_PIN         GPIO_Pin_11
#define TFTLCD_TOUCH_IIC_SDA_CLK         RCC_AHB1Periph_GPIOF
#define TFTLCD_TOUCH_IIC_SDA(x)          GPIO_WriteBit(TFTLCD_TOUCH_IIC_SDA_PORT, TFTLCD_TOUCH_IIC_SDA_PIN, (BitAction)x)
#define TFTLCD_TOUCH_IIC_READ_SDA()      GPIO_ReadInputDataBit(TFTLCD_TOUCH_IIC_SDA_PORT, TFTLCD_TOUCH_IIC_SDA_PIN)

/* IIC读写标志 */
#define TFTLCD_TOUCH_IIC_WRITE   0
#define TFTLCD_TOUCH_IIC_READ    1

/* 错误码 */
#define TFTLCD_TOUCH_IIC_EOK     0
#define TFTLCD_TOUCH_IIC_ERROR   1

/************************** 外部函数 **************************/
void tftlcd_touch_iic_init(uint8_t iic_addr);
uint8_t tftlcd_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len);
void tftlcd_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len);

#endif

#endif

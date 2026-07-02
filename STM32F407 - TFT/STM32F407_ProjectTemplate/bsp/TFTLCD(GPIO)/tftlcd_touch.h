#ifndef __TFTLCD_TOUCH_H
#define __TFTLCD_TOUCH_H

#include "tftlcd.h"

#if (tftlcd_USING_TOUCH != 0)

/* -------------------------- TFT触摸 引脚定义 -------------------------- */
/* PEN -> PB6 */
#define RCC_TOUCH_PEN_GPIO     RCC_AHB1Periph_GPIOB    
#define PORT_TOUCH_PEN_GPIO    GPIOB
#define GPIO_TOUCH_PEN_PIN     GPIO_Pin_6
#define TOUCH_READ_PEN()       GPIO_ReadInputDataBit(PORT_TOUCH_PEN_GPIO, GPIO_TOUCH_PEN_PIN)
#define TOUCH_PEN_OUT(x)       GPIO_WriteBit(PORT_TOUCH_PEN_GPIO, GPIO_TOUCH_PEN_PIN, (BitAction)x)

/* TCS -> PA8 */
#define RCC_TOUCH_TCS_GPIO     RCC_AHB1Periph_GPIOA    
#define PORT_TOUCH_TCS_GPIO    GPIOA
#define GPIO_TOUCH_TCS_PIN     GPIO_Pin_8
#define TOUCH_TCS_OUT(x)       GPIO_WriteBit(PORT_TOUCH_TCS_GPIO, GPIO_TOUCH_TCS_PIN, (BitAction)x)

/* ATK-MD0430触摸IIC通讯地址枚举 */
typedef enum
{
    tftlcd_TOUCH_IIC_ADDR_14 = 0x14,    /* 0x14 */
    tftlcd_TOUCH_IIC_ADDR_5D = 0x5D,    /* 0x5D */
} tftlcd_touch_iic_addr_t;

/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;     /* 触摸点X坐标 */
    uint16_t y;     /* 触摸点Y坐标 */
    uint16_t size;  /* 触摸点大小 */
} tftlcd_touch_point_t;

/* 错误代码 */
#define tftlcd_TOUCH_EOK                    0   /* 没有错误 */
#define tftlcd_TOUCH_ERROR                  1   /* 错误 */

/* 操作函数 */
uint8_t tftlcd_touch_init(void);
uint8_t tftlcd_touch_scan(tftlcd_touch_point_t *point, uint8_t cnt);

#endif /* tftlcd_USING_TOUCH */

#endif




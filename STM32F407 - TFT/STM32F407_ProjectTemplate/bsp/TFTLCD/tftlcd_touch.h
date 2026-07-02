#ifndef __TFTLCD_TOUCH_H
#define __TFTLCD_TOUCH_H

#include "sys.h"
#include "tftlcd.h"

/* 触摸功能使能 */
#define TFTLCD_USING_TOUCH    1

#if (TFTLCD_USING_TOUCH != 0)

/************************** 引脚宏定义 标准库风格 **************************/
/* PEN中断引脚 PB1 */
#define TFTLCD_TOUCH_PEN_GPIO_PORT        GPIOB
#define TFTLCD_TOUCH_PEN_GPIO_PIN         GPIO_Pin_1
#define TFTLCD_TOUCH_PEN_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define TFTLCD_TOUCH_READ_PEN()           GPIO_ReadInputDataBit(TFTLCD_TOUCH_PEN_GPIO_PORT, TFTLCD_TOUCH_PEN_GPIO_PIN)
#define TFTLCD_TOUCH_PEN(x)               GPIO_WriteBit(TFTLCD_TOUCH_PEN_GPIO_PORT, TFTLCD_TOUCH_PEN_GPIO_PIN, (BitAction)x)

/* TCS片选引脚 PC13 */
#define TFTLCD_TOUCH_TCS_GPIO_PORT        GPIOC
#define TFTLCD_TOUCH_TCS_GPIO_PIN         GPIO_Pin_13
#define TFTLCD_TOUCH_TCS_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define TFTLCD_TOUCH_TCS(x)               GPIO_WriteBit(TFTLCD_TOUCH_TCS_GPIO_PORT, TFTLCD_TOUCH_TCS_GPIO_PIN, (BitAction)x)

/************************** 类型定义 **************************/
/* IIC通讯地址枚举 */
typedef enum
{
    TFTLCD_TOUCH_IIC_ADDR_14 = 0x14,
    TFTLCD_TOUCH_IIC_ADDR_5D = 0x5D,
} tftlcd_touch_iic_addr_t;

/* 触摸点坐标结构体 */
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t size;
} tftlcd_touch_point_t;

/************************** 配置参数 **************************/
#define TFTLCD_TOUCH_TP_MAX      5       /* 最大5点触摸 */
#define TFTLCD_TOUCH_PID         "911"   /* 触摸模块PID */

/* 错误码 */
#define TFTLCD_TOUCH_EOK         0
#define TFTLCD_TOUCH_ERROR       1

/************************** 外部函数 **************************/
uint8_t tftlcd_touch_init(void);
uint8_t tftlcd_touch_scan(tftlcd_touch_point_t *point, uint8_t cnt);

#endif

#endif

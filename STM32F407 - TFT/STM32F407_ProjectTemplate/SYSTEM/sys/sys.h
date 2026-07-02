#ifndef __SYS_H
#define __SYS_H

#include "stm32f4xx.h"
#include "delay.h"
#include "oled.h"
#include "control.h"
#include "encoder.h"
#include "exti.h"
#include "bsp_tb6612.h"
#include "LED.h"
#include "Key.h"
#include "wit_c_sdk.h"
#include "REG.h"
#include "buzzer.h"
#include "usart3.h"
//#include "bsp_ultrasonic.h"
//#include "bsp_vl53l0x.h"
//#include "ADC.h"
//#include "MENU.h"
//#include "global_vars.h"
//#include "usart3_2.h"
////////////////////////////////////////////////////////////////////////////
#include "tftlcd.h"
//#include "tftlcd_GPIO.h"
//#include "tftlcd_TOUCH.h"
//#include "tftlcd_TOUCH_IIC.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定义一些常用的数据类型宏定义
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;
typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t vs8;
typedef __I int32_t vsc32;
typedef __I int16_t vsc16;
typedef __I int8_t vsc8;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;
typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;
typedef __I uint32_t vuc32;
typedef __I uint16_t vuc16;
typedef __I uint8_t vuc8;

// 位带操作，实现类似 51 的 IO 口操作
// 更多详情参考《CM3 权威指南》中的相关章节（87 页～92 页）。
// IO 口位带操作定义
// STM32F4
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOA_ODR_Addr (GPIOA_BASE+0x14) // 0x40020014
#define GPIOB_ODR_Addr (GPIOB_BASE+0x14) // 0x40020414
#define GPIOC_ODR_Addr (GPIOC_BASE+0x14) // 0x40020814
#define GPIOD_ODR_Addr (GPIOD_BASE+0x14) // 0x40020C14
#define GPIOE_ODR_Addr (GPIOE_BASE+0x14) // 0x40021014
#define GPIOF_ODR_Addr (GPIOF_BASE+0x14) // 0x40021414
#define GPIOG_ODR_Addr (GPIOG_BASE+0x14) // 0x40021814
#define GPIOH_ODR_Addr (GPIOH_BASE+0x14) // 0x40021C14
#define GPIOI_ODR_Addr (GPIOI_BASE+0x14) // 0x40022014
#define GPIOA_IDR_Addr (GPIOA_BASE+0x10) // 0x40020010
#define GPIOB_IDR_Addr (GPIOB_BASE+0x10) // 0x40020410
#define GPIOC_IDR_Addr (GPIOC_BASE+0x10) // 0x40020810
#define GPIOD_IDR_Addr (GPIOD_BASE+0x10) // 0x40020C10
#define GPIOE_IDR_Addr (GPIOE_BASE+0x10) // 0x40021010
#define GPIOF_IDR_Addr (GPIOF_BASE+0x10) // 0x40021410
#define GPIOG_IDR_Addr (GPIOG_BASE+0x10) // 0x40021810
#define GPIOH_IDR_Addr (GPIOH_BASE+0x10) // 0x40021C10
#define GPIOI_IDR_Addr (GPIOI_BASE+0x10) // 0x40022010

// IO 口操作，只对单一 IO 口有效
// 确保 n 的大小不超过 16！
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr,n) // 输入
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr,n) // 输入
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr,n) // 输入
#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n) // 输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr,n) // 输入
#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n) // 输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr,n) // 输入
#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr,n) // 输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr,n) // 输入
#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr,n) // 输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr,n) // 输入
/////////////////////////////////////////////////////////////////

// Ex_NVIC_Config 专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6
#define FTIR 1 // 下降沿触发
#define RTIR 2 // 上升沿触发
// JTAG 模式设置定义
#define JTAG_SWD_DISABLE 0X02
#define SWD_ENABLE 0X01
#define JTAG_SWD_ENABLE 0X00

extern char stop;
//extern u8 Fore,Back,Left,Right;
extern int PWM_MAX,PWM_MIN,a,flag_Num,flag_Num1,flag_a,flag_b,flag_c;
extern int MOTO1,MOTO2,flag_stop,Timer;
extern int KeyNum1,KeyNum2,KeyNum3,KeyNum4;
extern float pitch,roll,yaw; // 角度
extern short gyrox,gyroy,gyroz; // 角速度
extern short aacx,aacy,aacz; // 加速度
extern int Encoder_Left,Encoder_Right;
extern float Vertical_Kp,Vertical_Kd,Velocity_Kp,Velocity_Ki,Turn_Kd,Turn_Kd,Track_Kp,Track_Ki,Track_Kd,Target_Speed,length;
extern uint8_t PD9,PD8,PD11,PD10,PD13,PD12,PD15,PD14;
void NVIC_Config (void);


#endif


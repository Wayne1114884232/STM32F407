#ifndef __SYS_H
#define __SYS_H	 

// ====================== 头文件引入 ======================
#include "stm32f4xx.h"          // STM32F4 标准外设库（提供寄存器定义、外设驱动）
#include "board.h"              // 板级支持包（硬件初始化、引脚分配等板级配置）
#include "oled.h"               // OLED 显示屏驱动（实现屏幕显示控制）
#include "control.h"            // 控制算法模块（PID 控制、平衡控制等核心逻辑）
#include "encoder.h"            // 编码器驱动（读取电机编码器数据，计算转速）
#include "exti.h"               // 外部中断配置（处理按键、传感器等中断事件）
#include "bsp_tb6612.h"         // TB6612 电机驱动（控制电机转速和方向）
#include "IOI2C.h"              // I2C 通信驱动（用于 OLED、姿态传感器等 I2C 设备）
#include "Key.h"                // 按键驱动（处理按键输入事件）
#include "wit_c_sdk.h"          // 姿态传感器 SDK（JY901 等传感器的数据解析库）
#include "REG.h"                // 自定义寄存器定义（传感器/设备的寄存器映射）
//#include "buzzer.h"           // 蜂鸣器驱动（暂注释，需使用时取消注释）
#include "usart3.h"             // 串口 3 驱动（与姿态传感器或其他设备通信）
#include "bsp_ultrasonic.h"     // 超声波传感器驱动（测量距离）
#include "LED.h"                // LED 驱动（控制 LED 灯状态）

// 标准库头文件（字符串、IO、数学运算等基础功能）
#include <string.h>  
#include <stdio.h>  
#include <stdint.h> 
#include <stdlib.h> 
#include <math.h>   


// ====================== 常用数据类型定义 ======================
// 带符号整数类型（统一代码风格，增强可读性）
typedef int32_t  s32;   // 32 位带符号整数  
typedef int16_t s16;   // 16 位带符号整数  
typedef int8_t  s8;    // 8 位带符号整数  

// 常量类型（const 修饰，防止意外修改）
typedef const int32_t sc32;  // 32 位只读整数  
typedef const int16_t sc16;  // 16 位只读整数  
typedef const int8_t sc8;    // 8 位只读整数  

// volatile 修饰的变量（用于硬件寄存器，避免编译器优化）
typedef __IO int32_t  vs32;  // 32 位易变带符号整数（如外设寄存器）  
typedef __IO int16_t  vs16;  // 16 位易变带符号整数  
typedef __IO int8_t   vs8;   // 8 位易变带符号整数  

// 只读且易变的变量（__I 表示只读，volatile 防止优化）
typedef __I int32_t vsc32;   // 32 位只读易变整数（如状态寄存器）  
typedef __I int16_t vsc16;   // 16 位只读易变整数  
typedef __I int8_t vsc8;     // 8 位只读易变整数  

// 无符号整数类型  
typedef uint32_t  u32;  // 32 位无符号整数  
typedef uint16_t u16;  // 16 位无符号整数  
typedef uint8_t  u8;   // 8 位无符号整数  

// 无符号常量类型  
typedef const uint32_t uc32;  // 32 位只读无符号整数  
typedef const uint16_t uc16;  // 16 位只读无符号整数  
typedef const uint8_t uc8;    // 8 位只读无符号整数  

// 易变无符号变量（用于硬件寄存器）  
typedef __IO uint32_t  vu32;  // 32 位易变无符号整数  
typedef __IO uint16_t vu16;  // 16 位易变无符号整数  
typedef __IO uint8_t  vu8;   // 8 位易变无符号整数  

// 只读易变无符号变量  
typedef __I uint32_t vuc32;   // 32 位只读易变无符号整数  
typedef __I uint16_t vuc16;   // 16 位只读易变无符号整数  
typedef __I uint8_t vuc8;     // 8 位只读易变无符号整数  


// ====================== 位带操作宏（模拟 51 单片机 IO 控制） ======================
// 位带操作公式：将 GPIO 的某一位映射到独立内存地址，实现单比特操作  
// 参考《CM3 权威指南》（87~92 页），公式：BITBAND(addr, bitnum) = 0x20000000 + (addr&0x1FFFFF)*32 + bitnum*4  
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))  
#define MEM_ADDR(addr)  *((volatile unsigned long *)(addr))  // 内存地址解引用  
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  // 位地址解引用  

// 各 GPIO 端口的输出数据寄存器（ODR）地址  
#define GPIOA_ODR_Addr    (GPIOA_BASE + 0x14)  // GPIOA->ODR 地址：0x40020014  
#define GPIOB_ODR_Addr    (GPIOB_BASE + 0x14)  // GPIOB->ODR 地址：0x40020414  
#define GPIOC_ODR_Addr    (GPIOC_BASE + 0x14)  // GPIOC->ODR 地址：0x40020814  
#define GPIOD_ODR_Addr    (GPIOD_BASE + 0x14)  // GPIOD->ODR 地址：0x40020C14  
#define GPIOE_ODR_Addr    (GPIOE_BASE + 0x14)  // GPIOE->ODR 地址：0x40021014  
#define GPIOF_ODR_Addr    (GPIOF_BASE + 0x14)  // GPIOF->ODR 地址：0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE + 0x14)  // GPIOG->ODR 地址：0x40021814    
#define GPIOH_ODR_Addr    (GPIOH_BASE + 0x14)  // GPIOH->ODR 地址：0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE + 0x14)  // GPIOI->ODR 地址：0x40022014  

// 各 GPIO 端口的输入数据寄存器（IDR）地址  
#define GPIOA_IDR_Addr    (GPIOA_BASE + 0x10)  // GPIOA->IDR 地址：0x40020010  
#define GPIOB_IDR_Addr    (GPIOB_BASE + 0x10)  // GPIOB->IDR 地址：0x40020410  
#define GPIOC_IDR_Addr    (GPIOC_BASE + 0x10)  // GPIOC->IDR 地址：0x40020810  
#define GPIOD_IDR_Addr    (GPIOD_BASE + 0x10)  // GPIOD->IDR 地址：0x40020C10  
#define GPIOE_IDR_Addr    (GPIOE_BASE + 0x10)  // GPIOE->IDR 地址：0x40021010  
#define GPIOF_IDR_Addr    (GPIOF_BASE + 0x10)  // GPIOF->IDR 地址：0x40021410  
#define GPIOG_IDR_Addr    (GPIOG_BASE + 0x10)  // GPIOG->IDR 地址：0x40021810  
#define GPIOH_IDR_Addr    (GPIOH_BASE + 0x10)  // GPIOH->IDR 地址：0x40021C10  
#define GPIOI_IDR_Addr    (GPIOI_BASE + 0x10)  // GPIOI->IDR 地址：0x40022010  


// ====================== IO 口操作宏（简化 GPIO 位控制） ======================
// 输出控制：设置 GPIOx 的第 n 位电平（例：PAout(5) = 1; 置高 PA5）  
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr, n)  // 控制 GPIOA 第 n 位输出  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr, n)  // 控制 GPIOB 第 n 位输出  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr, n)  // 控制 GPIOC 第 n 位输出  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr, n)  // 控制 GPIOD 第 n 位输出  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr, n)  // 控制 GPIOE 第 n 位输出  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr, n)  // 控制 GPIOF 第 n 位输出  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr, n)  // 控制 GPIOG 第 n 位输出  

// 输入读取：获取 GPIOx 的第 n 位电平（例：if(PAin(5)) { ... } 读取 PA5 电平）  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr, n)  // 读取 GPIOA 第 n 位输入  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr, n)  // 读取 GPIOB 第 n 位输入  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr, n)  // 读取 GPIOC 第 n 位输入  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr, n)  // 读取 GPIOD 第 n 位输入  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr, n)  // 读取 GPIOE 第 n 位输入  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr, n)  // 读取 GPIOF 第 n 位输入  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr, n)  // 读取 GPIOG 第 n 位输入  


// ====================== 外部中断配置宏（简化 NVIC/EXTI 配置） ======================
// GPIO 端口编号（用于外部中断配置）  
#define GPIO_A 0  // GPIOA  
#define GPIO_B 1  // GPIOB  
#define GPIO_C 2  // GPIOC  
#define GPIO_D 3  // GPIOD  
#define GPIO_E 4  // GPIOE  
#define GPIO_F 5  // GPIOF  
#define GPIO_G 6  // GPIOG  

// 中断触发方式  
#define FTIR   1  // 下降沿触发（Falling Edge Trigger）  
#define RTIR   2  // 上升沿触发（Rising Edge Trigger）  

// JTAG/SWD 调试接口配置（切换调试模式，释放引脚为普通 IO）  
#define JTAG_SWD_DISABLE   0X02  // 禁用 JTAG 和 SWD（全功能 IO，但无法调试）  
#define SWD_ENABLE         0X01  // 仅启用 SWD（调试可用，释放 JTAG 引脚）  
#define JTAG_SWD_ENABLE    0X00  // 启用 JTAG 和 SWD（调试全功能，但占用引脚多）  


// ====================== 外部全局变量（跨文件共享数据） ======================
extern char stop;                  // 系统停止标志（1: 停止，0: 运行）  
// extern u8 Fore,Back,Left,Right;  // 方向控制标志（需使用时取消注释）  
extern int PWM_MAX, PWM_MIN;       // PWM 输出限幅（最大、最小值）  
extern int a, flag_Num, flag_Num1, flag_a, flag_b, flag_c;  // 控制标志位（状态切换、计数等）  
extern int MOTO1, MOTO2;           // 左右电机 PWM 输出值  
extern int flag_stop;              // 电机紧急停止标志（1: 停止）  
extern int KeyNum;                 // 按键事件编号（标识按下的按键）  
extern int Timer;                  // 定时器计数（延时或周期控制）  

// 姿态传感器数据  
extern float pitch, roll, yaw;    // 俯仰角、横滚角、偏航角（单位：度）  
extern short gyrox, gyroy, gyroz; // X/Y/Z 轴角速度（原始值，需转换为 °/s）  
extern short aacx, aacy, aacz;    // X/Y/Z 轴加速度（原始值，需转换为 g）  

// 电机与控制参数  
extern int Encoder_Left, Encoder_Right;  // 左右电机编码器计数值（速度反馈）  
extern float Track_Kp, Track_Ki;         // 循迹 PID 参数（比例、积分）  
extern float Target_Speed;               // 目标速度（速度环期望值）  
extern float length;                     // 距离测量值（如超声波测距）  

// extern uint8_t PC14,PC15,PA4,PA5,PA6,PA7,PB0,PB1;  // 引脚状态变量（需使用时取消注释）  
void NVIC_Config(void);


#endif


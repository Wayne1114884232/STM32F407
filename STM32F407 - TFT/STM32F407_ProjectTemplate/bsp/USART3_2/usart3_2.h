//#ifndef __USART3_2_H
//#define __USART3_2_H

//#include "stm32f4xx.h"
//#include "sys.h"
//#include <stdint.h>

//// 陀螺仪USART宏定义 (PB10=TX, PB11=RX -> USART3)
//#define GYRO_USART                  USART3
//#define GYRO_USART_CLK              RCC_APB1Periph_USART3
//#define GYRO_USART_CLK_ENABLE()     RCC_APB1PeriphClockCmd(GYRO_USART_CLK, ENABLE)

//// 陀螺仪GPIO宏定义
//#define GYRO_GPIO_PORT              GPIOB
//#define GYRO_GPIO_CLK               RCC_AHB1Periph_GPIOB
//#define GYRO_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(GYRO_GPIO_CLK, ENABLE)
//#define GYRO_TX_PIN                 GPIO_Pin_10
//#define GYRO_RX_PIN                 GPIO_Pin_11
//#define GYRO_TX_PIN_SOURCE          GPIO_PinSource10
//#define GYRO_RX_PIN_SOURCE          GPIO_PinSource11
//#define GYRO_USART_AF               GPIO_AF_USART3

//// 中断相关宏定义
//#define GYRO_USART_IRQn             USART3_IRQn
//#define GYRO_USART_IRQHandler       USART3_IRQHandler

//// 陀螺仪数据结构
//__packed typedef struct
//{
//    int16_t acc[3];       // 加速度计数据(x,y,z)
//    int16_t gyro[3];      // 陀螺仪数据(x,y,z)
//    int16_t roll;         // 横滚角
//    int16_t pitch;        // 俯仰角
//    int16_t yaw;          // 偏航角
//    uint8_t leve;         // 水平等级
//    int16_t temp;         // 温度值
//} Gyro_Data_t;

//// 全局变量声明
//extern uint8_t usart_rx_data[30];
//extern uint8_t Receive_ok;

//// 函数声明
//void Gyro_USART_Init(uint32_t baudrate);
//void Gyro_SendCommand(uint8_t addr, uint8_t ft_code, uint8_t start_reg, uint8_t reg_number);
//void USART_Send(uint8_t *Buffer, uint8_t Length);
//void USART_SendByte(uint8_t byte);

//#endif


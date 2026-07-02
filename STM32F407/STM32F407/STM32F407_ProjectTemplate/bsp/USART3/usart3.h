#ifndef __USART3_H
#define __USART3_H

#include "sys.h" 


void USART3_Send_Data(char data);
void USART3_Send_String(char *String);
void USART3_Init(u32 bound);					//串口3初始化函数
void USART3_IRQHandler(void);     	//串口3中断服务程序
//void JDY_SendAT(char *at_cmd);
//void JDY_ClearRxBuf(void);
//uint8_t JDY_WaitResponse(char *target, uint32_t timeout_ms);
//uint8_t JDY_ConnectSlave(char *mac);
#endif

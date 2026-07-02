#ifndef __USART3_H
#define __USART3_H

#include "sys.h" 

void Usart3Init(unsigned int uiBaud);
void USART3_IRQHandler(void);
void Uart3Send(unsigned char *p_data, unsigned int uiSize);

//void USART3_Send_Data(char data);
//void USART3_Send_String(char *String);
//void USART3_Init(u32 bound);					//串口1初始化函数
//void USART3_IRQHandler(void);     	//串口1中断服务程序

#endif

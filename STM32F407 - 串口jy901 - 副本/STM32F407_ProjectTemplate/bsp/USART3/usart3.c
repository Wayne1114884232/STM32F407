#include "sys.h"
#include "usart3.h"
#include "misc.h"
#include "wit_c_sdk.h"

void Usart3Init(unsigned int uiBaud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    // GPIOB-USART3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // USART3
    
//    // 485PA1
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOB, GPIO_Pin_1);
    
    // USART3 TX (PB10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // PB10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);  // USART3 AF
    
    // USART3 RX (PB11)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  // PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);  // USART3 AF
    
    // USART3
    USART_InitStructure.USART_BaudRate = uiBaud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    
    // USART3
    USART_Cmd(USART3, ENABLE);
    

}

void USART3_IRQHandler(void)
{
	unsigned char ucTemp;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART3);
		WitSerialDataIn(ucTemp);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
void Uart3Send(unsigned char *p_data, unsigned int uiSize)
{	
	unsigned int i;
	for(i = 0; i < uiSize; i++)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		USART_SendData(USART3, *p_data++);		
	}
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}


#include "usart3.h"	  
 
#define RX_BUF_SIZE 128
uint8_t rx_buf[RX_BUF_SIZE] = {0};
uint16_t rx_len = 0;

void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    //USART2_TX -> PC10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //USART2_RX -> PC11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART2);
    

    USART_InitStructure.USART_BaudRate = bound;         
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    
    USART_Cmd(USART3, ENABLE);
}


void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {

        char recv_data = USART_ReceiveData(USART3);

        USART3_Send_Data(recv_data);

        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void USART3_Send_Data(char data)
{
    USART_SendData(USART3, (uint8_t)data);
	
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void USART3_Send_String(char *String)
{
    uint16_t i = 0;
    
    while(String[i] != '\0')
    {
        USART3_Send_Data(String[i]);
        i++;
    }
}


//void JDY_SendAT(char *at_cmd) 
//{
//    char cmd[64] = {0};
//    sprintf(cmd, "%s\r\n", at_cmd); 
//    USART3_Send_String(cmd);
//}

//void JDY_ClearRxBuf(void)
//{
//    rx_len = 0;
//    memset(rx_buf, 0, RX_BUF_SIZE);
//}

//uint8_t JDY_WaitResponse(char *target, uint32_t timeout_ms)
//{
//    uint32_t start_time = SysTick->VAL;
//    while (1) {
//        if (strstr((char*)rx_buf, target) != NULL) {
//            return 1;
//        }
//        if ((start_time - SysTick->VAL) / (SystemCoreClock / 1000) > timeout_ms) {
//            return 0;
//        }
//    }
//}

//uint8_t JDY_ConnectSlave(char *mac) 
//{
//    char cmd[32] = {0};
//    sprintf(cmd, "AT+CONA%s", mac); // ??:AT+CONA<MAC>
//    
//    JDY_ClearRxBuf();
//    JDY_SendAT(cmd);
//    return JDY_WaitResponse("+CONNECTED", 5000); 
//}

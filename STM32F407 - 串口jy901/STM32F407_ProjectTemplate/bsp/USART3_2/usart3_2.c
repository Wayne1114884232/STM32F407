//#include "usart3_2.h"

//// 全局接收缓存
//uint8_t usart_rx_data[30] = {0};
//uint8_t Receive_ok = 0;
//static uint8_t rx_len = 0;

//// USART3中断服务函数
//void GYRO_USART_IRQHandler(void)
//{
//    if (USART_GetITStatus(GYRO_USART, USART_IT_RXNE) != RESET)
//    {
//        // 接收数据
//        usart_rx_data[rx_len++] = USART_ReceiveData(GYRO_USART);
//        
//        // 简单帧尾判断(根据陀螺仪协议调整，此处假设最大帧长29)
//        if (rx_len >= 29 || rx_len > sizeof(usart_rx_data)-1)
//        {
//            Receive_ok = 1;  // 标记接收完成
//            rx_len = 0;      // 重置接收长度
//        }
//        USART_ClearITPendingBit(GYRO_USART, USART_IT_RXNE);
//    }
//}

//// 初始化陀螺仪USART
//void Gyro_USART_Init(uint32_t baudrate)
//{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    USART_InitTypeDef USART_InitStruct;
//    
//    // 使能时钟
//    GYRO_GPIO_CLK_ENABLE();
//    GYRO_USART_CLK_ENABLE();
//    
//    // 配置GPIO复用功能
//    GPIO_PinAFConfig(GYRO_GPIO_PORT, GYRO_TX_PIN_SOURCE, GYRO_USART_AF);
//    GPIO_PinAFConfig(GYRO_GPIO_PORT, GYRO_RX_PIN_SOURCE, GYRO_USART_AF);
//    
//    // 配置TX引脚
//    GPIO_InitStruct.GPIO_Pin = GYRO_TX_PIN;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(GYRO_GPIO_PORT, &GPIO_InitStruct);
//    
//    // 配置RX引脚
//    GPIO_InitStruct.GPIO_Pin = GYRO_RX_PIN;
//    GPIO_Init(GYRO_GPIO_PORT, &GPIO_InitStruct);  // 复用输入模式
//    
//    // 配置USART
//    USART_InitStruct.USART_BaudRate = baudrate;
//    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
//    USART_InitStruct.USART_StopBits = USART_StopBits_1;
//    USART_InitStruct.USART_Parity = USART_Parity_No;
//    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(GYRO_USART, &USART_InitStruct);
//    
//    // 使能接收中断
//    USART_ITConfig(GYRO_USART, USART_IT_RXNE, ENABLE);
//    
//    // 使能USART
//    USART_Cmd(GYRO_USART, ENABLE);
//}

//// 发送单个字节
//void USART_SendByte(uint8_t byte)
//{
//    while (USART_GetFlagStatus(GYRO_USART, USART_FLAG_TC) == RESET);  // 等待发送完成
//    USART_SendData(GYRO_USART, byte);
//}

//// 发送带校验和的数据
//void USART_Send(uint8_t *Buffer, uint8_t Length)
//{
//    uint8_t i = 0;
//    uint8_t checksum = 0;
//    
//    // 计算校验和(前Length-1字节累加)
//    for (i = 0; i < Length - 1; i++)
//    {
//        checksum += Buffer[i];
//    }
//    Buffer[Length - 1] = checksum;  // 填充校验和
//    
//    // 发送数据
//    for (i = 0; i < Length; i++)
//    {
//        USART_SendByte(Buffer[i]);
//    }
//}

//// 发送陀螺仪命令帧
//void Gyro_SendCommand(uint8_t addr, uint8_t ft_code, uint8_t start_reg, uint8_t reg_number)
//{
//    uint8_t send_data[5] = {addr, ft_code, start_reg, reg_number, 0};  // 最后一位留作校验和
//    USART_Send(send_data, 5);  // 发送5字节(含校验和)
//}


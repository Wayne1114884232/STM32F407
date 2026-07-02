/*
 * Serial communication driver for STM32F407
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-07     LCKFB-LP    first version
 * 2024-XX-XX     Optimizer    Improved with circular buffer
 */
 
#include "bsp_uart.h" 
#include "stdio.h"
#include "string.h"

// 定义串口环形缓冲区大小
#define UART_RX_BUF_SIZE 256

// 串口接收环形缓冲区结构体
typedef struct {
    uint8_t buf[UART_RX_BUF_SIZE];  // 缓冲区数组
    uint16_t head;                  // 缓冲区头部指针
    uint16_t tail;                  // 缓冲区尾部指针
    uint16_t len;                   // 缓冲区中数据长度
} UART_Rx_Buf_t;

// 串口接收缓冲区实例
static UART_Rx_Buf_t uart1_rx_buf = {0};

void uart1_init(uint32_t __Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//IO��������������Ҫ���ø���ģʽ
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_9;//TX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF;//IO��������������Ҫ���ø���ģʽ
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType         = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd          = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_10;//RX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType         = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd          = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
	USART_InitTypeDef USART_InitStructure;//�������ô��ڵĽṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//��������1��ʱ��

	USART_DeInit(USART1);//�����˼�ǽ���˴��ڵ���������

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = __Baud;//���ò�����
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//�ֽڳ���Ϊ8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//û��У��λ
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//����������Ϊ�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //���ṩ���� 
	USART_Init(USART1,&USART_InitStructure);//����ز�����ʼ��������1
	
	USART_ClearFlag(USART1,USART_FLAG_RXNE);//��ʼ����ʱ���������λ

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��ʼ���ý����ж�

	USART_Cmd(USART1,ENABLE);//��������1
	
	NVIC_InitTypeDef NVIC_InitStructure;//�жϿ��ƽṹ���������

	NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;//�ж�ͨ��ָ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//ȷ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�����ô��ж�ͨ��
		
}

#if !defined(__MICROLIB)
//��ʹ��΢��Ļ�����Ҫ��������ĺ���
#if (__ARMCLIB_VERSION <= 6000000)
//�����������AC5  �Ͷ�����������ṹ��
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
	
	while( RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE) ){}
	
    return ch;
}



/******** USART1中断处理函数 ***********/
void USART1_IRQHandler(void)
{
    uint8_t rx_data;
    
    // 接收中断
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        // 读取接收数据
        rx_data = USART_ReceiveData(USART1);
        
        // 将数据写入环形缓冲区
        if (uart1_rx_buf.len < UART_RX_BUF_SIZE)
        {
            uart1_rx_buf.buf[uart1_rx_buf.head] = rx_data;
            uart1_rx_buf.head = (uart1_rx_buf.head + 1) % UART_RX_BUF_SIZE;
            uart1_rx_buf.len++;
        }
        // 缓冲区满时，丢弃最旧的数据
        else
        {
            uart1_rx_buf.buf[uart1_rx_buf.head] = rx_data;
            uart1_rx_buf.head = (uart1_rx_buf.head + 1) % UART_RX_BUF_SIZE;
            uart1_rx_buf.tail = (uart1_rx_buf.tail + 1) % UART_RX_BUF_SIZE;
        }
        
        // 清除中断标志位
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    
    // 溢出错误处理
    if(USART_GetITStatus(USART1, USART_IT_ORE) == SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
        USART_ReceiveData(USART1); // 读取数据以清除错误
    }
}



#include "exti.h"


//void jy901_EXTI_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStruct;
//	GPIO_InitTypeDef GPIO_InitStruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//开启时钟
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;/**【1】**///GPIO_Mode_AF_PP
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;//PB5配置为上拉输入
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);	
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);//
//	
//	EXTI_InitStruct.EXTI_Line=EXTI_Line5;
//	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
//	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStruct);
//}


void TIM7_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 99;        // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 8399;   // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM7, ENABLE);
}

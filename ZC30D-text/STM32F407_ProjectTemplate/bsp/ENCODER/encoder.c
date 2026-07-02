//#include "encoder.h"

//void Encoder_Init(uint16_t pre, uint16_t per) {
//    GPIO_InitTypeDef GPIO_InitStruct;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//    TIM_ICInitTypeDef TIM_ICInitStruct;
//    
//    // -------------------------- (MOTOR1:TIM5  PA0/PA1)--------------------------
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//    
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;    
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);

//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

//    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
//    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInitStruct.TIM_Period = per;         
//    TIM_TimeBaseInitStruct.TIM_Prescaler = pre - 1; 
//    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
//    
//    TIM_EncoderInterfaceConfig(TIM5, 
//                              TIM_EncoderMode_TI12,   
//                              TIM_ICPolarity_Rising,  
//                              TIM_ICPolarity_Rising);
//    
//    TIM_ICStructInit(&TIM_ICInitStruct);
//    TIM_ICInitStruct.TIM_ICFilter = 10;
//    TIM_ICInit(TIM5, &TIM_ICInitStruct);

//    TIM_SetCounter(TIM5, 0);
//    TIM_Cmd(TIM5, ENABLE);
//    
//    
//    // -------------------------- (MOTOR2:TIM3  PA6/PA7)--------------------------
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);

//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); 

//    TIM_EncoderInterfaceConfig(TIM3, 
//                              TIM_EncoderMode_TI12,   
//                              TIM_ICPolarity_Rising,  
//                              TIM_ICPolarity_Rising);

//    TIM_ICInit(TIM3, &TIM_ICInitStruct);

//    TIM_SetCounter(TIM3, 0);
//    TIM_Cmd(TIM3, ENABLE);
//    
//    
////    // -------------------------- (MOTOR3:TIM9  PE5/PE6)--------------------------
////    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

////    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
////    GPIO_Init(GPIOE, &GPIO_InitStruct);

////    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
////    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);

////    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct); 

////    TIM_EncoderInterfaceConfig(TIM9, 
////                              TIM_EncoderMode_TI12,   
////                              TIM_ICPolarity_Rising,  
////                              TIM_ICPolarity_Rising);

////    TIM_ICInit(TIM9, &TIM_ICInitStruct);

////    TIM_SetCounter(TIM9, 0);
////    TIM_Cmd(TIM9, ENABLE);
////    
////    
////    // -------------------------- (MOTOR4:TIM4  PB6/PB7)--------------------------
////    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
////    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

////    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
////    GPIO_Init(GPIOB, &GPIO_InitStruct);

////    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
////    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

////    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct); 

////    TIM_EncoderInterfaceConfig(TIM4, 
////                              TIM_EncoderMode_TI12,   
////                              TIM_ICPolarity_Rising,  
////                              TIM_ICPolarity_Rising);

////    TIM_ICInit(TIM4, &TIM_ICInitStruct);

////    TIM_SetCounter(TIM4, 0);
////    TIM_Cmd(TIM4, ENABLE);
//}

//int Read_Speed(Encoder_TypeDef encoder) {
//    int value = 0;

//    switch(encoder) {
//        case ENCODER_M1: 
//            value = (short)TIM_GetCounter(TIM5);
//            TIM_SetCounter(TIM5, 0);
//            break;
//        
//        case ENCODER_M2: 
//            value = (short)TIM_GetCounter(TIM3);
//            TIM_SetCounter(TIM3, 0);
//            break;
//        
////        case ENCODER_M3: 
////            value = (short)TIM_GetCounter(TIM9);
////            TIM_SetCounter(TIM9, 0);
////            break;
////        
////        case ENCODER_M4: 
////            value = (short)TIM_GetCounter(TIM4);
////            TIM_SetCounter(TIM4, 0);
////            break;
//        
//        default: 
//            value = 0;  
//            break;
//    }
//    
//    return value;
//}


//void TIM3_IRQHandler(void) {
//    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
//        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//    }
//}

////void TIM4_IRQHandler(void) {
////    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
////        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
////    }
////}

//void TIM5_IRQHandler(void) {
//    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
//        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
//    }
//}

////void TIM9_IRQHandler(void) {
////    if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET) {
////        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
////    }
////}


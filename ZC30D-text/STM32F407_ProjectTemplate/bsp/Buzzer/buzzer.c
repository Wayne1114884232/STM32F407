//#include "buzzer.h"

//void buzzer_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    // GPIOC
//    RCC_AHB1PeriphClockCmd(BUZZER_RCC, ENABLE);
//    
//    // PC13
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
//    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;          
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
//    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);
//    
//    BUZZER_OFF();  
//}


//void BUZZER_ON(void)
//{
//    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);  
//}

//void BUZZER_OFF(void)
//{
//    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN); 
//}

//void BUZZER_Toggle(void)
//{
//    GPIO_ToggleBits(BUZZER_PORT, BUZZER_PIN);  
//}


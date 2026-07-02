#include "sys.h" 

/*
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
*/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		// 
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // Highest priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
    
    // TIM2 interrupt configuration (Ultrasonic Timer)
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // Lower priority than TIM4
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_Init(&NVIC_InitStructure);
//    
//    // EXTI4 interrupt configuration (Ultrasonic ECHO)
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // Same as TIM2
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // Lower than TIM2
//    NVIC_Init(&NVIC_InitStructure);
	
//		NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
//		NVIC_Init(&NVIC_InitStructure);	

		NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_Init(&NVIC_InitStructure);		
}


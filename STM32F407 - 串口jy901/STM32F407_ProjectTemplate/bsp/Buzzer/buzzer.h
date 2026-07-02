#ifndef __BUZZER_H
#define __BUZZER_H

#include "sys.h"

// PC13
#define BUZZER_RCC    RCC_AHB1Periph_GPIOC  
#define BUZZER_PORT   GPIOC                 
#define BUZZER_PIN    GPIO_Pin_13           


void buzzer_init(void);  
void BUZZER_ON(void);    
void BUZZER_OFF(void);  
void BUZZER_Toggle(void); 

#endif


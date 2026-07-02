#ifndef _LED_H
#define _LED_H

#include "sys.h"

// LED (PB2)
#define LED1_PORT       GPIOB
#define LED1_PIN        GPIO_Pin_2
#define LED1_CLK        RCC_AHB1Periph_GPIOB 

// LED2 (PC0)
#define LED2_PORT       GPIOC
#define LED2_PIN        GPIO_Pin_0
#define LED2_CLK        RCC_AHB1Periph_GPIOC

// LED3 (PD6)
#define LED3_PORT       GPIOD
#define LED3_PIN        GPIO_Pin_6
#define LED3_CLK        RCC_AHB1Periph_GPIOD

// LED4 (PE1)
#define LED4_PORT       GPIOE
#define LED4_PIN        GPIO_Pin_8
#define LED4_CLK        RCC_AHB1Periph_GPIOE

void LED_Init(void);

// LED1
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Toggle(void);

// LED2
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Toggle(void);

// LED3
void LED3_ON(void);
void LED3_OFF(void);
void LED3_Toggle(void);

// LED4
void LED4_ON(void);
void LED4_OFF(void);
void LED4_Toggle(void);

#endif


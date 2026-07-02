#ifndef _BSP_ULTRASONIC_H_
#define _BSP_ULTRASONIC_H_

#include "sys.h"


#define RCC_SR04        RCC_AHB1Periph_GPIOE
#define PORT_SR04       GPIOE
#define GPIO_TRIG       GPIO_Pin_3  // Trigger Pin
#define GPIO_ECHO       GPIO_Pin_4  // Echo Pin 

#define RCC_TIMER       RCC_APB1Periph_TIM2
#define PORT_TIMER      TIM2
#define TIMER_IRQ       TIM2_IRQn
#define TIMER_IRQHandler TIM2_IRQHandler

void Ultrasonic_Init(void);
float Hcsr04GetLength(void);
uint32_t GetSysTickMs(void);

#endif


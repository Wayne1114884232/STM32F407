#ifndef _ENCODER_H
#define _ENCODER_H

#include "sys.h" 

typedef enum {
    ENCODER_M1 = 1,   // MOTOR1
    ENCODER_M2 = 2,   // MOTOR2
//    ENCODER_M3 = 3,   // MOTOR3
//    ENCODER_M4 = 4    // MOTOR4
} Encoder_TypeDef;

void TIM3_IRQHandler(void);                  // (TIM3)
void TIM5_IRQHandler(void);                  // (TIM5)

//void TIM4_IRQHandler(void);                  // (TIM4)
//void TIM9_IRQHandler(void);                  // (TIM9)

int  Read_Speed(Encoder_TypeDef encoder);    
void Encoder_Init(uint16_t pre, uint16_t per); 

#endif


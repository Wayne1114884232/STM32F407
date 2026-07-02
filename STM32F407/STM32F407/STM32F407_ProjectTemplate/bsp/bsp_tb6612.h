#ifndef _BSP_TB6612_H
#define _BSP_TB6612_H

#include "sys.h"

// -------------------------- TB6612-1(MOTOR1?MOTOR2)--------------------------
#define RCC_PWMA_TIMER         RCC_APB2Periph_TIM1   // TIM1??(APB2)
#define BSP_PWMA_TIMER         TIM1                  // TIM1
#define RCC_PWMA               RCC_AHB1Periph_GPIOE  // PWMA??PE9?GPIO??
#define PORT_PWMA              GPIOE                 // PWMA??
#define GPIO_PWMA              GPIO_Pin_9            // PWMA??PE9
#define GPIO_PWMA_SOURCE       GPIO_PinSource9     
#define AF_PWMA                GPIO_AF_TIM1          

#define RCC_PWMB               RCC_AHB1Periph_GPIOE  // PWMB??PE11?GPIO??
#define PORT_PWMB              GPIOE                 // PWMB??
#define GPIO_PWMB              GPIO_Pin_11           // PWMB??PE11
#define GPIO_PWMB_SOURCE       GPIO_PinSource11     
#define AF_PWMB                GPIO_AF_TIM1         

#define RCC_AIN1               RCC_AHB1Periph_GPIOE  // AIN1??PE12?GPIO??
#define PORT_AIN1              GPIOE                 // AIN1??
#define GPIO_AIN1              GPIO_Pin_12           // AIN1??PE12
#define AIN1_OUT(x)            GPIO_WriteBit(PORT_AIN1, GPIO_AIN1, (BitAction)x)

#define RCC_AIN2               RCC_AHB1Periph_GPIOE  // AIN2??PE13?GPIO??
#define PORT_AIN2              GPIOE                 // AIN2??
#define GPIO_AIN2              GPIO_Pin_13           // AIN2??PE13
#define AIN2_OUT(x)            GPIO_WriteBit(PORT_AIN2, GPIO_AIN2, (BitAction)x)

#define RCC_BIN1               RCC_AHB1Periph_GPIOE  // BIN1??PE14?GPIO??
#define PORT_BIN1              GPIOE                 // BIN1??
#define GPIO_BIN1              GPIO_Pin_14           // BIN1??PE14
#define BIN1_OUT(x)            GPIO_WriteBit(PORT_BIN1, GPIO_BIN1, (BitAction)x)

#define RCC_BIN2               RCC_AHB1Periph_GPIOE  // BIN2??PE15?GPIO??
#define PORT_BIN2              GPIOE                 // BIN2??
#define GPIO_BIN2              GPIO_Pin_15           // BIN2??PE15
#define BIN2_OUT(x)            GPIO_WriteBit(PORT_BIN2, GPIO_BIN2, (BitAction)x)

// -------------------------- TB6612-2(MOTOR3?MOTOR4)--------------------------
#define RCC_PWMC_TIMER         RCC_APB2Periph_TIM8   // TIM8??(APB2)
#define BSP_PWMC_TIMER         TIM8                  // TIM8
#define RCC_PWMC               RCC_AHB1Periph_GPIOC  // PWMC??PC7?GPIO??
#define PORT_PWMC              GPIOC                 // PWMC??
#define GPIO_PWMC              GPIO_Pin_7            // PWMC??PC7
#define GPIO_PWMC_SOURCE       GPIO_PinSource7      
#define AF_PWMC                GPIO_AF_TIM8          

#define RCC_PWMD               RCC_AHB1Periph_GPIOC  // PWMD??PC6?GPIO??
#define PORT_PWMD              GPIOC                 // PWMD??
#define GPIO_PWMD              GPIO_Pin_6            // PWMD??PC6
#define GPIO_PWMD_SOURCE       GPIO_PinSource6      
#define AF_PWMD                GPIO_AF_TIM8          

#define RCC_AIN3               RCC_AHB1Periph_GPIOB  // AIN3??PB15?GPIO??
#define PORT_AIN3              GPIOB                 // AIN3??
#define GPIO_AIN3              GPIO_Pin_15           // AIN3??PB15
#define AIN3_OUT(x)            GPIO_WriteBit(PORT_AIN3, GPIO_AIN3, (BitAction)x)

#define RCC_AIN4               RCC_AHB1Periph_GPIOB  // AIN4??PB14?GPIO??
#define PORT_AIN4              GPIOB                 // AIN4??
#define GPIO_AIN4              GPIO_Pin_14           // AIN4??PB14
#define AIN4_OUT(x)            GPIO_WriteBit(PORT_AIN4, GPIO_AIN4, (BitAction)x)

#define RCC_BIN3               RCC_AHB1Periph_GPIOB  // BIN3??PB12?GPIO??
#define PORT_BIN3              GPIOB                 // BIN3??
#define GPIO_BIN3              GPIO_Pin_12           // BIN3??PB12
#define BIN3_OUT(x)            GPIO_WriteBit(PORT_BIN3, GPIO_BIN3, (BitAction)x)

#define RCC_BIN4               RCC_AHB1Periph_GPIOB  // BIN4??PB13?GPIO??
#define PORT_BIN4              GPIOB                 // BIN4??
#define GPIO_BIN4              GPIO_Pin_13           // BIN4??PB13
#define BIN4_OUT(x)            GPIO_WriteBit(PORT_BIN4, GPIO_BIN4, (BitAction)x)


void TB6612_Init1(uint16_t pre, uint16_t per);  // TB6612-1(TIM1)
void TB6612_Init2(uint16_t pre, uint16_t per);  // TB6612-2(TIM8)
void IN_GPIO_INIT(void);                        // TB6612-1?IO
void IN_GPIO_INIT2(void);                       // TB6612-2?IO
void Load(int moto1, int moto2);                // MOTOR1?MOTOR2
void Load2(int moto3, int moto4);               // MOTOR3?MOTOR4
int GFP_abs(int p);                            
void Limit(int *motoA, int *motoB);            
void Limit2(int *motoC, int *motoD);            
void Stop(float *Med_Jiaodu, float *Jiaodu);

#endif


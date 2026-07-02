#include "bsp_tb6612.h"

void TB6612_Init1(uint16_t pre, uint16_t per)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 1. 
    RCC_APB2PeriphClockCmd(RCC_PWMA_TIMER, ENABLE);          // TIM1
    RCC_AHB1PeriphClockCmd(RCC_PWMA | RCC_PWMB | RCC_AHB1Periph_GPIOE, ENABLE); // GPIOE

    // 2. PWM
    // PWMA (PE9)
    GPIO_InitStructure.GPIO_Pin = GPIO_PWMA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PORT_PWMA, &GPIO_InitStructure);
    GPIO_PinAFConfig(PORT_PWMA, GPIO_PWMA_SOURCE, AF_PWMA);

    // PWMB (PE11)
    GPIO_InitStructure.GPIO_Pin = GPIO_PWMB;
    GPIO_Init(PORT_PWMB, &GPIO_InitStructure);
    GPIO_PinAFConfig(PORT_PWMB, GPIO_PWMB_SOURCE, AF_PWMB);

    // 3. TIM1
    TIM_TimeBaseStructure.TIM_Period = per;
    TIM_TimeBaseStructure.TIM_Prescaler = pre - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  
    TIM_TimeBaseInit(BSP_PWMA_TIMER, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;  

    // CH1(PWMA)
    TIM_OC1Init(BSP_PWMA_TIMER, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(BSP_PWMA_TIMER, TIM_OCPreload_Enable);

    // CH2(PWMB)
    TIM_OC2Init(BSP_PWMA_TIMER, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(BSP_PWMA_TIMER, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(BSP_PWMA_TIMER, ENABLE);
    TIM_CtrlPWMOutputs(BSP_PWMA_TIMER, ENABLE); 
    TIM_Cmd(BSP_PWMA_TIMER, ENABLE);

    IN_GPIO_INIT();
}

void TB6612_Init2(uint16_t pre, uint16_t per)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_PWMC_TIMER, ENABLE);          // TIM8
    RCC_AHB1PeriphClockCmd(RCC_PWMC | RCC_PWMD | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE); // GPIOC?GPIOB

    // 2. PWM
    // PWMC (PC7)
    GPIO_InitStructure.GPIO_Pin = GPIO_PWMC;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PORT_PWMC, &GPIO_InitStructure);
    GPIO_PinAFConfig(PORT_PWMC, GPIO_PWMC_SOURCE, AF_PWMC);

    // PWMD (PC6)
    GPIO_InitStructure.GPIO_Pin = GPIO_PWMD;
    GPIO_Init(PORT_PWMD, &GPIO_InitStructure);
    GPIO_PinAFConfig(PORT_PWMD, GPIO_PWMD_SOURCE, AF_PWMD);

    // 3. TIM8
    TIM_TimeBaseStructure.TIM_Period = per;
    TIM_TimeBaseStructure.TIM_Prescaler = pre - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(BSP_PWMC_TIMER, &TIM_TimeBaseStructure);

    // 4. PWM(PWM1)
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;  // ?????0

    // CH1(PWMD)
    TIM_OC1Init(BSP_PWMC_TIMER, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(BSP_PWMC_TIMER, TIM_OCPreload_Enable);

    // CH2(PWMC)
    TIM_OC2Init(BSP_PWMC_TIMER, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(BSP_PWMC_TIMER, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(BSP_PWMC_TIMER, ENABLE);
    TIM_CtrlPWMOutputs(BSP_PWMC_TIMER, ENABLE); 
    TIM_Cmd(BSP_PWMC_TIMER, ENABLE);

    IN_GPIO_INIT2();
}

void IN_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_AIN1 | GPIO_AIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PORT_AIN1, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_BIN1 | GPIO_BIN2;
    GPIO_Init(PORT_BIN1, &GPIO_InitStructure);
}

void IN_GPIO_INIT2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_AIN3 | GPIO_AIN4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PORT_AIN3, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_BIN3 | GPIO_BIN4;
    GPIO_Init(PORT_BIN3, &GPIO_InitStructure);
}


int GFP_abs(int p)
{
    return p > 0 ? p : -p;
}

void Load(int moto1, int moto2)
{
    // MOTOR1
    if (moto1 > 0) {AIN1_OUT(0); AIN2_OUT(1);}
    else           {AIN1_OUT(1); AIN2_OUT(0);}
    TIM_SetCompare1(TIM1, GFP_abs(moto1));  // CH1
   
    // MOTOR2
    if (moto2 > 0) {BIN1_OUT(0); BIN2_OUT(1);}
    else           {BIN1_OUT(1); BIN2_OUT(0);}
    TIM_SetCompare2(TIM1, GFP_abs(moto2));  // CH2
}

void Load2(int moto3, int moto4)
{
    // MOTOR3(TIM8_CH2)
    if (moto3 > 0) {AIN3_OUT(0); AIN4_OUT(1);}
    else           {AIN3_OUT(1); AIN4_OUT(0);}
    TIM_SetCompare2(TIM8, GFP_abs(moto3));  // CH2
    
    // MOTOR4(TIM8_CH1)
    if (moto4 > 0) {BIN3_OUT(0); BIN4_OUT(1);}
    else           {BIN3_OUT(1); BIN4_OUT(0);}
    TIM_SetCompare1(TIM8, GFP_abs(moto4));  // CH1
}


void Limit(int *motoA, int *motoB)
{
    if (*motoA > PWM_MAX) *motoA = PWM_MAX;
    if (*motoA < PWM_MIN) *motoA = PWM_MIN;
    if (*motoB > PWM_MAX) *motoB = PWM_MAX;
    if (*motoB < PWM_MIN) *motoB = PWM_MIN;
}


void Limit2(int *motoC, int *motoD)
{
    if (*motoC > PWM_MAX) *motoC = PWM_MAX;
    if (*motoC < PWM_MIN) *motoC = PWM_MIN;
    if (*motoD > PWM_MAX) *motoD = PWM_MAX;
    if (*motoD < PWM_MIN) *motoD = PWM_MIN;
}

char PWM_Zero = 0, stop = 0;
void Stop(float *Med_Jiaodu, float *Jiaodu)
{
    if (GFP_abs(*Jiaodu - *Med_Jiaodu) > 50)
    {
        Load(PWM_Zero, PWM_Zero);
        Load2(PWM_Zero, PWM_Zero);
        stop = 1;
    }
}

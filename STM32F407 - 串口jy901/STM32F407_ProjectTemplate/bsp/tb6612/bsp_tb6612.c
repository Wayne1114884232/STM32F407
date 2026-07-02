#include "bsp_tb6612.h"
#include "global_vars.h"

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




// 使用标准库的绝对值函数替代自定义函数
// int GFP_abs(int p)
// {
//     return p > 0 ? p : -p;
// }

void load(int moto1, int moto2)
{
    // 限制PWM值在安全范围内
    if (moto1 > PWM_MAX) moto1 = PWM_MAX;
    if (moto1 < PWM_MIN) moto1 = PWM_MIN;
    if (moto2 > PWM_MAX) moto2 = PWM_MAX;
    if (moto2 < PWM_MIN) moto2 = PWM_MIN;
    
    // 记录当前PWM值
    g_motor_ctrl.moto1 = moto1;
    g_motor_ctrl.moto2 = moto2;
    
    // MOTOR1方向控制和PWM输出
    if (moto1 > 0) {
        AIN1_OUT(0); 
        AIN2_OUT(1);
    } else if (moto1 < 0) {
        AIN1_OUT(1); 
        AIN2_OUT(0);
    } else {
        // 停止电机
        AIN1_OUT(0); 
        AIN2_OUT(0);
    }
    TIM_SetCompare1(TIM1, abs(moto1));  // CH1
   
    // MOTOR2方向控制和PWM输出
    if (moto2 > 0) {
        BIN1_OUT(0); 
        BIN2_OUT(1);
    } else if (moto2 < 0) {
        BIN1_OUT(1); 
        BIN2_OUT(0);
    } else {
        // 停止电机
        BIN1_OUT(0); 
        BIN2_OUT(0);
    }
    TIM_SetCompare2(TIM1, abs(moto2));  // CH2
}




void limit(int *motoA, int *motoB)
{
    // 参数有效性检查
    if (motoA == NULL || motoB == NULL) {
        return; // 避免空指针访问
    }
    
    // 限制PWM值在安全范围内
    if (*motoA > PWM_MAX) *motoA = PWM_MAX;
    if (*motoA < PWM_MIN) *motoA = PWM_MIN;
    if (*motoB > PWM_MAX) *motoB = PWM_MAX;
    if (*motoB < PWM_MIN) *motoB = PWM_MIN;
}

// 添加电机紧急停止函数
void emergency_stop(void)
{
    // 立即停止所有电机
    AIN1_OUT(0);
    AIN2_OUT(0);
    BIN1_OUT(0);
    BIN2_OUT(0);
    
    // 清除PWM输出
    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare2(TIM1, 0);
    
    // 更新全局变量
    g_motor_ctrl.moto1 = 0;
    g_motor_ctrl.moto2 = 0;
    g_motor_ctrl.flag_stop = 1;
}






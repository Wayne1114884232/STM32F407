#include "LED.h"

/**
  * @brief  初始化所有LED GPIO引脚
  * @param  无
  * @retval 无
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能所有LED对应的GPIO时钟
    RCC_AHB1PeriphClockCmd(LED1_CLK | LED2_CLK | LED3_CLK | LED4_CLK, ENABLE);
    
    // 配置LED1 (PB2)
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    
    // 配置LED2 (PC0)
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);  // 复用相同配置参数
    
    // 配置LED3 (PD6)
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
    
    // 配置LED4 (PE1)
    GPIO_InitStructure.GPIO_Pin = LED4_PIN;
    GPIO_Init(LED4_PORT, &GPIO_InitStructure);
    
    // 默认熄灭所有LED
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}

// LED1控制函数
void LED1_ON(void)  { GPIO_SetBits(LED1_PORT, LED1_PIN); }
void LED1_OFF(void) { GPIO_ResetBits(LED1_PORT, LED1_PIN); }
void LED1_Toggle(void) { GPIO_ToggleBits(LED1_PORT, LED1_PIN); }

// LED2控制函数
void LED2_ON(void)  { GPIO_SetBits(LED2_PORT, LED2_PIN); }
void LED2_OFF(void) { GPIO_ResetBits(LED2_PORT, LED2_PIN); }
void LED2_Toggle(void) { GPIO_ToggleBits(LED2_PORT, LED2_PIN); }

// LED3控制函数
void LED3_ON(void)  { GPIO_SetBits(LED3_PORT, LED3_PIN); }
void LED3_OFF(void) { GPIO_ResetBits(LED3_PORT, LED3_PIN); }
void LED3_Toggle(void) { GPIO_ToggleBits(LED3_PORT, LED3_PIN); }

// LED4控制函数
void LED4_ON(void)  { GPIO_SetBits(LED4_PORT, LED4_PIN); }
void LED4_OFF(void) { GPIO_ResetBits(LED4_PORT, LED4_PIN); }
void LED4_Toggle(void) { GPIO_ToggleBits(LED4_PORT, LED4_PIN); }



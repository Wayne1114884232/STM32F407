#include <board.h>

static __IO uint32_t g_system_tick = 0;


/**
 * This function will initial stm32 board.
 */
void board_init(void)
{
    /* NVIC Configuration */
#define NVIC_VTOR_MASK              0x3FFFFF80
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x10000000 */
    SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    SCB->VTOR  = (0x08000000 & NVIC_VTOR_MASK);
#endif

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
		SysTick->LOAD=0xFFFF; // 设置重装载值
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; // 启动计数器
	
//	RCC_ClocksTypeDef rcc;
//	RCC_GetClocksFreq(&rcc);// 读取系统时钟频率

}

/**
 -  @brief  使用内置的systick实现的微秒级延时
 -  @note   None
 -  @param  _us:需要延时的微秒数
 -  @retval None
*/
void delay_us(uint32_t _us)
{
    uint32_t temp;
    uint32_t reload;

    // 1. 保存SysTick当前配置（防止影响其他功能）
    uint32_t prev_ctrl = SysTick->CTRL;
    uint32_t prev_load = SysTick->LOAD;
    uint32_t prev_val  = SysTick->VAL;

    // 2. 配置SysTick时钟源（使用HCLK/8，兼容F1系列）
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  // 分频8倍
    reload = (SystemCoreClock / 8000000) * _us;  // 计算所需ticks(8分频下，1微秒对应的ticks)

    // 3. 配置SysTick定时器参数
    SysTick->LOAD = reload;       // 设置重装载值
    SysTick->VAL  = 0x00;         // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 启动计数器

    // 4. 等待延时完成(循环等待计数结束)
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));  // 等待COUNTFLAG置位

    // 5. 恢复SysTick原始配置(重要!避免影响其他功能)
    SysTick->CTRL = prev_ctrl;
    SysTick->LOAD = prev_load;
    SysTick->VAL  = prev_val;
}

/**
 -  @brief  调用上面的systick实现的毫秒级延时
 -  @note   None
 -  @param  _ms:需要延时的毫秒数
 -  @retval None
*/
void delay_ms(uint32_t _ms) { delay_us(_ms * 1000); }

void delay_1ms(uint32_t ms) { delay_us(ms * 1000); }

void delay_1us(uint32_t us) { delay_us(us); }


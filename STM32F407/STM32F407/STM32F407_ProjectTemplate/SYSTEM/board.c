/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * 

 Change Logs:
 * Date           Author       Notes
 * 2024-03-07     LCKFB-LP    first version
 */
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
		SysTick->LOAD=0xFFFF; // 清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; // 开始计数
	
//	RCC_ClocksTypeDef rcc;
//	RCC_GetClocksFreq(&rcc);//读取系统时钟频率

}

/**
 -  @brief  用内核的 systick 实现的微妙延时
 -  @note   None
 -  @param  _us:要延时的us数
 -  @retval None
*/
void delay_us(uint32_t _us)
{
    uint32_t temp;
    uint32_t reload;

    // 1. ???? SysTick ??(??????????)
    uint32_t prev_ctrl = SysTick->CTRL;
    uint32_t prev_load = SysTick->LOAD;
    uint32_t prev_val  = SysTick->VAL;

    // 2. ???? SysTick ??????(?? HCLK/8,? F1 ??)
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  // ??? 8 ??
    reload = (SystemCoreClock / 8000000) * _us;  // ?????? ticks(8???,? us ?? ticks)

    // 3. ??? SysTick ?????
    SysTick->LOAD = reload;       // ????
    SysTick->VAL  = 0x00;         // ?????
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // ????

    // 4. ??????(?????????????)
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));  // ?? COUNTFLAG ??

    // 5. ?? SysTick ????(??!????????)
    SysTick->CTRL = prev_ctrl;
    SysTick->LOAD = prev_load;
    SysTick->VAL  = prev_val;
}

/**
 -  @brief  调用用内核的 systick 实现的毫秒延时
 -  @note   None
 -  @param  _ms:要延时的ms数
 -  @retval None
*/
void delay_ms(uint32_t _ms) { delay_us(_ms * 1000); }

void delay_1ms(uint32_t ms) { delay_us(ms * 1000); }

void delay_1us(uint32_t us) { delay_us(us); }

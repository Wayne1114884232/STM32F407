#include "tftlcd_fsmc.h"

/**
 * @brief  FSMC初始化
 * @param  无
 * @retval 无
 */
void tftlcd_fsmc_init(void)
{
    GPIO_InitTypeDef                 GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef          FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef    read_timing;
    FSMC_NORSRAMTimingInitTypeDef    write_timing;

    /* 1. 时钟使能 */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    RCC_AHB1PeriphClockCmd(TFTLCD_RS_CLK  | TFTLCD_CS_CLK |
                           TFTLCD_RD_CLK  | TFTLCD_WR_CLK |
                           RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

    /* 2. GPIO初始化配置：复用推挽输出，50MHz，无上下拉 */
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;

    /* ==================== 初始化控制引脚 RS/CS/RD/WR ==================== */
    // RS: PF12
    GPIO_InitStructure.GPIO_Pin = TFTLCD_RS_PIN;
    GPIO_Init(TFTLCD_RS_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TFTLCD_RS_PORT, GPIO_PinSource12, TFTLCD_RS_AF);

    // CS: PG12
    GPIO_InitStructure.GPIO_Pin = TFTLCD_CS_PIN;
    GPIO_Init(TFTLCD_CS_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TFTLCD_CS_PORT, GPIO_PinSource12, TFTLCD_CS_AF);

    // RD: PD4
    GPIO_InitStructure.GPIO_Pin = TFTLCD_RD_PIN;
    GPIO_Init(TFTLCD_RD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TFTLCD_RD_PORT, GPIO_PinSource4, TFTLCD_RD_AF);

    // WR: PD5
    GPIO_InitStructure.GPIO_Pin = TFTLCD_WR_PIN;
    GPIO_Init(TFTLCD_WR_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(TFTLCD_WR_PORT, GPIO_PinSource5, TFTLCD_WR_AF);

    /* ==================== 初始化数据引脚 D0~D15 ==================== */
    // GPIOD: D0/D1/D2/D3/D13/D14/D15
    GPIO_InitStructure.GPIO_Pin = TFTLCD_D0_PIN  | TFTLCD_D1_PIN  | TFTLCD_D2_PIN  | TFTLCD_D3_PIN  |
                                  TFTLCD_D13_PIN| TFTLCD_D14_PIN| TFTLCD_D15_PIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // GPIOE: D4~D12
    GPIO_InitStructure.GPIO_Pin = TFTLCD_D4_PIN  | TFTLCD_D5_PIN  | TFTLCD_D6_PIN  | TFTLCD_D7_PIN  |
                                  TFTLCD_D8_PIN  | TFTLCD_D9_PIN  | TFTLCD_D10_PIN | TFTLCD_D11_PIN | TFTLCD_D12_PIN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* 配置数据引脚复用功能 */
    // GPIOD
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);  // D0
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);  // D1
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);  // D2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);  // D3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC);  // D13
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC);  // D14
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);  // D15
    // GPIOE
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FSMC);  // D4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FSMC);  // D5
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FSMC);  // D6
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);  // D7
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);  // D8
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);  // D9
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);  // D10
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);  // D11
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);  // D12

    /* ==================== FSMC 初始化 ==================== */
    FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);

    FSMC_NORSRAMInitStructure.FSMC_Bank                = TFTLCD_FSMC_BANK;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux       = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType           = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth      = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode      = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity   = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode             = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive     = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation       = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal           = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode         = FSMC_ExtendedMode_Enable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait     = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst           = FSMC_WriteBurst_Disable;

    /* 读时序配置 */
    read_timing.FSMC_AddressSetupTime      = TFTLCD_FSMC_READ_AST;
    read_timing.FSMC_AddressHoldTime       = 0;
    read_timing.FSMC_DataSetupTime         = TFTLCD_FSMC_READ_DST;
    read_timing.FSMC_BusTurnAroundDuration = 0;
    read_timing.FSMC_CLKDivision           = 0;
    read_timing.FSMC_DataLatency           = 0;
    read_timing.FSMC_AccessMode            = FSMC_AccessMode_A;

    /* 写时序配置 */
    write_timing.FSMC_AddressSetupTime      = TFTLCD_FSMC_WRITE_AST;
    write_timing.FSMC_AddressHoldTime       = 0;
    write_timing.FSMC_DataSetupTime         = TFTLCD_FSMC_WRITE_DST;
    write_timing.FSMC_BusTurnAroundDuration = 0;
    write_timing.FSMC_CLKDivision           = 0;
    write_timing.FSMC_DataLatency           = 0;
    write_timing.FSMC_AccessMode            = FSMC_AccessMode_A;

    /* 绑定时序 */
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &read_timing;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &write_timing;

    /* 初始化+使能Bank */
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(TFTLCD_FSMC_BANK, ENABLE);
}

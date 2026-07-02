#include "tftlcd_gpio.h"

/**
 * @brief       TFTLCD GPIO初始化
 * @param       无
 * @retval      无
 */
void tftlcd_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_RS_GPIO | RCC_CS_GPIO | RCC_RD_GPIO | RCC_WR_GPIO | RCC_DATA_GPIO, ENABLE);

    /* 2. 初始化 RS(PC8) 推挽输出 上拉 100M */
    GPIO_InitStructure.GPIO_Pin    = GPIO_RS_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
    GPIO_Init(PORT_RS_GPIO, &GPIO_InitStructure);

    /* 3. 初始化 CS(PC9) */
    GPIO_InitStructure.GPIO_Pin    = GPIO_CS_PIN;
    GPIO_Init(PORT_CS_GPIO, &GPIO_InitStructure);

    /* 4. 初始化 RD(PC6) */
    GPIO_InitStructure.GPIO_Pin    = GPIO_RD_PIN;
    GPIO_Init(PORT_RD_GPIO, &GPIO_InitStructure);

    /* 5. 初始化 WR(PC7) */
    GPIO_InitStructure.GPIO_Pin    = GPIO_WR_PIN;
    GPIO_Init(PORT_WR_GPIO, &GPIO_InitStructure);

    /* 6. 初始化 DATA(PB全引脚) */
    GPIO_InitStructure.GPIO_Pin    = GPIO_DATA_PIN;
    GPIO_Init(PORT_DATA_GPIO, &GPIO_InitStructure);

    /* 7. 默认电平 */
    RS_OUT(1);
    CS_OUT(1);
    RD_OUT(1);
    WR_OUT(1);
    DATA_OUT(0xFFFF);
}

/**
 * @brief       GPIO写命令
 * @param       cmd: 命令字
 * @retval      无
 */
void tftlcd_gpio_write_cmd(uint16_t cmd)
{
    RS_OUT(0);
    CS_OUT(0);
    DATA_OUT(cmd);
    WR_OUT(0);
    WR_OUT(1);
    CS_OUT(1);
}

/**
 * @brief       GPIO写数据
 * @param       dat: 数据
 * @retval      无
 */
void tftlcd_gpio_write_dat(uint16_t dat)
{
    RS_OUT(1);
    CS_OUT(0);
    DATA_OUT(dat);
    WR_OUT(0);
    WR_OUT(1);
    CS_OUT(1);
}

/**
 * @brief       GPIO写寄存器
 * @param       reg: 寄存器地址
 * @param       dat: 寄存器数据
 * @retval      无
 */
void tftlcd_gpio_write_reg(uint16_t reg, uint16_t dat)
{
    tftlcd_gpio_write_cmd(reg);
    tftlcd_gpio_write_dat(dat);
}

/**
 * @brief       GPIO读数据
 * @param       无
 * @retval      读取到的16位数据
 */
uint16_t tftlcd_gpio_read_dat(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint16_t dat;

    /* 1. DATA端口配置为上拉输入 */
    GPIO_InitStructure.GPIO_Pin    = GPIO_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_Init(PORT_DATA_GPIO, &GPIO_InitStructure);

    /* 2. 读时序 */
    RS_OUT(1);
    CS_OUT(0);
    RD_OUT(0);
    __NOP();      // F4 标准空操作
    __NOP();
    dat = DATA_IN();
    RD_OUT(1);
    CS_OUT(1);

    /* 3. DATA端口恢复为推挽输出 */
    GPIO_InitStructure.GPIO_Pin    = GPIO_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
    GPIO_Init(PORT_DATA_GPIO, &GPIO_InitStructure);

    return dat;
}




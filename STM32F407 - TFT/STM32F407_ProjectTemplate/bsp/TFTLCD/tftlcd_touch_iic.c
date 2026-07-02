#include "tftlcd_touch_iic.h"
#include "delay.h"

#if (TFTLCD_USING_TOUCH != 0)

/* IIC状态结构体 */
static struct
{
    uint8_t iic_addr;
} g_tftlcd_touch_iic_sta = {0};

/**
 * @brief  IIC延时
 */
static inline void tftlcd_touch_iic_delay(void)
{
    delay_us(2);
}

/**
 * @brief  IIC起始信号
 */
static void tftlcd_touch_iic_start(void)
{
    TFTLCD_TOUCH_IIC_SDA(1);
    TFTLCD_TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SDA(0);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
}

/**
 * @brief  IIC停止信号
 */
static void tftlcd_touch_iic_stop(void)
{
    TFTLCD_TOUCH_IIC_SDA(0);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
}

/**
 * @brief  等待ACK应答
 */
static uint8_t tftlcd_touch_iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    TFTLCD_TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();

    while (TFTLCD_TOUCH_IIC_READ_SDA())
    {
        waittime++;
        if (waittime > 250)
        {
            tftlcd_touch_iic_stop();
            rack = 1;
            break;
        }
    }

    TFTLCD_TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
    return rack;
}

/**
 * @brief  发送ACK应答
 */
static void tftlcd_touch_iic_ack(void)
{
    TFTLCD_TOUCH_IIC_SDA(0);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
}

/**
 * @brief  发送NACK应答
 */
static void tftlcd_touch_iic_nack(void)
{
    TFTLCD_TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TFTLCD_TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
}

/**
 * @brief  IIC发送一个字节
 */
static void tftlcd_touch_iic_send_byte(uint8_t dat)
{
    uint8_t t;
    for (t = 0; t < 8; t++)
    {
        TFTLCD_TOUCH_IIC_SDA( (BitAction)((dat & 0x80) >> 7) );
        tftlcd_touch_iic_delay();
        TFTLCD_TOUCH_IIC_SCL(1);
        tftlcd_touch_iic_delay();
        TFTLCD_TOUCH_IIC_SCL(0);
        dat <<= 1;
    }
    TFTLCD_TOUCH_IIC_SDA(1);
}

/**
 * @brief  IIC接收一个字节
 */
static uint8_t tftlcd_touch_iic_recv_byte(uint8_t ack)
{
    uint8_t i, dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        TFTLCD_TOUCH_IIC_SCL(1);
        tftlcd_touch_iic_delay();

        if (TFTLCD_TOUCH_IIC_READ_SDA()) dat++;

        TFTLCD_TOUCH_IIC_SCL(0);
        tftlcd_touch_iic_delay();
    }

    ack ? tftlcd_touch_iic_ack() : tftlcd_touch_iic_nack();
    return dat;
}

/**
 * @brief  IIC初始化（STM32F4标准库，与HAL库100%对齐）
 */
void tftlcd_touch_iic_init(uint8_t iic_addr)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能时钟 */
    RCC_AHB1PeriphClockCmd(TFTLCD_TOUCH_IIC_SCL_CLK | TFTLCD_TOUCH_IIC_SDA_CLK, ENABLE);

    /* SCL：推挽输出 + 上拉（与HAL库完全一致） */
    GPIO_InitStructure.GPIO_Pin   = TFTLCD_TOUCH_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 统一驱动速度
    GPIO_Init(TFTLCD_TOUCH_IIC_SCL_PORT, &GPIO_InitStructure);

    /* SDA：开漏输出 + **无上下拉**（修复BUG！与HAL库严格一致） */
    GPIO_InitStructure.GPIO_Pin   = TFTLCD_TOUCH_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; // 关键修复：删除上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 统一驱动速度
    GPIO_Init(TFTLCD_TOUCH_IIC_SDA_PORT, &GPIO_InitStructure);

    tftlcd_touch_iic_stop();
    g_tftlcd_touch_iic_sta.iic_addr = iic_addr;
}

/**
 * @brief  IIC写寄存器
 */
uint8_t tftlcd_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i, ret;

    tftlcd_touch_iic_start();
    tftlcd_touch_iic_send_byte((g_tftlcd_touch_iic_sta.iic_addr << 1) | TFTLCD_TOUCH_IIC_WRITE);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)(reg >> 8));
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)reg);
    tftlcd_touch_iic_wait_ack();

    for (i = 0; i < len; i++)
    {
        tftlcd_touch_iic_send_byte(buf[i]);
        ret = tftlcd_touch_iic_wait_ack();
        if (ret != 0) break;
    }

    tftlcd_touch_iic_stop();
    return ret ? TFTLCD_TOUCH_IIC_ERROR : TFTLCD_TOUCH_IIC_EOK;
}

/**
 * @brief  IIC读寄存器
 */
void tftlcd_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;

    tftlcd_touch_iic_start();
    tftlcd_touch_iic_send_byte((g_tftlcd_touch_iic_sta.iic_addr << 1) | TFTLCD_TOUCH_IIC_WRITE);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)(reg >> 8));
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)reg);
    tftlcd_touch_iic_wait_ack();

    tftlcd_touch_iic_start();
    tftlcd_touch_iic_send_byte((g_tftlcd_touch_iic_sta.iic_addr << 1) | TFTLCD_TOUCH_IIC_READ);
    tftlcd_touch_iic_wait_ack();

    for (i = 0; i < len - 1; i++)
    {
        buf[i] = tftlcd_touch_iic_recv_byte(1);
    }
    buf[i] = tftlcd_touch_iic_recv_byte(0);

    tftlcd_touch_iic_stop();
}

#endif

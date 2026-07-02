#include "tftlcd_touch_iic.h"
#include "delay.h"

#if (tftlcd_USING_TOUCH != 0)

/* 控制IIC通讯是读操作还是写操作 */
#define tftlcd_TOUCH_IIC_WRITE  0
#define tftlcd_TOUCH_IIC_READ   1

/* ATK-MD0430触摸IIC数据结构体 */
static struct
{
    uint8_t iic_addr;               /* IIC通讯地址 */
} g_tftlcd_touch_iic_sta = {0};

/**
 * @brief       IIC接口延时函数，用于控制IIC读写速度
 * @param       无
 * @retval      无
 */
static inline void tftlcd_touch_iic_delay(void)
{
    delay_us(2);
}

/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
static void tftlcd_touch_iic_start(void)
{
    TOUCH_IIC_SDA(1);
    TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SDA(0);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
}

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
static void tftlcd_touch_iic_stop(void)
{
    TOUCH_IIC_SDA(0);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
}

/**
 * @brief       等待IIC应答信号
 * @param       无
 * @retval      0: 应答信号接收成功
 *              1: 应答信号接收失败
 */
static uint8_t tftlcd_touch_iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;
    
    TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    
    while (TOUCH_IIC_READ_SDA())
    {
        waittime++;
        
        if (waittime > 250)
        {
            tftlcd_touch_iic_stop();
            rack = 1;
            break;
        }
    }
    
    TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
    
    return rack;
}

/**
 * @brief       产生ACK应答信号
 * @param       无
 * @retval      无
 */
static void tftlcd_touch_iic_ack(void)
{
    TOUCH_IIC_SDA(0);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
}

/**
 * @brief       不产生ACK应答信号
 * @param       无
 * @retval      无
 */
static void tftlcd_touch_iic_nack(void)
{
    TOUCH_IIC_SDA(1);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(1);
    tftlcd_touch_iic_delay();
    TOUCH_IIC_SCL(0);
    tftlcd_touch_iic_delay();
}

/**
 * @brief       IIC发送一个字节
 * @param       dat: 要发送的数据
 * @retval      无
 */
static void tftlcd_touch_iic_send_byte(uint8_t dat)
{
    uint8_t t;
    
    for (t=0; t<8; t++)
    {
				TOUCH_IIC_SDA( (BitAction)((dat & 0x80) >> 7) );
//        TOUCH_IIC_SDA((dat & 0x80) >> 7);
        tftlcd_touch_iic_delay();
        TOUCH_IIC_SCL(1);
        tftlcd_touch_iic_delay();
        TOUCH_IIC_SCL(0);
        dat <<= 1;
    }
    TOUCH_IIC_SDA(1);
}

/**
 * @brief       IIC接收一个字节
 * @param       ack: ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
static uint8_t tftlcd_touch_iic_recv_byte(uint8_t ack)
{
    uint8_t i;
    uint8_t dat = 0;
    
    for (i = 0; i < 8; i++ )
    {
        dat <<= 1;
        TOUCH_IIC_SCL(1);
        tftlcd_touch_iic_delay();
        
        if (TOUCH_IIC_READ_SDA())
        {
            dat++;
        }
        
        TOUCH_IIC_SCL(0);
        tftlcd_touch_iic_delay();
    }
    
    if (ack == 0)
    {
        tftlcd_touch_iic_nack();
    }
    else
    {
        tftlcd_touch_iic_ack();
    }

    return dat;
}

/**
 * @brief       初始化IIC接口
 * @param       iic_addr: IIC设备地址
 * @retval      无
 */
void tftlcd_touch_iic_init(uint8_t iic_addr)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* F407 使能 GPIOC 时钟（AHB1） */
    RCC_AHB1PeriphClockCmd(RCC_TOUCH_IIC_SCL_GPIO | RCC_TOUCH_IIC_SDA_GPIO, ENABLE);
    
    /* 初始化SCL引脚：PC0 推挽输出 上拉 100M（F4标准） */
    GPIO_InitStructure.GPIO_Pin    = GPIO_TOUCH_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;       // 输出模式
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;       // 推挽
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;        // 上拉
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;   // 高速
    GPIO_Init(PORT_TOUCH_IIC_SCL_GPIO, &GPIO_InitStructure);

    /* 初始化SDA引脚：PC3 开漏输出 上拉 100M（F4标准，IIC必须开漏） */
    GPIO_InitStructure.GPIO_Pin    = GPIO_TOUCH_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;       // 输出模式
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;       // 开漏
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;        // 上拉
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;   // 高速
    GPIO_Init(PORT_TOUCH_IIC_SDA_GPIO, &GPIO_InitStructure);
    
    tftlcd_touch_iic_stop();
    
    g_tftlcd_touch_iic_sta.iic_addr = iic_addr;
}

/**
 * @brief       写ATK-MD0430模块触摸寄存器
 * @param       reg: 待写寄存器地址
 *              buf: 待写入的数据
 *              len: 待写入数据的长度
 * @retval      0: 成功, 1: 失败
 */
uint8_t tftlcd_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    uint8_t ret;
    
    tftlcd_touch_iic_start();
    tftlcd_touch_iic_send_byte((g_tftlcd_touch_iic_sta.iic_addr << 1) | tftlcd_TOUCH_IIC_WRITE);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)reg & 0xFF);
    tftlcd_touch_iic_wait_ack();
    
    for (buf_index=0; buf_index<len; buf_index++)
    {
        tftlcd_touch_iic_send_byte(buf[buf_index]);
        ret = tftlcd_touch_iic_wait_ack();
        if (ret != 0)
        {
            break;
        }
    }
    
    tftlcd_touch_iic_stop();
    
    if (ret != 0)
    {
        return tftlcd_TOUCH_IIC_ERROR;
    }
    
    return tftlcd_TOUCH_IIC_EOK;
}

/**
 * @brief       读ATK-MD0430模块触摸寄存器
 * @param       reg: 待读寄存器地址
 *              buf: 读取的数据
 *              len: 待读取数据的长度
 * @retval      无
 */
void tftlcd_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    
    tftlcd_touch_iic_start();
    tftlcd_touch_iic_send_byte((g_tftlcd_touch_iic_sta.iic_addr << 1) | tftlcd_TOUCH_IIC_WRITE);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_send_byte((uint8_t)reg & 0xFF);
    tftlcd_touch_iic_wait_ack();
    tftlcd_touch_iic_start();
    tftlcd_touch_iic_send_byte((g_tftlcd_touch_iic_sta.iic_addr << 1) | tftlcd_TOUCH_IIC_READ);
    tftlcd_touch_iic_wait_ack();
    
    for (buf_index=0; buf_index<len - 1; buf_index++)
    {
        buf[buf_index] = tftlcd_touch_iic_recv_byte(1);
    }
    
    buf[buf_index] = tftlcd_touch_iic_recv_byte(0);
    
    tftlcd_touch_iic_stop();
}

#endif /* tftlcd_USING_TOUCH */



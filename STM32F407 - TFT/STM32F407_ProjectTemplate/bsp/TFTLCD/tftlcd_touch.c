#include "tftlcd_touch.h"
#include "tftlcd_touch_iic.h"
#include "delay.h"
#include <string.h>

#if (TFTLCD_USING_TOUCH != 0)

/* 触摸寄存器定义 */
#define TFTLCD_TOUCH_REG_CTRL        0x8040
#define TFTLCD_TOUCH_REG_PID         0x8140
#define TFTLCD_TOUCH_REG_TPINFO      0x814E
#define TFTLCD_TOUCH_REG_TP1         0x8150
#define TFTLCD_TOUCH_REG_TP2         0x8158
#define TFTLCD_TOUCH_REG_TP3         0x8160
#define TFTLCD_TOUCH_REG_TP4         0x8168
#define TFTLCD_TOUCH_REG_TP5         0x8170

/* 触摸状态掩码 */
#define TFTLCD_TOUCH_TPINFO_MASK_STA 0x80
#define TFTLCD_TOUCH_TPINFO_MASK_CNT 0x0F

/* 触摸点寄存器数组 */
static const uint16_t g_tftlcd_touch_tp_reg[TFTLCD_TOUCH_TP_MAX] = {
    TFTLCD_TOUCH_REG_TP1,
    TFTLCD_TOUCH_REG_TP2,
    TFTLCD_TOUCH_REG_TP3,
    TFTLCD_TOUCH_REG_TP4,
    TFTLCD_TOUCH_REG_TP5,
};

/**
 * @brief  触摸硬件初始化
 */
static void tftlcd_touch_hw_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_AHB1PeriphClockCmd(TFTLCD_TOUCH_PEN_GPIO_CLK | TFTLCD_TOUCH_TCS_GPIO_CLK, ENABLE);

    /* PEN引脚：浮空输入 */
    GPIO_InitStructure.GPIO_Pin   = TFTLCD_TOUCH_PEN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(TFTLCD_TOUCH_PEN_GPIO_PORT, &GPIO_InitStructure);

    /* TCS引脚：推挽输出上拉 */
    GPIO_InitStructure.GPIO_Pin   = TFTLCD_TOUCH_TCS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(TFTLCD_TOUCH_TCS_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  触摸硬件复位
 */
static void tftlcd_touch_hw_reset(tftlcd_touch_iic_addr_t addr)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* PEN临时配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin   = TFTLCD_TOUCH_PEN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TFTLCD_TOUCH_PEN_GPIO_PORT, &GPIO_InitStructure);

    switch (addr)
    {
        case TFTLCD_TOUCH_IIC_ADDR_14:
            TFTLCD_TOUCH_TCS(0);
            TFTLCD_TOUCH_PEN(0);
            TFTLCD_TOUCH_PEN(1);
            delay_ms(1);
            TFTLCD_TOUCH_TCS(1);
            delay_ms(10);
            break;

        case TFTLCD_TOUCH_IIC_ADDR_5D:
            TFTLCD_TOUCH_TCS(0);
            TFTLCD_TOUCH_PEN(0);
            delay_ms(1);
            TFTLCD_TOUCH_TCS(1);
            delay_ms(10);
            break;

        default: break;
    }

    /* PEN恢复浮空输入 */
    GPIO_InitStructure.GPIO_Pin   = TFTLCD_TOUCH_PEN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TFTLCD_TOUCH_PEN_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  触摸软件复位
 */
static void tftlcd_touch_sw_reset(void)
{
    uint8_t dat;
    dat = 0x02;
    tftlcd_touch_iic_write_reg(TFTLCD_TOUCH_REG_CTRL, &dat, 1);
    delay_ms(10);

    dat = 0x00;
    tftlcd_touch_iic_write_reg(TFTLCD_TOUCH_REG_CTRL, &dat, 1);
}

/**
 * @brief  读取触摸模块PID
 */
static void tftlcd_touch_get_pid(char *pid)
{
    tftlcd_touch_iic_read_reg(TFTLCD_TOUCH_REG_PID, (uint8_t *)pid, 4);
    pid[4] = '\0';
}

/**
 * @brief  触摸初始化
 */
uint8_t tftlcd_touch_init(void)
{
    char pid[5];

    tftlcd_touch_hw_init();
    tftlcd_touch_hw_reset(TFTLCD_TOUCH_IIC_ADDR_14);
    tftlcd_touch_iic_init(TFTLCD_TOUCH_IIC_ADDR_14);
    tftlcd_touch_get_pid(pid);

    if (strcmp(pid, TFTLCD_TOUCH_PID) != 0)
    {
        return TFTLCD_TOUCH_ERROR;
    }

    tftlcd_touch_sw_reset();
    return TFTLCD_TOUCH_EOK;
}

/**
 * @brief  触摸扫描
 */
uint8_t tftlcd_touch_scan(tftlcd_touch_point_t *point, uint8_t cnt)
{
    uint8_t tp_info, tp_cnt, point_index, dir;
    uint8_t tpn_info[6];
    tftlcd_touch_point_t point_raw;

    /* 入参合法性检查 */
    if ((cnt == 0) || (cnt > TFTLCD_TOUCH_TP_MAX) || (point == NULL)) 
    {
        return 0;
    }

    /* 补全：HAL库必备空指针校验，防止死机 */
    for (point_index=0; point_index<cnt; point_index++)
    {
        if (&point[point_index] == NULL)
        {
            return 0;
        }
    }

    /* 读取触摸状态 */
    tftlcd_touch_iic_read_reg(TFTLCD_TOUCH_REG_TPINFO, &tp_info, 1);
    if ((tp_info & TFTLCD_TOUCH_TPINFO_MASK_STA) == TFTLCD_TOUCH_TPINFO_MASK_STA)
    {
        tp_cnt = tp_info & TFTLCD_TOUCH_TPINFO_MASK_CNT;
        tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;

        /* 读取触摸点数据 */
        for (point_index = 0; point_index < tp_cnt; point_index++)
        {
            tftlcd_touch_iic_read_reg(g_tftlcd_touch_tp_reg[point_index], tpn_info, 6);
            point_raw.x = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
            point_raw.y = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
            point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];

            /* 坐标旋转 */
            dir = tftlcd_get_disp_dir();
            switch (dir)
            {
                case 0:  
                    point[point_index].x = tftlcd_get_lcd_width() - point_raw.y;
                    point[point_index].y = point_raw.x;
                    break;
                case 1:  
                    point[point_index].x = point_raw.x;
                    point[point_index].y = point_raw.y;
                    break;
                default:
                    point[point_index].x = 0;
                    point[point_index].y = 0;
                    break;
            }
            point[point_index].size = point_raw.size;
        }

        /* 清除触摸标志 */
        tp_info = 0;
        tftlcd_touch_iic_write_reg(TFTLCD_TOUCH_REG_TPINFO, &tp_info, 1);
        return tp_cnt;
    }
    return 0;
}

#endif

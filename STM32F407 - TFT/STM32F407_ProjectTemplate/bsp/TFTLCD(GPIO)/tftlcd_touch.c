#include "tftlcd_touch.h"
#include "tftlcd_touch_iic.h"
#include "delay.h"
#include <string.h>

#if (tftlcd_USING_TOUCH != 0)

/* ATK-MD0430模块触摸的PID */
#define tftlcd_TOUCH_PID1    "911"
#define tftlcd_TOUCH_PID2    "1158"

/* ATK-MD0430模块最大触摸点数量 */
#define tftlcd_TOUCH_TP_MAX 5

/* ATK-MD0430模块触摸部分寄存器定义 */
#define tftlcd_TOUCH_REG_CTRL           0x8040  /* 控制寄存器 */
#define tftlcd_TOUCH_REG_PID            0x8140  /* PID寄存器 */
#define tftlcd_TOUCH_REG_TPINFO         0x814E  /* 触摸状态寄存器 */
#define tftlcd_TOUCH_REG_TP1            0x8150  /* 触摸点1数据寄存器 */
#define tftlcd_TOUCH_REG_TP2            0x8158  /* 触摸点2数据寄存器 */
#define tftlcd_TOUCH_REG_TP3            0x8160  /* 触摸点3数据寄存器 */
#define tftlcd_TOUCH_REG_TP4            0x8168  /* 触摸点4数据寄存器 */
#define tftlcd_TOUCH_REG_TP5            0x8170  /* 触摸点5数据寄存器 */

/* 触摸状态寄存器掩码 */
#define tftlcd_TOUCH_TPINFO_MASK_STA    0x80
#define tftlcd_TOUCH_TPINFO_MASK_CNT    0x0F

/* 触摸点数据寄存器数组 */
static const uint16_t g_tftlcd_touch_tp_reg[tftlcd_TOUCH_TP_MAX] = {
    tftlcd_TOUCH_REG_TP1,
    tftlcd_TOUCH_REG_TP2,
    tftlcd_TOUCH_REG_TP3,
    tftlcd_TOUCH_REG_TP4,
    tftlcd_TOUCH_REG_TP5,
};

/**
 * @brief       TFT触摸硬件初始化
 * @param       无
 * @retval      无
 */
static void tftlcd_touch_hw_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* F407 使能 GPIOC 时钟（AHB1） */
    RCC_AHB1PeriphClockCmd(RCC_TOUCH_PEN_GPIO | RCC_TOUCH_TCS_GPIO, ENABLE);

    /* 初始化PEN引脚：PC1 浮空输入（F4标准写法） */
    GPIO_InitStructure.GPIO_Pin    = GPIO_TOUCH_PEN_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IN;        // 输入模式
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;    // 无上下拉 = 浮空
    GPIO_Init(PORT_TOUCH_PEN_GPIO, &GPIO_InitStructure);

    /* 初始化TCS引脚：PC13 推挽输出 上拉 100M（F4标准写法） */
    GPIO_InitStructure.GPIO_Pin    = GPIO_TOUCH_TCS_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;       // 输出模式
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;        // 上拉
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;   // 高速
    GPIO_Init(PORT_TOUCH_TCS_GPIO, &GPIO_InitStructure);

    /* 默认电平 */
    TOUCH_TCS_OUT(1);
    TOUCH_PEN_OUT(1);
}

/**
 * @brief       TFT触摸硬件复位
 * @param       addr: 复位后IIC地址
 * @retval      无
 */
static void tftlcd_touch_hw_reset(tftlcd_touch_iic_addr_t addr)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* PEN引脚切换为推挽输出（F4标准） */
    GPIO_InitStructure.GPIO_Pin    = GPIO_TOUCH_PEN_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
    GPIO_Init(PORT_TOUCH_PEN_GPIO, &GPIO_InitStructure);

    switch (addr)
    {
        case tftlcd_TOUCH_IIC_ADDR_14:
        {
            TOUCH_TCS_OUT(0);
            TOUCH_PEN_OUT(0);
            TOUCH_PEN_OUT(1);
            delay_ms(1);
            TOUCH_TCS_OUT(1);
            delay_ms(10);
            break;
        }
        case tftlcd_TOUCH_IIC_ADDR_5D:
        {
            TOUCH_TCS_OUT(0);
            TOUCH_PEN_OUT(0);
            delay_ms(1);
            TOUCH_TCS_OUT(1);
            delay_ms(10);
            break;
        }
        default:
            break;
    }

    /* PEN引脚恢复为浮空输入（F4标准） */
    GPIO_InitStructure.GPIO_Pin    = GPIO_TOUCH_PEN_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
    GPIO_Init(PORT_TOUCH_PEN_GPIO, &GPIO_InitStructure);
}

/**
 * @brief       TFT触摸软件复位
 * @param       无
 * @retval      无
 */
static void tftlcd_touch_sw_reset(void)
{
    uint8_t dat;

    dat = 0x02;
    tftlcd_touch_iic_write_reg(tftlcd_TOUCH_REG_CTRL, &dat, 1);
    delay_ms(10);

    dat = 0x00;
    tftlcd_touch_iic_write_reg(tftlcd_TOUCH_REG_CTRL, &dat, 1);
}

/**
 * @brief       获取触摸PID
 * @param       pid: 存储PID的缓冲区
 * @retval      无
 */
static void tftlcd_touch_get_pid(char *pid)
{
    tftlcd_touch_iic_read_reg(tftlcd_TOUCH_REG_PID, (uint8_t *)pid, 4);
    pid[4] = '\0';
}

/**
 * @brief       TFT触摸初始化
 * @param       无
 * @retval      状态
 */
uint8_t tftlcd_touch_init(void)
{
    char pid[5];

    tftlcd_touch_hw_init();
    tftlcd_touch_hw_reset(tftlcd_TOUCH_IIC_ADDR_14);
    tftlcd_touch_iic_init(tftlcd_TOUCH_IIC_ADDR_14);
    delay_ms(100);
    tftlcd_touch_get_pid(pid);

    if ((strcmp(pid, tftlcd_TOUCH_PID1) != 0) && (strcmp(pid, tftlcd_TOUCH_PID2) != 0))
    {
        return tftlcd_TOUCH_ERROR;
    }

    tftlcd_touch_sw_reset();
    return tftlcd_TOUCH_EOK;
}

/**
 * @brief       TFT触摸扫描
 * @param       point: 触摸点数据
 *              cnt  : 扫描点数
 * @retval      有效触摸点数
 */
uint8_t tftlcd_touch_scan(tftlcd_touch_point_t *point, uint8_t cnt)
{
    uint8_t tp_info;
    uint8_t tp_cnt;
    uint8_t point_index;
    tftlcd_lcd_disp_dir_t dir;
    uint8_t tpn_info[6];
    tftlcd_touch_point_t point_raw;

    if ((cnt == 0) || (cnt > tftlcd_TOUCH_TP_MAX))
    {
        return 0;
    }

    for (point_index=0; point_index<cnt; point_index++)
    {
        if (&point[point_index] == NULL)
        {
            return 0;
        }
    }

    tftlcd_touch_iic_read_reg(tftlcd_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
    if ((tp_info & tftlcd_TOUCH_TPINFO_MASK_STA) == tftlcd_TOUCH_TPINFO_MASK_STA)
    {
        tp_cnt = tp_info & tftlcd_TOUCH_TPINFO_MASK_CNT;
        tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;

        for (point_index=0; point_index<tp_cnt; point_index++)
        {
            tftlcd_touch_iic_read_reg(g_tftlcd_touch_tp_reg[point_index], tpn_info, sizeof(tpn_info));
            point_raw.x = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
            point_raw.y = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
            point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];

            dir = tftlcd_get_disp_dir();
            switch (dir)
            {
                case tftlcd_LCD_DISP_DIR_0:
                    point[point_index].x = point_raw.x;
                    point[point_index].y = point_raw.y;
                    break;

                case tftlcd_LCD_DISP_DIR_90:
                    point[point_index].x = point_raw.y;
                    point[point_index].y = tftlcd_get_lcd_height() - point_raw.x;
                    break;

                case tftlcd_LCD_DISP_DIR_180:
                    point[point_index].x = tftlcd_get_lcd_width() - point_raw.x;
                    point[point_index].y = tftlcd_get_lcd_height() - point_raw.y;
                    break;

                case tftlcd_LCD_DISP_DIR_270:
                    point[point_index].x = tftlcd_get_lcd_width() - point_raw.y;
                    point[point_index].y = point_raw.x;
                    break;
            }
            point[point_index].size = point_raw.size;
        }

        tp_info = 0;
        tftlcd_touch_iic_write_reg(tftlcd_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
        return tp_cnt;
    }
    else
    {
        return 0;
    }
}

#endif /* tftlcd_USING_TOUCH */



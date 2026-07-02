#ifndef __TFTLCD_H
#define __TFTLCD_H

#include "sys.h"

/* 定义是否使用触摸 */
#define TFTLCD_USING_TOUCH           1

/* 定义启用的字体 */
#define TFT_FONT_12               1
#define TFT_FONT_16               1
#define TFT_FONT_24               1
#define TFT_FONT_32               1

/* 默认字体配置 */
#if ((TFT_FONT_12 == 0) && (TFT_FONT_16 == 0) && (TFT_FONT_24 == 0) && (TFT_FONT_32 == 0))
#undef TFT_FONT_12
#define TFT_FONT_12 1
#endif

/* 屏幕PID定义 */
#define TFTLCD_PID_800480            0x748A
#define TFTLCD_PID_1024600           0x716A

/* 扫描方向定义 */
#define TFTLCD_SCAN_DIR_L2R_U2D      0x00
#define TFTLCD_SCAN_DIR_L2R_D2U      0x01
#define TFTLCD_SCAN_DIR_R2L_U2D      0x02
#define TFTLCD_SCAN_DIR_R2L_D2U      0x03
#define TFTLCD_SCAN_DIR_U2D_L2R      0x04
#define TFTLCD_SCAN_DIR_U2D_R2L      0x05
#define TFTLCD_SCAN_DIR_D2U_L2R      0x06
#define TFTLCD_SCAN_DIR_D2U_R2L      0x07

/* 显示方向定义 */
#define TFTLCD_DISP_DIR_0            0x00
#define TFTLCD_DISP_DIR_90           0x01

/* 字体定义 */
#if (TFT_FONT_12 != 0)
#define TFTLCD_FONT_12           12
#endif
#if (TFT_FONT_16 != 0)
#define TFTLCD_FONT_16           16
#endif
#if (TFT_FONT_24 != 0)
#define TFTLCD_FONT_24           24
#endif
#if (TFT_FONT_32 != 0)
#define TFTLCD_FONT_32           32
#endif

/* 数字显示模式 */
#define TFTLCD_NUM_SHOW_NOZERO       0x00
#define TFTLCD_NUM_SHOW_ZERO         0x01

/* 颜色定义 (RGB565) */
#define TFTLCD_WHITE                 0xFFFF
#define TFTLCD_BLACK                 0x0000
#define TFTLCD_BLUE                  0x001F
#define TFTLCD_BRED                  0XF81F
#define TFTLCD_GRED                  0XFFE0
#define TFTLCD_GBLUE                 0X07FF
#define TFTLCD_RED                   0xF800
#define TFTLCD_MAGENTA               0xF81F
#define TFTLCD_GREEN                 0x07E0
#define TFTLCD_CYAN                  0x7FFF
#define TFTLCD_YELLOW                0xFFE0
#define TFTLCD_BROWN                 0XBC40
#define TFTLCD_BRRED                 0XFC07

/* 错误码 */
#define TFTLCD_EOK                   0
#define TFTLCD_ERROR                 1
#define TFTLCD_EINVAL                2

/* 外部函数声明 */
uint8_t  tftlcd_init(void);
uint16_t tftlcd_get_lcd_width(void);
uint16_t tftlcd_get_lcd_height(void);

void     tftlcd_backlight_config(uint8_t percent);
void     tftlcd_display_on(void);
void     tftlcd_display_off(void);

uint8_t  tftlcd_set_scan_dir(uint8_t scan_dir);
uint8_t  tftlcd_set_disp_dir(uint8_t disp_dir);
uint8_t  tftlcd_get_scan_dir(void);
uint8_t  tftlcd_get_disp_dir(void);

void     tftlcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void     tftlcd_clear(uint16_t color);
void     tftlcd_draw_point(uint16_t x, uint16_t y, volatile uint16_t color);
uint16_t tftlcd_read_point(uint16_t x, uint16_t y);
void     tftlcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void     tftlcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void     tftlcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void     tftlcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);

void     tftlcd_show_char(uint16_t x, uint16_t y, char ch, uint8_t font, uint16_t color);
void     tftlcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, uint8_t font, uint16_t color);
void     tftlcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t mode, uint8_t font, uint16_t color);
void     tftlcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t font, uint16_t color);
void     tftlcd_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pic);

#endif

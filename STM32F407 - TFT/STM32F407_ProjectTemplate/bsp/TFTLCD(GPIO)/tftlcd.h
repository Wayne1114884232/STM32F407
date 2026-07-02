#ifndef __tftlcd_H
#define __tftlcd_H

#include "sys.h"   // 包含系统核心头文件（STM32标准库基础）

/******************************************************************************************
 * 【宏定义配置区】：ATK-MD0430 TFTLCD模块 基础配置
 ******************************************************************************************/
#define tftlcd_LCD_WIDTH            480     // LCD屏幕宽度（分辨率：480*800）
#define tftlcd_LCD_HEIGHT           800     // LCD屏幕高度

#define tftlcd_USING_TOUCH          1       // 1=使能触摸屏功能  0=关闭触摸屏功能

// 字体使能配置：1=使能  0=关闭（至少开启一种字体）
#define tftlcd_FONT_12              1       // 使能12号字体
#define tftlcd_FONT_16              1       // 使能16号字体
#define tftlcd_FONT_24              1       // 使能24号字体
#define tftlcd_FONT_32              1       // 使能32号字体

// 默认使能触摸（防止未定义报错）
#ifndef tftlcd_USING_TOUCH
#define tftlcd_USING_TOUCH 1
#endif

// 默认至少开启12号字体（防止无字体可用报错）
#if ((tftlcd_FONT_12 == 0) && (tftlcd_FONT_16 == 0) && (tftlcd_FONT_24 == 0) && (tftlcd_FONT_32 == 0))
#undef tftlcd_FONT_12
#define tftlcd_FONT_12 1
#endif

// 使能触摸时包含触摸屏驱动头文件
#if (tftlcd_USING_TOUCH != 0)
#include "tftlcd_touch.h"
#endif

/******************************************************************************************
 * 【硬件引脚配置区】：LCD背光控制引脚（STM32标准库）
 ******************************************************************************************/
#define tftlcd_BL_GPIO_PORT         GPIOB               // 背光控制GPIO端口
#define tftlcd_BL_GPIO_PIN          GPIO_Pin_15         // 背光控制GPIO引脚
#define tftlcd_BL_GPIO_CLK          RCC_AHB1Periph_GPIOB// 背光引脚时钟
#define tftlcd_BL_GPIO_CLK_ENABLE() RCC_AHB1PeriphClockCmd(tftlcd_BL_GPIO_CLK, ENABLE) // 时钟使能函数

// 背光IO操作宏：x=1点亮背光  x=0关闭背光
#define tftlcd_BL(x)                GPIO_WriteBit(tftlcd_BL_GPIO_PORT, tftlcd_BL_GPIO_PIN, (BitAction)x)

/******************************************************************************************
 * 【枚举类型定义区】：LCD工作模式枚举
 ******************************************************************************************/
/**
 * @brief  LCD扫描方向枚举（控制像素刷新顺序）
 */
typedef enum
{
    tftlcd_LCD_SCAN_DIR_L2R_U2D = 0x0000,   // 从左到右，从上到下（默认）
    tftlcd_LCD_SCAN_DIR_L2R_D2U = 0x0080,   // 从左到右，从下到上
    tftlcd_LCD_SCAN_DIR_R2L_U2D = 0x0040,   // 从右到左，从上到下
    tftlcd_LCD_SCAN_DIR_R2L_D2U = 0x00C0,   // 从右到左，从下到上
    tftlcd_LCD_SCAN_DIR_U2D_L2R = 0x0020,   // 从上到下，从左到右
    tftlcd_LCD_SCAN_DIR_U2D_R2L = 0x0060,   // 从上到下，从右到左
    tftlcd_LCD_SCAN_DIR_D2U_L2R = 0x00A0,   // 从下到上，从左到右
    tftlcd_LCD_SCAN_DIR_D2U_R2L = 0x00E0,   // 从下到上，从右到左
} tftlcd_lcd_scan_dir_t;

/**
 * @brief  LCD显示方向枚举（屏幕整体旋转）
 */
typedef enum
{
    tftlcd_LCD_DISP_DIR_0 = 0x00,           // 顺时针旋转0°（默认）
    tftlcd_LCD_DISP_DIR_90,                 // 顺时针旋转90°
    tftlcd_LCD_DISP_DIR_180,                // 顺时针旋转180°
    tftlcd_LCD_DISP_DIR_270,                // 顺时针旋转270°
} tftlcd_lcd_disp_dir_t;

/**
 * @brief  LCD显示字体枚举
 */
typedef enum
{
#if (tftlcd_FONT_12 != 0)
    tftlcd_LCD_FONT_12,             // 12号字体
#endif
#if (tftlcd_FONT_16 != 0)
    tftlcd_LCD_FONT_16,             // 16号字体
#endif
#if (tftlcd_FONT_24 != 0)
    tftlcd_LCD_FONT_24,             // 24号字体
#endif
#if (tftlcd_FONT_32 != 0)
    tftlcd_LCD_FONT_32,             // 32号字体
#endif
} tftlcd_lcd_font_t;

/**
 * @brief  数字显示模式枚举
 */
typedef enum
{
    tftlcd_NUM_SHOW_NOZERO = 0x00,  // 高位0不显示（如12显示为12，非0012）
    tftlcd_NUM_SHOW_ZERO,           // 高位0显示（如12显示为0012）
} tftlcd_num_mode_t;

/******************************************************************************************
 * 【颜色宏定义区】：RGB565格式颜色值（16位色）
 ******************************************************************************************/
#define tftlcd_WHITE        0xFFFF    // 白色
#define tftlcd_BLACK        0x0000    // 黑色
#define tftlcd_BLUE         0x001F    // 蓝色
#define tftlcd_BRED         0XF81F    // 蓝红色
#define tftlcd_GRED         0XFFE0    // 绿红色
#define tftlcd_GBLUE        0X07FF    // 绿蓝色
#define tftlcd_RED          0xF800    // 红色
#define tftlcd_MAGENTA      0xF81F    // 品红色
#define tftlcd_GREEN        0x07E0    // 绿色
#define tftlcd_CYAN         0x7FFF    // 青色
#define tftlcd_YELLOW       0xFFE0    // 黄色
#define tftlcd_BROWN        0XBC40    // 棕色
#define tftlcd_BRRED        0XFC07    // 棕红色
#define tftlcd_GRAY         0X8430    // 灰色

/******************************************************************************************
 * 【错误码定义区】：函数返回状态码
 ******************************************************************************************/
#define tftlcd_EOK          0   // 执行成功
#define tftlcd_ERROR        1   // 执行失败
#define tftlcd_EINVAL       2   // 传入参数非法

/******************************************************************************************
 * 【外部函数声明区】：LCD驱动对外接口函数
 ******************************************************************************************/
uint8_t tftlcd_init(void);                                                             // LCD初始化函数
uint16_t tftlcd_get_lcd_width(void);                                                   // 获取LCD宽度
uint16_t tftlcd_get_lcd_height(void);                                                  // 获取LCD高度
void tftlcd_backlight_on(void);                                                        // 开启LCD背光
void tftlcd_backlight_off(void);                                                       // 关闭LCD背光
void tftlcd_display_on(void);                                                          // 开启LCD显示
void tftlcd_display_off(void);                                                         // 关闭LCD显示
uint8_t tftlcd_set_scan_dir(tftlcd_lcd_scan_dir_t scan_dir);                            // 设置LCD扫描方向
uint8_t tftlcd_set_disp_dir(tftlcd_lcd_disp_dir_t disp_dir);                            // 设置LCD显示方向
tftlcd_lcd_scan_dir_t tftlcd_get_scan_dir(void);                                       // 获取当前扫描方向
tftlcd_lcd_disp_dir_t tftlcd_get_disp_dir(void);                                       // 获取当前显示方向
void tftlcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);  // 区域填充颜色
void tftlcd_clear(uint16_t color);                                                     // 清屏（全屏填充）
void tftlcd_draw_point(uint16_t x, uint16_t y, uint16_t color);                        // 画点
uint16_t tftlcd_read_point(uint16_t x, uint16_t y);                                     // 读点（获取指定坐标颜色）
void tftlcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);// 画线
void tftlcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);// 画矩形
void tftlcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);           // 画圆形
void tftlcd_show_char(uint16_t x, uint16_t y, char ch, tftlcd_lcd_font_t font, uint16_t color);// 显示单个字符
void tftlcd_show_string(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *str, tftlcd_lcd_font_t font, uint16_t color);// 显示字符串
void tftlcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, tftlcd_num_mode_t mode, tftlcd_lcd_font_t font, uint16_t color);// 扩展数字显示
void tftlcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, tftlcd_lcd_font_t font, uint16_t color);// 基础数字显示
void tftlcd_show_pic(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *pic);    // 显示图片

#endif


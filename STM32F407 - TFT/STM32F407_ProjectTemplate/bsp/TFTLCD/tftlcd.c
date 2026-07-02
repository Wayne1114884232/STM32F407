#include "tftlcd.h"
#include "tftlcd_font.h"
#include "tftlcd_fsmc.h"
#include "delay.h"

/* LCD时序结构体 */
typedef struct
{
    uint8_t  clock_freq_in_mhz;
    uint16_t hactive;
    uint8_t  hback_porch;
    uint8_t  hfront_porch;
    uint8_t  hsync_len;
    uint16_t vactive;
    uint8_t  vback_porch;
    uint8_t  vfront_porch;
    uint8_t  vsync_len;
} tftlcd_lcd_timing_t;

/* LCD参数结构体 */
typedef struct
{
    uint8_t  id;
    uint16_t pid;
    uint16_t width;
    uint16_t height;
    tftlcd_lcd_timing_t timing;
} tftlcd_lcd_param_t;

/* 参数表 */
static const tftlcd_lcd_param_t tftlcd_lcd_param[] = {
    {0,  TFTLCD_PID_800480,   800, 480, {33,  800,  46, 210,  1, 480, 23, 22, 1}},
    {1,  TFTLCD_PID_1024600, 1024, 600, {45, 1024, 140, 160, 20, 600, 20, 12, 3}},
};

/* 全局状态 */
static struct
{
    const tftlcd_lcd_param_t *param;
    uint16_t width;
    uint16_t height;
    uint8_t  disp_dir;
    uint8_t  scan_dir;
} g_tftlcd_sta = {0};

/* 读状态寄存器 */
uint16_t tftlcd_read_stsr(void)
{
    return TFTLCD_FSMC_REG;
}

/* 写寄存器 */
void tftlcd_set_reg(volatile uint16_t reg, volatile uint16_t dat)
{
    TFTLCD_FSMC_REG = reg;
    TFTLCD_FSMC_DAT = dat;
}

/* 读寄存器 */
uint16_t tftlcd_get_reg(volatile uint16_t reg)
{
    volatile uint16_t dat;
    TFTLCD_FSMC_REG = reg;
    dat = TFTLCD_FSMC_DAT;
    return dat;
}

/* 修改寄存器 */
void tftlcd_modify_reg(volatile uint16_t reg, uint16_t clrbit, uint16_t setbit)
{
    volatile uint16_t original;
    TFTLCD_FSMC_REG = reg;
    original = TFTLCD_FSMC_DAT;
    original &= ~(clrbit);
    original |= (setbit);
    TFTLCD_FSMC_DAT = original;
}

/* 读取屏幕ID */
uint8_t tftlcd_get_id(void)
{
    uint8_t id = 0;
    tftlcd_modify_reg(0x85, (3 << 0), 0);
    tftlcd_modify_reg(0x01, (1 << 5), 0);
    tftlcd_modify_reg(0xF3, 0, (1 << 7));
    delay_ms(100);

    id = (tftlcd_get_reg(0xF2) & (1 << 4)) >> 4;
    id |= (tftlcd_get_reg(0xF4) & (1 << 7)) >> 6;
    return id;
}

/* 根据ID配置参数 */
static uint8_t tftlcd_setup_param_by_id(uint8_t id)
{
    uint8_t index;
    for(index=0; index<(sizeof(tftlcd_lcd_param)/sizeof(tftlcd_lcd_param[0])); index++)
    {
        if(id == tftlcd_lcd_param[index].id)
        {
            g_tftlcd_sta.param = &tftlcd_lcd_param[index];
            return TFTLCD_EOK;
        }
    }
    return TFTLCD_EINVAL;
}

/* 初始化LT7381 */
static void tftlcd_setup_lt7381_by_param(const tftlcd_lcd_param_t *param)
{
    uint8_t hback_porch  = param->timing.hback_porch;
    uint8_t hfront_porch = param->timing.hfront_porch;
    uint8_t hsync_len    = param->timing.hsync_len;

    if(hback_porch < 8)    hback_porch += 8;
    if(hfront_porch < 8)   hfront_porch = 8;
    if(hsync_len < 8)      hsync_len = 8;

    while((tftlcd_read_stsr() & (1 << 1)) != 0);

    /* PLL配置 */
    tftlcd_set_reg(0x05, ((2 << 6) | (5 << 1) | ((param->timing.clock_freq_in_mhz >> 8) & 0x01)));
    tftlcd_set_reg(0x06, (param->timing.clock_freq_in_mhz & 0xFF));
    tftlcd_set_reg(0x07, ((2 << 6) | (5 << 1) | ((133 >> 8) & 0x01)));
    tftlcd_set_reg(0x08, (133 & 0xFF));
    tftlcd_set_reg(0x09, ((2 << 6) | (5 << 1) | ((100 >> 8) & 0x01)));
    tftlcd_set_reg(0x0A, (100 & 0xFF));

    tftlcd_modify_reg(0x00, 0, (1 << 7));
    while((tftlcd_get_reg(0x01) & (1 << 7)) == 0);

    /* SDRAM配置 */
    tftlcd_set_reg(0xE0, 0x20);
    tftlcd_set_reg(0xE1, 0x03);
    tftlcd_set_reg(0xE2, (64000 * (133 * 2)) / 2048);
    tftlcd_set_reg(0xE3, ((64000 * (133 * 2)) / 2048) >> 8);
    tftlcd_modify_reg(0xE4, 0, (1 << 0));
    while((tftlcd_get_reg(0xE4) & (1 << 0)) == 0);

    /* 基础配置 */
    tftlcd_modify_reg(0x01, (1 << 6) | (3 << 3), (2 << 3) | (1 << 0));
    tftlcd_modify_reg(0x02, (3 << 6), 0);
    tftlcd_modify_reg(0x03, (1 << 2) | (3 << 0), 0);

    /* Main Window */
    tftlcd_set_reg(0x20, 0 & ~(3 << 0));
    tftlcd_set_reg(0x21, 0 >> 8);
    tftlcd_set_reg(0x22, 0 >> 16);
    tftlcd_set_reg(0x23, 0 >> 24);
    tftlcd_set_reg(0x24, param->width & ~(3 << 0));
    tftlcd_set_reg(0x25, param->width >> 8);
    tftlcd_set_reg(0x26, 0 & ~(3 << 0));
    tftlcd_set_reg(0x27, 0 >> 8);
    tftlcd_set_reg(0x28, 0);
    tftlcd_set_reg(0x29, 0 >> 8);
    tftlcd_modify_reg(0x10, (3 << 2), (1 << 2));

    /* TFT时序 */
    tftlcd_modify_reg(0x10, (1 << 0), 0);
    tftlcd_modify_reg(0x12, (1 << 7), (1 << 7));
    tftlcd_modify_reg(0x13, (1 << 7)|(1 << 6)|(1 << 5)|(1 << 4), 0);
    tftlcd_set_reg(0x14, (param->timing.hactive / 8) - 1);
    tftlcd_set_reg(0x15, (param->timing.hactive % 8));
    tftlcd_set_reg(0x16, (hback_porch / 8) - 1);
    tftlcd_set_reg(0x17, (hback_porch % 8));
    tftlcd_set_reg(0x18, (hfront_porch / 8) - 1);
    tftlcd_set_reg(0x19, (hsync_len / 8) - 1);
    tftlcd_set_reg(0x1A, (param->timing.vactive - 1));
    tftlcd_set_reg(0x1B, (param->timing.vactive - 1) >> 8);
    tftlcd_set_reg(0x1C, (param->timing.vback_porch - 1));
    tftlcd_set_reg(0x1D, (param->timing.vback_porch - 1) >> 8);
    tftlcd_set_reg(0x1E, (param->timing.vfront_porch - 1));
    tftlcd_set_reg(0x1F, (param->timing.vsync_len - 1));

    /* Canvas/Active Window */
    tftlcd_set_reg(0x50, 0 & ~(3 << 0));
    tftlcd_set_reg(0x51, 0 >> 8);
    tftlcd_set_reg(0x52, 0 >> 16);
    tftlcd_set_reg(0x53, 0 >> 24);
    tftlcd_set_reg(0x54, param->width & ~(3 << 0));
    tftlcd_set_reg(0x55, param->width >> 8);
    tftlcd_modify_reg(0x5E, (1 << 2)|(3 << 0), (1 << 0));

    tftlcd_set_reg(0x56, 0);
    tftlcd_set_reg(0x57, 0 >> 8);
    tftlcd_set_reg(0x58, 0);
    tftlcd_set_reg(0x59, 0 >> 8);
    tftlcd_set_reg(0x5A, param->width);
    tftlcd_set_reg(0x5B, param->width >> 8);
    tftlcd_set_reg(0x5C, param->height);
    tftlcd_set_reg(0x5D, param->height >> 8);

    tftlcd_set_reg(0x5F, 0);
    tftlcd_set_reg(0x60, 0 >> 8);
    tftlcd_set_reg(0x61, 0);
    tftlcd_set_reg(0x62, 0 >> 8);

    /* 开启显示 */
    tftlcd_modify_reg(0x12, 0, (1 << 6));

    /* 背光PWM配置 */
    tftlcd_set_reg(0x84, (((param->timing.clock_freq_in_mhz * 2) * 1000000) / 500000) - 1);
    tftlcd_modify_reg(0x85, (3 << 6)|(3 << 2), 0);
    tftlcd_modify_reg(0x86, 0, (1 << 5));
    tftlcd_set_reg(0x8C, 0);
    tftlcd_set_reg(0x8D, 0 >> 8);
    tftlcd_set_reg(0x8E, 100);
    tftlcd_set_reg(0x8F, 100 >> 8);
    tftlcd_modify_reg(0x86, 0, (1 << 4));
}

/* 开始访问显存 */
void tftlcd_start_access_memory(void)
{
    TFTLCD_FSMC_REG = 0x04;
}

/* 配置坐标 */
void tftlcd_config_memory_access_coordinate(uint16_t x, uint16_t y)
{
    if(g_tftlcd_sta.disp_dir == TFTLCD_DISP_DIR_0)
    {
        x = x ^ y;
        y = x ^ y;
        x = x ^ y;
        y = g_tftlcd_sta.width - y - 1;
    }
    tftlcd_set_reg(0x5F, x);
    tftlcd_set_reg(0x60, x >> 8);
    tftlcd_set_reg(0x61, y);
    tftlcd_set_reg(0x62, y >> 8);
}

/* 配置窗口 */
void tftlcd_config_memory_access_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    if(g_tftlcd_sta.disp_dir == TFTLCD_DISP_DIR_0)
    {
        width  = width ^ height;
        height = width ^ height;
        width  = width ^ height;
        sx = sx ^ sy;
        sy = sx ^ sy;
        sx = sx ^ sy;
        sy = g_tftlcd_sta.width - sy - height;
    }
    tftlcd_set_reg(0x56, sx);
    tftlcd_set_reg(0x57, sx >> 8);
    tftlcd_set_reg(0x58, sy);
    tftlcd_set_reg(0x59, sy >> 8);
    tftlcd_set_reg(0x5A, width);
    tftlcd_set_reg(0x5B, width >> 8);
    tftlcd_set_reg(0x5C, height);
    tftlcd_set_reg(0x5D, height >> 8);
    tftlcd_config_memory_access_coordinate(sx, sy);
}

/* 平方函数 */
static uint32_t tftlcd_pow(uint8_t x, uint8_t y)
{
    uint8_t  loop;
    uint32_t res = 1;
    for(loop=0; loop<y; loop++) res *= x;
    return res;
}

/* LCD初始化 */
uint8_t tftlcd_init(void)
{
    uint8_t id, ret;
    tftlcd_fsmc_init();
    id  = tftlcd_get_id();
    ret = tftlcd_setup_param_by_id(id);

    if(ret != TFTLCD_EOK) return TFTLCD_ERROR;

    tftlcd_setup_lt7381_by_param(g_tftlcd_sta.param);
    tftlcd_set_disp_dir(TFTLCD_DISP_DIR_90);
    tftlcd_clear(TFTLCD_WHITE);
    tftlcd_backlight_config(100);
    tftlcd_display_on();

#if (TFTLCD_USING_TOUCH != 0)
    tftlcd_touch_init();
#endif
	
    return TFTLCD_EOK;
}

/* 获取宽度/高度 */
uint16_t tftlcd_get_lcd_width(void)  { return g_tftlcd_sta.width; }
uint16_t tftlcd_get_lcd_height(void) { return g_tftlcd_sta.height; }

/* 背光配置 */
void tftlcd_backlight_config(uint8_t percent)
{
    tftlcd_set_reg(0x8C, percent);
    tftlcd_set_reg(0x8D, percent >> 8);
    if(percent == 0)
        tftlcd_modify_reg(0x85, (3 << 2), 0);
    else
        tftlcd_modify_reg(0x85, 0, (2 << 2));
}

/* 开关显示 */
void tftlcd_display_on(void)  { tftlcd_modify_reg(0x12, 0, (1 << 6)); }
void tftlcd_display_off(void) { tftlcd_modify_reg(0x12, (1 << 6), 0); }

/* 设置扫描方向 */
uint8_t tftlcd_set_scan_dir(uint8_t scan_dir)
{
    if(g_tftlcd_sta.disp_dir == TFTLCD_DISP_DIR_0)
    {
        switch(scan_dir)
        {
            case TFTLCD_SCAN_DIR_L2R_U2D: scan_dir = TFTLCD_SCAN_DIR_D2U_L2R; break;
            case TFTLCD_SCAN_DIR_R2L_U2D: scan_dir = TFTLCD_SCAN_DIR_U2D_L2R; break;
            case TFTLCD_SCAN_DIR_U2D_R2L: scan_dir = TFTLCD_SCAN_DIR_L2R_U2D; break;
            case TFTLCD_SCAN_DIR_D2U_R2L: scan_dir = TFTLCD_SCAN_DIR_R2L_U2D; break;
            default: return TFTLCD_EINVAL;
        }
    }
    else
    {
        switch(scan_dir)
        {
            case TFTLCD_SCAN_DIR_L2R_U2D:
            case TFTLCD_SCAN_DIR_R2L_U2D:
            case TFTLCD_SCAN_DIR_U2D_L2R:
            case TFTLCD_SCAN_DIR_D2U_L2R: break;
            default: return TFTLCD_EINVAL;
        }
    }

    switch(scan_dir)
    {
        case TFTLCD_SCAN_DIR_L2R_U2D:
            tftlcd_modify_reg(0x02, (3 << 4)|(3 << 1), (0 << 4)|(0 << 1)); break;
        case TFTLCD_SCAN_DIR_R2L_U2D:
            tftlcd_modify_reg(0x02, (3 << 4)|(3 << 1), (1 << 4)|(1 << 1)); break;
        case TFTLCD_SCAN_DIR_U2D_L2R:
            tftlcd_modify_reg(0x02, (3 << 4)|(3 << 1), (2 << 4)|(2 << 1)); break;
        case TFTLCD_SCAN_DIR_D2U_L2R:
            tftlcd_modify_reg(0x02, (3 << 4)|(3 << 1), (3 << 4)|(3 << 1)); break;
        default: return TFTLCD_EINVAL;
    }
    g_tftlcd_sta.scan_dir = scan_dir;
    return TFTLCD_EOK;
}

/* 设置显示方向 */
uint8_t tftlcd_set_disp_dir(uint8_t disp_dir)
{
    switch(disp_dir)
    {
        case TFTLCD_DISP_DIR_0:
            g_tftlcd_sta.width  = g_tftlcd_sta.param->height;
            g_tftlcd_sta.height = g_tftlcd_sta.param->width;
            break;
        case TFTLCD_DISP_DIR_90:
            g_tftlcd_sta.width  = g_tftlcd_sta.param->width;
            g_tftlcd_sta.height = g_tftlcd_sta.param->height;
            break;
        default: return TFTLCD_EINVAL;
    }
    g_tftlcd_sta.disp_dir = disp_dir;
    tftlcd_set_scan_dir(TFTLCD_SCAN_DIR_L2R_U2D);
    return TFTLCD_EOK;
}

uint8_t tftlcd_get_scan_dir(void) { return g_tftlcd_sta.scan_dir; }
uint8_t tftlcd_get_disp_dir(void) { return g_tftlcd_sta.disp_dir; }

/* 区域填充 */
void tftlcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint16_t w = ex - sx + 1;
    uint16_t h = ey - sy + 1;
    uint32_t size = w * h;
    uint32_t i;

    tftlcd_config_memory_access_window(sx, sy, w, h);
    tftlcd_start_access_memory();

    for(i=0; i<size; i++)
    {
        if((i % 8) == 0)
        {
            while((tftlcd_read_stsr() & (1 << 7)) != 0);
            tftlcd_start_access_memory();
        }
        TFTLCD_FSMC_DAT = color;
    }
    tftlcd_config_memory_access_window(0,0,g_tftlcd_sta.width,g_tftlcd_sta.height);
}

/* 清屏 */
void tftlcd_clear(uint16_t color)
{
    tftlcd_fill(0,0,g_tftlcd_sta.width-1,g_tftlcd_sta.height-1,color);
}

/* 画点/读点 */
void tftlcd_draw_point(uint16_t x, uint16_t y, volatile uint16_t color)
{
    tftlcd_config_memory_access_coordinate(x,y);
    tftlcd_start_access_memory();
    TFTLCD_FSMC_DAT = color;
}

uint16_t tftlcd_read_point(uint16_t x, uint16_t y)
{
    volatile uint16_t color;
    if(x >= g_tftlcd_sta.width || y >= g_tftlcd_sta.height) return TFTLCD_EINVAL;

    tftlcd_config_memory_access_coordinate(x,y);
    tftlcd_start_access_memory();
    color = TFTLCD_FSMC_DAT;
    delay_us(2);
    color = TFTLCD_FSMC_DAT;
    return color;
}

/* 画线/矩形/圆 */
void tftlcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t dx = (x1<x2)?(x2-x1):(x1-x2);
    uint16_t dy = (y1<y2)?(y2-y1):(y1-y2);
    int16_t  sx = (x1<x2)?1:-1;
    int16_t  sy = (y1<y2)?1:-1;
    int16_t  err = dx - dy;
    int16_t  err2;

    tftlcd_draw_point(x2,y2,color);
    while((x1!=x2) || (y1!=y2))
    {
        tftlcd_draw_point(x1,y1,color);
        err2 = err << 1;
        if(err2 > -dy){ err -= dy; x1 += sx; }
        if(err2 <  dx){ err += dx; y1 += sy; }
    }
}

void tftlcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    tftlcd_draw_line(x1,y1,x2,y1,color);
    tftlcd_draw_line(x1,y2,x2,y2,color);
    tftlcd_draw_line(x1,y1,x1,y2,color);
    tftlcd_draw_line(x2,y1,x2,y2,color);
}

void tftlcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    int32_t tx = -r, ty = 0, err = 2 - 2 * r, err2;
    do
    {
        tftlcd_draw_point(x-tx,y+ty,color);
        tftlcd_draw_point(x+tx,y+ty,color);
        tftlcd_draw_point(x+tx,y-ty,color);
        tftlcd_draw_point(x-tx,y-ty,color);
        err2 = err;
        if(err2 <= ty){ ty++; err += ty*2+1; if((-tx==ty)&&(err2<=tx))err2=0; }
        if(err2 >  tx){ tx++; err += tx*2+1; }
    }while(tx <= 0);
}

void tftlcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i,imax=((uint32_t)r*707)/1000+1,sqmax=(uint32_t)r*r+r/2,xr=r;
    tftlcd_draw_line(x-r,y,x+r,y,color);
    for(i=1;i<=imax;i++)
    {
        if((i*i+xr*xr)>sqmax)
        {
            if(xr>imax)
            {
                tftlcd_draw_line(x-i+1,y+xr,x+i-1,y+xr,color);
                tftlcd_draw_line(x-i+1,y-xr,x+i-1,y-xr,color);
            }
            xr--;
        }
        tftlcd_draw_line(x-xr,y+i,x+xr,y+i,color);
        tftlcd_draw_line(x-xr,y-i,x+xr,y-i,color);
    }
}

/* 字符/字符串/数字显示 */
void tftlcd_show_char(uint16_t x, uint16_t y, char ch, uint8_t font, uint16_t color)
{
    const uint8_t *p_ch;
    uint8_t w,h,size,offset,i,j,bits;
    uint8_t wx=0,wy=0;

    offset = ch - ' ';
    switch(font)
    {
#if (TFTLCD_FONT_12 != 0)
        case TFTLCD_FONT_12:
            p_ch = tftlcd_font_1206[offset];
            w = 6; h=12; size=12; break;
#endif
#if (TFTLCD_FONT_16 != 0)
        case TFTLCD_FONT_16:
            p_ch = tftlcd_font_1608[offset];
            w = 8; h=16; size=16; break;
#endif
#if (TFTLCD_FONT_24 != 0)
        case TFTLCD_FONT_24:
            p_ch = tftlcd_font_2412[offset];
            w =12; h=24; size=36; break;
#endif
#if (TFTLCD_FONT_32 != 0)
        case TFTLCD_FONT_32:
            p_ch = tftlcd_font_3216[offset];
            w =16; h=32; size=64; break;
#endif
        default: return;
    }
    if(x+w>g_tftlcd_sta.width || y+h>g_tftlcd_sta.height) return;

    for(i=0;i<size;i++)
    {
        bits = p_ch[i];
        for(j=0;j<8;j++)
        {
            if(bits & 0x80) tftlcd_draw_point(x+wx,y+wy,color);
            wy++;
            if(wy == h){ wy=0; wx++; break; }
            bits <<= 1;
        }
    }
}

void tftlcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, uint8_t font, uint16_t color)
{
    uint8_t w,h;
    uint16_t x0=x,y0=y,xmax,ymax;

    switch(font)
    {
#if (TFTLCD_FONT_12 != 0)
        case TFTLCD_FONT_12: w=6;h=12;break;
#endif
#if (TFTLCD_FONT_16 != 0)
        case TFTLCD_FONT_16: w=8;h=16;break;
#endif
#if (TFTLCD_FONT_24 != 0)
        case TFTLCD_FONT_24: w=12;h=24;break;
#endif
#if (TFTLCD_FONT_32 != 0)
        case TFTLCD_FONT_32: w=16;h=32;break;
#endif
        default:return;
    }
    xmax = (x+width+1)>g_tftlcd_sta.width ? g_tftlcd_sta.width : (x+width+1);
    ymax = (y+height+1)>g_tftlcd_sta.height? g_tftlcd_sta.height:(y+height+1);

    while((*str>=' ')&&(*str<='~'))
    {
        if(x+w>xmax){x=x0;y+=h;}
        if(y+h>ymax){y=y0;x=x0;}
        tftlcd_show_char(x,y,*str,font,color);
        x+=w;
        str++;
    }
}

void tftlcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t mode, uint8_t font, uint16_t color)
{
    uint8_t w,i,n,first=0;
    char c;

    switch(font)
    {
#if (TFTLCD_FONT_12 != 0)
        case TFTLCD_FONT_12: w=6;break;
#endif
#if (TFTLCD_FONT_16 != 0)
        case TFTLCD_FONT_16: w=8;break;
#endif
#if (TFTLCD_FONT_24 != 0)
        case TFTLCD_FONT_24: w=12;break;
#endif
#if (TFTLCD_FONT_32 != 0)
        case TFTLCD_FONT_32: w=16;break;
#endif
        default:return;
    }
    c = (mode==TFTLCD_NUM_SHOW_NOZERO) ? ' ':'0';

    for(i=0;i<len;i++)
    {
        n = (num/tftlcd_pow(10,len-i-1))%10;
        if((first==0)&&(i<len-1))
        {
            if(n==0){ tftlcd_show_char(x+w*i,y,c,font,color); continue; }
            else first=1;
        }
        tftlcd_show_char(x+w*i,y,n+'0',font,color);
    }
}

void tftlcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t font, uint16_t color)
{
    tftlcd_show_xnum(x,y,num,len,TFTLCD_NUM_SHOW_NOZERO,font,color);
}

/* 显示图片 */
void tftlcd_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pic)
{
    uint32_t size = width * height;
    uint32_t i;

    tftlcd_config_memory_access_window(x,y,width,height);
    tftlcd_start_access_memory();

    for(i=0;i<size;i++,pic++)
    {
        if((i%8)==0)
        {
            while((tftlcd_read_stsr() & (1 <<7))!=0);
            tftlcd_start_access_memory();
        }
        TFTLCD_FSMC_DAT = *pic;
    }
    tftlcd_config_memory_access_window(0,0,g_tftlcd_sta.width,g_tftlcd_sta.height);
}


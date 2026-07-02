#include "tftlcd.h"
#include "tftlcd_font.h"
#include "tftlcd_gpio.h"
#include "delay.h"

/* ATK-MD0430模块LCD驱动器ID */
#define tftlcd_CHIP_ID1          0x5510
#define tftlcd_CHIP_ID2          0x9806

/* ATK-MD0430模块LCD扫描方向 */
#define tftlcd_SCAN_DIR_L2R_U2D (0x0000)
#define tftlcd_SCAN_DIR_L2R_D2U (0x0080)
#define tftlcd_SCAN_DIR_R2L_U2D (0x0040)
#define tftlcd_SCAN_DIR_R2L_D2U (0x00C0)
#define tftlcd_SCAN_DIR_U2D_L2R (0x0020)
#define tftlcd_SCAN_DIR_U2D_R2L (0x0060)
#define tftlcd_SCAN_DIR_D2U_L2R (0x00A0)
#define tftlcd_SCAN_DIR_D2U_R2L (0x00E0)

/* ATK-MD0430模块状态数据结构体 */
static struct
{
    uint16_t chip_id;                   
    uint16_t width;                     
    uint16_t height;                    
    tftlcd_lcd_scan_dir_t scan_dir; 
    tftlcd_lcd_disp_dir_t disp_dir; 
} g_tftlcd_sta = {0};

/**
 * @brief       ATK-MD0430模块硬件初始化（标准库版，对标你的TB6612）
 * @param       无
 * @retval      无
 */
static void tftlcd_hw_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能GPIO时钟（标准库） */
    tftlcd_BL_GPIO_CLK_ENABLE();
    
    /* 初始化BL背光引脚：推挽输出、上拉、100MHz高速 */
    GPIO_InitStructure.GPIO_Pin    = tftlcd_BL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
    GPIO_Init(tftlcd_BL_GPIO_PORT, &GPIO_InitStructure);
    
    tftlcd_BL(0);  // 默认关闭背光
}

/**
 * @brief       获取ATK-MD0430模块驱动器ID
 * @param       无
 * @retval      无
 */
static inline uint16_t tftlcd_get_chip_id(void)
{
    uint16_t chip_id;
    
    tftlcd_gpio_write_reg(0xF000, 0x0055);
    tftlcd_gpio_write_reg(0xF001, 0x00AA);
    tftlcd_gpio_write_reg(0xF002, 0x0052);
    tftlcd_gpio_write_reg(0xF003, 0x0008);
    tftlcd_gpio_write_reg(0xF004, 0x0001);
    
    tftlcd_gpio_write_cmd(0xC500);
    chip_id = tftlcd_gpio_read_dat() << 8;
    tftlcd_gpio_write_cmd(0xC501);
    chip_id |= (tftlcd_gpio_read_dat()) & 0x00FF;
    
    if (chip_id != tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0XD3);
        chip_id = tftlcd_gpio_read_dat();
        chip_id = tftlcd_gpio_read_dat();
        chip_id = tftlcd_gpio_read_dat() << 8;
        chip_id |= tftlcd_gpio_read_dat();
    }
    
    return chip_id;
}

/**
 * @brief       ATK-MD0430模块寄存器初始化
 * @param       无
 * @retval      无
 */
static void tftlcd_reg_init(void)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_reg(0xF000, 0x55);
        tftlcd_gpio_write_reg(0xF001, 0xAA);
        tftlcd_gpio_write_reg(0xF002, 0x52);
        tftlcd_gpio_write_reg(0xF003, 0x08);
        tftlcd_gpio_write_reg(0xF004, 0x01);
        tftlcd_gpio_write_reg(0xB000, 0x0D);
        tftlcd_gpio_write_reg(0xB001, 0x0D);
        tftlcd_gpio_write_reg(0xB002, 0x0D);
        tftlcd_gpio_write_reg(0xB600, 0x34);
        tftlcd_gpio_write_reg(0xB601, 0x34);
        tftlcd_gpio_write_reg(0xB602, 0x34);
        tftlcd_gpio_write_reg(0xB100, 0x0D);
        tftlcd_gpio_write_reg(0xB101, 0x0D);
        tftlcd_gpio_write_reg(0xB102, 0x0D);
        tftlcd_gpio_write_reg(0xB700, 0x34);
        tftlcd_gpio_write_reg(0xB701, 0x34);
        tftlcd_gpio_write_reg(0xB702, 0x34);
        tftlcd_gpio_write_reg(0xB200, 0x00);
        tftlcd_gpio_write_reg(0xB201, 0x00);
        tftlcd_gpio_write_reg(0xB202, 0x00);
        tftlcd_gpio_write_reg(0xB800, 0x24);
        tftlcd_gpio_write_reg(0xB801, 0x24);
        tftlcd_gpio_write_reg(0xB802, 0x24);
        tftlcd_gpio_write_reg(0xBF00, 0x01);
        tftlcd_gpio_write_reg(0xB300, 0x0F);
        tftlcd_gpio_write_reg(0xB301, 0x0F);
        tftlcd_gpio_write_reg(0xB302, 0x0F);
        tftlcd_gpio_write_reg(0xB900, 0x34);
        tftlcd_gpio_write_reg(0xB901, 0x34);
        tftlcd_gpio_write_reg(0xB902, 0x34);
        tftlcd_gpio_write_reg(0xB500, 0x08);
        tftlcd_gpio_write_reg(0xB501, 0x08);
        tftlcd_gpio_write_reg(0xB502, 0x08);
        tftlcd_gpio_write_reg(0xC200, 0x03);
        tftlcd_gpio_write_reg(0xBA00, 0x24);
        tftlcd_gpio_write_reg(0xBA01, 0x24);
        tftlcd_gpio_write_reg(0xBA02, 0x24);
        tftlcd_gpio_write_reg(0xBC00, 0x00);
        tftlcd_gpio_write_reg(0xBC01, 0x78);
        tftlcd_gpio_write_reg(0xBC02, 0x00);
        tftlcd_gpio_write_reg(0xBD00, 0x00);
        tftlcd_gpio_write_reg(0xBD01, 0x78);
        tftlcd_gpio_write_reg(0xBD02, 0x00);
        tftlcd_gpio_write_reg(0xBE00, 0x00);
        tftlcd_gpio_write_reg(0xBE01, 0x64);
        tftlcd_gpio_write_reg(0xD100, 0x00);
        tftlcd_gpio_write_reg(0xD101, 0x33);
        tftlcd_gpio_write_reg(0xD102, 0x00);
        tftlcd_gpio_write_reg(0xD103, 0x34);
        tftlcd_gpio_write_reg(0xD104, 0x00);
        tftlcd_gpio_write_reg(0xD105, 0x3A);
        tftlcd_gpio_write_reg(0xD106, 0x00);
        tftlcd_gpio_write_reg(0xD107, 0x4A);
        tftlcd_gpio_write_reg(0xD108, 0x00);
        tftlcd_gpio_write_reg(0xD109, 0x5C);
        tftlcd_gpio_write_reg(0xD10A, 0x00);
        tftlcd_gpio_write_reg(0xD10B, 0x81);
        tftlcd_gpio_write_reg(0xD10C, 0x00);
        tftlcd_gpio_write_reg(0xD10D, 0xA6);
        tftlcd_gpio_write_reg(0xD10E, 0x00);
        tftlcd_gpio_write_reg(0xD10F, 0xE5);
        tftlcd_gpio_write_reg(0xD110, 0x01);
        tftlcd_gpio_write_reg(0xD111, 0x13);
        tftlcd_gpio_write_reg(0xD112, 0x01);
        tftlcd_gpio_write_reg(0xD113, 0x54);
        tftlcd_gpio_write_reg(0xD114, 0x01);
        tftlcd_gpio_write_reg(0xD115, 0x82);
        tftlcd_gpio_write_reg(0xD116, 0x01);
        tftlcd_gpio_write_reg(0xD117, 0xCA);
        tftlcd_gpio_write_reg(0xD118, 0x02);
        tftlcd_gpio_write_reg(0xD119, 0x00);
        tftlcd_gpio_write_reg(0xD11A, 0x02);
        tftlcd_gpio_write_reg(0xD11B, 0x01);
        tftlcd_gpio_write_reg(0xD11C, 0x02);
        tftlcd_gpio_write_reg(0xD11D, 0x34);
        tftlcd_gpio_write_reg(0xD11E, 0x02);
        tftlcd_gpio_write_reg(0xD11F, 0x67);
        tftlcd_gpio_write_reg(0xD120, 0x02);
        tftlcd_gpio_write_reg(0xD121, 0x84);
        tftlcd_gpio_write_reg(0xD122, 0x02);
        tftlcd_gpio_write_reg(0xD123, 0xA4);
        tftlcd_gpio_write_reg(0xD124, 0x02);
        tftlcd_gpio_write_reg(0xD125, 0xB7);
        tftlcd_gpio_write_reg(0xD126, 0x02);
        tftlcd_gpio_write_reg(0xD127, 0xCF);
        tftlcd_gpio_write_reg(0xD128, 0x02);
        tftlcd_gpio_write_reg(0xD129, 0xDE);
        tftlcd_gpio_write_reg(0xD12A, 0x02);
        tftlcd_gpio_write_reg(0xD12B, 0xF2);
        tftlcd_gpio_write_reg(0xD12C, 0x02);
        tftlcd_gpio_write_reg(0xD12D, 0xFE);
        tftlcd_gpio_write_reg(0xD12E, 0x03);
        tftlcd_gpio_write_reg(0xD12F, 0x10);
        tftlcd_gpio_write_reg(0xD130, 0x03);
        tftlcd_gpio_write_reg(0xD131, 0x33);
        tftlcd_gpio_write_reg(0xD132, 0x03);
        tftlcd_gpio_write_reg(0xD133, 0x6D);
        tftlcd_gpio_write_reg(0xD200, 0x00);
        tftlcd_gpio_write_reg(0xD201, 0x33);
        tftlcd_gpio_write_reg(0xD202, 0x00);
        tftlcd_gpio_write_reg(0xD203, 0x34);
        tftlcd_gpio_write_reg(0xD204, 0x00);
        tftlcd_gpio_write_reg(0xD205, 0x3A);
        tftlcd_gpio_write_reg(0xD206, 0x00);
        tftlcd_gpio_write_reg(0xD207, 0x4A);
        tftlcd_gpio_write_reg(0xD208, 0x00);
        tftlcd_gpio_write_reg(0xD209, 0x5C);
        tftlcd_gpio_write_reg(0xD20A, 0x00);
        tftlcd_gpio_write_reg(0xD20B, 0x81);
        tftlcd_gpio_write_reg(0xD20C, 0x00);
        tftlcd_gpio_write_reg(0xD20D, 0xA6);
        tftlcd_gpio_write_reg(0xD20E, 0x00);
        tftlcd_gpio_write_reg(0xD20F, 0xE5);
        tftlcd_gpio_write_reg(0xD210, 0x01);
        tftlcd_gpio_write_reg(0xD211, 0x13);
        tftlcd_gpio_write_reg(0xD212, 0x01);
        tftlcd_gpio_write_reg(0xD213, 0x54);
        tftlcd_gpio_write_reg(0xD214, 0x01);
        tftlcd_gpio_write_reg(0xD215, 0x82);
        tftlcd_gpio_write_reg(0xD216, 0x01);
        tftlcd_gpio_write_reg(0xD217, 0xCA);
        tftlcd_gpio_write_reg(0xD218, 0x02);
        tftlcd_gpio_write_reg(0xD219, 0x00);
        tftlcd_gpio_write_reg(0xD21A, 0x02);
        tftlcd_gpio_write_reg(0xD21B, 0x01);
        tftlcd_gpio_write_reg(0xD21C, 0x02);
        tftlcd_gpio_write_reg(0xD21D, 0x34);
        tftlcd_gpio_write_reg(0xD21E, 0x02);
        tftlcd_gpio_write_reg(0xD21F, 0x67);
        tftlcd_gpio_write_reg(0xD220, 0x02);
        tftlcd_gpio_write_reg(0xD221, 0x84);
        tftlcd_gpio_write_reg(0xD222, 0x02);
        tftlcd_gpio_write_reg(0xD223, 0xA4);
        tftlcd_gpio_write_reg(0xD224, 0x02);
        tftlcd_gpio_write_reg(0xD225, 0xB7);
        tftlcd_gpio_write_reg(0xD226, 0x02);
        tftlcd_gpio_write_reg(0xD227, 0xCF);
        tftlcd_gpio_write_reg(0xD228, 0x02);
        tftlcd_gpio_write_reg(0xD229, 0xDE);
        tftlcd_gpio_write_reg(0xD22A, 0x02);
        tftlcd_gpio_write_reg(0xD22B, 0xF2);
        tftlcd_gpio_write_reg(0xD22C, 0x02);
        tftlcd_gpio_write_reg(0xD22D, 0xFE);
        tftlcd_gpio_write_reg(0xD22E, 0x03);
        tftlcd_gpio_write_reg(0xD22F, 0x10);
        tftlcd_gpio_write_reg(0xD230, 0x03);
        tftlcd_gpio_write_reg(0xD231, 0x33);
        tftlcd_gpio_write_reg(0xD232, 0x03);
        tftlcd_gpio_write_reg(0xD233, 0x6D);
        tftlcd_gpio_write_reg(0xD300, 0x00);
        tftlcd_gpio_write_reg(0xD301, 0x33);
        tftlcd_gpio_write_reg(0xD302, 0x00);
        tftlcd_gpio_write_reg(0xD303, 0x34);
        tftlcd_gpio_write_reg(0xD304, 0x00);
        tftlcd_gpio_write_reg(0xD305, 0x3A);
        tftlcd_gpio_write_reg(0xD306, 0x00);
        tftlcd_gpio_write_reg(0xD307, 0x4A);
        tftlcd_gpio_write_reg(0xD308, 0x00);
        tftlcd_gpio_write_reg(0xD309, 0x5C);
        tftlcd_gpio_write_reg(0xD30A, 0x00);
        tftlcd_gpio_write_reg(0xD30B, 0x81);
        tftlcd_gpio_write_reg(0xD30C, 0x00);
        tftlcd_gpio_write_reg(0xD30D, 0xA6);
        tftlcd_gpio_write_reg(0xD30E, 0x00);
        tftlcd_gpio_write_reg(0xD30F, 0xE5);
        tftlcd_gpio_write_reg(0xD310, 0x01);
        tftlcd_gpio_write_reg(0xD311, 0x13);
        tftlcd_gpio_write_reg(0xD312, 0x01);
        tftlcd_gpio_write_reg(0xD313, 0x54);
        tftlcd_gpio_write_reg(0xD314, 0x01);
        tftlcd_gpio_write_reg(0xD315, 0x82);
        tftlcd_gpio_write_reg(0xD316, 0x01);
        tftlcd_gpio_write_reg(0xD317, 0xCA);
        tftlcd_gpio_write_reg(0xD318, 0x02);
        tftlcd_gpio_write_reg(0xD319, 0x00);
        tftlcd_gpio_write_reg(0xD31A, 0x02);
        tftlcd_gpio_write_reg(0xD31B, 0x01);
        tftlcd_gpio_write_reg(0xD31C, 0x02);
        tftlcd_gpio_write_reg(0xD31D, 0x34);
        tftlcd_gpio_write_reg(0xD31E, 0x02);
        tftlcd_gpio_write_reg(0xD31F, 0x67);
        tftlcd_gpio_write_reg(0xD320, 0x02);
        tftlcd_gpio_write_reg(0xD321, 0x84);
        tftlcd_gpio_write_reg(0xD322, 0x02);
        tftlcd_gpio_write_reg(0xD323, 0xA4);
        tftlcd_gpio_write_reg(0xD324, 0x02);
        tftlcd_gpio_write_reg(0xD325, 0xB7);
        tftlcd_gpio_write_reg(0xD326, 0x02);
        tftlcd_gpio_write_reg(0xD327, 0xCF);
        tftlcd_gpio_write_reg(0xD328, 0x02);
        tftlcd_gpio_write_reg(0xD329, 0xDE);
        tftlcd_gpio_write_reg(0xD32A, 0x02);
        tftlcd_gpio_write_reg(0xD32B, 0xF2);
        tftlcd_gpio_write_reg(0xD32C, 0x02);
        tftlcd_gpio_write_reg(0xD32D, 0xFE);
        tftlcd_gpio_write_reg(0xD32E, 0x03);
        tftlcd_gpio_write_reg(0xD32F, 0x10);
        tftlcd_gpio_write_reg(0xD330, 0x03);
        tftlcd_gpio_write_reg(0xD331, 0x33);
        tftlcd_gpio_write_reg(0xD332, 0x03);
        tftlcd_gpio_write_reg(0xD333, 0x6D);
        tftlcd_gpio_write_reg(0xD400, 0x00);
        tftlcd_gpio_write_reg(0xD401, 0x33);
        tftlcd_gpio_write_reg(0xD402, 0x00);
        tftlcd_gpio_write_reg(0xD403, 0x34);
        tftlcd_gpio_write_reg(0xD404, 0x00);
        tftlcd_gpio_write_reg(0xD405, 0x3A);
        tftlcd_gpio_write_reg(0xD406, 0x00);
        tftlcd_gpio_write_reg(0xD407, 0x4A);
        tftlcd_gpio_write_reg(0xD408, 0x00);
        tftlcd_gpio_write_reg(0xD409, 0x5C);
        tftlcd_gpio_write_reg(0xD40A, 0x00);
        tftlcd_gpio_write_reg(0xD40B, 0x81);
        tftlcd_gpio_write_reg(0xD40C, 0x00);
        tftlcd_gpio_write_reg(0xD40D, 0xA6);
        tftlcd_gpio_write_reg(0xD40E, 0x00);
        tftlcd_gpio_write_reg(0xD40F, 0xE5);
        tftlcd_gpio_write_reg(0xD410, 0x01);
        tftlcd_gpio_write_reg(0xD411, 0x13);
        tftlcd_gpio_write_reg(0xD412, 0x01);
        tftlcd_gpio_write_reg(0xD413, 0x54);
        tftlcd_gpio_write_reg(0xD414, 0x01);
        tftlcd_gpio_write_reg(0xD415, 0x82);
        tftlcd_gpio_write_reg(0xD416, 0x01);
        tftlcd_gpio_write_reg(0xD417, 0xCA);
        tftlcd_gpio_write_reg(0xD418, 0x02);
        tftlcd_gpio_write_reg(0xD419, 0x00);
        tftlcd_gpio_write_reg(0xD41A, 0x02);
        tftlcd_gpio_write_reg(0xD41B, 0x01);
        tftlcd_gpio_write_reg(0xD41C, 0x02);
        tftlcd_gpio_write_reg(0xD41D, 0x34);
        tftlcd_gpio_write_reg(0xD41E, 0x02);
        tftlcd_gpio_write_reg(0xD41F, 0x67);
        tftlcd_gpio_write_reg(0xD420, 0x02);
        tftlcd_gpio_write_reg(0xD421, 0x84);
        tftlcd_gpio_write_reg(0xD422, 0x02);
        tftlcd_gpio_write_reg(0xD423, 0xA4);
        tftlcd_gpio_write_reg(0xD424, 0x02);
        tftlcd_gpio_write_reg(0xD425, 0xB7);
        tftlcd_gpio_write_reg(0xD426, 0x02);
        tftlcd_gpio_write_reg(0xD427, 0xCF);
        tftlcd_gpio_write_reg(0xD428, 0x02);
        tftlcd_gpio_write_reg(0xD429, 0xDE);
        tftlcd_gpio_write_reg(0xD42A, 0x02);
        tftlcd_gpio_write_reg(0xD42B, 0xF2);
        tftlcd_gpio_write_reg(0xD42C, 0x02);
        tftlcd_gpio_write_reg(0xD42D, 0xFE);
        tftlcd_gpio_write_reg(0xD42E, 0x03);
        tftlcd_gpio_write_reg(0xD42F, 0x10);
        tftlcd_gpio_write_reg(0xD430, 0x03);
        tftlcd_gpio_write_reg(0xD431, 0x33);
        tftlcd_gpio_write_reg(0xD432, 0x03);
        tftlcd_gpio_write_reg(0xD433, 0x6D);
        tftlcd_gpio_write_reg(0xD500, 0x00);
        tftlcd_gpio_write_reg(0xD501, 0x33);
        tftlcd_gpio_write_reg(0xD502, 0x00);
        tftlcd_gpio_write_reg(0xD503, 0x34);
        tftlcd_gpio_write_reg(0xD504, 0x00);
        tftlcd_gpio_write_reg(0xD505, 0x3A);
        tftlcd_gpio_write_reg(0xD506, 0x00);
        tftlcd_gpio_write_reg(0xD507, 0x4A);
        tftlcd_gpio_write_reg(0xD508, 0x00);
        tftlcd_gpio_write_reg(0xD509, 0x5C);
        tftlcd_gpio_write_reg(0xD50A, 0x00);
        tftlcd_gpio_write_reg(0xD50B, 0x81);
        tftlcd_gpio_write_reg(0xD50C, 0x00);
        tftlcd_gpio_write_reg(0xD50D, 0xA6);
        tftlcd_gpio_write_reg(0xD50E, 0x00);
        tftlcd_gpio_write_reg(0xD50F, 0xE5);
        tftlcd_gpio_write_reg(0xD510, 0x01);
        tftlcd_gpio_write_reg(0xD511, 0x13);
        tftlcd_gpio_write_reg(0xD512, 0x01);
        tftlcd_gpio_write_reg(0xD513, 0x54);
        tftlcd_gpio_write_reg(0xD514, 0x01);
        tftlcd_gpio_write_reg(0xD515, 0x82);
        tftlcd_gpio_write_reg(0xD516, 0x01);
        tftlcd_gpio_write_reg(0xD517, 0xCA);
        tftlcd_gpio_write_reg(0xD518, 0x02);
        tftlcd_gpio_write_reg(0xD519, 0x00);
        tftlcd_gpio_write_reg(0xD51A, 0x02);
        tftlcd_gpio_write_reg(0xD51B, 0x01);
        tftlcd_gpio_write_reg(0xD51C, 0x02);
        tftlcd_gpio_write_reg(0xD51D, 0x34);
        tftlcd_gpio_write_reg(0xD51E, 0x02);
        tftlcd_gpio_write_reg(0xD51F, 0x67);
        tftlcd_gpio_write_reg(0xD520, 0x02);
        tftlcd_gpio_write_reg(0xD521, 0x84);
        tftlcd_gpio_write_reg(0xD522, 0x02);
        tftlcd_gpio_write_reg(0xD523, 0xA4);
        tftlcd_gpio_write_reg(0xD524, 0x02);
        tftlcd_gpio_write_reg(0xD525, 0xB7);
        tftlcd_gpio_write_reg(0xD526, 0x02);
        tftlcd_gpio_write_reg(0xD527, 0xCF);
        tftlcd_gpio_write_reg(0xD528, 0x02);
        tftlcd_gpio_write_reg(0xD529, 0xDE);
        tftlcd_gpio_write_reg(0xD52A, 0x02);
        tftlcd_gpio_write_reg(0xD52B, 0xF2);
        tftlcd_gpio_write_reg(0xD52C, 0x02);
        tftlcd_gpio_write_reg(0xD52D, 0xFE);
        tftlcd_gpio_write_reg(0xD52E, 0x03);
        tftlcd_gpio_write_reg(0xD52F, 0x10);
        tftlcd_gpio_write_reg(0xD530, 0x03);
        tftlcd_gpio_write_reg(0xD531, 0x33);
        tftlcd_gpio_write_reg(0xD532, 0x03);
        tftlcd_gpio_write_reg(0xD533, 0x6D);
        tftlcd_gpio_write_reg(0xD600, 0x00);
        tftlcd_gpio_write_reg(0xD601, 0x33);
        tftlcd_gpio_write_reg(0xD602, 0x00);
        tftlcd_gpio_write_reg(0xD603, 0x34);
        tftlcd_gpio_write_reg(0xD604, 0x00);
        tftlcd_gpio_write_reg(0xD605, 0x3A);
        tftlcd_gpio_write_reg(0xD606, 0x00);
        tftlcd_gpio_write_reg(0xD607, 0x4A);
        tftlcd_gpio_write_reg(0xD608, 0x00);
        tftlcd_gpio_write_reg(0xD609, 0x5C);
        tftlcd_gpio_write_reg(0xD60A, 0x00);
        tftlcd_gpio_write_reg(0xD60B, 0x81);
        tftlcd_gpio_write_reg(0xD60C, 0x00);
        tftlcd_gpio_write_reg(0xD60D, 0xA6);
        tftlcd_gpio_write_reg(0xD60E, 0x00);
        tftlcd_gpio_write_reg(0xD60F, 0xE5);
        tftlcd_gpio_write_reg(0xD610, 0x01);
        tftlcd_gpio_write_reg(0xD611, 0x13);
        tftlcd_gpio_write_reg(0xD612, 0x01);
        tftlcd_gpio_write_reg(0xD613, 0x54);
        tftlcd_gpio_write_reg(0xD614, 0x01);
        tftlcd_gpio_write_reg(0xD615, 0x82);
        tftlcd_gpio_write_reg(0xD616, 0x01);
        tftlcd_gpio_write_reg(0xD617, 0xCA);
        tftlcd_gpio_write_reg(0xD618, 0x02);
        tftlcd_gpio_write_reg(0xD619, 0x00);
        tftlcd_gpio_write_reg(0xD61A, 0x02);
        tftlcd_gpio_write_reg(0xD61B, 0x01);
        tftlcd_gpio_write_reg(0xD61C, 0x02);
        tftlcd_gpio_write_reg(0xD61D, 0x34);
        tftlcd_gpio_write_reg(0xD61E, 0x02);
        tftlcd_gpio_write_reg(0xD61F, 0x67);
        tftlcd_gpio_write_reg(0xD620, 0x02);
        tftlcd_gpio_write_reg(0xD621, 0x84);
        tftlcd_gpio_write_reg(0xD622, 0x02);
        tftlcd_gpio_write_reg(0xD623, 0xA4);
        tftlcd_gpio_write_reg(0xD624, 0x02);
        tftlcd_gpio_write_reg(0xD625, 0xB7);
        tftlcd_gpio_write_reg(0xD626, 0x02);
        tftlcd_gpio_write_reg(0xD627, 0xCF);
        tftlcd_gpio_write_reg(0xD628, 0x02);
        tftlcd_gpio_write_reg(0xD629, 0xDE);
        tftlcd_gpio_write_reg(0xD62A, 0x02);
        tftlcd_gpio_write_reg(0xD62B, 0xF2);
        tftlcd_gpio_write_reg(0xD62C, 0x02);
        tftlcd_gpio_write_reg(0xD62D, 0xFE);
        tftlcd_gpio_write_reg(0xD62E, 0x03);
        tftlcd_gpio_write_reg(0xD62F, 0x10);
        tftlcd_gpio_write_reg(0xD630, 0x03);
        tftlcd_gpio_write_reg(0xD631, 0x33);
        tftlcd_gpio_write_reg(0xD632, 0x03);
        tftlcd_gpio_write_reg(0xD633, 0x6D);
        tftlcd_gpio_write_reg(0xF000, 0x55);
        tftlcd_gpio_write_reg(0xF001, 0xAA);
        tftlcd_gpio_write_reg(0xF002, 0x52);
        tftlcd_gpio_write_reg(0xF003, 0x08);
        tftlcd_gpio_write_reg(0xF004, 0x00);
        tftlcd_gpio_write_reg(0xB100, 0xCC);
        tftlcd_gpio_write_reg(0xB101, 0x00);
        tftlcd_gpio_write_reg(0xB600, 0x05);
        tftlcd_gpio_write_reg(0xB700, 0x70);
        tftlcd_gpio_write_reg(0xB701, 0x70);
        tftlcd_gpio_write_reg(0xB800, 0x01);
        tftlcd_gpio_write_reg(0xB801, 0x03);
        tftlcd_gpio_write_reg(0xB802, 0x03);
        tftlcd_gpio_write_reg(0xB803, 0x03);
        tftlcd_gpio_write_reg(0xBC00, 0x02);
        tftlcd_gpio_write_reg(0xBC01, 0x00);
        tftlcd_gpio_write_reg(0xBC02, 0x00);
        tftlcd_gpio_write_reg(0xC900, 0xD0);
        tftlcd_gpio_write_reg(0xC901, 0x02);
        tftlcd_gpio_write_reg(0xC902, 0x50);
        tftlcd_gpio_write_reg(0xC903, 0x50);
        tftlcd_gpio_write_reg(0xC904, 0x50);
        tftlcd_gpio_write_reg(0x3500, 0x00);
        tftlcd_gpio_write_reg(0x3A00, 0x55);
        tftlcd_gpio_write_cmd(0x1100);
        delay_us(120);
        tftlcd_gpio_write_cmd(0x2900);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0xFF); 
        tftlcd_gpio_write_dat(0xFF);
        tftlcd_gpio_write_dat(0x98);
        tftlcd_gpio_write_dat(0x06);
        tftlcd_gpio_write_cmd(0xBC);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x0F);
        tftlcd_gpio_write_dat(0x61);
        tftlcd_gpio_write_dat(0xFF);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x0B);
        tftlcd_gpio_write_dat(0x10);
        tftlcd_gpio_write_dat(0x37);
        tftlcd_gpio_write_dat(0x63);
        tftlcd_gpio_write_dat(0xFF);
        tftlcd_gpio_write_dat(0xFF);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0xFF);
        tftlcd_gpio_write_dat(0x52);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x40);
        tftlcd_gpio_write_cmd(0xBD);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x23);
        tftlcd_gpio_write_dat(0x45);
        tftlcd_gpio_write_dat(0x67);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0x23);
        tftlcd_gpio_write_dat(0x45);
        tftlcd_gpio_write_dat(0x67);
        tftlcd_gpio_write_cmd(0xBE);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0xAB);
        tftlcd_gpio_write_dat(0x60);
        tftlcd_gpio_write_dat(0x22);
        tftlcd_gpio_write_dat(0x22);
        tftlcd_gpio_write_dat(0x22);
        tftlcd_gpio_write_dat(0x22);
        tftlcd_gpio_write_dat(0x22);
        tftlcd_gpio_write_cmd(0xC7);
        tftlcd_gpio_write_dat(0x36);
        tftlcd_gpio_write_cmd(0xED);
        tftlcd_gpio_write_dat(0x7F);
        tftlcd_gpio_write_dat(0x0F);
        tftlcd_gpio_write_cmd(0XC0);
        tftlcd_gpio_write_dat(0x0F);
        tftlcd_gpio_write_dat(0x0B);
        tftlcd_gpio_write_dat(0x0A);
        tftlcd_gpio_write_cmd(0XFC);
        tftlcd_gpio_write_dat(0x08);
        tftlcd_gpio_write_cmd(0XDF); 
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x20);
        tftlcd_gpio_write_cmd(0XF3);
        tftlcd_gpio_write_dat(0x74);
        tftlcd_gpio_write_cmd(0xB4);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_cmd(0xF7);
        tftlcd_gpio_write_dat(0x82);
        tftlcd_gpio_write_cmd(0xB1);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x13);
        tftlcd_gpio_write_dat(0x13); 
        tftlcd_gpio_write_cmd(0XF2);
        tftlcd_gpio_write_dat(0x80);
        tftlcd_gpio_write_dat(0x04);
        tftlcd_gpio_write_dat(0x40);
        tftlcd_gpio_write_dat(0x28);
        tftlcd_gpio_write_cmd(0xC1);
        tftlcd_gpio_write_dat(0x17);
        tftlcd_gpio_write_dat(0x88);
        tftlcd_gpio_write_dat(0x88);
        tftlcd_gpio_write_dat(0x20);
        tftlcd_gpio_write_cmd(0xE0);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x0A);
        tftlcd_gpio_write_dat(0x12);
        tftlcd_gpio_write_dat(0x10);
        tftlcd_gpio_write_dat(0x0E);
        tftlcd_gpio_write_dat(0x20);
        tftlcd_gpio_write_dat(0xCC);
        tftlcd_gpio_write_dat(0x07);
        tftlcd_gpio_write_dat(0x06);
        tftlcd_gpio_write_dat(0x0B);
        tftlcd_gpio_write_dat(0x0E);
        tftlcd_gpio_write_dat(0x0F);
        tftlcd_gpio_write_dat(0x0D);
        tftlcd_gpio_write_dat(0x15);
        tftlcd_gpio_write_dat(0x10);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_cmd(0xE1);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x0B);
        tftlcd_gpio_write_dat(0x13);
        tftlcd_gpio_write_dat(0x0D);
        tftlcd_gpio_write_dat(0x0E);
        tftlcd_gpio_write_dat(0x1B);
        tftlcd_gpio_write_dat(0x71);
        tftlcd_gpio_write_dat(0x06);
        tftlcd_gpio_write_dat(0x06);
        tftlcd_gpio_write_dat(0x0A);
        tftlcd_gpio_write_dat(0x0F);
        tftlcd_gpio_write_dat(0x0E);
        tftlcd_gpio_write_dat(0x0F);
        tftlcd_gpio_write_dat(0x15);
        tftlcd_gpio_write_dat(0x0C);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_cmd(0x2a);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x01);
        tftlcd_gpio_write_dat(0xdf);
        tftlcd_gpio_write_cmd(0x2b);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_dat(0x03);
        tftlcd_gpio_write_dat(0x1f);
        tftlcd_gpio_write_cmd(0x3A);
        tftlcd_gpio_write_dat(0x55);
        tftlcd_gpio_write_cmd(0x36);
        tftlcd_gpio_write_dat(0x00);
        tftlcd_gpio_write_cmd(0x11);
        delay_ms(120);   
        tftlcd_gpio_write_cmd(0x29);  
        delay_ms(20);  
        tftlcd_gpio_write_cmd(0x2C);
    }
}

/**
 * @brief       设置ATK-MD0430模块列地址
 * @param       无
 * @retval      无
 */
static void tftlcd_set_column_address(uint16_t sc, uint16_t ec)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0x2A00);
        tftlcd_gpio_write_dat((uint8_t)(sc >> 8) & 0xFF);
        tftlcd_gpio_write_cmd(0x2A01);
        tftlcd_gpio_write_dat((uint8_t)sc & 0xFF);
        tftlcd_gpio_write_cmd(0x2A02);
        tftlcd_gpio_write_dat((uint8_t)(ec >> 8) & 0xFF);
        tftlcd_gpio_write_cmd(0x2A03);
        tftlcd_gpio_write_dat((uint8_t)ec & 0xFF);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0x2A);
        tftlcd_gpio_write_dat((uint8_t)(sc >> 8) & 0xFF);
        tftlcd_gpio_write_dat((uint8_t)sc & 0xFF);
        tftlcd_gpio_write_dat((uint8_t)(ec >> 8) & 0xFF);
        tftlcd_gpio_write_dat((uint8_t)ec & 0xFF);
    }
}

/**
 * @brief       设置ATK-MD0430模块页地址
 * @param       无
 * @retval      无
 */
static void tftlcd_set_page_address(uint16_t sp, uint16_t ep)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0x2B00);
        tftlcd_gpio_write_dat((uint8_t)(sp >> 8) & 0xFF);
        tftlcd_gpio_write_cmd(0x2B01);
        tftlcd_gpio_write_dat((uint8_t)sp & 0xFF);
        tftlcd_gpio_write_cmd(0x2B02);
        tftlcd_gpio_write_dat((uint8_t)(ep >> 8) & 0xFF);
        tftlcd_gpio_write_cmd(0x2B03);
        tftlcd_gpio_write_dat((uint8_t)ep & 0xFF);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0x2B);
        tftlcd_gpio_write_dat((uint8_t)(sp >> 8) & 0xFF);
        tftlcd_gpio_write_dat((uint8_t)sp & 0xFF);
        tftlcd_gpio_write_dat((uint8_t)(ep >> 8) & 0xFF);
        tftlcd_gpio_write_dat((uint8_t)ep & 0xFF);
    }
}

/**
 * @brief       开始写ATK-MD0430模块显存
 * @param       无
 * @retval      无
 */
static void tftlcd_start_write_memory(void)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0x2C00);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0x2C);
    }
}

/**
 * @brief       开始读ATK-MD0430模块显存
 * @param       无
 * @retval      无
 */
static void tftlcd_start_read_memory(void)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0x2E00);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0x2E);
    }
}

/**
 * @brief       平方函数，x^y
 * @param       x: 底数
 *              y: 指数
 * @retval      x^y
 */
static uint32_t tftlcd_pow(uint8_t x, uint8_t y)
{
    uint8_t loop;
    uint32_t res = 1;
    
    for (loop=0; loop<y; loop++)
    {
        res *= x;
    }
    
    return res;
}

/**
 * @brief       ATK-MD0430模块初始化
 * @param       无
 * @retval      tftlcd_EOK  : tftlcd模块初始化成功
 *              tftlcd_ERROR: tftlcd模块初始化失败
 */
uint8_t tftlcd_init(void)
{
    uint16_t chip_id;
#if (tftlcd_USING_TOUCH != 0)
    uint8_t ret;
#endif
    
    tftlcd_hw_init();               
    tftlcd_gpio_init();             
    chip_id = tftlcd_get_chip_id(); 
    if ((chip_id != tftlcd_CHIP_ID1) && (chip_id != tftlcd_CHIP_ID2))
    {
        return tftlcd_ERROR;
    }
    else
    {
        g_tftlcd_sta.chip_id = chip_id;
        g_tftlcd_sta.width = tftlcd_LCD_WIDTH;
        g_tftlcd_sta.height = tftlcd_LCD_HEIGHT;
    }
    tftlcd_reg_init();
    tftlcd_set_disp_dir(tftlcd_LCD_DISP_DIR_0);
    tftlcd_clear(tftlcd_WHITE);
    tftlcd_display_on();
    tftlcd_backlight_on();
#if (tftlcd_USING_TOUCH != 0)
    ret = tftlcd_touch_init();
    if (ret != tftlcd_TOUCH_EOK)
    {
        return tftlcd_ERROR;
    }
#endif
    
    return tftlcd_EOK;
}

/**
 * @brief       获取ATK-MD0430模块LCD宽度
 * @param       无
 * @retval      ATK-MD0430模块LCD宽度
 */
uint16_t tftlcd_get_lcd_width(void)
{
    return g_tftlcd_sta.width;
}

/**
 * @brief       获取ATK-MD0430模块LCD高度
 * @param       无
 * @retval      ATK-MD0430模块LCD高度
 */
uint16_t tftlcd_get_lcd_height(void)
{
    return g_tftlcd_sta.height;
}

/**
 * @brief       开启ATK-MD0430模块LCD背光
 * @param       无
 * @retval      无
 */
void tftlcd_backlight_on(void)
{
    tftlcd_BL(1);
}

/**
 * @brief       关闭ATK-MD0430模块LCD背光
 * @param       无
 * @retval      无
 */
void tftlcd_backlight_off(void)
{
    tftlcd_BL(0);
}

/**
 * @brief       开启ATK-MD0430模块LCD显示
 * @param       无
 * @retval      无
 */
void tftlcd_display_on(void)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0x2900);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0x29);
    }
}

/**
 * @brief       关闭ATK-MD0430模块LCD显示
 * @param       无
 * @retval      无
 */
void tftlcd_display_off(void)
{
    if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
    {
        tftlcd_gpio_write_cmd(0x2800);
    }
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
        tftlcd_gpio_write_cmd(0x28);
    }
}

/**
 * @brief       设置ATK-MD0430模块LCD扫描方向
 * @param       scan_dir: 扫描方向
 * @retval      状态码
 */
uint8_t tftlcd_set_scan_dir(tftlcd_lcd_scan_dir_t scan_dir)
{
    uint16_t regval = 0;
    uint16_t dirreg = 0;
	
    switch (g_tftlcd_sta.disp_dir)
    {
        case tftlcd_LCD_DISP_DIR_0:
        {
            regval = (uint16_t)scan_dir;
            break;
        }
        case tftlcd_LCD_DISP_DIR_90:
        {
            switch (scan_dir)
            {
                case tftlcd_LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = tftlcd_LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = tftlcd_LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = tftlcd_LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = tftlcd_LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = tftlcd_LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = tftlcd_LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = tftlcd_LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = tftlcd_LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                default:
                {
                    return tftlcd_EINVAL;
                }
            }
            break;
        }
        case tftlcd_LCD_DISP_DIR_180:
        {
            switch (scan_dir)
            {
                case tftlcd_LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = tftlcd_LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = tftlcd_LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = tftlcd_LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = tftlcd_LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = tftlcd_LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = tftlcd_LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = tftlcd_LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = tftlcd_LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                default:
                {
                    return tftlcd_EINVAL;
                }
            }
            break;
        }
        case tftlcd_LCD_DISP_DIR_270:
        {
            switch (scan_dir)
            {
                case tftlcd_LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = tftlcd_LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = tftlcd_LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = tftlcd_LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = tftlcd_LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = tftlcd_LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = tftlcd_LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = tftlcd_LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                case tftlcd_LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = tftlcd_LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                default:
                {
                    return tftlcd_EINVAL;
                }
            }
            break;
        }
        default:
        {
            return tftlcd_ERROR;
        }
    }
    
    g_tftlcd_sta.scan_dir = (tftlcd_lcd_scan_dir_t)regval;
	
	if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID1)
	{
		dirreg = 0X3600;
	}
    else if (g_tftlcd_sta.chip_id == tftlcd_CHIP_ID2)
    {
		dirreg = 0X36; 
	}
	
    tftlcd_gpio_write_reg(dirreg, regval);
    tftlcd_set_column_address(0, g_tftlcd_sta.width - 1);
    tftlcd_set_page_address(0, g_tftlcd_sta.height - 1);
    
    return tftlcd_EOK;
}

/**
 * @brief       设置ATK-MD0430模块LCD显示方向
 * @param       disp_dir: 显示方向
 * @retval      状态码
 */
uint8_t tftlcd_set_disp_dir(tftlcd_lcd_disp_dir_t disp_dir)
{
    switch (disp_dir)
    {
        case tftlcd_LCD_DISP_DIR_0:
        {
            g_tftlcd_sta.width = tftlcd_LCD_WIDTH;
            g_tftlcd_sta.height = tftlcd_LCD_HEIGHT;
            break;
        }
        case tftlcd_LCD_DISP_DIR_90:
        {
            g_tftlcd_sta.width = tftlcd_LCD_HEIGHT;
            g_tftlcd_sta.height = tftlcd_LCD_WIDTH;
            break;
        }
        case tftlcd_LCD_DISP_DIR_180:
        {
            g_tftlcd_sta.width = tftlcd_LCD_WIDTH;
            g_tftlcd_sta.height = tftlcd_LCD_HEIGHT;
            break;
        }
        case tftlcd_LCD_DISP_DIR_270:
        {
            g_tftlcd_sta.width = tftlcd_LCD_HEIGHT;
            g_tftlcd_sta.height = tftlcd_LCD_WIDTH;
            break;
        }
        default:
        {
            return tftlcd_EINVAL;
        }
    }
    
    g_tftlcd_sta.disp_dir = disp_dir;
    tftlcd_set_scan_dir(tftlcd_LCD_SCAN_DIR_L2R_U2D);
    
    return tftlcd_EOK;
}

/**
 * @brief       获取ATK-MD0430模块LCD扫描方向
 * @param       无
 * @retval      扫描方向
 */
tftlcd_lcd_scan_dir_t tftlcd_get_scan_dir(void)
{
    return g_tftlcd_sta.scan_dir;
}

/**
 * @brief       获取ATK-MD0430模块LCD显示方向
 * @param       无
 * @retval      显示方向
 */
tftlcd_lcd_disp_dir_t tftlcd_get_disp_dir(void)
{
    return g_tftlcd_sta.disp_dir;
}

/**
 * @brief       ATK-MD0430模块LCD区域填充
 * @param       xs,ys: 起始坐标  xe,ye: 结束坐标  color: 颜色
 * @retval      无
 */
void tftlcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color)
{
    uint16_t x_index;
    uint16_t y_index;
    
    tftlcd_set_column_address(xs, xe);
    tftlcd_set_page_address(ys, ye);
    tftlcd_start_write_memory();
    for (y_index=ys; y_index<=ye; y_index++)
    {
        for (x_index=xs; x_index<= xe; x_index++)
        {
            tftlcd_gpio_write_dat(color);
        }
    }
}

/**
 * @brief       ATK-MD0430模块LCD清屏
 * @param       color: 清屏颜色
 * @retval      无
 */
void tftlcd_clear(uint16_t color)
{
    tftlcd_fill(0, 0, tftlcd_LCD_WIDTH - 1, tftlcd_LCD_HEIGHT - 1, color);
}

/**
 * @brief       ATK-MD0430模块LCD画点
 * @param       x,y: 坐标  color: 颜色
 * @retval      无
 */
void tftlcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    tftlcd_set_column_address(x, x);
    tftlcd_set_page_address(y, y);
    tftlcd_start_write_memory();
    tftlcd_gpio_write_dat(color);
}

/**
 * @brief       ATK-MD0430模块LCD读点
 * @param       x,y: 坐标
 * @retval      颜色值
 */
uint16_t tftlcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t color;
    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;
    
    if ((x >= g_tftlcd_sta.width) || (y >= g_tftlcd_sta.height))
    {
        return tftlcd_EINVAL;
    }
    
    tftlcd_set_column_address(x, x);
    tftlcd_set_page_address(y, y);
    tftlcd_start_read_memory();
    
    color = tftlcd_gpio_read_dat();
    color = tftlcd_gpio_read_dat();
    color_r = (uint8_t)(color >> 11) & 0x1F;
    color_g = (uint8_t)(color >> 2) & 0x3F;
    color = tftlcd_gpio_read_dat();
    color_b = (uint8_t)(color >> 11) & 0x1F;
    
    return (uint16_t)(color_r << 11) | (color_g << 5) | color_b;
}

/* 以下画线、矩形、圆形、字符、字符串、数字、图片函数 逻辑完全保留 */
void tftlcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t x_delta;
    uint16_t y_delta;
    int16_t x_sign;
    int16_t y_sign;
    int16_t error;
    int16_t error2;
    
    x_delta = (x1 < x2) ? (x2 - x1) : (x1 - x2);
    y_delta = (y1 < y2) ? (y2 - y1) : (y1 - y2);
    x_sign = (x1 < x2) ? 1 : -1;
    y_sign = (y1 < y2) ? 1 : -1;
    error = x_delta - y_delta;
    
    tftlcd_draw_point(x2, y2, color);
    
    while ((x1 != x2) || (y1 != y2))
    {
        tftlcd_draw_point(x1, y1, color);
        
        error2 = error << 1;
        if (error2 > -y_delta)
        {
            error -= y_delta;
            x1 += x_sign;
        }
        
        if (error2 < x_delta)
        {
            error += x_delta;
            y1 += y_sign;
        }
    }
}

void tftlcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    tftlcd_draw_line(x1, y1, x2, y1, color);
    tftlcd_draw_line(x1, y2, x2, y2, color);
    tftlcd_draw_line(x1, y1, x1, y2, color);
    tftlcd_draw_line(x2, y1, x2, y2, color);
}

void tftlcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    int32_t x_t;
    int32_t y_t;
    int32_t error;
    int32_t error2;
    
    x_t = -r;
    y_t = 0;
    error = 2 - 2 * r;
    
    do {
        tftlcd_draw_point(x - x_t, y + y_t, color);
        tftlcd_draw_point(x + x_t, y + y_t, color);
        tftlcd_draw_point(x + x_t, y - y_t, color);
        tftlcd_draw_point(x - x_t, y - y_t, color);
        
        error2 = error;
        if (error2 <= y_t)
        {
            y_t++;
            error = error + (y_t * 2 + 1);
            if ((-x_t == y_t) && (error2 <= x_t))
            {
                error2 = 0;
            }
        }
        
        if (error2 > x_t)
        {
            x_t++;
            error = error + (x_t * 2 + 1);
        }
    } while (x_t <= 0);
}

void tftlcd_show_char(uint16_t x, uint16_t y, char ch, tftlcd_lcd_font_t font, uint16_t color)
{
    const uint8_t *ch_code;
    uint8_t ch_width;
    uint8_t ch_height;
    uint8_t ch_size;
    uint8_t ch_offset;
    uint8_t byte_index;
    uint8_t byte_code;
    uint8_t bit_index;
    uint8_t width_index = 0;
    uint8_t height_index = 0;
    
    ch_offset = ch - ' ';
    
    switch (font)
    {
#if (tftlcd_FONT_12 != 0)
        case tftlcd_LCD_FONT_12:
        {
            ch_code = tftlcd_font_1206[ch_offset];
            ch_width = tftlcd_FONT_12_CHAR_WIDTH;
            ch_height = tftlcd_FONT_12_CHAR_HEIGHT;
            ch_size = tftlcd_FONT_12_CHAR_SIZE;
            break;
        }
#endif
#if (tftlcd_FONT_16 != 0)
        case tftlcd_LCD_FONT_16:
        {
            ch_code = tftlcd_font_1608[ch_offset];
            ch_width = tftlcd_FONT_16_CHAR_WIDTH;
            ch_height = tftlcd_FONT_16_CHAR_HEIGHT;
            ch_size = tftlcd_FONT_16_CHAR_SIZE;
            break;
        }
#endif
#if (tftlcd_FONT_24 != 0)
        case tftlcd_LCD_FONT_24:
        {
            ch_code = tftlcd_font_2412[ch_offset];
            ch_width = tftlcd_FONT_24_CHAR_WIDTH;
            ch_height = tftlcd_FONT_24_CHAR_HEIGHT;
            ch_size = tftlcd_FONT_24_CHAR_SIZE;
            break;
        }
#endif
#if (tftlcd_FONT_32 != 0)
        case tftlcd_LCD_FONT_32:
        {
            ch_code = tftlcd_font_3216[ch_offset];
            ch_width = tftlcd_FONT_32_CHAR_WIDTH;
            ch_height = tftlcd_FONT_32_CHAR_HEIGHT;
            ch_size = tftlcd_FONT_32_CHAR_SIZE;
            break;
        }
#endif
        default:
        {
            return;
        }
    }
    
    if ((x + ch_width > tftlcd_LCD_WIDTH) || (y + ch_height > tftlcd_LCD_HEIGHT))
    {
        return;
    }
    
    for (byte_index=0; byte_index<ch_size; byte_index++)
    {
        byte_code = ch_code[byte_index];
        for (bit_index=0; bit_index<8; bit_index++)
        {
            if ((byte_code & 0x80) != 0)
            {
                tftlcd_draw_point(x + width_index, y + height_index, color);
            }
            height_index++;
            if (height_index == ch_height)
            {
                height_index = 0;
                width_index++;
                break;
            }
            byte_code <<= 1;
        }
    }
}

void tftlcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, tftlcd_lcd_font_t font, uint16_t color)
{
    uint8_t ch_width;
    uint8_t ch_height;
    uint16_t x_raw;
    uint16_t y_raw;
    uint16_t x_limit;
    uint16_t y_limit;
    
    switch (font)
    {
#if (tftlcd_FONT_12 != 0)
        case tftlcd_LCD_FONT_12:
        {
            ch_width = tftlcd_FONT_12_CHAR_WIDTH;
            ch_height = tftlcd_FONT_12_CHAR_HEIGHT;
            break;
        }
#endif
#if (tftlcd_FONT_16 != 0)
        case tftlcd_LCD_FONT_16:
        {
            ch_width = tftlcd_FONT_16_CHAR_WIDTH;
            ch_height = tftlcd_FONT_16_CHAR_HEIGHT;
            break;
        }
#endif
#if (tftlcd_FONT_24 != 0)
        case tftlcd_LCD_FONT_24:
        {
            ch_width = tftlcd_FONT_24_CHAR_WIDTH;
            ch_height = tftlcd_FONT_24_CHAR_HEIGHT;
            break;
        }
#endif
#if (tftlcd_FONT_32 != 0)
        case tftlcd_LCD_FONT_32:
        {
            ch_width = tftlcd_FONT_32_CHAR_WIDTH;
            ch_height = tftlcd_FONT_32_CHAR_HEIGHT;
            break;
        }
#endif
        default:
        {
            return;
        }
    }
    
    x_raw = x;
    y_raw = y;
    x_limit = ((x + width + 1) > tftlcd_LCD_WIDTH) ? tftlcd_LCD_WIDTH : (x + width + 1);
    y_limit = ((y + height + 1) > tftlcd_LCD_HEIGHT) ? tftlcd_LCD_HEIGHT : (y + height + 1);
    
    while ((*str >= ' ') && (*str <= '~'))
    {
        if (x + ch_width >= x_limit)
        {
            x = x_raw;
            y += ch_height;
        }
        
        if (y + ch_height >= y_limit)
        {
            y = x_raw;
            x = y_raw;
        }
        
        tftlcd_show_char(x, y, *str, font, color);
        
        x += ch_width;
        str++;
    }
}

void tftlcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, tftlcd_num_mode_t mode, tftlcd_lcd_font_t font, uint16_t color)
{
    uint8_t ch_width;
    uint8_t len_index;
    uint8_t num_index;
    uint8_t first_nozero = 0;
    char pad;
    
    switch (font)
    {
#if (tftlcd_FONT_12 != 0)
        case tftlcd_LCD_FONT_12:
        {
            ch_width = tftlcd_FONT_12_CHAR_WIDTH;
            break;
        }
#endif
#if (tftlcd_FONT_16 != 0)
        case tftlcd_LCD_FONT_16:
        {
            ch_width = tftlcd_FONT_16_CHAR_WIDTH;
            break;
        }
#endif
#if (tftlcd_FONT_24 != 0)
        case tftlcd_LCD_FONT_24:
        {
            ch_width = tftlcd_FONT_24_CHAR_WIDTH;
            break;
        }
#endif
#if (tftlcd_FONT_32 != 0)
        case tftlcd_LCD_FONT_32:
        {
            ch_width = tftlcd_FONT_32_CHAR_WIDTH;
            break;
        }
#endif
        default:
        {
            return;
        }
    }
    
    switch (mode)
    {
        case tftlcd_NUM_SHOW_NOZERO:
        {
            pad = ' ';
            break;
        }
        case tftlcd_NUM_SHOW_ZERO:
        {
            pad = '0';
            break;
        }
        default:
        {
            return;
        }
    }
    
    for (len_index=0; len_index<len; len_index++)
    {
        num_index = (num / tftlcd_pow(10, len - len_index - 1)) % 10;
        if ((first_nozero == 0) && (len_index < (len - 1)))
        {
            if (num_index == 0)
            {
                tftlcd_show_char(x + ch_width * len_index, y, pad, font, color);
                continue;
            }
            else
            {
                first_nozero = 1;
            }
        }
        
        tftlcd_show_char(x + ch_width * len_index, y, num_index + '0', font, color);
    }
}

void tftlcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, tftlcd_lcd_font_t font, uint16_t color)
{
    tftlcd_show_xnum(x, y, num, len, tftlcd_NUM_SHOW_NOZERO, font, color);
}

void tftlcd_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic)
{
    uint16_t x_index;
    uint16_t y_index;
    
    if ((x + width > tftlcd_LCD_WIDTH) || (y + height > tftlcd_LCD_HEIGHT))
    {
        return;
    }
    
    tftlcd_set_column_address(x, x + width - 1);
    tftlcd_set_page_address(y, y + height - 1);
    tftlcd_start_write_memory();
    for (y_index=y; y_index<=(y + height); y_index++)
    {
        for (x_index=x; x_index<=(x + width); x_index++)
        {
            tftlcd_gpio_write_dat(*pic);
            pic++;
        }
    }
}


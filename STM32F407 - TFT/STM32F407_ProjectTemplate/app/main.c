#include "sys.h" 
#include "board.h"
#include "OLED.h"

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
static volatile char s_cDataUpdate1 = 0,s_cDataUpdate2 = 0,s_cDataUpdate3 = 0,s_cDataUpdate4 = 0, s_cCmd = 0xff;
const uint32_t c_uiBaud[10] = {0, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

//Gyro_Data_t my_gyro= {0};
//uint8_t sum = 0,i = 0;

// 姿态角数据（单位：度）
float pitch, roll, yaw;             // 俯仰角、横滚角、偏航角
// 陀螺仪数据（原始16位整数，需转换为°/s）
short gyrox, gyroy, gyroz;          // X、Y、Z轴角速度
// 加速度计数据（原始16位整数，需转换为g）
short aacx, aacy, aacz;             // X、Y、Z轴加速度
// 编码器数据（单位：脉冲/周期）
int Encoder_Left, Encoder_Right;    // 左右电机编码器值（速度反馈）
// 电机控制参数
int PWM_MAX = 1000, PWM_MIN = -1000; // PWM输出限幅（最大值和最小值）
int MOTO1, MOTO2, flag_stop;         // 左右电机PWM输出值、停车标志位

uint16_t adc_value;


//static void SensorUartSend(uint8_t *p_data, uint32_t uiSize)
//{
//	Uart3Send(p_data, uiSize);
//}

//static void Delayms(uint16_t ucMs)
//{
//	delay_ms(ucMs);
//}

//static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
//{
//	int i;
//    for(i = 0; i < uiRegNum; i++)
//    {
//        switch(uiReg)
//        {
////            case AX:
////            case AY:
//            case AZ:
//				s_cDataUpdate1 |= ACC_UPDATE;
//            break;
////            case GX:
////            case GY:
//            case GZ:
//				s_cDataUpdate2 |= GYRO_UPDATE;
//            break;
////            case HX:
////            case HY:
//            case HZ:
//				s_cDataUpdate4 |= MAG_UPDATE;
//            break;
////            case Roll:
////            case Pitch:
//            case Yaw:
//				s_cDataUpdate3 |= ANGLE_UPDATE;
//            break;
////            default:
////				s_cDataUpdate1 |= READ_UPDATE;
////			break;
//        }
//		uiReg++;
//    }
//}

//#define PI (float)(3.1415926)

static float cube[8][3] = {
    {-32, -32, -32},
    {-32, +32, -32},
    {+32, +32, -32},
    {+32, -32, -32},
    {-32, -32, +32},
    {-32, +32, +32},
    {+32, +32, +32},
    {+32, -32, +32}
};

static uint8_t line_id[24] = {
    1, 2, 2, 3,
    3, 4, 4, 1,
    5, 6, 6, 7,
    7, 8, 8, 5,
    8, 4, 7, 3,
    6, 2, 5, 1
};

/**
 * @brief       计算矩阵乘法
 * @param       a      : 矩阵a
 *              b[3][3]: 矩阵b
 * @retval      计算结果
 */
static float *demo_matconv(float *a, float b[3][3])
{
    float res[3];
    uint8_t res_index;
    uint8_t a_index;
    
    for (res_index=0; res_index<3; res_index++)
    {
        res[res_index] = b[res_index][0] * a[0] + b[res_index][1] * a[1] + b[res_index][2] * a[2];
    }
    
    for (a_index=0; a_index<3; a_index++)
    {
        a[a_index] = res[a_index];
    }
    
    return a;
}

/**
 * @brief       旋转向量
 * @param       point: 需要旋转的向量
 *              x    : X轴旋转量
 *              y    : Y轴旋转量
 *              z    : Z轴旋转量
 * @retval      计算结果
 */
static void demo_rotate(float *point, float x, float y, float z)
{
    float rx[3][3];
    float ry[3][3];
    float rz[3][3];
    
    x /= PI;
    y /= PI;
    z /= PI;
    
    rx[0][0] = cos(x);
    rx[0][1] = 0;
    rx[0][2] = sin(x);
    rx[1][0] = 0;
    rx[1][1] = 1;
    rx[1][2] = 0;
    rx[2][0] = -sin(x);
    rx[2][1] = 0;
    rx[2][2] = cos(x);
    
    ry[0][0] = 1;
    ry[0][1] = 0;
    ry[0][2] = 0;
    ry[1][0] = 0;
    ry[1][1] = cos(y);
    ry[1][2] = -sin(y);
    ry[2][0] = 0;
    ry[2][1] = sin(y);
    ry[2][2] = cos(y);
    
    rz[0][0] = cos(z);
    rz[0][1] = -sin(z);
    rz[0][2] = 0;
    rz[1][0] = sin(z);
    rz[1][1] = cos(z);
    rz[1][2] = 0;
    rz[2][0] = 0;
    rz[2][1] = 0;
    rz[2][2] = 1;
    
    demo_matconv(demo_matconv(demo_matconv(point, rz), ry), rx);
}

/**
 * @brief       演示立方体3D旋转
 * @param       无
 * @retval      无
 */
static void demo_show_cube(void)
{
    uint8_t point_index;
    uint8_t line_index;
    static uint16_t x = 240;
    static uint16_t y = 441;
    static tftlcd_touch_point_t point[1];
    
    /* ATK-MD0430模块触摸扫描 */
    if (tftlcd_touch_scan(point, sizeof(point) / sizeof(point[0])) == sizeof(point) / sizeof(point[0]))
    {
        if ((point[0].x > 56) && (point[0].x < (tftlcd_get_lcd_width() - 56)) && (point[0].y > 138) && (point[0].y < (tftlcd_get_lcd_height() - 56)))
        {
            x = point[0].x;
            y = point[0].y;
        }
    }
    
    for (point_index=0; point_index<8; point_index++)
    {
        demo_rotate(cube[point_index], 0.05f, 0.03f, 0.02f);
    }
    
    for (line_index=0; line_index<24; line_index+=2)
    {
        /* ATK-MD0430模块LCD画线段 */
        tftlcd_draw_line(   x + cube[line_id[line_index] - 1][0],
                                y + cube[line_id[line_index] - 1][1],
                                x + cube[line_id[line_index + 1] - 1][0],
                                y + cube[line_id[line_index + 1] - 1][1],
                                tftlcd_BLUE);
    }
    
    delay_ms(20);
    
    tftlcd_fill(x - 56, y - 56, x + 56, y + 56, tftlcd_WHITE);
}

int main(void)	
{
	NVIC_Config();
	board_init();

//	OLED_Init();

//	Encoder_Init(84-1,65535);
//	TB6612_Init1(168,1000);
//	TB6612_Init2(168,1000);	
	
//	Usart3Init(115200);
//	WitInit(WIT_PROTOCOL_NORMAL, 0x50);
//	WitSerialWriteRegister(SensorUartSend);
//	WitRegisterCallBack(SensorDataUpdata);
//	WitDelayMsRegister(Delayms);
	
//	Gyro_USART_Init(115200);
	
//	buzzer_init();
//	TIM7_Init();
//	Key_Init();
	LED_Init();
//	Ultrasonic_Init();
//	Menu_Init();
	
//	OLED_ShowString(0,0,(uint8_t *)"roll:",8,1);
//	OLED_ShowString(0,8,(uint8_t *)"pitch:",8,1);
//	OLED_ShowString(0,16,(uint8_t *)"yaw:",8,1);
//	OLED_ShowString(0,24,(uint8_t *)"KeyNum1:",8,1);
//	OLED_ShowString(0,32,(uint8_t *)"KeyNum2:",8,1);
//	OLED_ShowString(0,40,(uint8_t *)"KeyNum3:",8,1);
//	OLED_ShowString(0,48,(uint8_t *)"KeyNum4:",8,1);
//	OLED_ShowString(0,56,(uint8_t *)"Left:",8,1);
//		Gyro_SendCommand(0xa4, 0x06, 0x03, 0x01);  // 查询模式命令
//		delay_ms(3);

    uint8_t ret;
    
    /* 初始化ATK-MD0430模块 */
    ret = tftlcd_init();
    if (ret != 0)
    {
        printf("ATK-MD0430 init failed!\r\n");
        while (1)
        {
            LED1_Toggle();
            delay_ms(200);
        }
    }
    
    /* ATK-MD0430模块LCD清屏 */
    tftlcd_clear(tftlcd_WHITE);
    /* ATK-MD0430模块LCD显示字符串 */
    tftlcd_show_string(10, 10, tftlcd_LCD_WIDTH, 32, "STM32", tftlcd_LCD_FONT_32, tftlcd_RED);
    tftlcd_show_string(10, 42, tftlcd_LCD_WIDTH, 24, "ATK-MD0430", tftlcd_LCD_FONT_24, tftlcd_RED);
    tftlcd_show_string(10, 66, tftlcd_LCD_WIDTH, 16, "ATOM@ALIENTEK", tftlcd_LCD_FONT_16, tftlcd_RED);
    

  while(1)	
	{
			demo_show_cube();
//		Menu_Update();
		

//		Gyro_SendCommand(0xa4, 0x03, 0x08, 0x1B);  // 读取数据命令(0x08~0x22寄存器)
//		delay_ms(10);
//		for(sum=0,i=0; i<(usart_rx_data[3]+4); i++) //rgb_data[3]=3
//		sum+=usart_rx_data[i];
//		// 拷贝数据到结构体(从第4字节开始为有效数据)
//		memcpy(&my_gyro, &usart_rx_data[4], sizeof(my_gyro));
//		roll=my_gyro.roll;pitch=my_gyro.pitch;yaw=my_gyro.yaw;
//		gyrox=my_gyro.gyro[0];gyroy=my_gyro.gyro[1];gyroz=my_gyro.gyro[2];
//		aacx=my_gyro.acc[0];aacy=my_gyro.acc[1];aacz=my_gyro.acc[2];
		
//		if(click1()==1)
//		{
//			KeyNum1+=1;
//			Timer=0;
//			KeyNum1%=8;
//		}
//		if(click2()==1)
//		{
//			KeyNum2+=1;
//			Timer=0;
//			KeyNum2%=5;
//		}		
//		if(click3()==1)
//		{
//			KeyNum3+=1;
//			Timer=0;
//			KeyNum3%=3;
//		}	
//		if(click4()==1)
//		{
//			KeyNum4+=1;
//			Timer=0;
//			KeyNum4%=2;
//		}
//		if (Key_IsComboPressed(KEY_1, KEY_2)) 
//		{
//			System_Reset();
//		}
//		KeyID_TypeDef key;
//		KeyEventType_TypeDef event;
//		if (Key_GetEvent(&key, &event)) 
//		{
//			if (event == KEY_EVENT_LONG_PRESS && key == KEY_1) 
//			{
//					EnterSettingsMode();
//			}
//		}



//		adc_value = ADC_GetAverageValue(ADC_CHANNEL, 20);
//			OLED_ShowNum(64,0,roll,4,8,1);
//			OLED_ShowNum(64,8,pitch,4,8,1);
//			OLED_ShowNum(64,16,yaw,4,8,1);
//			OLED_ShowNum(64,24,KeyNum1,4,8,1);
//			OLED_ShowNum(64,32,KeyNum2,4,8,1);
//			OLED_ShowNum(64,40,KeyNum3,4,8,1);
//			OLED_ShowNum(64,48,KeyNum4,4,8,1);
//			OLED_ShowNum(64,56,Encoder_Left,4,8,1);
//			OLED_Refresh();
	}
}

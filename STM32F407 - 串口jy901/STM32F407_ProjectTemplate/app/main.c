#include "sys.h" 
#include "board.h"
#include "OLED.h"
#include "global_vars.h"

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
static volatile char s_cDataUpdate1 = 0,s_cDataUpdate2 = 0,s_cDataUpdate3 = 0,s_cDataUpdate4 = 0, s_cCmd = 0xff;
const uint32_t c_uiBaud[10] = {0, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};


// 变量声明
//static uint16_t adc_value;                  // ADC采样值
static uint32_t oled_refresh_timer = 0;     // OLED刷新定时器
#define OLED_REFRESH_INTERVAL 50            // OLED刷新间隔（单位：毫秒）

// 函数声明
static void sensor_uart_send(uint8_t *p_data, uint32_t uiSize);  // 传感器串口发送函数
static void sensor_data_update(uint32_t uiReg, uint32_t uiRegNum);  // 传感器数据更新回调
static void delay_ms_wrapper(uint16_t ucMs);  // 延时函数包装器


int main(void)
{
	// 系统初始化
	NVIC_Config();                              // NVIC中断配置
	board_init();                               // 开发板初始化

	OLED_Init();                                // OLED显示屏初始化

	Encoder_Init(84-1, 65535);                  // 编码器初始化
	TB6612_Init1(168, 1000);                    // TB6612电机驱动初始化
	
	// 传感器初始化
	Usart3Init(115200);                         // USART3初始化，用于传感器通信
	WitInit(WIT_PROTOCOL_NORMAL, 0x50);         // 初始化WIT传感器SDK
	WitSerialWriteRegister(sensor_uart_send);   // 注册串口发送函数
	WitRegisterCallBack(sensor_data_update);    // 注册数据更新回调
	WitDelayMsRegister(delay_ms_wrapper);       // 注册延时函数
	
	TIM7_Init();                                // TIM7初始化，用于系统计时
	Key_Init();                                 // 按键初始化
	LED_Init();                                 // LED初始化
	Ultrasonic_Init();                          // 超声波传感器初始化
	trailing_init();                            // 拖尾功能初始化
	
	// OLED初始显示
	OLED_ShowString(0, 0, (uint8_t *)"roll:", 8, 1);    // 显示ROLL标签
	OLED_ShowString(0, 8, (uint8_t *)"pitch:", 8, 1);   // 显示PITCH标签
	OLED_ShowString(0, 16, (uint8_t *)"yaw:", 8, 1);     // 显示YAW标签
	OLED_ShowString(0, 24, (uint8_t *)"KeyNum1:", 8, 1);  // 显示按键1标签
	OLED_ShowString(0, 32, (uint8_t *)"KeyNum2:", 8, 1);  // 显示按键2标签
	OLED_ShowString(0, 40, (uint8_t *)"KeyNum3:", 8, 1);  // 显示按键3标签
	OLED_ShowString(0, 48, (uint8_t *)"KeyNum4:", 8, 1);  // 显示按键4标签
	OLED_ShowString(0, 56, (uint8_t *)"Left:", 8, 1);     // 显示左编码器标签
	Set_Target_Distance(0.21);                  // 设置目标距离
//		Gyro_SendCommand(0xa4, 0x06, 0x03, 0x01);  // 查询模式命令
//		delay_ms(3);
  while(1)	
	{
		// 按键扫描
		if(click1()==1)
		{
			g_system_state.keynum1 += 1;
			Timer=0;
			g_system_state.keynum1 %= 8;
		}
		if(click2()==1)
		{
			g_system_state.keynum2 += 1;
			Timer=0;
			g_system_state.keynum2 %= 5;
		}		
		if(click3()==1)
		{
			g_system_state.keynum3 += 1;
			Timer=0;
			g_system_state.keynum3 %= 3;
		}
		if(click4()==1)
		{
			g_system_state.keynum4 += 1;
			Timer=0;
			g_system_state.keynum4 %= 2;
		}

		// 控制OLED刷新频率，避免频繁刷新消耗CPU资源
		if (g_system_state.timer - oled_refresh_timer >= OLED_REFRESH_INTERVAL)
		{
			oled_refresh_timer = g_system_state.timer;
			
			// 更新OLED显示内容
			OLED_ShowNum(64, 0, g_sensor_data.roll, 4, 8, 1);       // 显示ROLL角度
			OLED_ShowNum(64, 8, g_sensor_data.pitch, 4, 8, 1);      // 显示PITCH角度
			OLED_ShowNum(64, 16, g_sensor_data.yaw, 4, 8, 1);        // 显示YAW角度
			OLED_ShowNum(64, 24, g_system_state.keynum1, 4, 8, 1);   // 显示按键1计数
			OLED_ShowNum(64, 32, g_system_state.keynum2, 4, 8, 1);   // 显示按键2计数
			OLED_ShowNum(64, 40, g_system_state.keynum3, 4, 8, 1);   // 显示按键3计数
			OLED_ShowNum(64, 48, g_system_state.keynum4, 4, 8, 1);   // 显示按键4计数
			OLED_ShowNum(64, 56, g_motor_ctrl.encoder_left, 4, 8, 1); // 显示左编码器值
			
			// 刷新OLED
			OLED_Refresh();
		}
	}
}

// 传感器串口发送函数 - 包装Uart3Send函数
static void sensor_uart_send(uint8_t *p_data, uint32_t uiSize)
{
	Uart3Send(p_data, uiSize);  // 调用Uart3Send发送数据
}

// 延时函数包装器 - 避免函数名冲突
static void delay_ms_wrapper(uint16_t ucMs)
{
	delay_ms(ucMs);  // 调用系统延时函数
}

//// 传感器数据更新回调函数
//static void sensor_data_update(uint32_t uiReg, uint32_t uiRegNum)
//{
//	uint32_t reg_end = uiReg + uiRegNum;
//    
//	// 批量处理传感器数据更新
//    while (uiReg < reg_end)
//    {
//        switch(uiReg)
//        {
//            case AX:  // 加速度X轴
//            case AY:  // 加速度Y轴
//            case AZ:  // 加速度Z轴
//                s_cDataUpdate1 |= ACC_UPDATE;  // 加速度数据更新标志
//                // 将加速度数据更新到全局变量
//                if (uiReg >= AX && uiReg <= AZ)
//                {
//                    g_sensor_data.acc[uiReg - AX] = iReg[uiReg];
//                }
//                break;
//                
//            case GX:  // 陀螺仪X轴
//            case GY:  // 陀螺仪Y轴
//            case GZ:  // 陀螺仪Z轴
//                s_cDataUpdate2 |= GYRO_UPDATE; // 陀螺仪数据更新标志
//                // 将陀螺仪数据更新到全局变量
//                if (uiReg >= GX && uiReg <= GZ)
//                {
//                    g_sensor_data.gyro[uiReg - GX] = iReg[uiReg];
//                }
//                break;
//                
//            case HX:  // 磁力计X轴
//            case HY:  // 磁力计Y轴
//            case HZ:  // 磁力计Z轴
//                s_cDataUpdate4 |= MAG_UPDATE;  // 磁力计数据更新标志
//                // 将磁力计数据更新到全局变量
//                if (uiReg >= HX && uiReg <= HZ)
//                {
//                    g_sensor_data.mag[uiReg - HX] = iReg[uiReg];
//                }
//                break;
//                
//            case Roll:  // 横滚角
//                g_sensor_data.roll = iReg[uiReg];
//                // 无break，继续处理Pitch和Yaw
//                
//            case Pitch: // 俯仰角
//                g_sensor_data.pitch = iReg[uiReg];
//                // 无break，继续处理Yaw
//                
//            case Yaw:   // 偏航角
//                g_sensor_data.yaw = iReg[uiReg];
//                s_cDataUpdate3 |= ANGLE_UPDATE;// 角度数据更新标志
//                break;
//                
//            default:
//                // 其他寄存器数据，暂时不处理
//                break;
//        }
//        
//        uiReg++;  // 处理下一个寄存器
//    }
//}


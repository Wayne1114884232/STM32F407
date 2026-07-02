#include "sys.h" 
#include "board.h"


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

int main(void)	
{
	NVIC_Config();
	board_init();

	OLED_Init();
//	buzzer_init();

	Encoder_Init(84-1,65535);
	TB6612_Init1(168,1000);   
	
//	USART3_Init(115200);
	
	IIC_Init();
	WitInit(WIT_PROTOCOL_I2C, 0x50);
	WitI2cFuncRegister(IICwriteBytes, IICreadBytes);
	WitRegisterCallBack(CopeSensorData);
	
	TIM7_Init();
	LED_Init();
	Ultrasonic_Init();
	
	OLED_ShowString(0,7,(uint8_t *)"roll:",8,1);
	OLED_ShowString(0,15,(uint8_t *)"gyrox:",8,1);
	OLED_ShowString(0,23,(uint8_t *)"pitch:",8,1);
	OLED_ShowString(0,31,(uint8_t *)"gyroy:",8,1);
	OLED_ShowString(0,39,(uint8_t *)"yaw:",8,1);
	OLED_ShowString(0,47,(uint8_t *)"gyroz:",8,1);
//	OLED_ShowString(0,55,(uint8_t *)"length:",8,1);
  while(1)	
	{
//			if(click() == 1)
//			{
//					KeyID_TypeDef key = Key_GetNum();
//					
//					switch(key)
//					{
//							case KEY_1:  // PC5
//									KeyNum = (KeyNum + 1) % 8;
//									break;
//									
//							case KEY_2:  // PE7
//									flag2 = !flag2;
//									break;
//									
//							case KEY_3:  // PB1
//									flag3 = !flag3;
//									break;
//									
//							case KEY_4:  // PE8
//									flag4 = !flag4;
//									break;
//									
//							default:
//									break;
//					}
//			}
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
		OLED_ShowNum(64,7,roll,4,8,1);//1
		OLED_ShowNum(64,15,gyrox,4,8,1);//2
		OLED_ShowNum(64,23,pitch,4,8,1);//3
		OLED_ShowNum(64,31,gyroy,4,8,1);//4
		OLED_ShowNum(64,39,yaw,4,8,1);//5
		OLED_ShowNum(64,47,gyroz,4,8,1);
//		OLED_ShowNum(64,55,length,4,8,1);
		OLED_Refresh();
	}
}




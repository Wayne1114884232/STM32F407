#include "sys.h" 
#include "board.h"
//#include "OLED.h"

//#define ACC_UPDATE		0x01
//#define GYRO_UPDATE		0x02
//#define ANGLE_UPDATE	0x04
//#define MAG_UPDATE		0x08
//#define READ_UPDATE		0x80
//static volatile char s_cDataUpdate1 = 0,s_cDataUpdate2 = 0,s_cDataUpdate3 = 0,s_cDataUpdate4 = 0, s_cCmd = 0xff;
//const uint32_t c_uiBaud[10] = {0, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
/*电阻式压力传感器 施加力是输入是高电平
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)==1)
	{
		LED2_ON();
	}else LED2_OFF();

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
    GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);  // GPIOD
*/
//Gyro_Data_t my_gyro= {0};
//uint8_t sum = 0,i = 0;

//// 姿态角数据（单位：度）
//float pitch, roll, yaw;             // 俯仰角、横滚角、偏航角
//// 陀螺仪数据（原始16位整数，需转换为°/s）
//short gyrox, gyroy, gyroz;          // X、Y、Z轴角速度
//// 加速度计数据（原始16位整数，需转换为g）
//short aacx, aacy, aacz;             // X、Y、Z轴加速度
//// 编码器数据（单位：脉冲/周期）
//int Encoder_Left, Encoder_Right;    // 左右电机编码器值（速度反馈）
//// 电机控制参数
//int PWM_MAX = 1000, PWM_MIN = -1000; // PWM输出限幅（最大值和最小值）
//int MOTO1, MOTO2, flag_stop;         // 左右电机PWM输出值、停车标志位

//uint16_t adc_value;

//static void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
//static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
//static void Delayms(uint16_t ucMs);


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
	TIM7_Init();
//	Key_Init();
	LED_Init();
//	Ultrasonic_Init();
//	trailing_init();
//	Menu_Init();
	
//	OLED_ShowString(0,0,(uint8_t *)"roll:",8,1);
//	OLED_ShowString(0,8,(uint8_t *)"pitch:",8,1);
//	OLED_ShowString(0,16,(uint8_t *)"yaw:",8,1);
//	OLED_ShowString(0,24,(uint8_t *)"KeyNum1:",8,1);
//	OLED_ShowString(0,32,(uint8_t *)"KeyNum2:",8,1);
//	OLED_ShowString(0,40,(uint8_t *)"KeyNum3:",8,1);
//	OLED_ShowString(0,48,(uint8_t *)"KeyNum4:",8,1);
//	OLED_ShowString(0,56,(uint8_t *)"Left:",8,1);
//	Set_Target_Distance(0.21);
//		Gyro_SendCommand(0xa4, 0x06, 0x03, 0x01);  // 查询模式命令
//		delay_ms(3);
  while(1)	
	{
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

void TIM7_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM7,TIM_IT_Update)!= RESET)// 瀹氭椂涓柇瑙﹀彂
	{	
		LED4_Toggle();
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);// 娓呴櫎涓柇鏍囧織浣?
}

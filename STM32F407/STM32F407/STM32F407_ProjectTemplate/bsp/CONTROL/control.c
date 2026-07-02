#include "control.h"
#include "Tracking.h"

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80

static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum);

/*ç›®æ ‡æ¨¡å—è¾“å‡ºåŒæ¥å£ï¼Œå­¦ä¹ å¹¶å‚è€ƒå¼€æºä»£ç å®ç°åŠŸèƒ½ã€‚ç¡®ä¿å¹³è¡¡è½¦ç¨³å®šè¿è¡Œåˆ°åŠŸèƒ½è°ƒè¯•å®Œæˆã€‚*/

extern uint8_t ucRegIndex;
extern uint16_t usRegDataBuff[4];
extern uint32_t uiRegDataLen;

float Med_Angle=0;			// æœºæ¢°ä¸­å€¼ã€‚---åˆå§‹æ ¡å‡†åçš„å¹³è¡¡è½¦æœºæ¢°ä¸­å€¼å³é›¶ç‚¹
float Target_Speed=0;	// ç›®æ ‡é€Ÿåº¦ï¼ˆå‰è¿›åé€€ï¼‰ã€‚---åŒé—­ç¯æ§åˆ¶ï¼Œç”¨äºè°ƒèŠ‚å°è½¦å½“å‰è¡Œè¿›çš„å®é™…é€Ÿåº¦
float Turn_Speed=0;			// ç›®æ ‡é€Ÿåº¦ï¼ˆè½¬å‘ï¼‰

float
	Vertical_Kp=300,//280,//270,//+
	Vertical_Kd=0;//23;//23;//+
float
	Velocity_Kp=-0,//-125,//-120,//-
	Velocity_Ki=-0;//-0.625;//-0.6;//-
float
	Turn_Kd=0,//+
	Turn_Kp=0;//

float 
	Track_Kp=0,			//180
	Track_Ki=0,			//1
	Track_Kd=0;			//+ è½¨è¿¹æ§åˆ¶å¾®åˆ†

float  
	diffe_kp=0,
  diffe_ki=0,
  diffe_kd=0;

float 
	Pos_Kp = 5.0f, 
	Pos_Ki = 0.2f,
	Pos_Kd = 0.5f; 

float Pos_Target,Pos_Current,Total_Distance,Velocity_Left,Velocity_Right;
static float Pos_Err,Pos_LastErr,Pos_Integral,Pos_Derivative,Pos_Output;
uint8_t Pos_ReachFlag;

int Vertical_out,Velocity_out,Turn_out,Timer=0;// ç›´ç«‹ç¯&é€Ÿåº¦ç¯&è½¬å‘ç¯ çš„è¾“å‡ºå€¼
int flag_Num=0,flag_Numdelay=0,Action_speed=0,flag_turn=1,Encoder_S=0;
int flag_a,flag_b,flag_c,flag_Num1,Yaw_Data;
float length;

int Vertical(float Med,float Angle,float gyro_Y);// å‡½æ•°å£°æ˜
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC); 
int different_PD(int16_t Encoder_Left,int16_t Encoder_Right,float Gyro_Z);
int16_t Read_turn(int flag_turn);
//void mode1(void);
//void mode2(void);
//void mode3(void);
//void mode4(void);
//void mode5(void);
//void mode6(void);
//void mode7(void);


void TIM7_IRQHandler(void)
{
	int PWM_out;
	Timer++;
	if (TIM_GetITStatus(TIM7,TIM_IT_Update)!= RESET)
	{
			
			Encoder_Left=-Read_Speed(ENCODER_M1);// ç”µæœºæµ‹é€Ÿç¼–ç å™¨ï¼Œç”±äºå®‰è£…æ–¹å‘ç›¸å·®180åº¦ï¼Œä¸ºä½¿ç¼–ç å™¨è¾“å‡ºæ­£æ–¹å‘ä¸€è‡´ï¼Œæ­¤å¤„å–å
			Encoder_Right=-Read_Speed(ENCODER_M2);
			
			WitReadReg(AX,12);
			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;

			Get_Velocity_Form_Encoder(Encoder_Left, Encoder_Right);
			Update_Distance();
			if (Pos_Target != 0){PosPID_Calculate();Target_Speed = Pos_Output;}
			
			//2ã€å°†æ•°æ®è¿›è¡Œæ»¤æ³¢å¤„ç†ï¼Œè®¡ç®—æ§åˆ¶é‡
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								// é€Ÿåº¦ç¯				
			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	// ç›´ç«‹ç¯
			Turn_out=Read_turn(flag_turn);																								// è½¬å‘ç¯
			PWM_out=Vertical_out+Velocity_out;// æ€»è¾“å‡º

			//3ã€å°†æ§åˆ¶é‡åŠ è½½åˆ°ç”µæœºé©±åŠ¨ï¼Œå®ç°é¢„æœŸç›®æ ‡
			MOTO1=PWM_out-Turn_out;// å·¦ç”µæœº
			MOTO2=PWM_out+Turn_out;// å³ç”µæœº
			Limit(&MOTO1,&MOTO2);	 // PWMé™å¹…			
			Load(MOTO1,MOTO2);		 // åŠ è½½åˆ°ç”µæœºé©±åŠ¨
			Stop(&Med_Angle,&roll);
	}
//			if((flag_Numdelay==0)&&(Timer>300))
//			{
//				flag_Numdelay=1;
//				Timer=0;
//			}
//			
//			if((KeyNum==1)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=38;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//				}
//			mode1();
//			}
//			if((KeyNum==2)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=38;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//					flag_c=1;
//				}
//			mode2();
//			}
//			if((KeyNum==3)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=35;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//					flag_c=2;
//				}
//			mode3();
//			}
//			if((KeyNum==4)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=30;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//					flag_c=4;
//				}
//			mode4();
//			}
//			if((KeyNum==5)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=38;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//					flag_c=3;
//				}
//			mode5();
//			}
//			if((KeyNum==6)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=38;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//					flag_c=5;
//				}
//			mode6();
//			}
//			if((KeyNum==7)&&(flag_Numdelay==1))
//			{
//				if(Action_speed==0)
//				{
//					Target_Speed=38;
//					Track_Kp=0;
//					Track_Ki=0;
//					Action_speed=1;
//					flag_c=7;
//				}
//			mode7();
//			}	
//	length = Hcsr04GetLength();

	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);// æ¸…é™¤ä¸­æ–­æ ‡å¿—ä½
}

/*********************
ç›´ç«‹ç¯PDæ§åˆ¶ï¼šKp*åå·®+Kd*åå·®å¾®åˆ†

è¾“å…¥ï¼šç›®æ ‡è§’åº¦ã€å®é™…è§’åº¦ã€å®é™…è§’é€Ÿåº¦
è¾“å‡ºï¼šç›´ç«‹ç¯è¾“å‡º
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}

/*********************
é€Ÿåº¦ç¯PIï¼šKp*åå·®+Ki*åå·®ç§¯åˆ†
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,EnC_Err_Lowout,EnC_Err_Lowout_last;
	float a=0.8;
	
	//1.è®¡ç®—é€Ÿåº¦åå·®	Encoder_Err=((encoder_left+encoder_right)-Target);// æ¶ˆé™¤é™æ€è¯¯å·®ï¼šå½“é€Ÿåº¦ä¸º"0"æ—¶çš„è§’åº¦ï¼Œå³ä¸ºæœºæ¢°ä¸­å€¼
	
	//2.å¯¹é€Ÿåº¦åå·®è¿›è¡Œæ»¤æ³¢å¤„ç†
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;// ä½¿è¾“å‡ºæ›´å¹³æ»‘ï¼Œæ»¤é™¤é«˜é¢‘å™ªå£°ï¼Œé˜²æ­¢é€Ÿåº¦æ³¢åŠ¨
	EnC_Err_Lowout_last=EnC_Err_Lowout;// é˜²æ­¢é€Ÿåº¦æ³¢åŠ¨å½±å“ç›´ç«‹ç¯çš„ç¨³å®šæ€§
	
	//3.å¯¹é€Ÿåº¦åå·®è¿›è¡Œç§¯åˆ†è¿ç®—
	Encoder_S+=EnC_Err_Lowout;
	
	//4.ç§¯åˆ†é™å¹…
	Encoder_S=Encoder_S;
	
//4.ç§¯åˆ†é™å¹…
// å°†ç§¯åˆ†å€¼é™åˆ¶åœ¨[-10000, 10000]èŒƒå›´å†…ï¼Œé˜²æ­¢ç§¯åˆ†é¥±å’Œ
Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);

// å½“stopæ ‡å¿—è¢«ç½®ä½æ—¶ï¼Œæ¸…é›¶ç§¯åˆ†å¹¶æ¸…é™¤æ ‡å¿—
// ç”¨äºç´§æ€¥åœæ­¢æˆ–é‡æ–°åˆå§‹åŒ–ç§¯åˆ†å™¨
if(stop==1)Encoder_S=0,stop=0;

//5.é€Ÿåº¦ç¯æ§åˆ¶é‡è®¡ç®—
// æ ¹æ®æ»¤æ³¢åçš„é€Ÿåº¦è¯¯å·®å’Œç§¯åˆ†å€¼è®¡ç®—æœ€ç»ˆè¾“å‡º
// Velocity_Kp: é€Ÿåº¦ç¯æ¯”ä¾‹ç³»æ•°ï¼Œè´Ÿæ•°è¡¨ç¤ºè´Ÿåé¦ˆ
// Velocity_Ki: é€Ÿåº¦ç¯ç§¯åˆ†ç³»æ•°ï¼Œè´Ÿæ•°è¡¨ç¤ºè´Ÿåé¦ˆ
PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
return PWM_out;
}

/*********************
è½¬å‘æ§åˆ¶ç¯ï¼š
å…¬å¼=Kd*Zè½´è§’é€Ÿåº¦ + Kp*è½¬å‘æ§åˆ¶è¾“å…¥

è¯´æ˜ï¼š
å®ç°ä¸€ä¸ªå…¸å‹çš„PDæ§åˆ¶å™¨ï¼ŒKdé¡¹ç”¨äºæ§åˆ¶è½¬å‘çš„å“åº”é€Ÿåº¦ï¼Œ
Kpé¡¹ç”¨äºæ§åˆ¶è½¬å‘çš„å¼ºåº¦ã€‚
*********************/
int Turn(int gyro_Z, int RC)
{
    int PWM_out;
    // å®ç°ä¸€ä¸ªæ ‡å‡†çš„PDæ§åˆ¶å™¨ï¼ŒKdç”¨äºé˜»å°¼ï¼ŒKpç”¨äºæ¯”ä¾‹æ§åˆ¶
    PWM_out = Turn_Kd * gyro_Z + Turn_Kp * RC;
    return PWM_out;
}

// è½¨è¿¹è·Ÿè¸ªæ§åˆ¶
// ç”¨äºæ²¿ç€ç‰¹å®šè½¨è¿¹è¡Œé©¶çš„æ§åˆ¶ç®—æ³•
int Track_Turn(int RC)
{
    int PWM_Out;
    float err = 0;
    static float Track_s,last_RC; // è½¨è¿¹è¯¯å·®ç§¯åˆ†é¡¹
    
	    // å¤„ç†å¾ªè¿¹ä¸¢å¤±çš„æƒ…å†µ
    if (RC == 0) {
        // å¾ªè¿¹ä¸¢å¤±æ—¶ä½¿ç”¨ä¸Šä¸€æ¬¡æœ‰æ•ˆå€¼æˆ–æ‰‹åŠ¨æ§åˆ¶è¾“å…¥
        RC = (last_RC != 0) ? last_RC : (float)RC;
    } else {
        // æ›´æ–°ä¸Šä¸€æ¬¡å¾ªè¿¹å€¼
        last_RC = RC;
    }
		
    err = RC - 0; // è®¡ç®—å½“å‰è½¨è¿¹è¯¯å·®
    Track_s += err; // è¯¯å·®ç§¯åˆ†
    
		if (Track_s > 100) Track_s = 100;
    if (Track_s < -100) Track_s = -100;
		
		float derivative = err - last_RC;
    // PIDæ§åˆ¶è®¡ç®—ï¼šæ¯”ä¾‹é¡¹ + ç§¯åˆ†é¡¹ + å¾®åˆ†é¡¹
    PWM_Out = err * Track_Kp + Track_Ki * Track_s + derivative * Track_Kd;
    return PWM_Out;
}

// å·®é€Ÿæ§åˆ¶ - åŸåœ°è½¬å‘
// é€šè¿‡å·¦å³è½®é€Ÿå·®å®ç°ç²¾ç¡®è½¬å‘æ§åˆ¶
int different_PD(int16_t Encoder_Left, int16_t Encoder_Right, float Gyro_Z)
{
    int PWM_OUT, Err;
    static float diff_s; // å·®é€Ÿè¯¯å·®ç§¯åˆ†é¡¹
    
    Err = Encoder_Left - Encoder_Right; // è®¡ç®—å·¦å³è½®é€Ÿå·®
    
    diff_s += Err; // è½®é€Ÿå·®ç§¯åˆ†
    
    // PIDæ§åˆ¶è®¡ç®—ï¼šæ¯”ä¾‹é¡¹ + ç§¯åˆ†é¡¹ + å¾®åˆ†é¡¹
    PWM_OUT = diffe_kp * Err + diffe_ki * diff_s + diffe_kd * Gyro_Z;

    return PWM_OUT;
}

// è½¬å‘æ§åˆ¶æ¨¡å¼é€‰æ‹©å™¨
// æ ¹æ®flag_turnå‚æ•°é€‰æ‹©ä¸åŒçš„è½¬å‘æ§åˆ¶ç®—æ³•
int16_t Read_turn(int flag_turn)
{
    int16_t Value_turn;
    switch(flag_turn)
    {
        case 0: Value_turn = (int16_t)Track_Turn(Tracking_InnerRing()); break; // è½¨è¿¹è·Ÿè¸ªæ¨¡å¼
        case 1: Value_turn = (int16_t)Turn(gyroz, Turn_Speed); break; // æ™®é€šè½¬å‘æ¨¡å¼
        case 2: Value_turn = (int16_t)different_PD(Encoder_Left, Encoder_Right, gyroz); break; // å·®é€Ÿæ§åˆ¶æ¨¡å¼
    }
    return Value_turn;
}

// ä»ç¼–ç å™¨æ•°æ®è®¡ç®—å®é™…é€Ÿåº¦
// è¾“å…¥ï¼šå·¦å³ç¼–ç å™¨è®¡æ•°å€¼
// è¾“å‡ºï¼šå·¦å³è½®å®é™…çº¿é€Ÿåº¦(m/s)
void Get_Velocity_Form_Encoder(int encoder_left, int encoder_right)
{
    // ç¼–ç å™¨å€¼è½¬æ¢ä¸ºçº¿é€Ÿåº¦(m/s)
    // è®¡ç®—å…¬å¼ï¼šç¼–ç å™¨è„‰å†²æ•° * è½®å‘¨é•¿ / (ç¼–ç å™¨ç²¾åº¦ * å‡é€Ÿæ¯” * ç¼–ç å™¨å€æ•° * é‡‡æ ·æ—¶é—´)
    Velocity_Left = (float)encoder_left * PI * Diameter_67 / (Encoder_precision * Reduction_Ratio * EncoderMultiples * DT);
    Velocity_Right = (float)encoder_right * PI * Diameter_67 / (Encoder_precision * Reduction_Ratio * EncoderMultiples * DT);
}

// ä½ç½®PIDæ§åˆ¶å™¨è®¡ç®—
// è®¡ç®—ç›®æ ‡ä½ç½®ä¸å½“å‰ä½ç½®çš„è¯¯å·®ï¼Œå¹¶ç”Ÿæˆé€Ÿåº¦æ§åˆ¶å‘½ä»¤
void PosPID_Calculate(void) 
{
    // è®¡ç®—ä½ç½®è¯¯å·®
    Pos_Err = Pos_Target - Pos_Current;

    // è¯¯å·®ç§¯åˆ†ï¼Œå¸¦ç§¯åˆ†é™å¹…é˜²æ­¢é¥±å’Œ
    Pos_Integral += Pos_Err;
    Pos_Integral = LimitFloat(Pos_Integral, -POS_MAX_INTEGRAL, POS_MAX_INTEGRAL);  

    // è¯¯å·®å¾®åˆ†è®¡ç®—
    Pos_Derivative = (Pos_Err - Pos_LastErr) / DT;

    // PIDè¾“å‡ºè®¡ç®—ï¼Œå¸¦è¾“å‡ºé™å¹…
    Pos_Output = Pos_Kp * Pos_Err + Pos_Ki * Pos_Integral + Pos_Kd * Pos_Derivative;
    Pos_Output = LimitFloat(Pos_Output, -POS_MAX_OUTPUT, POS_MAX_OUTPUT);  

    // åˆ¤æ–­æ˜¯å¦è¾¾åˆ°ç›®æ ‡ä½ç½®
    if (fabs(Pos_Err) < POS_ERR_THRESHOLD) {
        Pos_ReachFlag = 1;  // æ ‡è®°å·²åˆ°è¾¾ç›®æ ‡ä½ç½®
        Pos_Output = 0;     // åœæ­¢è¾“å‡ºï¼Œé˜²æ­¢è¿‡å†²
    } else {
        Pos_ReachFlag = 0;  // æœªåˆ°è¾¾ç›®æ ‡ä½ç½®
    }

    // ä¿å­˜å½“å‰è¯¯å·®ç”¨äºä¸‹æ¬¡å¾®åˆ†è®¡ç®—
    Pos_LastErr = Pos_Err;
}

// æ›´æ–°è¡Œé©¶è·ç¦»
// æ ¹æ®ç¼–ç å™¨æ•°æ®ç´¯è®¡è®¡ç®—è¡Œé©¶æ€»è·ç¦»
void Update_Distance(void) 
{
    // è®¡ç®—ä½ç§»ï¼š(å·¦å³ç¼–ç å™¨å¹³å‡å€¼) * è½®å‘¨é•¿ / (ç¼–ç å™¨ç²¾åº¦ * å‡é€Ÿæ¯” * ç¼–ç å™¨å€æ•°)
    float displacement = ((float)(Encoder_Left + Encoder_Right) / 2.0f) * 
                         (PI * Diameter_67) / (Encoder_precision * Reduction_Ratio * EncoderMultiples);
    Pos_Current += displacement;      // æ›´æ–°å½“å‰ä½ç½®
    Total_Distance += fabs(displacement); // ç´¯è®¡æ€»è¡Œé©¶è·ç¦»
}

// è®¾ç½®ç›®æ ‡è¡Œé©¶è·ç¦»
// åŸºäºå½“å‰ä½ç½®è®¾ç½®æ–°çš„ç›®æ ‡ä½ç½®
void Set_Target_Distance(float distance) 
{
    Pos_Target = Pos_Current + distance;  // è®¾ç½®ç›¸å¯¹ç›®æ ‡ä½ç½®
    Pos_ReachFlag = 0;  // é‡ç½®åˆ°è¾¾æ ‡å¿—
}

// æµ®ç‚¹æ•°é™å¹…å‡½æ•°
// å°†å€¼é™åˆ¶åœ¨[min, max]èŒƒå›´å†…
float LimitFloat(float value, float min, float max)
{
    if (value > max) return max;
    else if (value < min) return min;
    else return value;
}

////×óµ½ÓÒB1 B0 A7 A6 A5 A4 C15 C14
////×óµ½ÓÒB1    A7    A5        C14 
//void mode1(void)
//{
//			int PWM_out;
//			if(Timer>=200)
//			{
//				flag_stop=1;
//			}
//			if((KeyNum==1)&&(flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=0;Encoder_S=0;Track_Kp=0;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			if(flag_a==1&&Timer>10)
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				flag_a=0;
//			}
//			
//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}

//void mode2(void)
//{
//			int PWM_out; 
//			
//			if((flag_Num<5)&&(Timer>=250))
//			{
//				flag_stop=1;
//			}
//		
//			if((flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=160;}
//					if(flag_Num1==4){flag_a=1;flag_Num1=5;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=160;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			else if((KeyNum==2)&&(flag_stop==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				if(flag_b==2)
//				{
//					flag_Num+=1;
//					if(flag_Num1==2){flag_a=1;flag_Num1=3;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=50;Track_Kp=0;}
//					if(flag_Num1==6){flag_a=2;flag_Num1=7;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=15;Track_Kp=0;Encoder_S=0;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			if((flag_a==1)&&(Timer>10))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			
//			if((flag_Num1==1)&&(PA4==0)&&(PA5==0)&&(PA6==0)&&(PA7==0))
//			{
//				flag_a=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);
//			}
//			if((flag_Num1==1)&&(Timer>400)&&(PA4==1)&&(PA5==1)&&(PA6==1)&&(PA7==1)&&(PB0==1))
//			{
//				flag_b=2;
//				flag_Num1=2;
//				flag_c=6;
//			}
//			if((flag_Num1==3)&&(Timer>200)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				flag_b=0;		
//				flag_Num1=4;
//			}
//			if((flag_Num1==5)&&(Timer>300)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;
//				flag_Num1=6;
//			}
//			if((flag_Num1==7)&&(Timer>10)&&(flag_a==2))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				flag_a=0;
//			}

//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}

//void mode3(void)
//{
//			int PWM_out; 
//			
//			if((flag_Num<5)&&(Timer>=300))
//			{
//				flag_stop=1;
//			}

//			if((flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=35;Track_Kp=160;}
//					if(flag_Num1==4){flag_a=1;flag_Num1=5;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=160;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			else if((KeyNum==3)&&(flag_stop==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				if(flag_b==2)
//				{				
//					flag_Num+=1;
//					if(flag_Num1==2){flag_a=2;flag_Num1=3;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=5;Track_Kp=0;}
//					if(flag_Num1==6){flag_a=3;flag_Num1=7;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=0;Track_Kp=0;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			if(flag_a==1&&Timer>10)
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			
//			if((flag_Num1==1)&&(Timer==250))
//			{
//				flag_a=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);
//			}
//			if((flag_Num1==1)&&(Timer>300)&&(PB1==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1)&&(PC15==1))
//			{
//				flag_b=2;		
//				flag_Num1=2;
//			}
//			if((flag_a==2)&&(Timer>10)&&(flag_Num1==3))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=-10;
//				flag_a=0;
//				Timer=0;
//				flag_turn=1;
//				Turn_Kp=10;
//				Turn_Speed=45;
//			}
//			if((flag_turn==1)&&(Timer>83))
//			{
//				flag_turn=0;
//				Target_Speed=40;
//				Timer=0;
//				flag_c=6;
//			}
////			if((flag_turn==0)&&(flag_c==6)&&(Timer>400)&&(flag_Num1<4))
////			{
////				flag_a=0;
////				flag_Num1=20;
////				Track_Kp=180;
////				Timer=0;
////			}
//			if((flag_Num1==3)&&(flag_turn==0)&&(Timer>50)&&((PB0==0)|(PA7==0)|(PA6==0)|(PA5==0)|(PA4==0)))
//			{
//				flag_b=0;
//				flag_Num1=4;
//			}
//			if((flag_Num1==5)&&(Timer>100)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;
//				flag_Num1=6;
//				Encoder_S=0;
//			}
//			if((flag_a==3)&&(Timer>10)&&(flag_Num1==7))
//			{
//				Target_Speed=0;
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			
//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}


//void mode4(void)
//{
//			int PWM_out; 
//			
//			if((flag_Num<5)&&(Timer>=300))
//			{
//				flag_stop=1;
//			}
//			
//			if((flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=30;Track_Kp=160;}
//					if(flag_Num1==2){flag_a=2;flag_Num1=3;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=2;Track_Kp=0;Encoder_S=0;}
//					if(flag_Num1==10){flag_a=1;flag_Num1=11;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=160;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;				
//				}
//			}
//			else if((flag_stop==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				if(flag_b==2)
//				{
//					flag_Num+=1;
//					if(flag_Num1==7){flag_a=2;flag_Num1=8;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=5;Track_Kp=0;}
//					if(flag_Num1==12){flag_a=3;flag_Num1=13;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=10;Track_Kp=0;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			if(flag_a==1&&Timer>10)
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			if((flag_Num1==1)&&(Timer>300))
//			{
//				Track_Kp=0;
//			}
//			if((flag_Num1==1)&&(Timer>50)&&(PA6==0)&&(PA5==0)&&(PA7==0)&&(PB0==0))
//			{
//				flag_b=0;
//				flag_Num1=2;
//			}
//			if((flag_a==2)&&(flag_Num1==3)&&(Timer>50))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=3;
//			}
//			if((flag_a==3)&&(Timer>100))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=4;
//			}
//			if((flag_a==4)&&(Timer>150))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=5;
//			}
//			if((Timer>200)&&(flag_a==5))
//			{
//				Target_Speed=5;
//				Track_Kp=160;
//				flag_a=0;
//				Timer=0;
//				flag_Num1=4;
//			}
//			if((flag_Num1==4)&&(flag_a==0)&&(Timer>50))
//			{
//				Target_Speed=-10;
//				Track_Kp=0;
//				Encoder_S=0;
//				Timer=0;
//				flag_Num1=5;
//			}
//			if((flag_Num1==5)&&(Timer>100))
//			{
//				Target_Speed=20;
//				Track_Kp=160;
//				flag_Num1=6;
//				Timer=150;
//			}
//			if((flag_Num1==6)&&(Timer>200)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;
//				flag_Num1=7;
//			}
//			if((flag_Num1==8)&&(Timer>10))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//				Timer=0;
//				flag_Num1=9;
//				flag_turn=1;
//				Target_Speed=-10;
//				Turn_Kp=10;
//				Turn_Speed=45;
//			}
//			if((flag_turn==1)&&(Timer>82))
//			{
//				flag_turn=0;
//				Target_Speed=40;
//				Timer=0;
//				flag_c=6;
//			}
////			if((flag_turn==0)&&(flag_c==6)&&(Timer>400)&&(flag_Num1<10))
////			{
////				flag_a=0;
////				flag_Num1=20;
////				Track_Kp=160;
////				Timer=0;
////			}
//			if((flag_turn==0)&&(Timer>20)&&(flag_Num1==9)&&((PB1==0)|(PB0==0)|(PA7==0)|(PA6==0)|(PA5==0)|(PA4==0)|(PC15==0)|(PC14==0)))
//			{
//				flag_b=0;
//				flag_Num1=10;
//			}
//			if((flag_Num1==11)&&(Timer>200)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;
//				flag_Num1=12;
//				Encoder_S=0;
//			}
//			if((flag_a==3)&&(Timer>10)&&(flag_Num1==13))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				flag_a=0;
//			}

//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}

//void mode5(void)
//{
//			int PWM_out; 
//			
//			if((flag_Num<5)&&(Timer>=250))
//			{
//				flag_stop=1;
//			}
//			if((flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{					
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=35;Track_Kp=150;}
//					if(flag_Num1==6){flag_a=1;flag_Num1=7;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=45;Track_Kp=180;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			else if((flag_stop==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				if(flag_b==2)
//				{
//					flag_Num+=1;
//					if(flag_Num1==4){flag_a=1;flag_Num1=5;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=0;}
//					if(flag_Num1==8){flag_a=2;flag_Num1=9;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=0;Track_Kp=0;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;				
//				}
//			}
//			if(flag_a==1&&Timer>10)
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			
//			if((flag_a==0)&&(flag_Num1==1)&&((Timer>280)|((PA6==0)&&(PA7==0)&&(PA5==0))))//240
//			{
//				GPIO_SetBits(GPIOA,GPIO_Pin_15);
//				flag_Num1=2;
//				Target_Speed=-1;
//				Encoder_S=0;
//				Track_Kp=0;
//				Timer=0;
//				USART2_Send_Data(9);
//			}
//			if((flag_Num1==2)&&(Timer>100))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_Num1=3;
//				flag_b=3;
//			}
//			if((flag_Num1==3)&&(Timer>600)&&(flag_b==3))
//			{
//				Target_Speed=30;
//				Track_Kp=200;
//				flag_b=1;
//				flag_Num1=4;
//				Timer=0;
//			}
//			if((flag_Num1==4)&&(Timer>100)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;
//				flag_Num1=4;
//			}
//			if((flag_Num1==5)&&(Timer>200)&&((PB0==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				flag_b=0;		
//				flag_Num1=6;
//			}
//			if((flag_Num1==7)&&(Timer>300)&&((PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1)))
//			{
//				flag_b=2;
//				flag_Num1=8;
//				Encoder_S=0;
//			}
//			if((flag_a==2)&&(Timer>10)&&(flag_Num1==9))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				flag_a=0;
//			}
//			
//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}
// 
//void mode6(void)
//{
//			int PWM_out; 
//			
//			if((flag_Num<5)&&(Timer>=250))
//			{
//				flag_stop=1;
//			}
//			
//			if((flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=30;Track_Kp=160;}
//					if(flag_Num1==2){flag_a=2;flag_Num1=3;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=2;Track_Kp=0;Encoder_S=0;}
//					if(flag_Num1==10){flag_a=1;flag_Num1=11;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=180;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			else if((flag_stop==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				if(flag_b==2)
//				{
//				flag_Num+=1;
//				if(flag_Num1==8){flag_a=1;flag_Num1=9;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=50;Track_Kp=0;}
//				if(flag_Num1==12){flag_a=3;flag_Num1=13;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=0;Track_Kp=0;}
//				flag_b=1;
//				Timer=0;
//				flag_stop=0;				
//				}
//			}
//			if(flag_a==1&&Timer>10)
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			
//			if((flag_Num1==1)&&(Timer>100)&&(PA7==0)&&(PA5==0)&&(PA6==0))
//			{
//				flag_b=0;
//				flag_Num1=2;
//			}
//			if((flag_a==2)&&(flag_Num1==3)&&(Timer>10))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=3;
//				USART2_Send_Data(9);
//			}
//			if((flag_a==3)&&(Timer>25))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=4;
//			}
//			if((flag_a==4)&&(Timer>50))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=5;
//			}
//			if((flag_a==5)&&(Timer>75))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=6;
//			}
//			if((flag_a==6)&&(Timer>100))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=7;
//			}
//			if((flag_a==7)&&(Timer>125))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=8;
//			}
//			if((flag_a==8)&&(Timer>150))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=9;
//			}
//			if((flag_a==9)&&(Timer>175))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=10;
//			}
//			if((flag_a==10)&&(Timer>200))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=11;
//			}
//			if((flag_a==11)&&(Timer>225))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=12;
//			}
//			if((flag_a==12)&&(Timer>250))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=13;
//			}
//			if((flag_a==13)&&(Timer>275))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=14;
//			}
//			if((flag_a==14)&&(Timer>300))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_Num1=4;
//				flag_a=15;
//			}
//			if((flag_Num1==4)&&(Timer>300))
//			{
//				Target_Speed=5;
//				Timer=0;
//				flag_a=0;
//				flag_Num1=5;
//			}
//			if((flag_Num1==5)&&(Timer>50))
//			{
//				Target_Speed=-10;
//				Encoder_S=0;
//				Timer=0;
//				flag_Num1=6;
//			}
//			if((flag_Num1==6)&&(Timer>100))
//			{
//				Target_Speed=20;
//				flag_c=6;
//				Track_Kp=160;
//				flag_Num1=7;
//				Timer=250;
//			}
//			if((flag_Num1==7)&&(Timer>300)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;		
//				flag_Num1=8;
//			}
//			if((flag_Num1==9)&&(Timer>200)&&((PB1==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				flag_b=0;		
//				flag_Num1=10;
//			}
//			if((flag_Num1==11)&&(Timer>300)&&((PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1)))
//			{
//				flag_b=2;
//				flag_Num1=12;
//				Encoder_S=0;
//			}
//			if((flag_Num1==13)&&(flag_a==3)&&(Timer>20))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				flag_a=0;
//			}

//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}

//void mode7(void)
//{
//			int PWM_out; 
//			
//			if((flag_Num<5)&&(Timer>=250))
//			{
//				flag_stop=1;
//			}
//			
//			if((flag_stop==1)&&((PB0==0)|(PB1==0)|(PA4==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				if(flag_b==0)
//				{
//					flag_Num+=1;
//					if(flag_Num1==0){flag_a=1;flag_Num1=1;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=30;Track_Kp=160;}
//					if(flag_Num1==2){flag_a=1;flag_Num1=3;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=30;Track_Kp=160;}
//					if(flag_Num1==3){flag_a=2;flag_Num1=4;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=2;Track_Kp=0;Encoder_S=0;}
//					if(flag_Num1==11){flag_a=1;flag_Num1=12;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=40;Track_Kp=180;}
//					flag_b=1;
//					Timer=0;
//					flag_stop=0;
//				}
//			}
//			else if((flag_stop==1)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				if(flag_b==2)
//				{
//				flag_Num+=1;
//				if(flag_Num1==9){flag_a=1;flag_Num1=10;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=50;Track_Kp=0;}
//				if(flag_Num1==13){flag_a=3;flag_Num1=14;GPIO_SetBits(GPIOA,GPIO_Pin_15);Target_Speed=0;Track_Kp=0;}
//				flag_b=1;
//				Timer=0;
//				flag_stop=0;				
//				}
//			}
//			if(flag_a==1&&Timer>10)
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				flag_a=0;
//			}
//			
//			if((flag_Num1==1)&&(Timer>100)&&(PA7==0)&&(PA5==0)&&(PA6==0))
//			{
//				flag_b=0;
//				flag_Num1=2;
//				Timer=0;
//			}
//			if((flag_Num1==2)&&(Timer>300)&&(PA5==0)&&(PA6==0)&&(PA7==0))
//			{
//				flag_b=0;
//				flag_Num1=3;
//			}
//			if((flag_a==2)&&(flag_Num1==4)&&(Timer>10))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=3;
//				USART2_Send_Data(9);
//			}
//			if((flag_a==3)&&(Timer>25))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=4;
//			}
//			if((flag_a==4)&&(Timer>50))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=5;
//			}
//			if((flag_a==5)&&(Timer>75))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=6;
//			}
//			if((flag_a==6)&&(Timer>100))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=7;
//			}
//			if((flag_a==7)&&(Timer>125))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=8;
//			}
//			if((flag_a==8)&&(Timer>150))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=9;
//			}
//			if((flag_a==9)&&(Timer>175))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=10;
//			}
//			if((flag_a==10)&&(Timer>200))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=11;
//			}
//			if((flag_a==11)&&(Timer>225))
//			{
//				Target_Speed=1;
//				Encoder_S=0;
//				flag_a=12;
//			}
//			if((flag_a==12)&&(Timer>250))
//			{
//				Target_Speed=2;
//				Encoder_S=0;
//				flag_a=13;
//			}
//			if((flag_a==13)&&(Timer>275))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_a=14;
//			}
//			if((flag_a==14)&&(Timer>300))
//			{
//				Target_Speed=0;
//				Encoder_S=0;
//				flag_Num1=5;
//				flag_a=15;
//			}
//			if((flag_Num1==5)&&(Timer>300))
//			{
//				Target_Speed=5;
//				Track_Kp=160;
//				Timer=0;
//				flag_a=0;
//				flag_Num1=6;
//			}
//			if((flag_Num1==6)&&(Timer>50))
//			{
//				Target_Speed=-10;
//				Track_Kp=0;
//				Encoder_S=0;
//				Timer=0;
//				flag_Num1=7;
//				flag_c=6;
//			}
//			if((flag_Num1==7)&&(Timer>100))
//			{
//				Target_Speed=20;
//				Track_Kp=160;
//				flag_Num1=8;
//				Timer=250;
//			}
//			if((flag_Num1==8)&&(Timer>300)&&(PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1))
//			{
//				flag_b=2;		
//				flag_Num1=9;
//			}
//			if((flag_Num1==10)&&(Timer>200)&&((PB1==0)|(PA5==0)|(PA6==0)|(PA7==0)|(PC15==0)|(PC14==0)))
//			{
//				flag_b=0;		
//				flag_Num1=11;
//			}
//			if((flag_Num1==12)&&(Timer>300)&&((PB0==1)&&(PA7==1)&&(PA6==1)&&(PA5==1)&&(PA4==1)))
//			{
//				flag_b=2;
//				flag_Num1=13;
//				Encoder_S=0;
//			}
//			if((flag_Num1==14)&&(flag_a==3)&&(Timer>20))
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//				Target_Speed=0;
//				flag_a=0;
//			}

//			Encoder_Left=-Read_Speed(2);//µç»úÊÇÏà¶Ô°²×°£¬¸ÕºÃÏà²î180¶È£¬ÎªÁË±àÂëÆ÷Êä³ö¼«ĞÔÒ»ÖÂ£¬¾ÍĞèÒª¶ÔÆäÖĞÒ»¸öÈ¡·´¡£
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2¡¢½«Êı¾İÑ¹Èë±Õ»·¿ØÖÆÖĞ£¬¼ÆËã³ö¿ØÖÆÊä³öÁ¿¡£
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//ËÙ¶È»·				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//Ö±Á¢»·
//			Turn_out=Read_turn(flag_turn);																								//×ªÏò»·
//			PWM_out=Vertical_out+Velocity_out;//×îÖÕÊä³ö

//			//3¡¢°Ñ¿ØÖÆÊä³öÁ¿¼ÓÔØµ½µç»úÉÏ£¬Íê³É×îÖÕµÄµÄ¿ØÖÆ¡£
//			MOTO1=PWM_out-Turn_out;//×óµç»ú
//			MOTO2=PWM_out+Turn_out;//ÓÒµç»ú
//			Limit(&MOTO1,&MOTO2);	 //PWMÏŞ·ù			
//			Load(MOTO1,MOTO2);		 //¼ÓÔØµ½µç»úÉÏ¡£
//			Stop(&Med_Angle,&roll);
//}

void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum)
{
	int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)
        {
//            case AX:
//            case AY:
            case AZ:
				s_cDataUpdate |= ACC_UPDATE;
            break;
//            case GX:
//            case GY:
            case GZ:
				s_cDataUpdate |= GYRO_UPDATE;
            break;
//            case HX:
//            case HY:
            case HZ:
				s_cDataUpdate |= MAG_UPDATE;
            break;
//            case Roll:
//            case Pitch:
            case Yaw:
				s_cDataUpdate |= ANGLE_UPDATE;
            break;
            default:
				s_cDataUpdate |= READ_UPDATE;
			break;
        }
		uiReg++;
    }
}

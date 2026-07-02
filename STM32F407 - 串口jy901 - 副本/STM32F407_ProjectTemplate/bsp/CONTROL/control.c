#include "control.h"
#include "Tracking.h"
#include "Key.h"

/*目标模块输出双接口，学习并参考开源代码实现功能。确保平衡车稳定运行到功能调试完成。*/
// 陀螺仪数据结构体

extern uint8_t ucRegIndex;
extern uint16_t usRegDataBuff[4];
extern uint32_t uiRegDataLen;

//float Med_Angle=0;		// 机械中值。---初始校准后的平衡车机械中值即零点
float Target_Speed=10;		// 目标速度（前进后退）。---双闭环控制，用于调节小车当前行进的实际速度
float Turn_Speed=0;			// 目标速度（转向）
float Pos_Target=0;		// 单位：
//float
//	Vertical_Kp=300,//280,//270,//+
//	Vertical_Kd=0;//23;//23;//+
float
	Velocity_Kp=-100,//-125,//-120,//-
	Velocity_Ki=-0.5;//-0.625;//-0.6;//-
float
	Turn_Kd=0,//+
	Turn_Kp=0;//

float 
	Track_Kp=0,			//180
	Track_Ki=0,			//1
	Track_Kd=0;			//+ 轨迹控制微分

float  
	diffe_kp=0,
  diffe_ki=0,
  diffe_kd=0;

float 
	Pos_Kp = 5.0f, 
	Pos_Ki = 0.2f,
	Pos_Kd = 0.5f; 

float Pos_Current,Total_Distance,Velocity_Left,Velocity_Right;
static float Pos_Err,Pos_LastErr,Pos_Integral,Pos_Derivative,Pos_Output;
uint8_t Pos_ReachFlag;

int Vertical_out,Velocity_out,Turn_out,Timer=0;// 直立环&速度环&转向环 的输出值
int flag_Num=0,flag_Numdelay=0,Action_speed=0,flag_turn=1,Encoder_S=0;
int flag_a,flag_b,flag_c,flag_Num1,Yaw_Data;
float length;

int Vertical(float Med,float Angle,float gyro_Y);// 函数声明
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
	if (TIM_GetITStatus(TIM7,TIM_IT_Update)!= RESET)// 定时中断触发
	{
			Encoder_Left=-Read_Speed(ENCODER_M3);// 电机测速编码器，由于安装方向相差180度，为使编码器输出正方向一致，此处取反
			Encoder_Right=Read_Speed(ENCODER_M4);

			CopeWitData(ucRegIndex,usRegDataBuff,uiRegDataLen);
			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;

//			Get_Velocity_Form_Encoder(Encoder_Left, Encoder_Right);
//			Update_Distance();
//			if (Pos_Target != 0){PosPID_Calculate();Target_Speed = Pos_Output;}
			
			//2、将数据进行滤波处理，计算控制量
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								// 速度环				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																// 直立环
			Turn_out=Read_turn(flag_turn);																								// 转向环
			PWM_out=Velocity_out;// 总输出

			//3、将控制量加载到电机驱动，实现预期目标
			MOTO1=PWM_out-Turn_out;// 左电机
			MOTO2=PWM_out+Turn_out;// 右电机
			Limit2(&MOTO1,&MOTO2);	 // PWM限幅			
			Load2(MOTO1,MOTO2);		 // 加载到电机驱动
//			Stop(&Med_Angle,&roll);
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
//					Target_Speed=38;
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
//					Target_Speed=38;
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
//		Key_Scan();
//	length=Hcsr04GetLength();
//	LED1_Toggle();
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);// 清除中断标志位
}

/*********************
直立环PD控制：Kp*偏差+Kd*偏差微分

输入：目标角度、实际角度、实际角速度
输出：直立环输出
*********************/
//int Vertical(float Med,float Angle,float gyro_Y)
//{
//	int PWM_out;
//	
//	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
//	return PWM_out;
//}

/*********************
速度环PI：Kp*偏差+Ki*偏差积分
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,EnC_Err_Lowout,EnC_Err_Lowout_last;
	float a=0.8;
	
	//1.计算速度偏差	
	Encoder_Err=((encoder_left+encoder_right)-Target);// 消除静态误差：当速度为"0"时的角度，即为机械中值
	
	//2.对速度偏差进行滤波处理
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;// 使输出更平滑，滤除高频噪声，防止速度波动
	EnC_Err_Lowout_last=EnC_Err_Lowout;// 防止速度波动影响直立环的稳定性
	
	//3.对速度偏差进行积分运算
	Encoder_S+=EnC_Err_Lowout;
	
	//4.积分限幅
	Encoder_S=Encoder_S;
	
	//4.积分限幅
	// 将积分值限制在[-10000, 10000]范围内，防止积分饱和
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);

	// 当stop标志被置位时，清零积分并清除标志
	// 用于紧急停止或重新初始化积分器
//	if(stop==1)Encoder_S=0,stop=0;

	//5.速度环控制量计算
	// 根据滤波后的速度误差和积分值计算最终输出
	// Velocity_Kp: 速度环比例系数，负数表示负反馈
	// Velocity_Ki: 速度环积分系数，负数表示负反馈
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}

/*********************
转向控制环：
公式=Kd*Z轴角速度 + Kp*转向控制输入

说明：
实现一个典型的PD控制器，Kd项用于控制转向的响应速度，
Kp项用于控制转向的强度。
*********************/
int Turn(int gyro_Z, int RC)
{
    int PWM_out;
    // 实现一个标准的PD控制器，Kd用于阻尼，Kp用于比例控制
    PWM_out = Turn_Kd * gyro_Z + Turn_Kp * RC;
    return PWM_out;
}

// 轨迹跟踪控制
// 用于沿着特定轨迹行驶的控制算法
int Track_Turn(int RC)
{
    int PWM_Out;
    float err = 0;
    static float Track_s,last_RC; // 轨迹误差积分项
    
	    // 处理循迹丢失的情况
    if (RC == 0) {
        // 循迹丢失时使用上一次有效值或手动控制输入
        RC = (last_RC != 0) ? last_RC : (float)RC;
    } else {
        // 更新上一次循迹值
        last_RC = RC;
    }
		
    err = RC - 0; // 计算当前轨迹误差
    Track_s += err; // 误差积分
    
		if (Track_s > 100) Track_s = 100;
    if (Track_s < -100) Track_s = -100;
		
		float derivative = err - last_RC;
    // PID控制计算：比例项 + 积分项 + 微分项
    PWM_Out = err * Track_Kp + Track_Ki * Track_s + derivative * Track_Kd;
    return PWM_Out;
}

// 差速控制 - 原地转向
// 通过左右轮速差实现精确转向控制
int different_PD(int16_t Encoder_Left, int16_t Encoder_Right, float Gyro_Z)
{
    int PWM_OUT, Err;
    static float diff_s; // 差速误差积分项
    
    Err = Encoder_Left - Encoder_Right; // 计算左右轮速差
    
    diff_s += Err; // 轮速差积分
    
    // PID控制计算：比例项 + 积分项 + 微分项
    PWM_OUT = diffe_kp * Err + diffe_ki * diff_s + diffe_kd * Gyro_Z;

    return PWM_OUT;
}

// 转向控制模式选择器
// 根据flag_turn参数选择不同的转向控制算法
int16_t Read_turn(int flag_turn)
{
    int16_t Value_turn;
    switch(flag_turn)
    {
        case 0: Value_turn = (int16_t)Track_Turn(Tracking_InnerRing()); break; // 轨迹跟踪模式
        case 1: Value_turn = (int16_t)Turn(gyroz, Turn_Speed); break; // 普通转向模式
        case 2: Value_turn = (int16_t)different_PD(Encoder_Left, Encoder_Right, gyroz); break; // 差速控制模式
    }
    return Value_turn;
}

// 从编码器数据计算实际速度
// 输入：左右编码器计数值
// 输出：左右轮实际线速度(m/s)
void Get_Velocity_Form_Encoder(int encoder_left, int encoder_right)
{
    // 编码器值转换为线速度(m/s)
    // 计算公式：编码器脉冲数 * 轮周长 / (编码器精度 * 减速比 * 编码器倍数 * 采样时间)
    Velocity_Left = (float)encoder_left * PI * Diameter_67 / (Encoder_precision * Reduction_Ratio * EncoderMultiples * DT);
    Velocity_Right = (float)encoder_right * PI * Diameter_67 / (Encoder_precision * Reduction_Ratio * EncoderMultiples * DT);
}

// 位置PID控制器计算
// 计算目标位置与当前位置的误差，并生成速度控制命令
void PosPID_Calculate(void) 
{
    // 计算位置误差
    Pos_Err = Pos_Target - Pos_Current;

    // 误差积分，带积分限幅防止饱和
    Pos_Integral += Pos_Err;
    Pos_Integral = LimitFloat(Pos_Integral, -POS_MAX_INTEGRAL, POS_MAX_INTEGRAL);  

    // 误差微分计算
    Pos_Derivative = (Pos_Err - Pos_LastErr) / DT;

    // PID输出计算，带输出限幅
    Pos_Output = Pos_Kp * Pos_Err + Pos_Ki * Pos_Integral + Pos_Kd * Pos_Derivative;
    Pos_Output = LimitFloat(Pos_Output, -POS_MAX_OUTPUT, POS_MAX_OUTPUT);  

    // 判断是否达到目标位置
    if (fabs(Pos_Err) < POS_ERR_THRESHOLD) {
        Pos_ReachFlag = 1;  // 标记已到达目标位置
        Pos_Output = 0;     // 停止输出，防止过冲
    } else {
        Pos_ReachFlag = 0;  // 未到达目标位置
    }

    // 保存当前误差用于下次微分计算
    Pos_LastErr = Pos_Err;
}

// 更新行驶距离
// 根据编码器数据累计计算行驶总距离
void Update_Distance(void) 
{
    // 计算位移：(左右编码器平均值) * 轮周长 / (编码器精度 * 减速比 * 编码器倍数)
    float displacement = ((float)(Encoder_Left + Encoder_Right) / 2.0f) * 
                         (PI * Diameter_67) / (Encoder_precision * Reduction_Ratio * EncoderMultiples);
    Pos_Current += displacement;      // 更新当前位置
    Total_Distance += fabs(displacement); // 累计总行驶距离
}

// 设置目标行驶距离
// 基于当前位置设置新的目标位置
void Set_Target_Distance(float distance) 
{
    Pos_Target = Pos_Current + distance;  // 设置相对目标位置
    Pos_ReachFlag = 0;  // 重置到达标志
}

// 浮点数限幅函数
// 将值限制在[min, max]范围内
float LimitFloat(float value, float min, float max)
{
    if (value > max) return max;
    else if (value < min) return min;
    else return value;
}

//左到右 PD9 PD8 PD11 PD10 PD13 PD12 PD15 PD14
//void mode1(void)
//{
//			int PWM_out;
//			if(Timer>=200)
//			{
//				flag_stop=1;
//			}
//			if((KeyNum==1)&&(flag_stop==1)&&((PD9==0)|(PD8==0)|(PD11==0)|(PD10==0)|(PD13==0)|(PD12==0)|(PD15==0)|(PD14==0)))
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
//			// 读取编码器数据（负号处理电机安装方向）
//			Encoder_Left = -Read_Speed(ENCODER_M1);  // 左电机编码器（方向调整：根据电机安装方向取反）
//			Encoder_Right = -Read_Speed(ENCODER_M2); // 右电机编码器（方向调整）

//			// 从JY901姿态传感器读取数据（16位整数格式）
//			WitReadReg(AX, 12);  // 读取加速度、角速度、角度数据（共12个寄存器）
//			// 数据转换：将传感器原始数据转换为物理单位（浮点数）角度/角速度/加速度
//			roll=sReg[Roll]/32768.0f * 180.0f;pitch=sReg[Pitch]/32768.0f * 180.0f;yaw=sReg[Yaw]/32768.0f * 180.0f;  
//			gyrox=sReg[GX]/32768.0f * 2000.0f;gyroy=sReg[GY]/32768.0f * 2000.0f;gyroz=sReg[GZ]/32768.0f * 2000.0f;  
//			aacx=sReg[AX]/32768.0f * 16.0f;aacy=sReg[AY]/32768.0f * 16.0f;aacz =sReg[AZ]/32768.0f * 16.0f;  

//			// 2. 三闭环PID控制算法：计算直立、速度、转向三个控制环的输出
//			Velocity_out = Velocity(Target_Speed, Encoder_Left, Encoder_Right);  // 速度环PID输出
////			Vertical_out = Vertical(Med_Angle, roll, gyrox);                     // 直立环PID输出
//			Turn_out = Read_turn(flag_turn);                                     // 转向环PID输出

//			// 合并直立环和速度环输出（核心控制量）
//			PWM_out = Vertical_out + Velocity_out;  // 基础PWM输出 = 直立控制 + 速度控制

//			// 3. 左右电机PWM分配：根据转向控制量调整左右电机输出
//			MOTO1 = PWM_out - Turn_out;  // 左电机PWM = 基础输出 - 转向修正（左转时左电机减速）
//			MOTO2 = PWM_out + Turn_out;  // 右电机PWM = 基础输出 + 转向修正（左转时右电机加速）

//			// 电机输出限幅保护（防止PWM超出范围）
//			Limit(&MOTO1, &MOTO2);  // 限制PWM在安全范围内（如±7200）
//			// 加载PWM值到电机驱动（实际控制电机）
//			Load(MOTO1, MOTO2);  // 通过定时器输出PWM波控制电机转速和方向
//			// 安全检查：判断是否需要停止（如倾角过大）
////			Stop(&Med_Angle, &roll);  // 检测平衡状态，必要时切断电机输出
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

//			// 读取编码器数据（负号处理电机安装方向）
//			Encoder_Left = -Read_Speed(ENCODER_M1);  // 左电机编码器（方向调整：根据电机安装方向取反）
//			Encoder_Right = -Read_Speed(ENCODER_M2); // 右电机编码器（方向调整）

//			// 从JY901姿态传感器读取数据（16位整数格式）
//			WitReadReg(AX, 12);  // 读取加速度、角速度、角度数据（共12个寄存器）
//			// 数据转换：将传感器原始数据转换为物理单位（浮点数）角度/角速度/加速度
//			roll=sReg[Roll]/32768.0f * 180.0f;pitch=sReg[Pitch]/32768.0f * 180.0f;yaw=sReg[Yaw]/32768.0f * 180.0f;  
//			gyrox=sReg[GX]/32768.0f * 2000.0f;gyroy=sReg[GY]/32768.0f * 2000.0f;gyroz=sReg[GZ]/32768.0f * 2000.0f;  
//			aacx=sReg[AX]/32768.0f * 16.0f;aacy=sReg[AY]/32768.0f * 16.0f;aacz =sReg[AZ]/32768.0f * 16.0f;  

//			// 2. 三闭环PID控制算法：计算直立、速度、转向三个控制环的输出
//			Velocity_out = Velocity(Target_Speed, Encoder_Left, Encoder_Right);  // 速度环PID输出
//			Vertical_out = Vertical(Med_Angle, roll, gyrox);                     // 直立环PID输出
//			Turn_out = Read_turn(flag_turn);                                     // 转向环PID输出

//			// 合并直立环和速度环输出（核心控制量）
//			PWM_out = Vertical_out + Velocity_out;  // 基础PWM输出 = 直立控制 + 速度控制

//			// 3. 左右电机PWM分配：根据转向控制量调整左右电机输出
//			MOTO1 = PWM_out - Turn_out;  // 左电机PWM = 基础输出 - 转向修正（左转时左电机减速）
//			MOTO2 = PWM_out + Turn_out;  // 右电机PWM = 基础输出 + 转向修正（左转时右电机加速）

//			// 电机输出限幅保护（防止PWM超出范围）
//			Limit(&MOTO1, &MOTO2);  // 限制PWM在安全范围内（如±7200）
//			// 加载PWM值到电机驱动（实际控制电机）
//			Load(MOTO1, MOTO2);  // 通过定时器输出PWM波控制电机转速和方向
//			// 安全检查：判断是否需要停止（如倾角过大）
//			Stop(&Med_Angle, &roll);  // 检测平衡状态，必要时切断电机输出
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
//			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2��������ѹ��ջ������У�����������������
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//�ٶȻ�				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//ֱ����
//			Turn_out=Read_turn(flag_turn);																								//ת��
//			PWM_out=Vertical_out+Velocity_out;//�������

//			//3���ѿ�����������ص�����ϣ�������յĵĿ��ơ�
//			MOTO1=PWM_out-Turn_out;//����
//			MOTO2=PWM_out+Turn_out;//�ҵ��
//			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
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

//			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2��������ѹ��ջ������У�����������������
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//�ٶȻ�				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//ֱ����
//			Turn_out=Read_turn(flag_turn);																								//ת��
//			PWM_out=Vertical_out+Velocity_out;//�������

//			//3���ѿ�����������ص�����ϣ�������յĵĿ��ơ�
//			MOTO1=PWM_out-Turn_out;//����
//			MOTO2=PWM_out+Turn_out;//�ҵ��
//			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
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
//			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2��������ѹ��ջ������У�����������������
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//�ٶȻ�				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//ֱ����
//			Turn_out=Read_turn(flag_turn);																								//ת��
//			PWM_out=Vertical_out+Velocity_out;//�������

//			//3���ѿ�����������ص�����ϣ�������յĵĿ��ơ�
//			MOTO1=PWM_out-Turn_out;//����
//			MOTO2=PWM_out+Turn_out;//�ҵ��
//			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
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

//			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2��������ѹ��ջ������У�����������������
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//�ٶȻ�				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//ֱ����
//			Turn_out=Read_turn(flag_turn);																								//ת��
//			PWM_out=Vertical_out+Velocity_out;//�������

//			//3���ѿ�����������ص�����ϣ�������յĵĿ��ơ�
//			MOTO1=PWM_out-Turn_out;//����
//			MOTO2=PWM_out+Turn_out;//�ҵ��
//			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
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

//			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=-Read_Speed(4);

//			WitReadReg(AX,12);
//			roll=sReg[Roll] / 32768.0f * 180.0f;pitch=sReg[Pitch] / 32768.0f * 180.0f;yaw=sReg[Yaw] / 32768.0f * 180.0f;
//			gyrox=sReg[GX]/ 32768.0f * 2000.0f;gyroy=sReg[GY]/ 32768.0f * 2000.0f;gyroz=sReg[GZ]/ 32768.0f * 2000.0f;
//			aacx=sReg[AX]/ 32768.0f * 16.0f;aacy=sReg[AY]/ 32768.0f * 16.0f;aacz=sReg[AZ]/ 32768.0f * 16.0f;
//			
//			if(s_cDataUpdate & GYRO_UPDATE)s_cDataUpdate &= ~GYRO_UPDATE;
//			if(s_cDataUpdate & ANGLE_UPDATE)s_cDataUpdate &= ~ANGLE_UPDATE;
//			
//			//2��������ѹ��ջ������У�����������������
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);								//�ٶȻ�				
//			Vertical_out=Vertical(Med_Angle,roll,gyrox);																	//ֱ����
//			Turn_out=Read_turn(flag_turn);																								//ת��
//			PWM_out=Vertical_out+Velocity_out;//�������

//			//3���ѿ�����������ص�����ϣ�������յĵĿ��ơ�
//			MOTO1=PWM_out-Turn_out;//����
//			MOTO2=PWM_out+Turn_out;//�ҵ��
//			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
//			Stop(&Med_Angle,&roll);
//}

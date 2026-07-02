#ifndef  _CONTROL_H
#define  _CONTROL_H

#include "sys.h" 
#include "math.h"

#define POS_MAX_OUTPUT 300      // 最大速度
#define POS_MAX_INTEGRAL 500    // 积分限幅
#define POS_ERR_THRESHOLD 0.01f // 误差
#define DT 0.01f               // (10ms)

#define PI 3.1415926f
#define Diameter_67 0.067f   //直径,单位-m
#define Control_Frequency 100.0f //F-频率 100hz,10ms
#define EncoderMultiples 4      //编码器倍频系数
#define Reduction_Ratio 30.0f   //减速比
#define Encoder_precision 13.0f //编码器精度 ppr

extern float Pos_Target;       
extern float Pos_Current;    
extern float Total_Distance;   
extern uint8_t Pos_ReachFlag;  

extern float Pos_Kp, Pos_Ki, Pos_Kd;

void PosPID_Calculate(void);
void Update_Distance(void);
void Set_Target_Distance(float distance);
float LimitFloat(float value, float min, float max);
void Get_Velocity_Form_Encoder(int encoder_left, int encoder_right);

//int Vertical(float Med,float Angle,float gyro_Y);
//int Velocity(int Target,int encoder_left,int encoder_right);
int Velocity_Left(int Target, int encoder_left);
int Velocity_Right(int Target, int encoder_right);
int Turn(int gyro_Z,int RC);
void TIM7_IRQHandler(void);

#endif

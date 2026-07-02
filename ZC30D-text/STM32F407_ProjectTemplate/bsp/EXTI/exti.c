#include "exti.h"



void TIM7_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 9999;        // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 8399;   // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM7, ENABLE);
}

///**
//  * TIM3定时器中断服务函数
//  * 功能：处理TIM3定时器更新中断，执行平衡车控制算法的主循环
//  * 中断频率：由TIM3的ARR和PSC寄存器配置决定，通常设置为5-10ms
//  */
//void TIM3_IRQHandler(void)
//{
//    // 检查定时器更新中断标志位
//    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
//    {
//        // 清除中断标志位，防止重复触发中断
//        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//        
//        // 1. 读取传感器数据
//        // 读取MPU6050陀螺仪和加速度计数据
//        MPU_Get_Gyroscope(&gx, &gy, &gz);
//        MPU_Get_Accelerometer(&ax, &ay, &az);
//        
//        // 2. 姿态解算
//        // 使用互补滤波或卡尔曼滤波融合传感器数据，计算倾角
//        Get_Angle(ax, ay, az, gx, gy, gz);
//        
//        // 3. 读取编码器数据
//        // 获取左右轮编码器计数值
//        Encoder_Left = (short)TIM_GetCounter(TIM2);
//        Encoder_Right = -(short)TIM_GetCounter(TIM4); // 右轮编码器反向安装
//        TIM_SetCounter(TIM2, 0);  // 清零编码器计数
//        TIM_SetCounter(TIM4, 0);  // 清零编码器计数
//        
//        // 4. 计算实际速度
//        Get_Velocity_Form_Encoder(Encoder_Left, Encoder_Right);
//        
//        // 5. 更新行驶距离
//        Update_Distance();
//        
//        // 6. 位置环计算（如果启用位置控制）
//        if (Position_Control_Enable) {
//            PosPID_Calculate();
//            Velocity_Target = Pos_Output;  // 位置环输出作为速度环输入
//        }
//        
//        // 7. 直立环控制计算
//        Vertical_out = Vertical(Angle_Balance, Gyro_Balance);
//        
//        // 8. 速度环控制计算
//        Velocity_out = Velocity(Velocity_Target, Encoder_Left, Encoder_Right);
//        
//        // 9. 转向环控制计算
//        Turn_out = Read_turn(Turn_Mode);
//        
//        // 10. 计算最终PWM输出
//        // 组合三个控制环的输出，得到左右电机PWM控制量
//        Moto1 = Vertical_out + Velocity_out - Turn_out;  // 左电机
//        Moto2 = Vertical_out + Velocity_out + Turn_out;  // 右电机
//        
//        // 11. PWM输出限幅
//        Moto1 = LIMIT(Moto1, -PWM_MAX, PWM_MAX);
//        Moto2 = LIMIT(Moto2, -PWM_MAX, PWM_MAX);
//        
//        // 12. 设置电机PWM
//        Set_Pwm(Moto1, Moto2);
//        
//        // 13. 其他任务（如LED闪烁、数据上传等）
//        // 可添加调试信息或状态指示代码
//    }
//}



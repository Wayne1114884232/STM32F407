#ifndef __GLOBAL_VARS_H
#define __GLOBAL_VARS_H

#include "sys.h"

// 电机控制参数
#define PWM_MAX 1000
#define PWM_MIN -1000

// 传感器数据结构体
typedef struct {
    int16_t roll;    // 横滚角
    int16_t pitch;   // 俯仰角
    int16_t yaw;     // 偏航角
    int16_t acc[3];  // 加速度数据 [X, Y, Z]
    int16_t gyro[3]; // 陀螺仪数据 [X, Y, Z]
    int16_t mag[3];  // 磁力计数据 [X, Y, Z]
} Sensor_Data_t;

// 电机控制结构体
typedef struct {
    int encoder_left;    // 左电机编码器值
    int encoder_right;   // 右电机编码器值
    int moto1;           // 左电机PWM输出值
    int moto2;           // 右电机PWM输出值
    uint8_t flag_stop;   // 停车标志位
} Motor_Control_t;

// 系统状态结构体
typedef struct {
    uint8_t keynum1;     // 按键1状态
    uint8_t keynum2;     // 按键2状态
    uint8_t keynum3;     // 按键3状态
    uint8_t keynum4;     // 按键4状态
    uint16_t adc_value;  // ADC采样值
    uint32_t timer;      // 系统计时器
} System_State_t;

// 全局变量声明
extern Sensor_Data_t g_sensor_data;
extern Motor_Control_t g_motor_ctrl;
extern System_State_t g_system_state;

// 菜单变量结构体
typedef struct {
    char name[20];         // 变量名称
    int32_t* var_ptr;      // 指向全局变量的指针
    int32_t min_val;       // 最小值
    int32_t max_val;       // 最大值
    int32_t step_normal;   // 普通步长（短按）
    int32_t step_fast;     // 快速步长（长按）
} MenuVar_TypeDef;

// 模式选择变量
extern int32_t MODE;
// 全局变量管理数组
extern MenuVar_TypeDef menu_vars[];
extern uint8_t menu_var_count;

#endif


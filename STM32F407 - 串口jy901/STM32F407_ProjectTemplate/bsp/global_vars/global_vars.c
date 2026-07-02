#include "global_vars.h"

// 定义全局变量
Sensor_Data_t g_sensor_data = {0};
Motor_Control_t g_motor_ctrl = {0};
System_State_t g_system_state = {0};

// 模式选择变量
int32_t MODE=0;

// 菜单变量配置（名称、指针、范围、普通步长、快速步长）
MenuVar_TypeDef menu_vars[] = {
    {"MODE",         &MODE,          0,  10,  1,  8},    // 短按±1，长按±8
    // 可以根据需要添加更多可配置变量
};

// 计算菜单变量数量
uint8_t menu_var_count = sizeof(menu_vars) / sizeof(MenuVar_TypeDef);


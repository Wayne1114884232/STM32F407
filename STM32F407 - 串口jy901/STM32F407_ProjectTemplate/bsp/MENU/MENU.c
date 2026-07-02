//#include "menu.h"
//#include "delay.h"
//#include "global_vars.h"
//#include <stdio.h>

//// 菜单状态变量
//MenuMode current_mode = MODE_NORMAL;
//uint8_t current_menu_index = 0;
//KeyID_TypeDef current_key;
//KeyEventType_TypeDef current_event;
//int32_t temp_var_value;  // 临时存储变量原始值

///**
// * @brief  菜单初始化
// */
//void Menu_Init(void) {
//    OLED_Clear();
//    OLED_ShowString(0, 0, (uint8_t *)"System Ready", 16, 0);
//    delay_ms(1000);
//}

///**
// * @brief  按键处理逻辑
// */
//static void Key_Handle(void) {
//    uint8_t has_event = Key_GetEvent(&current_key, &current_event);
//    if (!has_event) return;

//    // 1. 正常模式：长按KEY_1进入一级菜单
//    if (current_mode == MODE_NORMAL) {
//        if (current_key == KEY_1 && current_event == KEY_EVENT_LONG_PRESS) {
//            current_mode = MODE_MENU_SELECT;
//            current_menu_index = 0;
//        }
//        return;
//    }

//    // 2. 一级菜单（变量选择）
//    if (current_mode == MODE_MENU_SELECT) {
//        switch (current_key) {
//            case KEY_1:  // 返回正常模式
//                current_mode = MODE_NORMAL;
//                break;
//            case KEY_2:  // 上选变量
//                current_menu_index = (current_menu_index == 0) ? 
//                                   (menu_var_count-1) : (current_menu_index-1);
//                break;
//            case KEY_3:  // 下选变量
//                current_menu_index = (current_menu_index == menu_var_count-1) ? 
//                                   0 : (current_menu_index+1);
//                break;
//            case KEY_4:  // 进入二级菜单（修改值）
//                current_mode = MODE_VALUE_EDIT;
//                temp_var_value = *menu_vars[current_menu_index].var_ptr;  // 保存原始值
//                break;
//            default:
//                break;
//        }
//        return;
//    }

//    // 3. 二级菜单（数值修改）
//    if (current_mode == MODE_VALUE_EDIT) {
//        MenuVar_TypeDef* var = &menu_vars[current_menu_index];
//        int32_t step = (current_event == KEY_EVENT_LONG_PRESS) ? 
//                     var->step_fast : var->step_normal;  // 长按用快速步长
//        
//        switch (current_key) {
//            case KEY_1:  // 不保存，返回一级菜单
//                *var->var_ptr = temp_var_value;  // 恢复原始值
//                current_mode = MODE_MENU_SELECT;
//                break;
//            case KEY_2:  // 增加值
//                *var->var_ptr = (*var->var_ptr + step > var->max_val) ? 
//                              var->max_val : (*var->var_ptr + step);
//                break;
//            case KEY_3:  // 减少值
//                *var->var_ptr = (*var->var_ptr - step < var->min_val) ? 
//                              var->min_val : (*var->var_ptr - step);
//                break;
//            case KEY_4:  // 保存值，返回一级菜单
//                current_mode = MODE_MENU_SELECT;
//                break;
//            default:
//                break;
//        }
//    }
//}

///**
// * @brief  正常模式显示（优化字体大小和布局）
// */
//void OLED_Show_Normal(void) {
//    OLED_Clear();
//    OLED_ShowString(0, 0, (uint8_t *)"Normal Mode", 8, 0);  // 标题用12号字
//    
//    char buf[20];
//    // 显示系统参数（8号字，紧凑布局）
//		sprintf(buf, "Roll:%lf", roll);
//    OLED_ShowString(0, 12, (uint8_t *)buf, 8, 0);
//    
//    sprintf(buf, "Pitch: %lf", pitch);
//    OLED_ShowString(0, 20, (uint8_t *)buf, 8, 0);
//    
//    sprintf(buf, "Yaw: %lf", yaw);
//    OLED_ShowString(0, 28, (uint8_t *)buf, 8, 0);
//    
//    sprintf(buf, "Speed_L: %d", Encoder_Left);
//    OLED_ShowString(0, 36, (uint8_t *)buf, 8, 0);
//    
//    sprintf(buf, "MODE: %d°", MODE);
//    OLED_ShowString(0, 44, (uint8_t *)buf, 8, 0);
//    
////    sprintf(buf, "PD9: %d°", PD9);
////    OLED_ShowString(0, 8, (uint8_t *)buf, 8, 0);
////		sprintf(buf, "PD8: %d°", PD8);
////    OLED_ShowString(0, 16, (uint8_t *)buf, 8, 0);
////		sprintf(buf, "PD11: %d°", PD11);
////    OLED_ShowString(0, 24, (uint8_t *)buf, 8, 0);
////	  sprintf(buf, "PD10: %d°", PD10);
////    OLED_ShowString(0, 32, (uint8_t *)buf, 8, 0);
////		sprintf(buf, "PD13: %d°", PD13);
////    OLED_ShowString(0, 40, (uint8_t *)buf, 8, 0);
////		sprintf(buf, "PD12: %d°", PD12);
////    OLED_ShowString(0, 48, (uint8_t *)buf, 8, 0);
////		sprintf(buf, "PD15: %d°", PD15);
////    OLED_ShowString(0, 56, (uint8_t *)buf, 8, 0);
////		sprintf(buf, "PD14: %d°", PD14);
////    OLED_ShowString(0, 64, (uint8_t *)buf, 8, 0);
//    
//    // 操作提示（12号字）
//    OLED_ShowString(0, 60, (uint8_t *)"Long KEY1: Menu", 12, 0);
//}

///**
// * @brief  一级菜单显示（优化字体大小和布局）
// */
//static void OLED_Show_MenuSelect(void) {
//    OLED_Clear();
//    OLED_ShowString(0, 0, (uint8_t *)"Select Variable", 12, 0);  // 标题12号字
//    
//    // 变量列表（8号字，每行间隔8像素）
//    uint8_t visible_items = menu_var_count > 7 ? 7 : menu_var_count;  // 最多显示7项
//    
//    // 计算显示偏移（确保当前选中项可见）
//    uint8_t start_index = 0;
//    if (current_menu_index >= visible_items) {
//        start_index = current_menu_index - visible_items + 1;
//    }
//    
//    for (uint8_t i = 0; i < visible_items; i++) {
//        uint8_t real_index = start_index + i;
//        uint8_t is_selected = (real_index == current_menu_index);
//        
//        // 选中项用">"标记并反显
//        OLED_ShowString(0, 12 + i*8, 
//                      is_selected ? (uint8_t *)">" : (uint8_t *)" ", 
//                      8, is_selected);
//        
//        // 显示变量名称（8号字）
//        OLED_ShowString(16, 12 + i*8, (uint8_t *)menu_vars[real_index].name, 8, is_selected);
//    }
//    
//    // 操作提示（12号字）
//    OLED_ShowString(0, 60, (uint8_t *)"KEY1:Back KEY4:Edit", 12, 0);
//}

///**
// * @brief  二级菜单显示（优化字体大小和布局）
// */
//static void OLED_Show_ValueEdit(void) {
//    OLED_Clear();
//    MenuVar_TypeDef* var = &menu_vars[current_menu_index];
//    
//    // 变量名称（12号字）
//    OLED_ShowString(0, 0, (uint8_t *)var->name, 12, 0);
//    
//    // 当前值（16号字，突出显示）
//    char buf[20];
//    sprintf(buf, "Value: %d", *var->var_ptr);
//    OLED_ShowString(0, 16, (uint8_t *)buf, 12, 0);
//    
//    // 步长提示（8号字）
//    sprintf(buf, "Short±%d Long±%d", var->step_normal, var->step_fast);
//    OLED_ShowString(0, 36, (uint8_t *)buf, 8, 0);
//    
//    // 操作提示（12号字）
//    OLED_ShowString(0, 48, (uint8_t *)"KEY1:Cancel KEY4:Save", 12, 0);
//}

///**
// * @brief  菜单主循环
// */
//void Menu_Update(void) {
//    Key_Handle();  // 处理按键事件

//    // 根据当前模式更新显示
//    switch (current_mode) {
//        case MODE_NORMAL:
//            OLED_Show_Normal();
//            break;
//        case MODE_MENU_SELECT:
//            OLED_Show_MenuSelect();
//            break;
//        case MODE_VALUE_EDIT:
//            OLED_Show_ValueEdit();
//            break;
//    }
//    OLED_Refresh();
//}


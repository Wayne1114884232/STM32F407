#include "key.h"
#include "delay.h"

//// 按键状态结构体
//typedef struct {
//    uint8_t debounce_cnt;          // 消抖计数器
//    uint8_t is_pressed;            // 按键是否按下(消抖后)
//    uint16_t press_time;           // 按下持续时间(ms)
//    uint8_t has_event;             // 是否有未处理事件
//    KeyEventType_TypeDef event;    // 事件类型
//} KeyState_TypeDef;


// 全局变量
int KeyNum1,KeyNum2,KeyNum3,KeyNum4;  // 外部引用的按键编号
//static KeyState_TypeDef key_states[4]; // 四个按键的状态

/**
 * @brief  初始化所有按键
 * @param  无
 * @retval 无
 */
void Key_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能所有按键对应的GPIO时钟
    RCC_AHB1PeriphClockCmd(BUTTON1_RCC | BUTTON2_RCC | BUTTON3_RCC | BUTTON4_RCC, ENABLE);
    
    // 配置所有按键为上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      // 上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
    
    // 配置PC5 (按键1)
    GPIO_InitStructure.GPIO_Pin = BUTTON1_PIN;
    GPIO_Init(BUTTON1_PORT, &GPIO_InitStructure);
    
    // 配置PE7 (按键2)
    GPIO_InitStructure.GPIO_Pin = BUTTON2_PIN;
    GPIO_Init(BUTTON2_PORT, &GPIO_InitStructure);
    
    // 配置PB1 (按键3)
    GPIO_InitStructure.GPIO_Pin = BUTTON3_PIN;
    GPIO_Init(BUTTON3_PORT, &GPIO_InitStructure);
    
    // 配置PE8 (按键4)
    GPIO_InitStructure.GPIO_Pin = BUTTON4_PIN;
    GPIO_Init(BUTTON4_PORT, &GPIO_InitStructure);
    
//    // 初始化按键状态
//    for (uint8_t i = 0; i < 4; i++) {
//        key_states[i].debounce_cnt = 0;
//        key_states[i].is_pressed = 0;
//        key_states[i].press_time = 0;
//        key_states[i].has_event = 0;
//        key_states[i].event = KEY_EVENT_NONE;
//    }
}

u8 click1(void)
{
	static u8 flag_key=1;
	if(flag_key&&Button1==0) 
	{
		flag_key=0;
		return 1;
	}
	else if(1==Button1)			flag_key=1;
	return 0;
}
u8 click2(void)
{
	static u8 flag_key=1;
	if(flag_key&&Button2==0) 
	{
		flag_key=0;
		return 1;
	}
	else if(1==Button2)			flag_key=1;
	return 0;
}
u8 click3(void)
{
	static u8 flag_key=1;
	if(flag_key&&Button3==0) 
	{
		flag_key=0;
		return 1;
	}
	else if(1==Button3)			flag_key=1;
	return 0;
}
u8 click4(void)
{
	static u8 flag_key=1;
	if(flag_key&&Button4==0) 
	{
		flag_key=0;
		return 1;
	}
	else if(1==Button4)			flag_key=1;
	return 0;
}
///**
// * @brief  扫描所有按键（需在定时中断中调用，建议1ms调用一次）
// * @param  无
// * @retval 当前按下的按键ID（若多个按键同时按下，返回ID最小的按键）
// */
//KeyID_TypeDef Key_Scan(void) {
//    KeyID_TypeDef pressed_key = KEY_NONE;
//    
//    // 扫描按键1 (PC5)
//    if (GPIO_ReadInputDataBit(BUTTON1_PORT, BUTTON1_PIN) == Bit_RESET) {
//        if (key_states[0].debounce_cnt < KEY_DEBOUNCE_TIME) {
//            key_states[0].debounce_cnt++;
//            if (key_states[0].debounce_cnt >= KEY_DEBOUNCE_TIME) {
//                key_states[0].is_pressed = 1;
//                key_states[0].press_time = 0;
//                if (pressed_key == KEY_NONE) pressed_key = KEY_1;
//            }
//        } else if (key_states[0].is_pressed) {
//            key_states[0].press_time++;
//            if (key_states[0].press_time == KEY_LONG_PRESS_TIME) {
//                key_states[0].has_event = 1;
//                key_states[0].event = KEY_EVENT_LONG_PRESS;
//            }
//        }
//    } else {
//        if (key_states[0].is_pressed) {
//            key_states[0].is_pressed = 0;
//            if (key_states[0].press_time < KEY_LONG_PRESS_TIME && key_states[0].press_time >= KEY_DEBOUNCE_TIME) {
//                key_states[0].has_event = 1;
//                key_states[0].event = KEY_EVENT_CLICK;
//            }
//        }
//        key_states[0].debounce_cnt = 0;
//    }
//    
//    // 扫描按键2 (PE7)
//    if (GPIO_ReadInputDataBit(BUTTON2_PORT, BUTTON2_PIN) == Bit_RESET) {
//        if (key_states[1].debounce_cnt < KEY_DEBOUNCE_TIME) {
//            key_states[1].debounce_cnt++;
//            if (key_states[1].debounce_cnt >= KEY_DEBOUNCE_TIME) {
//                key_states[1].is_pressed = 1;
//                key_states[1].press_time = 0;
//                if (pressed_key == KEY_NONE) pressed_key = KEY_2;
//            }
//        } else if (key_states[1].is_pressed) {
//            key_states[1].press_time++;
//            if (key_states[1].press_time == KEY_LONG_PRESS_TIME) {
//                key_states[1].has_event = 1;
//                key_states[1].event = KEY_EVENT_LONG_PRESS;
//            }
//        }
//    } else {
//        if (key_states[1].is_pressed) {
//            key_states[1].is_pressed = 0;
//            if (key_states[1].press_time < KEY_LONG_PRESS_TIME && key_states[1].press_time >= KEY_DEBOUNCE_TIME) {
//                key_states[1].has_event = 1;
//                key_states[1].event = KEY_EVENT_CLICK;
//            }
//        }
//        key_states[1].debounce_cnt = 0;
//    }
//    
//    // 扫描按键3 (PB1)
//    if (GPIO_ReadInputDataBit(BUTTON3_PORT, BUTTON3_PIN) == Bit_RESET) {
//        if (key_states[2].debounce_cnt < KEY_DEBOUNCE_TIME) {
//            key_states[2].debounce_cnt++;
//            if (key_states[2].debounce_cnt >= KEY_DEBOUNCE_TIME) {
//                key_states[2].is_pressed = 1;
//                key_states[2].press_time = 0;
//                if (pressed_key == KEY_NONE) pressed_key = KEY_3;
//            }
//        } else if (key_states[2].is_pressed) {
//            key_states[2].press_time++;
//            if (key_states[2].press_time == KEY_LONG_PRESS_TIME) {
//                key_states[2].has_event = 1;
//                key_states[2].event = KEY_EVENT_LONG_PRESS;
//            }
//        }
//    } else {
//        if (key_states[2].is_pressed) {
//            key_states[2].is_pressed = 0;
//            if (key_states[2].press_time < KEY_LONG_PRESS_TIME && key_states[2].press_time >= KEY_DEBOUNCE_TIME) {
//                key_states[2].has_event = 1;
//                key_states[2].event = KEY_EVENT_CLICK;
//            }
//        }
//        key_states[2].debounce_cnt = 0;
//    }
//    
//    // 扫描按键4 (PE8)
//    if (GPIO_ReadInputDataBit(BUTTON4_PORT, BUTTON4_PIN) == Bit_RESET) {
//        if (key_states[3].debounce_cnt < KEY_DEBOUNCE_TIME) {
//            key_states[3].debounce_cnt++;
//            if (key_states[3].debounce_cnt >= KEY_DEBOUNCE_TIME) {
//                key_states[3].is_pressed = 1;
//                key_states[3].press_time = 0;
//                if (pressed_key == KEY_NONE) pressed_key = KEY_4;
//            }
//        } else if (key_states[3].is_pressed) {
//            key_states[3].press_time++;
//            if (key_states[3].press_time == KEY_LONG_PRESS_TIME) {
//                key_states[3].has_event = 1;
//                key_states[3].event = KEY_EVENT_LONG_PRESS;
//            }
//        }
//    } else {
//        if (key_states[3].is_pressed) {
//            key_states[3].is_pressed = 0;
//            if (key_states[3].press_time < KEY_LONG_PRESS_TIME && key_states[3].press_time >= KEY_DEBOUNCE_TIME) {
//                key_states[3].has_event = 1;
//                key_states[3].event = KEY_EVENT_CLICK;
//            }
//        }
//        key_states[3].debounce_cnt = 0;
//    }
//    
//    return pressed_key;
//}

///**
// * @brief  获取当前按下的按键ID（兼容旧版接口）
// * @param  无
// * @retval 当前按下的按键ID（无按键按下返回KEY_NONE）
// */
//KeyID_TypeDef Key_GetNum(void) {
//    if (key_states[0].is_pressed) return KEY_1;
//    if (key_states[1].is_pressed) return KEY_2;
//    if (key_states[2].is_pressed) return KEY_3;
//    if (key_states[3].is_pressed) return KEY_4;
//    return KEY_NONE;
//}

///**
// * @brief  检测按键单击事件（兼容旧版接口）
// * @param  无
// * @retval 1-检测到单击事件，0-未检测到
// */
//u8 click(void) {
//    for (uint8_t i = 0; i < 4; i++) {
//        if (key_states[i].has_event && key_states[i].event == KEY_EVENT_CLICK) {
//            key_states[i].has_event = 0;
//            return 1;
//        }
//    }
//    return 0;
//}

///**
// * @brief  检查指定按键是否被按下
// * @param  key: 要检查的按键ID
// * @retval 1-按键被按下，0-按键未被按下
// */
//u8 Key_IsPressed(KeyID_TypeDef key) {
//    if (key >= KEY_1 && key <= KEY_4) {
//        return key_states[key - 1].is_pressed;
//    }
//    return 0;
//}

///**
// * @brief  检查两个按键是否同时被按下
// * @param  key1, key2: 要检查的按键ID
// * @retval 1-两个按键同时被按下，0-未同时按下
// */
//u8 Key_IsComboPressed(KeyID_TypeDef key1, KeyID_TypeDef key2) {
//    if (key1 < KEY_1 || key1 > KEY_4 || key2 < KEY_1 || key2 > KEY_4) {
//        return 0;
//    }
//    return (key_states[key1 - 1].is_pressed && key_states[key2 - 1].is_pressed);
//}

///**
// * @brief  获取按键事件
// * @param  key: 指向存储按键ID的变量
// * @param  event: 指向存储事件类型的变量
// * @retval 1-有事件发生，0-无事件
// */
//u8 Key_GetEvent(KeyID_TypeDef* key, KeyEventType_TypeDef* event) {
//    for (uint8_t i = 0; i < 4; i++) {
//        if (key_states[i].has_event) {
//            *key = (KeyID_TypeDef)(i + 1);
//            *event = key_states[i].event;
//            key_states[i].has_event = 0;
//            return 1;
//        }
//    }
//    return 0;
//}


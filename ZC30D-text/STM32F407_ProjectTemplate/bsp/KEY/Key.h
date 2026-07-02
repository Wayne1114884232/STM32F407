#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define BUTTON1_PIN    GPIO_Pin_5    // PC5 (按键1)
#define BUTTON1_PORT   GPIOC
#define BUTTON1_RCC    RCC_AHB1Periph_GPIOC

#define BUTTON2_PIN    GPIO_Pin_7    // PE7 (按键2)
#define BUTTON2_PORT   GPIOE
#define BUTTON2_RCC    RCC_AHB1Periph_GPIOE

#define BUTTON3_PIN    GPIO_Pin_1    // PB1 (按键3)
#define BUTTON3_PORT   GPIOB
#define BUTTON3_RCC    RCC_AHB1Periph_GPIOB

#define BUTTON4_PIN    GPIO_Pin_8    // PE8 (按键4)
#define BUTTON4_PORT   GPIOE
#define BUTTON4_RCC    RCC_AHB1Periph_GPIOE

#define Button1 PCin(5)
#define Button2 PEin(7)
#define Button3 PBin(1)
#define Button4 PEin(8)

void Key_Init(void);
u8 click1(void);
u8 click2(void);
u8 click3(void);
u8 click4(void);
//// 按键消抖相关参数
//#define KEY_DEBOUNCE_TIME   2      // 消抖时间(ms)
//#define KEY_LONG_PRESS_TIME 200    // 长按时间(ms) - 改为3秒

//typedef enum {
//    KEY_NONE = 0, 
//    KEY_1    = 1,  // PC5
//    KEY_2    = 2,  // PE7
//    KEY_3    = 3,  // PB1
//    KEY_4    = 4   // PE8
//} KeyID_TypeDef;

//typedef enum {
//    KEY_EVENT_NONE = 0,     
//    KEY_EVENT_CLICK = 1,     
//    KEY_EVENT_LONG_PRESS = 2 
//} KeyEventType_TypeDef;

//void Key_Init(void);                          
//KeyID_TypeDef Key_Scan(void);
//KeyID_TypeDef Key_GetNum(void);               
//u8 click(void);                             
//u8 Key_IsPressed(KeyID_TypeDef key);         
//u8 Key_IsComboPressed(KeyID_TypeDef key1, KeyID_TypeDef key2); 
//u8 Key_GetEvent(KeyID_TypeDef* key, KeyEventType_TypeDef* event); 

#endif


#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define BUTTON1_PIN    GPIO_Pin_5    // PC5
#define BUTTON1_PORT   GPIOC
#define BUTTON1_RCC    RCC_AHB1Periph_GPIOC

#define BUTTON2_PIN    GPIO_Pin_7    // PE7
#define BUTTON2_PORT   GPIOE
#define BUTTON2_RCC    RCC_AHB1Periph_GPIOE

#define BUTTON3_PIN    GPIO_Pin_1    // PB1
#define BUTTON3_PORT   GPIOB
#define BUTTON3_RCC    RCC_AHB1Periph_GPIOB

#define BUTTON4_PIN    GPIO_Pin_8    // PE8
#define BUTTON4_PORT   GPIOE
#define BUTTON4_RCC    RCC_AHB1Periph_GPIOE

typedef enum {
    KEY_NONE = 0, 
    KEY_1    = 1,  // PC5
    KEY_2    = 2,  // PE7
    KEY_3    = 3,  // PB1
    KEY_4    = 4   // PE8
} KeyID_TypeDef;

typedef enum {
    KEY_EVENT_NONE = 0,     
    KEY_EVENT_CLICK = 1,     
    KEY_EVENT_LONG_PRESS = 2 
} KeyEventType_TypeDef;

void Key_Init(void);                          
KeyID_TypeDef Key_GetNum(void);               
u8 click(void);                             
u8 Key_IsPressed(KeyID_TypeDef key);         
u8 Key_IsComboPressed(KeyID_TypeDef key1, KeyID_TypeDef key2); 
u8 Key_GetEvent(KeyID_TypeDef* key, KeyEventType_TypeDef* event); 

#endif


//#include "bsp_ultrasonic.h"

//// State machine for ultrasonic sensor
//typedef enum {
//    HCSR04_IDLE,         // Idle state
//    HCSR04_TRIG_SEND,    // Sending trigger signal
//    HCSR04_WAIT_RISE,    // Waiting for ECHO rising edge
//    HCSR04_WAIT_FALL,    // Waiting for ECHO falling edge
//    HCSR04_CALCULATE     // Calculating distance
//} HCSR04_State;

//// Global variables
//static HCSR04_State state = HCSR04_IDLE;
//static uint32_t start_time = 0;    // ECHO rising edge time (us)
//static uint32_t end_time = 0;      // ECHO falling edge time (us)
//static uint32_t measure_count = 0; // Measurement counter
//static float distance_buf[10];     // Distance buffer (10 measurements)
//static float distance = 0.0f;      // Final distance (cm)
//static uint32_t trig_start_us = 0; // Trigger start time (us, from TIM2)
//static uint32_t last_measure_ms = 0; // Last measurement time (ms)

//// Constants (TRIG_DURATION will be used)
//static const uint32_t TRIG_DURATION = 20;    // Trigger pulse width (us)
//static const uint32_t MEASURE_INTERVAL = 100; // Measurement interval (ms)
//static const uint32_t MAX_TIMEOUT_US = 50000; // Maximum timeout (us)

//// Get system time in milliseconds (requires SysTick initialized)
//uint32_t GetSysTickMs(void) {
//    return SysTick->VAL / (SystemCoreClock / 1000);
//}

//// Initialize timer for timing (1us resolution)
//static void HCSR04_Timer_Init(void) {
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//    RCC_APB1PeriphClockCmd(RCC_TIMER, ENABLE);

//    // 84MHz / 84 = 1MHz ? 1 count = 1us
//    TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
//    TIM_TimeBaseStructure.TIM_Prescaler = 83;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(PORT_TIMER, &TIM_TimeBaseStructure);
//    
//    TIM_ITConfig(PORT_TIMER, TIM_IT_Update, ENABLE);
//    TIM_Cmd(PORT_TIMER, ENABLE);
//}

//// Initialize GPIO (TRIG output, ECHO input with interrupt)
//static void HCSR04_GPIO_Init(void) {
//    GPIO_InitTypeDef GPIO_InitStructure;
//    EXTI_InitTypeDef EXTI_InitStructure;

//    RCC_AHB1PeriphClockCmd(RCC_SR04, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//    // TRIG pin (Output)
//    GPIO_InitStructure.GPIO_Pin = GPIO_TRIG;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(PORT_SR04, &GPIO_InitStructure);
//    GPIO_ResetBits(PORT_SR04, GPIO_TRIG);

//    // ECHO pin (Input with interrupt)
//    GPIO_InitStructure.GPIO_Pin = GPIO_ECHO;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(PORT_SR04, &GPIO_InitStructure);

//    // Configure EXTI interrupt
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//}

//// ECHO interrupt handler
//void EXTI4_IRQHandler(void) {
//    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
//        if (GPIO_ReadInputDataBit(PORT_SR04, GPIO_ECHO) == Bit_SET) {
//            if (state == HCSR04_WAIT_RISE) {
//                start_time = PORT_TIMER->CNT;
//                state = HCSR04_WAIT_FALL;
//            }
//        } else {
//            if (state == HCSR04_WAIT_FALL) {
//                end_time = PORT_TIMER->CNT;
//                state = HCSR04_CALCULATE;
//            }
//        }
//        EXTI_ClearITPendingBit(EXTI_Line4);
//    }
//}

//// Timer interrupt handler
//void TIM2_IRQHandler(void) {
//    if (TIM_GetITStatus(PORT_TIMER, TIM_IT_Update) != RESET) {
//        if (state == HCSR04_WAIT_RISE || state == HCSR04_WAIT_FALL) {
//            state = HCSR04_IDLE;
//        }
//        TIM_ClearITPendingBit(PORT_TIMER, TIM_IT_Update);
//    }
//}

//// Non-blocking state machine processing function
//static void HCSR04_Process(void) {
//    uint32_t current_ms = GetSysTickMs();
//    uint32_t current_us = PORT_TIMER->CNT; // Get current TIM2 count (us)

//    switch (state) {
//        case HCSR04_IDLE:
//            if (current_ms - last_measure_ms >= MEASURE_INTERVAL) {
//                GPIO_SetBits(PORT_SR04, GPIO_TRIG);
//                trig_start_us = current_us; // Record trigger start time (us)
//                state = HCSR04_TRIG_SEND;
//            }
//            break;

//        case HCSR04_TRIG_SEND:
//            // Use TRIG_DURATION to control pulse width (20us)
//            if (current_us - trig_start_us >= TRIG_DURATION) { 
//                GPIO_ResetBits(PORT_SR04, GPIO_TRIG);
//                state = HCSR04_WAIT_RISE;
//            }
//            break;

//        case HCSR04_WAIT_RISE:
//        case HCSR04_WAIT_FALL:
//            break;

//        case HCSR04_CALCULATE: {
//            uint32_t time_us = end_time - start_time;
//            float current_dist = (time_us > MAX_TIMEOUT_US) ? -1 : (time_us / 58.0f);
//            
//            distance_buf[measure_count % 10] = current_dist;
//            measure_count++;
//            if (measure_count % 10 == 0) {
//                float sum = 0;
//                uint8_t valid = 0;
//                for (uint8_t i = 0; i < 10; i++) {
//                    if (distance_buf[i] > 0) {
//                        sum += distance_buf[i];
//                        valid++;
//                    }
//                }
//                distance = (valid > 0) ? (sum / valid) : distance;
//            }
//            
//            last_measure_ms = current_ms;
//            state = HCSR04_IDLE;
//            break;
//        }
//    }
//}

//void Ultrasonic_Init(void) {
//    HCSR04_GPIO_Init();
//    HCSR04_Timer_Init();
//    for (uint8_t i = 0; i < 10; i++) distance_buf[i] = 0;
//    state = HCSR04_IDLE;
//}

//float Hcsr04GetLength(void) {
//    HCSR04_Process();
//    return distance;
//}


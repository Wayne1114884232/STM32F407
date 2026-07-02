//#ifndef __ADC_H
//#define __ADC_H

//#include "sys.h"

//// ####################### 可配置参数 #######################
//// 配置 ADC 外设（ADC1/ADC2/ADC3）
//#define ADCx             ADC1
//// 配置 ADC 通道对应的 GPIO 端口和引脚（例：PA5 对应 ADC1 通道5）
//#define ADC_GPIO_PORT    GPIOA
//#define ADC_GPIO_PIN     GPIO_Pin_5
//#define ADC_GPIO_CLK     RCC_AHB1Periph_GPIOA
//// 配置 ADC 通道号（需与引脚对应，参考数据手册）
//#define ADC_CHANNEL      ADC_Channel_5
//// 配置 ADC 采样时间（根据需求选择，值越大精度越高但速度越慢）
//#define ADC_SAMPLE_TIME  ADC_SampleTime_480Cycles
//// ##########################################################

//// 函数声明
//void ADC_Module_Init(void);                      // 初始化 ADC 模块
//uint16_t ADC_GetSingleValue(uint8_t channel);    // 获取单通道单次 ADC 值
//uint16_t ADC_GetAverageValue(uint8_t channel, uint8_t times);  // 获取平均值

//#endif


//#include "ADC.h"

///**
// * @brief  初始化 ADC 模块（含 GPIO 配置、ADC 外设配置）
// * @param  无
// * @retval 无
// */
//void ADC_Module_Init(void) {
//    GPIO_InitTypeDef GPIO_InitStruct;
//    ADC_CommonInitTypeDef ADC_CommonInitStruct;
//    ADC_InitTypeDef ADC_InitStruct;

//    // 1. 使能时钟（GPIO 时钟和 ADC 时钟）
//    RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK, ENABLE);                  // GPIO 时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);           // ADC1 时钟（根据 ADCx 修改）

//    // 2. 配置 GPIO 为模拟输入模式
//    GPIO_InitStruct.GPIO_Pin = ADC_GPIO_PIN;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;      // 模拟输入模式
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;// 速度不影响模拟输入
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  // 模拟输入无需上下拉
//    GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStruct);

//    // 3. ADC 公共配置（适用于所有 ADC 外设）
//    ADC_DeInit();  // 复位 ADC 外设，确保初始状态一致
//    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  // 禁用 DMA
//    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                // 独立模式（单 ADC 工作）
//    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;             // 分频：APB2时钟/4（F407 APB2默认84MHz，ADC时钟=21MHz，不超过36MHz）
//    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // 采样延迟（多通道时用）
//    ADC_CommonInit(&ADC_CommonInitStruct);

//    // 4. 配置 ADC 外设参数
//    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;  // 单次转换模式（非连续）
//    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  // 数据右对齐
//    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // 无外部触发，软件触发
//    ADC_InitStruct.ADC_NbrOfConversion = 1;  // 转换通道数量：1个
//    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;  // 12位分辨率（0~4095）
//    ADC_InitStruct.ADC_ScanConvMode = DISABLE;  // 非扫描模式（单通道）
//    ADC_Init(ADCx, &ADC_InitStruct);  // 初始化 ADC 外设

//    // 5. 使能 ADC
//    ADC_Cmd(ADCx, ENABLE);
//}

///**
// * @brief  获取单通道单次 ADC 转换值
// * @param  channel：ADC 通道号（如 ADC_Channel_5）
// * @retval 转换后的 12 位数值（0~4095）
// */
//uint16_t ADC_GetSingleValue(uint8_t channel) {
//    // 配置规则通道：ADC外设、通道号、转换顺序、采样时间
//    ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SAMPLE_TIME);
//    
//    // 软件触发 ADC 转换
//    ADC_SoftwareStartConv(ADCx);
//    
//    // 等待转换完成（EOC 标志置位）
//    while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
//    
//    // 清除转换完成标志（可选，部分库版本会自动清除）
//    ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
//    
//    // 返回转换值
//    return ADC_GetConversionValue(ADCx);
//}

///**
// * @brief  获取单通道多次采样的平均值（滤波效果）
// * @param  channel：ADC 通道号
// * @param  times：采样次数（建议 5~20 次）
// * @retval 平均值（0~4095）
// */
//uint16_t ADC_GetAverageValue(uint8_t channel, uint8_t times) {
//    uint32_t sum = 0;  // 累加和（用 32 位避免溢出）
//    uint8_t i;

//    // 多次采样并累加
//    for (i = 0; i < times; i++) {
//        sum += ADC_GetSingleValue(channel);
//        delay_ms(5);  // 采样间隔，避免连续采样误差
//    }

//    // 返回平均值
//    return (uint16_t)(sum / times);
//}



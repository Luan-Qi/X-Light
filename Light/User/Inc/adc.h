#ifndef _ADC_h
#define _ADC_h

#include "at32f421_clock.h"

#define ADC_CRM				CRM_GPIOB_PERIPH_CLOCK	//定义OLED端口时钟总线		
#define ADC_GPIO			GPIOB
#define ADC_Pin				GPIO_PINS_0
#define ADC_Channel		ADC_CHANNEL_8

void AdcInit(void);	//adc初始化函数
uint16_t GetAdc(adc_channel_select_type ch);	//获得ADC值
uint16_t GetAdcAverage(adc_channel_select_type ch,uint8_t times);	//求取平均值
void GetAdcUse(void);//获取要使用的AD值

#endif


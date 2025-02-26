#include "at32f421_clock.h"
#include <stdio.h>
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "spi.h"
#include "led.h"
#include "adc.h"
#include "button.h"
#include "x9c103s.h"
#include "radio.h"
#include "sx1280.h"
#include "pingpong.h"

extern uint8_t light_flag;

uint16_t battery_volt = 0;
uint16_t battery_volt_min = 0;

uint8_t light_index = 1;

uint8_t key = 0;
uint8_t SOS_mode_flag = 0;
uint8_t shut_down_flag = 0;

int main(void)
{
  system_clock_config();
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  uart_print_init(115200);
	clkout_config();
	delay_init();
	AdcInit();
	x9c103s_init();
	power_init();
	button_init();
	//sx1280_init();
	//长按开机，完成后亮灯等待松开
	button_keep_startup();
	while(button_scan(1)==0){LED_Charge(system_time,100,900);}
	button_keep_finish();
	//开机初始化，完成后闪烁
	x9c103s_min();
	battery_volt = GetAdcAverage(ADC_Channel,6)*3330/4095*3;
	battery_volt_min = battery_volt;
	printf("battery_volt: %d mV\r\n", battery_volt);
	delay_ms(400);LED(1);delay_ms(100);LED(0);LIGHT(1);
	system_update();
	
  while(1)
  {
		key = button_scan(0);
		switch(key)
		{
			case 1://加亮度
				if(light_flag==1)
				{
					light_index++;
					if(light_index>Light_index_max) light_index = Light_index_max;
					light_low_low(light_index);
				}
				break;
			case 2://减亮度
				if(light_flag==1)
				{
					light_index--;
					if(light_index<Light_index_min) light_index = Light_index_min;
				}
				break;
			case 3://同时按下开关灯
				light_switch();
				system_update();
				break;
			case 4://长按关机
				shut_down_flag = 1;
				break;
			case 6://微微模式
				if(light_flag==1)
				{
					light_low_low(light_index);
				}
				break;
		}
		light_change(light_flag,light_index);
		battery_volt = GetAdcAverage(ADC_Channel,5)*3330/4095*3;
		if(battery_volt<battery_volt_min) battery_volt_min = battery_volt;
		if(battery_volt>battery_volt_min+200) battery_volt_min = battery_volt;
		if(battery_LED_Hander(system_time,light_flag,battery_volt_min)==1) shut_down_flag = 2;
		printf("battery_volt: %d mV\r\n", battery_volt_min);
		shut_down_flag = system_long_check(1-light_flag)==1 ? 1 : shut_down_flag;
		//shut_down_flag = 0;
		if(shut_down_flag>0)
		{
			LED(1);
			if(shut_down_flag>1)
			{
				do{
					LED_SOS(&TMR1_PWM_Set);
					key = button_scan(0);
					if(key==3) SOS_mode_flag = 1 - SOS_mode_flag;
				}while(SOS_mode_flag==1);
			}
			delay_ms(300);LIGHT(0);power_off();
			while(button_scan(1)!=0){}
			while(1){}
		}
  }
}


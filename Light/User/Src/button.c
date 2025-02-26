#include "button.h"
#include "delay.h"
#include "timer.h"
#include "led.h"

/*按键引脚初始化*/
void button_init(void)
{
	gpio_init_type gpio_init_struct;

	crm_periph_clock_enable(BUTTON0_GPIO_CRM, TRUE);
	crm_periph_clock_enable(BUTTON1_GPIO_CRM, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pins = BUTTON0_PIN | BUTTON1_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(BUTTON0_GPIO, &gpio_init_struct);
}

uint32_t key_time = 0;

uint8_t button_scan(uint8_t mode)
{
	static uint8_t button_up = 1;
	static uint8_t button_down = 0;
	static uint32_t long_button = 0;
	static uint32_t last_button = 0;
	if(mode){button_up=0;button_down = 0;}
	if(button_up==1)
	{
		if(BUTTON0==RESET||BUTTON1==RESET)
		{
			delay_ms(20);button_up = 0;
			long_button = system_time;
		}
		return 0;
	}
	if(button_up==0&&button_down==0)
	{
		button_down=1;
		if(BUTTON0==RESET&&BUTTON1==RESET){return 3;}
		else if(BUTTON0==RESET)
		{
			if(mode==0&&system_time-last_button<220){return 5;}
			else{last_button = system_time;return 1;}
		}
		else if(BUTTON1==RESET)
		{
			if(mode==0&&system_time-last_button<220){return 6;}
			else{last_button = system_time;return 2;}
		}
	}
	if(button_up==0)
	{
		if(BUTTON0==SET&&BUTTON1==SET)
		{
			button_up = 1;button_down = 0;return 0;
		}
		if(system_time-long_button>2000) 
		{
			if(BUTTON0==RESET||BUTTON1==RESET)
			{
				return 4;
			}
		}
	}
	return 0;
}

void button_keep_startup(void)
{
	delay_ms(100);LED(0);
	delay_ms(2000);
}

void button_keep_finish(void)
{
	power_on();LED(1);
	while(button_scan(1)!=0){}
	LED(0);
}

uint32_t current_time = 0;

uint8_t family_index = 0;
uint8_t type_index = 0;

uint32_t press_down_time = 0;
uint32_t press_up_time = 0;
uint8_t butten_flag = 0;

uint8_t button_hander(void)
{
//	if(butten_flag==0)
//	{
//		if(button_scan()==1)
//		{
//			press_down_time = system_time;
//			butten_flag = 1;
//		}
//	}
//	else
//	{
//		if(button_scan()==0)
//		{
//			press_up_time = system_time;
//			butten_flag = 0;
//			if((press_up_time-press_down_time)>1000) {shut_down_flag = 1;return 2;}
//			else if((press_up_time-press_down_time)>500) {family_index++;type_index = 0;return 2;}
//			else return 1;
//		}
//	}
	return 0;
}

void button_delay_ms(uint32_t nms)
{
	current_time = system_time;
	while((system_time-current_time)<nms)
	{
		switch(button_hander())
		{
			case 1:type_index++;break;
			case 2:type_index = 5;break;
		}
	}
}


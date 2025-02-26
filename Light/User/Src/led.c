#include "led.h"
#include "timer.h"
#include "delay.h"
#include "x9c103s.h"

void power_init(void)
{
	gpio_init_type gpio_init_struct;
	/* enable the led clock */
	crm_periph_clock_enable(POWER_CRM, TRUE);
	crm_periph_clock_enable(LIGHT_CRM, TRUE);
	crm_periph_clock_enable(LED_CRM, TRUE);
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);
	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = POWER_EN_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(POWER_GPIO, &gpio_init_struct);
	//LED
	gpio_init_struct.gpio_pins = LED_PIN;
	gpio_init(LED_GPIO, &gpio_init_struct);
	//Light
	gpio_init_struct.gpio_pins =  LIGHT_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(LIGHT_GPIO, &gpio_init_struct);
#ifndef MOS_MODE
	//MOS
	gpio_init_struct.gpio_pins =  MOS_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
	gpio_init(MOS_GPIO, &gpio_init_struct);
#endif
	//default_set
	gpio_bits_write(POWER_GPIO, POWER_EN_PIN, FALSE);
	gpio_bits_write(LED_GPIO, LED_PIN, TRUE);
	gpio_bits_write(LIGHT_GPIO, LIGHT_PIN, FALSE);
#ifndef MOS_MODE
	gpio_bits_write(MOS_GPIO, MOS_PIN, FALSE);
#endif
}


void power_on(void)
{
	gpio_bits_write(POWER_GPIO, POWER_EN_PIN, TRUE);	
}

void power_off(void)
{
	gpio_bits_write(POWER_GPIO, POWER_EN_PIN, FALSE);
}

uint8_t light_flag = 0;

void light_on(void)
{
#ifndef MOS_MODE
	gpio_bits_write(MOS_GPIO, MOS_PIN, TRUE);
#endif
	light_flag = 1;
}

void light_off(void)
{
#ifndef MOS_MODE
	gpio_bits_write(MOS_GPIO, MOS_PIN, FALSE);
#endif
	light_flag = 0;
}

void light_switch(void)
{
	switch(light_flag)
	{
		case 0:
			light_on();
			return;
		case 1:
			light_off();
			return;
	}
}

void light_low_low(uint8_t index)
{
	static uint8_t light = 25;
	if(index==1)
	{
		if(light==0||light>25) light = 5;
		TMR1_PWM_Set(light);
		light -= 5;
	}
	else
	{
		light = 25;
	}
	return;
}

void light_change(uint8_t flag, uint8_t index)
{
	static uint8_t last_index = 0;
	static uint8_t last_flag = 0;
	if(index==last_index&&flag==last_flag) return;
	if(index<Light_index_min||index>Light_index_max) return;
#ifdef MOS_MODE
	if(flag==1){TMR1_PWM_Set(index*25);}
	else{TMR1_PWM_Set(0);}
#else
	x9c103s_change(index);
#endif
	last_index = index;
	last_flag = flag;
	return;
}

void LED_Charge(uint32_t time, uint16_t delay1, uint16_t delay2)
{
	static uint8_t charge_index = 0;
	static uint32_t current = 0;
	switch(charge_index)
	{
		case 0:
			if(time-current>delay1) 
			{
				LED(0);
				current = time;
				charge_index = 1;
			}
			else{return;}
			break;
		case 1:
			if(time-current>delay2) 
			{
				LED(1);
				current = time;
				charge_index = 0;
			}
			else{return;}
			break;
	}
}

uint8_t battery_LED_Hander(uint32_t time, uint8_t flag, uint16_t volt)
{
	static uint32_t LED_time = 0;
	static uint8_t LED_index = 0;
	if((flag==0&&volt>7900)||(flag==1&&volt>7800))
	{
		switch(LED_index)
		{
			case 0:
				if(time-LED_time<=300) return 0;
				LED(1);LED_index = 1;
				break;
			case 1:
				if(time-LED_time<=100) return 0;
				LED(0);LED_index = 2;
				break;
			case 2:
				if(time-LED_time<=100) return 0;
				LED(1);LED_index = 3;
				break;
			case 3:
				if(time-LED_time<=100) return 0;
				LED(0);LED_index = 4;
				break;
			case 4:
				if(time-LED_time<=100) return 0;
				LED(1);LED_index = 5;
				break;
			case 5:
				if(time-LED_time<=100) return 0;
				LED(0);LED_index = 6;
				break;
			case 6:
				if(time-LED_time<=100) return 0;
				LED(1);LED_index = 7;
				break;
			case 7:
				if(time-LED_time<=100) return 0;
				LED(0);LED_index = 0;
				break;
		}
	}
	else if((flag==0&&volt>7400)||(flag==1&&volt>7200))
	{
		switch(LED_index)
		{
			case 0:
				if(time-LED_time<=300) return 0;
				LED(1);LED_index = 1;
				break;
			case 1:
				if(time-LED_time<=140) return 0;
				LED(0);LED_index = 2;
				break;
			case 2:
				if(time-LED_time<=140) return 0;
				LED(1);LED_index = 3;
				break;
			case 3:
				if(time-LED_time<=140) return 0;
				LED(0);LED_index = 4;
				break;
			case 4:
				if(time-LED_time<=140) return 0;
				LED(1);LED_index = 5;
				break;
			case 5:
				if(time-LED_time<=140) return 0;
				LED(0);LED_index = 0;
				break;
			default:
				LED_index = 0;
		}
	}
	else if((flag==0&&volt>7000)||(flag==1&&volt>6600))
	{
		switch(LED_index)
		{
			case 0:
				if(time-LED_time<=490) return 0;
				LED(1);LED_index = 1;
				break;
			case 1:
				if(time-LED_time<=170) return 0;
				LED(0);LED_index = 2;
				break;
			case 2:
				if(time-LED_time<=170) return 0;
				LED(1);LED_index = 3;
				break;
			case 3:
				if(time-LED_time<=170) return 0;
				LED(0);LED_index = 0;
				break;
			default:
				LED_index = 0;
		}
	}
	else if((flag==0&&volt>6400)||(flag==1&&volt>6000))
	{
		switch(LED_index)
		{
			case 0:
				if(time-LED_time<=750) return 0;
				LED(1);LED_index = 1;
				break;
			case 1:
				if(time-LED_time<=250) return 0;
				LED(0);LED_index = 2;
				break;
			default:
				LED_index = 0;
		}
	}
	else
	{
		return 1;
	}
	LED_time = time;
	return 0;
}

void LED_SOS(void function(uint32_t value))
{
	function(0);delay_ms(50);
	function(10);delay_ms(50);//S
	function(0);delay_ms(50);
	function(10);delay_ms(50);
	function(0);delay_ms(50);
	function(10);delay_ms(50);
	function(0);delay_ms(100);
	function(10);delay_ms(100);//O
	function(0);delay_ms(100);
	function(10);delay_ms(100);
	function(0);delay_ms(100);
	function(10);delay_ms(100);
	function(0);delay_ms(100);
	function(10);delay_ms(50);//S
	function(0);delay_ms(50);
	function(10);delay_ms(50);
	function(0);delay_ms(50);
	function(10);delay_ms(50);
	function(0);
}

void LED_endless(void)
{
	while(1)
	{
		LED(0);delay_ms(500);
		LED(1);delay_ms(500);
	}
}

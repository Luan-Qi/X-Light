#include "x9c103s.h"
#include "delay.h"
#include "led.h"

void x9c103s_init(void)
{
	gpio_init_type gpio_init_struct;
	
  /* gpioa gpiob clock enable */
  crm_periph_clock_enable(X9C_CRM, TRUE);
	
	/* gpio configuration */
	gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = X9C_CS_PIN | X9C_U_D_PIN | X9C_INC_PIN;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(X9C_GPIO, &gpio_init_struct);
	
}

uint8_t X9C_shift_count = 0;
uint8_t X9C_shift_flag = 0;

void x9c103s_max(void)
{
	X9C_INC(1);X9C_CS(0);X9C_U_D(1);delay_ms(5);
	for(int i=100;i>0;i--)
	{
		X9C_INC(0);delay_ms(5);
		X9C_INC(1);delay_ms(5);
	}
	X9C_CS(1);
}

void x9c103s_min(void)
{
	X9C_INC(1);X9C_CS(0);X9C_U_D(0);delay_ms(2);
	for(int i=100;i>0;i--)
	{
		X9C_INC(0);delay_ms(2);
		X9C_INC(1);delay_ms(2);
	}
	X9C_CS(1);
}

void x9c103s_test(uint8_t key)
{
	if(key!=0)
	{
		X9C_CS(0);
		switch(key)
		{
			case 1:
				X9C_U_D(1);
				break;
			case 2:
				X9C_U_D(0);
				break;
		}
		delay_ms(10);
		X9C_INC(0);delay_ms(50);
		X9C_INC(1);delay_ms(50);
		X9C_CS(1);
		LED(1);delay_ms(100);
		LED(0);
	}
}

uint8_t X9C_INC_current = 0;
const uint8_t X9C_INC_MODE[4] = {22,24,25,26};

void x9c103s_change(uint8_t index)
{
	uint8_t X9C_INC_hope = X9C_INC_MODE[index-1];
	uint8_t X9C_INC_buffer = 0;
	if(X9C_INC_hope>X9C_INC_current)
	{
		X9C_U_D(1);
		X9C_INC_buffer = X9C_INC_hope - X9C_INC_current;
	}
	else
	{
		X9C_U_D(0);
		X9C_INC_buffer = X9C_INC_current - X9C_INC_hope;
	}
	X9C_INC_current = X9C_INC_hope;
	if(X9C_INC_buffer>X9C_INC_MAX) return;
	X9C_CS(0);delay_ms(3);
	while(X9C_INC_buffer>0)
	{
		X9C_INC(0);delay_ms(3);
		X9C_INC(1);delay_ms(3);
		X9C_INC_buffer--;
	}
	X9C_CS(1);
	return;
}


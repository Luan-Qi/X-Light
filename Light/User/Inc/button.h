#ifndef __BUTTON_H
#define __BUTTON_H

#include "stdio.h"
#include "at32f421_clock.h"

#define BUTTON0_GPIO_CRM		CRM_GPIOB_PERIPH_CLOCK
#define BUTTON0_GPIO				GPIOB
#define BUTTON0_PIN					GPIO_PINS_3

#define BUTTON1_GPIO_CRM		CRM_GPIOB_PERIPH_CLOCK
#define BUTTON1_GPIO				GPIOB
#define BUTTON1_PIN					GPIO_PINS_7

#define BUTTON0 gpio_input_data_bit_read(BUTTON0_GPIO, BUTTON0_PIN)
#define BUTTON1 gpio_input_data_bit_read(BUTTON1_GPIO, BUTTON1_PIN)

void button_init(void);
uint8_t button_scan(uint8_t mode);
void button_keep_startup(void);
void button_keep_finish(void);
uint8_t button_hander(void);
void button_delay_ms(uint32_t nms);

#endif


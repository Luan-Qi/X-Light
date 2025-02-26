#ifndef __LED_H
#define __LED_H

#include "stdio.h"
#include "at32f421_clock.h"

#define POWER_CRM				CRM_GPIOA_PERIPH_CLOCK
#define POWER_GPIO			GPIOA
#define POWER_EN_PIN		GPIO_PINS_15

#define LIGHT_CRM				CRM_GPIOA_PERIPH_CLOCK
#define LIGHT_GPIO			GPIOA
#define LIGHT_PIN				GPIO_PINS_12

#define LED_CRM					CRM_GPIOB_PERIPH_CLOCK
#define LED_GPIO				GPIOB
#define LED_PIN					GPIO_PINS_1

#define MOS_MODE				1
#define MOS_CRM					CRM_GPIOA_PERIPH_CLOCK
#define MOS_GPIO				GPIOA
#define MOS_PIN					GPIO_PINS_8

#define LED(x)  do{ x ? \
										gpio_bits_write(LED_GPIO, LED_PIN, TRUE) : \
										gpio_bits_write(LED_GPIO, LED_PIN, FALSE); \
								}while(0)      /* LED·­×ª */

#define LIGHT(x)  do{ x ? \
										gpio_bits_write(LIGHT_GPIO, LIGHT_PIN, TRUE) : \
										gpio_bits_write(LIGHT_GPIO, LIGHT_PIN, FALSE); \
								}while(0)      /* LED·­×ª */

extern uint8_t light_flag;

#define Light_index_max 4
#define Light_index_min 1

void power_init(void);
void power_on(void);
void power_off(void);
void light_on(void);
void light_off(void);
void light_switch(void);
void light_low_low(uint8_t index);
void light_change(uint8_t flag, uint8_t index);
void LED_Charge(uint32_t time, uint16_t delay1, uint16_t delay2);
uint8_t battery_LED_Hander(uint32_t time, uint8_t flag, uint16_t volt);
void LED_SOS(void function(uint32_t value));
void LED_endless(void);

#endif



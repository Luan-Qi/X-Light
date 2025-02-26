#ifndef __X9C103S_H
#define __X9C103S_H

#include "at32f421_clock.h"

#define X9C_CRM					CRM_GPIOB_PERIPH_CLOCK
#define X9C_GPIO				GPIOB

#define X9C_CS_PIN			GPIO_PINS_6
#define X9C_U_D_PIN			GPIO_PINS_5
#define X9C_INC_PIN			GPIO_PINS_4

#define X9C_CS(x)   do{ x ? \
                      gpio_bits_write(X9C_GPIO, X9C_CS_PIN, TRUE) : \
                      gpio_bits_write(X9C_GPIO, X9C_CS_PIN, FALSE); \
										}while(0)      /* CS·­×ª */

#define X9C_U_D(x)  do{ x ? \
                      gpio_bits_write(X9C_GPIO, X9C_U_D_PIN, TRUE) : \
                      gpio_bits_write(X9C_GPIO, X9C_U_D_PIN, FALSE); \
										}while(0)      /* U_D·­×ª */

#define X9C_INC(x)  do{ x ? \
                      gpio_bits_write(X9C_GPIO, X9C_INC_PIN, TRUE) : \
                      gpio_bits_write(X9C_GPIO, X9C_INC_PIN, FALSE); \
										}while(0)      /* INC·­×ª */

#define X9C_INC_MAX				31

extern uint8_t X9C_shift_count;

void x9c103s_init(void);
void x9c103s_max(void);
void x9c103s_min(void);
void x9c103s_change(uint8_t index);

#endif


#ifndef __TIMER_H
#define __TIMER_H

#include "stdio.h"
#include "at32f421_clock.h"

#define dull_time 30000
#define max_system_time 7200000

extern uint32_t system_time;
extern uint32_t system_last_update;

void clkout_config(void);
void TMR1_PWM_Set(uint32_t value);
void system_update(void);
uint8_t system_long_check(uint8_t mode);

#endif



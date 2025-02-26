#ifndef __HW_H
#define __HW_H

#include "at32f421_clock.h"

void HwInit(void);
void GpioWrite(gpio_type* GPIOx, uint16_t GPIO_Pin, uint32_t value);
uint32_t GpioRead(gpio_type* GPIOx, uint16_t GPIO_Pin);
void HAL_Delay(uint32_t nms);
void SpiInOut(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size);
void SpiIn(uint8_t *txBuffer, uint16_t size);

#endif


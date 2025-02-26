#include "hw.h"
#include "spi.h"
#include "delay.h"

void HwInit(void)
{
	spi_config();
}

void GpioWrite(gpio_type* GPIOx, uint16_t GPIO_Pin, uint32_t value)
{
	if(value==1){gpio_bits_write(GPIOx, GPIO_Pin, TRUE);}
	else{gpio_bits_write(GPIOx, GPIO_Pin, FALSE);}
}

uint32_t GpioRead(gpio_type* GPIOx, uint16_t GPIO_Pin)
{
	return gpio_input_data_bit_read(GPIOx,GPIO_Pin);
}

void HAL_Delay(uint32_t nms)
{
	delay_ms(nms);
}

void SpiInOut(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size)
{
	spi_InOut(txBuffer, rxBuffer, size);
}

void SpiIn(uint8_t *txBuffer, uint16_t size)
{
	spi_In(txBuffer, size);
}


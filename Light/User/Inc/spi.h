#ifndef __SPI_H
#define __SPI_H			  	 
#include "at32f421_clock.h"

#define RADIO_CRM 					CRM_GPIOA_PERIPH_CLOCK

#define RADIO_NSS_PIN       GPIO_PINS_4
#define RADIO_NSS_PORT      GPIOA

#define RADIO_MOSI_PIN      GPIO_PINS_7
#define RADIO_MOSI_PORT     GPIOA

#define RADIO_MISO_PIN      GPIO_PINS_6
#define RADIO_MISO_PORT     GPIOA

#define RADIO_SCK_PIN       GPIO_PINS_5
#define RADIO_SCK_PORT      GPIOA

#define RADIO_nRESET_PIN    GPIO_PINS_3
#define RADIO_nRESET_PORT   GPIOA

#define RADIO_BUSY_PIN      GPIO_PINS_2
#define RADIO_BUSY_PORT     GPIOA

//#define RADIO_DIO2_Pin 		 	GPIO_PINS_4
//#define RADIO_DIO2_GPIO_Port 	GPIOB

//#define RADIO_DIO1_Pin 		 	GPIO_PINS_4
//#define RADIO_DIO1_GPIO_Port 	GPIOB

//#define RADIO_DIO3_Pin 		 	GPIO_PINS_4
//#define RADIO_DIO3_GPIO_Port 	GPIOB

//#define RADIO_DIOx_PIN      GPIO_PINS_4
//#define RADIO_DIOx_PORT     GPIOB

void spi_config(void);
void spi_InOut(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size);
void spi_In(uint8_t *txBuffer, uint16_t size);

#endif



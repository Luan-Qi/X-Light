#include "spi.h"

void spi_config(void)
{
	gpio_init_type gpio_initstructure;
	spi_init_type spi_init_struct;
	
  crm_periph_clock_enable(RADIO_CRM, TRUE);
  gpio_default_para_init(&gpio_initstructure);
	
	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE5, GPIO_MUX_0);
  //gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_0);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE7, GPIO_MUX_0);
  
  /* spi1 sck pin */
  gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;
  gpio_initstructure.gpio_pins = RADIO_SCK_PIN;
  gpio_init(RADIO_SCK_PORT, &gpio_initstructure);
  /* spi1 mosi pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_NONE;
  gpio_initstructure.gpio_pins = RADIO_MOSI_PIN;
  gpio_init(RADIO_MOSI_PORT, &gpio_initstructure);
	/* spi1 miso pin */
	gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
	gpio_initstructure.gpio_mode           = GPIO_MODE_INPUT;
	gpio_initstructure.gpio_pins = RADIO_MISO_PIN;
  //gpio_init(RADIO_MISO_PORT, &gpio_initstructure);
	/* spi1 cs pin */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
	gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT;
	gpio_initstructure.gpio_pins = RADIO_NSS_PIN;
  gpio_init(RADIO_NSS_PORT, &gpio_initstructure);
	/* sx1280 reset pin */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
	gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT;
	gpio_initstructure.gpio_pins = RADIO_nRESET_PIN;
  gpio_init(RADIO_nRESET_PORT, &gpio_initstructure);
	/* sx1280 busy pin */
	gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
	gpio_initstructure.gpio_mode           = GPIO_MODE_ANALOG;
	gpio_initstructure.gpio_pins = RADIO_BUSY_PIN;
  gpio_init(RADIO_BUSY_PORT, &gpio_initstructure);
	
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
	
  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = 			SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode =				SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = 			SPI_MCLK_DIV_8;
  spi_init_struct.first_bit_transmission = 	SPI_FIRST_BIT_MSB;
  spi_init_struct.frame_bit_num = 					SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = 					SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = 						SPI_CLOCK_PHASE_1EDGE;
  spi_init_struct.cs_mode_selection = 			SPI_CS_SOFTWARE_MODE;
  spi_init(SPI1, &spi_init_struct);
	
  spi_enable(SPI1, TRUE);
	
	gpio_bits_write(RADIO_NSS_PORT, RADIO_NSS_PIN, TRUE);
	gpio_bits_write(RADIO_nRESET_PORT, RADIO_nRESET_PIN, TRUE);
}

void spi_InOut(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size)
{
	uint32_t spi_index = 0;
	uint8_t tx_data, rx_data;
	#ifdef USE_DMA
		blockingDmaFlag = true;
		HAL_SPI_TransmitReceive_DMA( &SpiHandle, txBuffer, rxBuffer, size );
		WAIT_FOR_BLOCKING_FLAG
	#else
		while(spi_index < size)
		{
			while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
			tx_data = *txBuffer;
			spi_i2s_data_transmit(SPI1, tx_data);
			txBuffer += sizeof(uint8_t);
			
			while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET);
			rx_data = spi_i2s_data_receive(SPI1);
			*rxBuffer = rx_data;
			rxBuffer += sizeof(uint8_t);
			
			spi_index++;
		}
		while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) == SET);
	#endif
}

void spi_In(uint8_t *txBuffer, uint16_t size)
{
	uint32_t spi_index = 0;
	uint8_t tx_data;
	#ifdef USE_DMA
		blockingDmaFlag = true;
		HAL_SPI_Transmit_DMA( &SpiHandle, txBuffer, size );
		WAIT_FOR_BLOCKING_FLAG
	#else
		while(spi_index < size)
		{
			while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
			tx_data = *txBuffer;
			spi_i2s_data_transmit(SPI1, tx_data);
			txBuffer += sizeof(uint8_t);
			
			spi_index++;
		}
		while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) == SET);
	#endif
}



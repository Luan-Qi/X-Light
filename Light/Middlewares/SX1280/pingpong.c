/*!
 * \file      pingpong.c
 *
 * \brief     pingpong implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2018 Semtech
 *
 * \endcode
 *
 * \author    Semtech
 */
 
/* Includes ------------------------------------------------------------------*/

#include <string.h>
#include "pingpong.h"
#include "hw.h"
#include "led.h"
#include "usart.h"
#include "radio.h"
#include "sx1280.h"

/*!
 * \brief Used to display firmware version UART flow
 */
#define FIRMWARE_VERSION    ( ( char* )"Firmware Version: 5709de2d" )

/*!
 * Select mode of operation for the Ping Ping application
 */
//#define MODE_BLE
#define MODE_LORA
//#define MODE_GFSK
//#define MODE_FLRC

#define RF_BL_ADV_CHANNEL_38         						2426000000 // Hz
#define RF_BL_ADV_CHANNEL_0          						2404000000 // Hz

/*!
 * \brief Defines the nominal frequency
 */
#define RF_FREQUENCY                                RF_BL_ADV_CHANNEL_0 // Hz

/*!
 * \brief Defines the output power in dBm
 *
 * \remark The range of the output power is [-18..+13] dBm
 */
#define TX_OUTPUT_POWER                             13

/*!
 * \brief Defines the buffer size, i.e. the payload size
 */
#define BUFFER_SIZE                                 20

/*!
 * \brief Number of tick size steps for tx timeout
 */
#define TX_TIMEOUT_VALUE                            10000 // ms

/*!
 * \brief Number of tick size steps for rx timeout
 */
#define RX_TIMEOUT_VALUE                            1000 // ms

/*!
 * \brief Size of ticks (used for Tx and Rx timeout)
 */
#define RX_TIMEOUT_TICK_SIZE                        RADIO_TICK_SIZE_1000_US

/*!
 * \brief Defines the size of the token defining message type in the payload
 */
#define PINGPONGSIZE                                4

/*!
 * \brief Defines the states of the application
 */
typedef enum
{
    APP_LOWPOWER,
    APP_RX,
    APP_RX_TIMEOUT,
    APP_RX_ERROR,
    APP_TX,
    APP_TX_TIMEOUT,
}AppStates_t;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( void );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( IrqErrorCode_t );

/*!
 * \brief Define the possible message type for this application
 */
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

/*!
 * \brief All the callbacks are stored in a structure
 */
RadioCallbacks_t Callbacks =
{
    &OnTxDone,        // txDone
    &OnRxDone,        // rxDone
    NULL,             // syncWordDone
    NULL,             // headerDone
    &OnTxTimeout,     // txTimeout
    &OnRxTimeout,     // rxTimeout
    &OnRxError,       // rxError
    NULL,             // rangingDone
    NULL,             // cadDone
};

/*!
 * \brief The size of the buffer
 */
uint8_t BufferSize = BUFFER_SIZE;

/*!
 * \brief The buffer
 */
uint8_t Buffer[BUFFER_SIZE];

/*!
 * \brief Mask of IRQs to listen to in rx mode
 */
uint16_t RxIrqMask = IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT;

/*!
 * \brief Mask of IRQs to listen to in tx mode
 */
uint16_t TxIrqMask = IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT;

/*!
 * \brief The State of the application
 */
AppStates_t AppState = APP_LOWPOWER;

#if defined( MODE_BLE )
/*!
 * \brief In case of BLE, the payload must contain the header
 */
typedef union
{
    struct BleAdvHeaderField_s
    {
        uint8_t pduType: 4;
        uint8_t rfu1:2;
        uint8_t txAddr:1;
        uint8_t rxAddr:1;
        uint8_t length:6;
        uint8_t rfu2:2;
    } Fields;
    uint8_t Serial[ 2 ];
}BleAdvHeaders_t;
BleAdvHeaders_t ble_header_adv;
#endif // MODE_BLE

PacketParams_t packetParams;

PacketStatus_t packetStatus;

void sx1280_init( void )
{
	uint16_t Version = 0;
	//bool isMaster = true;
	ModulationParams_t modulationParams;

	HwInit( );
	HAL_Delay( 500 );                   // let DC/DC power ramp up

	Radio.Init( &Callbacks );
	//Radio.SetRegulatorMode( USE_DCDC ); // Can also be set in LDO mode but consume more power
	memset( &Buffer, 0x00, BufferSize );

	printf( "SX1280 Ping Pong Demo Application. %s\r\n", FIRMWARE_VERSION );
	printf( "Radio firmware version 0x%x\r\n", Version );
	Version = Radio.GetFirmwareVersion();
	LED_endless();
	//LORA mode
	printf( "Ping Pong running in LORA mode\n\r" );
	modulationParams.PacketType = PACKET_TYPE_LORA;
	modulationParams.Params.LoRa.SpreadingFactor = LORA_SF12;
	modulationParams.Params.LoRa.Bandwidth = LORA_BW_1600;
	modulationParams.Params.LoRa.CodingRate = LORA_CR_LI_4_7;

	packetParams.PacketType = PACKET_TYPE_LORA;
	packetParams.Params.LoRa.PreambleLength = 12;
	packetParams.Params.LoRa.HeaderType = LORA_PACKET_VARIABLE_LENGTH;
	packetParams.Params.LoRa.PayloadLength = BUFFER_SIZE;
	packetParams.Params.LoRa.CrcMode = LORA_CRC_ON;
	packetParams.Params.LoRa.InvertIQ = LORA_IQ_NORMAL;

	Radio.SetStandby( STDBY_RC );
	Radio.SetPacketType( modulationParams.PacketType );
	Radio.SetModulationParams( &modulationParams );
	Radio.SetPacketParams( &packetParams );
	Radio.SetRfFrequency( RF_FREQUENCY );
	Radio.SetBufferBaseAddresses( 0x00, 0x00 );
	Radio.SetTxParams( TX_OUTPUT_POWER, RADIO_RAMP_02_US );
	
	SX1280SetPollingMode( );

	Radio.SetDioIrqParams( RxIrqMask, RxIrqMask, IRQ_RADIO_NONE, IRQ_RADIO_NONE );

	Radio.SetRx( ( TickTime_t ) { RX_TIMEOUT_TICK_SIZE, RX_TIMEOUT_VALUE } );
	AppState = APP_LOWPOWER;

	while( 1 )
	{
		SX1280ProcessIrqs( );
		HAL_Delay( 1000 );
	}
}

void OnTxDone( void )
{
    AppState = APP_TX;
}

void OnRxDone( void )
{
    AppState = APP_RX;
}

void OnTxTimeout( void )
{
    AppState = APP_TX_TIMEOUT;
    printf( "<>>>>>>>>TXE\n\r" ); 
}

void OnRxTimeout( void )
{
    AppState = APP_RX_TIMEOUT;
}

void OnRxError( IrqErrorCode_t errorCode )
{
    AppState = APP_RX_ERROR;
    printf( "RXE<>>>>>>>>\n\r" ); 
}

void OnRangingDone( IrqRangingCode_t val )
{
}

void OnCadDone( bool channelActivityDetected )
{
}





/** LTC2983.c
 * Driver for LTC2983 chip related functions. The IC uses SPI to communicate to
 * to MCU. IsoSPI is not required since temperature sensors are galvanically isolated.
 * regular SPI to isoSPI.
 * If using SPI: MCU --SPI--> LTC2983
 * If using isoSPI: MCU --SPI--> LTC6820 --isoSPI--> LTC2983
 */

#include "LTC2983.h"


#include "UART.h"

const uint8_t READ_CMD = 0x03;
const uint8_t WRITE_CMD = 0x02;
const uint16_t CHANNEL_RESULTS_OFFSET = 0x0010;



/** LTC2983_Init
 * Initializes and configures LTC2983 chip 
 * @preconditions		SPI_Init8 and GPIO Pins: PC6-8 & PB13-15 are initialized
 */
void LTC2983_Init(void){
	// CMD(0), Channel Address(1-2), Data or Command(3-6)
	uint8_t message[7] = {WRITE_CMD, 0x02, 0x00, 0xF4, 0x00, 0x00, 0x00};
	uint8_t maskConfig[7] = {WRITE_CMD, 0x00, 0xF4, 0x00, 0x0F, 0xFF, 0xFF};	

	// Wait until LTC2983 is ready starting up (might not be necessary due to timing)
	while(!LTC2983_Ready()) {
		//printf("Delay Init\n\r");
	};
	
	// Set channels for board 1 to Direct ADC mode
	for(board tempBoard = TEMP_CS1; tempBoard <= TEMP_CS1; tempBoard++) {
		for(uint32_t i = 0; i < BOARD_CS1; i++) {
			Board_Select(tempBoard, 0);
			SPI_WriteMulti8(message, 7);
			Board_Select(tempBoard, 1);
			for(uint32_t i = 0; i < 80000; i++) {}
printf("Channel %d\r\n", i+1);
			message[2] += 4;					// Increment to next channel

		}
		message[2] = 0;			// reset channel selection
		
		// Set multichannel conversion setting
		Board_Select(tempBoard, 0);
		SPI_WriteMulti8(maskConfig, 7);
		Board_Select(tempBoard, 1);
	}
 
	while(!LTC2983_Ready()) {
		//printf("Delay Init\n\r");
	};
}

/** Board_Select
 * Uses specified CS pins for selected board.
 * @param enumerated type Board. List in "Settings.h". If correct board isn't selected, don't do anything
 * @param state of the CS line (1 or 0). Note that SPI CS lines are HIGH or 1 during idle
 */
void Board_Select(board tempBoard, bool state) {
	switch(tempBoard) {
		case TEMP_CS1: 
			if(state) GPIOB->ODR |= GPIO_Pin_13;
			else GPIOB->ODR &= ~GPIO_Pin_13;
			break;
		case TEMP_CS2:
			if(state) GPIOB->ODR |= GPIO_Pin_14;
			else GPIOB->ODR &= ~GPIO_Pin_14;
			break;
			
		case TEMP_CS3:
			if(state) GPIOB->ODR |= GPIO_Pin_15;
			else GPIOB->ODR &= ~GPIO_Pin_15;
			break;
		
		case TEMP_CS4:
			if(state) GPIOC->ODR |= GPIO_Pin_6;
			else GPIOC->ODR &= ~GPIO_Pin_6;
			break;
		
		case TEMP_CS5:
			if(state) GPIOC->ODR |= GPIO_Pin_7;
			else GPIOC->ODR &= ~GPIO_Pin_7;
			break;
			
		case TEMP_CS6:	
			if(state) GPIOC->ODR |= GPIO_Pin_8;
			else GPIOC->ODR &= ~GPIO_Pin_8;
			break;
			
		default:	return;
	}

}





/** LTC2983_Ready
 * Checks if LTC2984 is ready after startup
 * @return unsigned int 16-bit data
 */
bool LTC2983_Ready(void){
	uint8_t message[3] = {READ_CMD, 0x00, 0x00};
	uint8_t result;

	GPIOB->ODR &= ~GPIO_Pin_13;
	SPI_WriteReadMulti8(message, 3, &result, 1, true);
	for(uint32_t i = 0; i < 80000; i++) {}
	GPIOB->ODR |= GPIO_Pin_13;
//printf("Is board ready? ");
//for(uint32_t i = 0; i < 10000; i++);
  if((result & 0xC0) == 0x40) {
//		printf("true\r\n");
		printf("Board is Ready : 0x%x\n\r", result);
		return true;
	}
  else {
//		printf("False :( - 0x%x\r\n", result);
		return false;
	}
	
}



/** LTC2983_MeasureSingleChannel
 * Sends command to LTC2983 to gather and save all ADC values
 * @param 16 bit channel size in a 1 byte array
 * @return signed 32-bit measurements from specified channel.
 *       returns a -1 if invalid ADC measurement
 */
int32_t LTC2983_MeasureSingleChannel(void){
	// Data Messages Declarations
	uint8_t result[4];
	uint8_t status[3];
	
	uint8_t message[4] = {WRITE_CMD, 0x00, 0x00, 0x81};	 // Command, Address, Data
				// message for start conversion
	uint8_t receive[3] = {READ_CMD, 0x00, 0x10};
				// Read conversion result from channel 1 

	GPIOB->ODR &= ~GPIO_Pin_13;
	SPI_WriteMulti8(message, 4);
	GPIOB->ODR |= GPIO_Pin_13;
	
	
	while(!LTC2983_Ready()) {
		
		GPIOB->ODR &= ~GPIO_Pin_13;
		SPI_WriteMulti8(message, 4);			// restart conversion
		GPIOB->ODR |= GPIO_Pin_13;
		
		//printf("Not ready.\n\r");
	}

	GPIOB->ODR &= ~GPIO_Pin_13;
	SPI_WriteReadMulti8(receive, 3, result, 4, true);
	GPIOB->ODR |= GPIO_Pin_13;	
	
	//printf("Raw Result: 0x%x,%x,%x,%x\n\r", result[0], result[1], result[2], result[3]);
	//printf("Result received\r\n");
	//printf("Or: %u \r\n", *(uint32_t *)result);
		
	//printf("%d.%d\r\n", result[1]>>5, (result[1]&0x1F) * 10000 /32);
	
	if((result[0] & 0x01) == 0x01) {
		return (*(int32_t *)result & 0x007FFFFF);
	} else {
		return -1;
	}
		
}



/** LTC2983_StartMeasuringADC
 * Starts direct ADC conversion for all channels of selected board
 * @note Conversions are initiated consecutively so if you read the Command
 *		Status Register or the INTERRUPT pin, then it won't be ready until All
 *		channels are finished converting.
 * @param Selected Temperature Board CS (enumerated type in "Settings.h")
 */
void LTC2983_StartMeasuringADC(board temperatureBoard) {
	// CMD(0), Channel Address(1-2), Data or Command(3-6)
	uint8_t startConversion[4] = {WRITE_CMD, 0x00, 0x00, 0x80};
	
	// Start multichannel conversion for all channels.
	Board_Select(temperatureBoard, 0);
	SPI_WriteMulti8(startConversion, 4);
	Board_Select(temperatureBoard, 1);
	
	// Channel conversions are initiated consecutively and CSR won't be ready until all channels are ready.
	while(!LTC2983_Ready()) {
		//printf("Board not ready\n\r");
	}
	
}



/**	LTC2983_ReadConversions
 * Reads all channels from temperature board and stores in buffer
 * @param pointer to a buffer to store conversion results
 * @param temperature board CS (enumerated type in "Settings.h")
 * @param number of Channels, use "Settings.h" definitions
 * @preconditions All channels on the board finished conversion before running this function
 */
void LTC2983_ReadConversions(int32_t *Buf, board temperatureBoard, uint8_t numOfChannels) {
	uint8_t readConversionResult[3] = {READ_CMD, 0x00, 0x10};
	uint8_t result[4];
	
	for(uint32_t i = 0; i < numOfChannels; i++) {
		Board_Select(temperatureBoard, 0);
		SPI_WriteReadMulti8(readConversionResult, 3, result, 4, true);
		for(uint32_t i = 0; i < 80000; i++){}
		Board_Select(temperatureBoard, 1);
		
		readConversionResult[2] += 4;				// Increment to next channel address	
		
		if((result[0] & 0x01) == 0x01) {
			//(*(int32_t *)result & 0x007FFFFF);
			Buf[i] = *(int32_t *)result & 0x007FFFFF;
printf("woot\r\n");
		} else {
			Buf[i] = -1;
printf(":(\n\r");
		}
		
printf("Channel %d - ADC raw value: %d\r\n", i+1, Buf[i]);
	}
	
}




/** LTC2983_ReadChannel
 * Reads the 24 bit ADC value at a specified channel
 * @preconditions Specified channel to be read finished conversion before running this function
 * @param Selected temperature board CS (enumerated type in "Settings.h")
 * @param unsigned int 16-bit channel number
 * @return signed 32-bit (unconverted) data for channel
 */
int32_t LTC2983_ReadChannel(board temperatureBoard, uint8_t channel) {
	uint8_t convert[2];
	uint16_t channelAddr;
	uint8_t singleChannelResult[4];
	
	channelAddr = (channel * 4) + CHANNEL_RESULTS_OFFSET;		// Get address of the channel (every 4 bytes in memory)
	
	convert[1] = (uint8_t)(channelAddr & 0x00FF);						// Isolate channel address (4 bytes long) into 2 separate bytes
	convert[0] = (uint8_t)((channelAddr & 0xFF00) >> 8);	
	
	uint8_t readSingleChannel[3] = {READ_CMD, convert[0], convert[1]};		// Message for SPI
	
	Board_Select(temperatureBoard, 0);
	SPI_WriteReadMulti8(readSingleChannel, 3, singleChannelResult, 4, true);
	Board_Select(temperatureBoard, 1);
	
} 







/** UART.h
 * UART driver. Mainly for communication with PuTTY.
 */

/***** Unlike SPI, UART cannot have multiple modules connected to the same tx/rx line *****/

/** Pins:
 * UART1:
 *		tx : PA9
 *		rx : PA10
 * UART3:
 *		tx : PB10
 *		rx : PC5
 */
 
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "config.h"

// This is required to use printf
struct __FILE{
	int file;
};

// Global variables
//static uint32_t TxErrorCnt = 0;
//static uint32_t RxErrorCnt = 0;
FILE __stdout;
FILE __stdin;

// Private Function Prototypes
void copySoftwareToHardware(void);
void copyHardwareToSoftware(void);

// Public Function Definitions
/** UART1_Init
 * Initializes UART1 Module
 * Pins: 
 *		tx : PA9
 *		rx : PA10
 * @param baud rate: 9600, 115200, 500000
 */
void UART1_Init(uint32_t baud){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	//NVIC_InitTypeDef NVIC_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// Initialize PA9 and PA10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// Initialize USART1
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}

/** UART1_Write
 * Sends data through tx pin for UART1
 * @param ptr to char buffer, size of buffer
 */
void UART1_Write(char *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		while((USART1->SR&USART_SR_TC) == 0);	// Wait until transmission is done
		USART1->DR = txBuf[i] & 0xFF;
	}
}

/** UART1_Read
 * Sends data through tx pin for UART1
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART1_Read(char *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		while((USART1->SR&USART_SR_RXNE) == 0);
		rxBuf[i] = USART1->DR & 0xFF;
	}
}

/** UART3_Init
 * Initializes UART1 Module
 * Pins: 
 *		tx : PB10
 *		rx : PC5
 * @param baud rate: 9600, 115200, 500000
 */
void UART3_Init(uint32_t baud){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// Initialize PB10 and PC5
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_USART3);
	
	// Initialize USART3
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);
}

/** UART3_Write
 * Sends data through tx pin for UART3
 * @param ptr to char buffer, size of buffer
 */
void UART3_Write(char *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		while((USART3->SR&USART_SR_TC) == 0);	// Wait until transmission is done
		USART3->DR = txBuf[i] & 0xFF;
	}
}

/** UART3_Read
 * Reads data from rx pin for UART3
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART3_Read(char *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		while((USART3->SR&USART_SR_RXNE) == 0);
		rxBuf[i] = USART3->DR & 0xFF;
	}
}

void UART1_OutChar(char data){
	while((USART1->SR&USART_SR_TC) == 0);	// Wait until transmission is done
	USART1->DR = data & 0xFF;
}

void UART3_OutChar(char data){
	while((USART3->SR&USART_SR_TC) == 0);	// Wait until transmission is done
	USART3->DR = data & 0xFF;
}

// this is used for printf to output to the usb uart
int fputc(int ch, FILE *f){
	UART3_Write((char *)&ch, 1);
  return 1;
}

int fgetc(FILE *f){
	char letter;
	UART3_Read(&letter, 1);
	return (int)letter;
}
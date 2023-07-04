#ifndef UART_H
#define UART_H

#include "common.h"


#define BAUD_RATE 9600
#define UART_TX_PORTE22 22 // Page 162 datasheet
#define UART_RX_PORTE23 23 // Page 162 datasheet
#define UART2_INT_PRIO 128


extern volatile uint8_t rx_data;


void initUART2(uint32_t);

void UART2_IRQHandler(void);

#endif


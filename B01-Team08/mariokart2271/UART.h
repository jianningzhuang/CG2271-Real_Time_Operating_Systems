#ifndef UART_H
#define UART_H

#include "common.h"


#define BAUD_RATE 9600
#define UART_TX_PORTE22 22 // Page 162 datasheet
#define UART_RX_PORTE23 23 // Page 162 datasheet
#define UART2_INT_PRIO 128

#define Q_SIZE 32

typedef struct {
  uint8_t Data[Q_SIZE];
  unsigned int Head; // points to oldest data element
  unsigned int Tail; // points to next free space
  unsigned int Size; // quantity of elements in queue
} Q_T;

extern Q_T rx_q;

extern volatile uint8_t rx_data;

void Q_Init(Q_T * q);

int Q_Empty(Q_T * q);

int Q_Full(Q_T * q);

int Q_Enqueue(Q_T * q, uint8_t d);

uint8_t Q_Dequeue(Q_T * q);

void initUART2(uint32_t);

void UART2_IRQHandler(void);

#endif


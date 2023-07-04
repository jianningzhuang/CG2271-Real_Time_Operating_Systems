#ifndef COMMON_H
#define COMMON_H

#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define FUNCTION_MASK(x) (x & 0xF0)  //first 4 bits determine function
#define ACTION_MASK(x) (x & 0x0F) //last 4 bits determine action
#define MASK(x) (1 << (x))

extern osSemaphoreId_t autoSem;
extern osSemaphoreId_t brainSem;
extern osSemaphoreId_t motorSem;
extern osSemaphoreId_t musicSem;
extern osSemaphoreId_t debugSem;
extern osSemaphoreId_t ledSem;

/* Delay Function */
void delay(volatile uint32_t nof);

uint32_t getMaskPort(PORT_Type *port);


#endif


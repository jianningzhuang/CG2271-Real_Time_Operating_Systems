#ifndef COMMON_H
#define COMMON_H

#include "MKL25Z4.h"                    // Device header

#define MASK(x) (1 << (x))


/* Delay Function */
void delay(volatile uint32_t nof);


#endif


#include "common.h"

osSemaphoreId_t brainSem, motorSem, musicSem, debugSem;

void delay(volatile uint32_t nof){
	while (nof!=0){
		__asm("NOP");
		nof--;
	}
}


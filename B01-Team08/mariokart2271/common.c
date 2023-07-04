#include "common.h"

osSemaphoreId_t autoSem, brainSem, motorSem, musicSem, debugSem, ledSem;

void delay(volatile uint32_t nof){
	while (nof!=0){
		__asm("NOP");
		nof--;
	}
}

uint32_t getMaskPort(PORT_Type *port) {
  if (port == PORTA)
    return SIM_SCGC5_PORTA_MASK;
  else if (port == PORTB)
    return SIM_SCGC5_PORTB_MASK;
  else if (port == PORTC)
    return SIM_SCGC5_PORTC_MASK;
  else if (port == PORTD)
    return SIM_SCGC5_PORTD_MASK;
  else
    return SIM_SCGC5_PORTE_MASK;
}

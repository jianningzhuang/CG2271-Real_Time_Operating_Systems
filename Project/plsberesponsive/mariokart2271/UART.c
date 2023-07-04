#include "UART.h"

volatile uint8_t rx_data = 0x00;

Q_T rx_q;

void Q_Init(Q_T * q) {
  unsigned int i;
  for (i=0; i<Q_SIZE; i++)
  q->Data[i] = 0; // to simplify our lives when debugging
  q->Head = 0;
  q->Tail = 0;
  q->Size = 0;
}

int Q_Empty(Q_T * q) {
  return q->Size == 0;
}

int Q_Full(Q_T * q) {
  return q->Size == Q_SIZE;
}

int Q_Enqueue(Q_T * q, uint8_t d) {
  // What if queue is full?
  if (!Q_Full(q)) {
    q->Data[q->Tail++] = d;
    q->Tail %= Q_SIZE;
    q->Size++;
    return 1; // success
  } else
    return 0; // failure
}

uint8_t Q_Dequeue(Q_T * q) {
  // Must check to see if queue is empty before dequeueing
  uint8_t t=0;
  if (!Q_Empty(q)) {
    t = q->Data[q->Head];
    q->Data[q->Head++] = 0; // to simplify debugging
    q->Head %= Q_SIZE;
    q->Size--;
  }
  return t;
}


// Init UART2
void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	// Enable clocking to the two different peripheral block
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	
	// (Alt 4 GPIO config) to receive
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	// TE and RE is to enable both transmitter and receiver
	// Clearing because we are setting up the module (setting to 0) 
	// Good practice to disable before configuration
	UART2->C2 &= ~((UART_C2_RE_MASK));
	
	// Default System Clock is 48MHz [system clock clocks to CPU]
	// Bus clock is to clock some subsystem, just how some system is designed
	// UART runs at half the system clock rate
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
	divisor = bus_clock / (baud_rate * 16); // [* 16 because oversampling internally to account for some noise -> unique to this controller]
	UART2->BDH = UART_BDH_SBR(divisor >> 8); // BDH = Buad Rate High
	UART2->BDL = UART_BDL_SBR(divisor); // BDL = Baud Rate low
	
	UART2->C1 = 0; // No parity, 8 bits, so set everything to 0
	UART2->S2 = 0; // Not configuring anything
	UART2->C3 = 0; // In case you want to send a 9th bit data then can use this. Can enable interrupts if you want to handle errors
	
	//enable interrupts for UART
	NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIO);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	// Turn on transmit and receive interrupt enable TIE, RIE
	//UART2->C2 |= ((UART_C2_TIE_MASK) | (UART_C2_RIE_MASK));
	
	//only need receive interrupt
	UART2->C2 |= UART_C2_RIE_MASK;
	
	
	// Turn on transmit and receive after done with all configuration
	UART2->C2 |= ((UART_C2_RE_MASK));
	
}	



//Technically don't need transmitter since KL25Z only receives data
void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//receiver
	if(UART2->S1 & UART_S1_RDRF_MASK) { //RDRF = 1 when UART2_D is full
		rx_data = UART2->D; //RDRF = 0 (clears) after reading from UART2_D
		osSemaphoreRelease(brainSem);
	}
	
	//error cases
	if (UART2->S1 & (UART_S1_OR_MASK 
                  | UART_S1_NF_MASK
                  | UART_S1_FE_MASK 
                  | UART_S1_PF_MASK)) {
    // handle the error
    
    // clear the flag
  }
}


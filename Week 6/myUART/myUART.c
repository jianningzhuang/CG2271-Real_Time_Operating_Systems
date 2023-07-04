#include "MKL25Z4.h"                    // Device header

// Current code will run at 48 MHz core clk freq and 24MHz Bus clk freq

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22 // Page 162 datasheet
#define UART_RX_PORTE23 23 // Page 162 datasheet
#define UART2_INT_PRIO 128

#define FUNCTION_MASK(x) (x & 0xF0)  //first 4 bits determine function
#define LED_FUNCTION 0x30

#define LED_MASK(x) (x & 0x0F) //last 4 bits determine LED
#define LED_RED_OFF 0x0
#define LED_RED_ON 0x1
#define LED_GREEN_OFF 0x2
#define LED_GREEN_ON 0x3
#define LED_BLUE_OFF 0x4
#define LED_BLUE_ON 0x5

#define RED_LED           18     // PortB Pin 18
#define GREEN_LED         19     // PortB Pin 19
#define BLUE_LED          1      // PortD Pin 1
#define MASK(x)           (1 << (x))

typedef enum {
	red = 0,
	green = 1,
	blue = 2,
} color_t;

typedef enum {
	led_off = 0,
	led_on = 1,
} state_t;

void initLED(void) {
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
	
}

//turn off all 3 colours
void offRGB(void){
	PTB->PDOR = MASK(RED_LED) | MASK(GREEN_LED);
	PTD->PDOR = MASK(BLUE_LED);
}

//only turn specific colour on/off
//can have multiple colours on at same time => colour mixing
void ledControl(color_t color, state_t state){ 
	switch (color){
		case red:
			if (state == led_off) {
				PTB->PDOR |= MASK(RED_LED);
			}
			else {
				PTB->PDOR &= ~MASK(RED_LED);  //active low LED
			}
			break;
		case green:
			if (state == led_off) {
				PTB->PDOR |= MASK(GREEN_LED);
			}
			else {
				PTB->PDOR &= ~MASK(GREEN_LED);  //active low LED
			}
			break;
		case blue:
			if (state == led_off) {
				PTD->PDOR |= MASK(BLUE_LED);
			}
			else {
				PTD->PDOR &= ~MASK(BLUE_LED);  //active low LED
			}
			break;
		default:
			offRGB();
	}
}

volatile uint8_t rx_data = 0x00;

// Init UART2
void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	// Enable clocking to the two different peripheral block
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// (Alt 4 GPIO config) to transmit
	PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4); 
	
	// (Alt 4 GPIO config) to receive
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	// TE and RE is to enable both transmitter and receiver
	// Clearing because we are setting up the module (setting to 0) 
	// Good practice to disable before configuration
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
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
	UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
}	

//Technically don't need transmitter since KL25Z only receives data
void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//receiver
	if(UART2->S1 & UART_S1_RDRF_MASK) { //RDRF = 1 when UART2_D is full
		rx_data = UART2->D; //RDRF = 0 (clears) after reading from UART2_D
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

// UART2 Transmit Poll
void UART2_Transmit_Poll(uint8_t data) {
	// check if transmit data register is empty
	// when empty you put a new data in
	while(!(UART2->S1 & UART_S1_TDRE_MASK)); //polling, exit while loop when TDRE = 1 (empty)
	
	//writes data to buffer, clears TDRE
	//can be seen at PTE22 after moving to shifter
	//waveform => start + MSB to LSB + end
	UART2->D = data;
}

// UART2 Receive Poll
uint8_t UART2_Receive_Poll(void) {
	// read data only when full
	while(!(UART2->S1 & UART_S1_RDRF_MASK)); //polling, exit while loop when RDRF = 1 (full)
	
	
	return (UART2->D);
}

// Delay function
static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void execute_data(uint8_t rx_data) {
	if(FUNCTION_MASK(rx_data) == LED_FUNCTION) {
		if(LED_MASK(rx_data) == LED_RED_OFF) {
			ledControl(red, led_off);
		}
		if(LED_MASK(rx_data) == LED_RED_ON) {
			ledControl(red, led_on);
		}
		if(LED_MASK(rx_data) == LED_GREEN_OFF) {
			ledControl(green, led_off);
		}
		if(LED_MASK(rx_data) == LED_GREEN_ON) {
			ledControl(green, led_on);
		}
		if(LED_MASK(rx_data) == LED_BLUE_OFF) {
			ledControl(blue, led_off);
		}
		if(LED_MASK(rx_data) == LED_BLUE_ON) {
			ledControl(blue, led_on);
		}
	}
	else {
		offRGB();
	}
}


int main(void) {
	//uint8_t rx_data = 0x69;
	
	SystemCoreClockUpdate();
	initLED();
	initUART2(BAUD_RATE);
	
	offRGB();
	
	while(1) {
		// RX and TX
		//UART2_Transmit_Poll(0x31);
		//rx_data = UART2_Receive_Poll();
		execute_data(rx_data);
		//delay(0x800000);
		
		//UART2_Transmit_Poll(0x30);
		//rx_data = UART2_Receive_Poll();
		//execute_data(rx_data);
		//delay(0x800000);
		
		
		//UART2_Transmit_Poll(0x33);
		//rx_data = UART2_Receive_Poll();
		//execute_data(rx_data);
		//delay(0x800000);
		
		//UART2_Transmit_Poll(0x32);
		//rx_data = UART2_Receive_Poll();
		//execute_data(rx_data);
		//delay(0x800000);
		
		//UART2_Transmit_Poll(0x35);
		//rx_data = UART2_Receive_Poll();
		//execute_data(rx_data);
		//delay(0x800000);
		
		//UART2_Transmit_Poll(0x34);
		//rx_data = UART2_Receive_Poll();
		//execute_data(rx_data);
		//delay(0x800000);
	}
}	


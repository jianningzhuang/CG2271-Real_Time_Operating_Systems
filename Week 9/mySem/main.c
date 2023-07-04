/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

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

#define SW_POS      6   //PORTD Pin 6
#define RED_LED     18  //PortB Pin 18
#define GREEN_LED   19  //PortB Pin 19
#define BLUE_LED    1   //PortD Pin 1
#define MASK(x)     (1 << (x))

typedef enum {
	red = 0,
	green = 1,
	blue = 2,
} color_t;

typedef enum {
	led_off = 0,
	led_on = 1,
} state_t;
 
osSemaphoreId_t redSem;
osSemaphoreId_t greenSem;

void initSwitch(void) {
	
	//Enable Clock for PORTD
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; //Good practice and reusable code
	
	//Configure PCR
	PORTD->PCR[SW_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[SW_POS] |= (PORT_PCR_MUX(1) |   //Select GPIO
												 PORT_PCR_PE_MASK |  //Enable pull resistor
												 PORT_PCR_PS_MASK |  //Enable pullup
												 PORT_PCR_IRQC(0x0a)); //b'1010 for falling edge trigger (active low switch)
	
	//Set Switch Bit as Input
	PTD->PDDR &= ~MASK(SW_POS);
	
	//Enable Interrupts
	NVIC_SetPriority(PORTD_IRQn, 2);  //Set priority to 2 (PORTD_IRQn is defined 31 in header file)
	NVIC_ClearPendingIRQ(PORTD_IRQn); //Good practice in case interrupt triggered before your code
	NVIC_EnableIRQ(PORTD_IRQn);       //Start registering interrupts
	
}

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

void offRGB(void){
	PTB->PDOR = MASK(RED_LED) | MASK(GREEN_LED);
	PTD->PDOR = MASK(BLUE_LED);
}

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

static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void PORTD_IRQHandler() {
	//Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);  //Good practice or legacy code
	
	delay(0x80000); //delay to help with debouncing, else many semaphores might be released
	//osSemaphoreRelease(mySem); //if release too many, might have error if exceed count

	//Clear INT Flag in ISFR (remains set until logic 1 is manually written)
	PORTD->ISFR |= MASK(SW_POS); //Write 1 to clear interrupt on PORTD Pin 6
}

volatile uint8_t rx_data = 0x00;

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
	UART2->C2 &= ~(UART_C2_RE_MASK);
	
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
	UART2->C2 |= (UART_C2_RE_MASK);
}	

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//receiver
	if(UART2->S1 & UART_S1_RDRF_MASK) { //RDRF = 1 when UART2_D is full
		//osSemaphoreRelease(mySem); // release semaphore when receiving data packet
		rx_data = UART2->D; //RDRF = 0 (clears) after reading from UART2_D
		if(FUNCTION_MASK(rx_data) == LED_FUNCTION) {
			if(LED_MASK(rx_data) == LED_RED_ON) {
				osSemaphoreRelease(redSem);
			}
			if(LED_MASK(rx_data) == LED_GREEN_ON) {
				osSemaphoreRelease(greenSem);
			}
		}
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


void led_red_thread(void *argument) {
	for (;;) {
		osSemaphoreAcquire(redSem, osWaitForever);
		
		ledControl(red, led_on);
		osDelay(1000);
		ledControl(red, led_off);
		osDelay(1000);
		
		//osSemaphoreRelease(mySem);
	}
}

void led_green_thread(void *argument) {
	for (;;) {
		osSemaphoreAcquire(greenSem, osWaitForever);
		
		ledControl(green, led_on);
		osDelay(1000);
		ledControl(green, led_off);
		osDelay(1000);
		
		//osSemaphoreRelease(mySem);
	}
}
 
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initSwitch();
	initUART2(BAUD_RATE);
	initLED();
	offRGB();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	redSem = osSemaphoreNew(1, 0, NULL);
	greenSem = osSemaphoreNew(1, 0, NULL);
	osThreadNew(led_red_thread, NULL, NULL);
	osThreadNew(led_green_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}

//mySem = osSemaphoreNew(1, 1, NULL); binary semaphore very similar to Mutex

//mySem = osSemaphoreNew(1, 0, NULL); binary semaphore with initial count set to 0 meaning event to unblock threads has not happened yet

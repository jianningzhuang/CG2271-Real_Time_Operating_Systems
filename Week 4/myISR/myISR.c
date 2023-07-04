#include "MKL25Z4.h"             // Device header

#define SW_POS            6      //PORTD Pin 6
#define RED_LED           18     // PortB Pin 18
#define GREEN_LED         19     // PortB Pin 19
#define BLUE_LED          1      // PortD Pin 1
#define MASK(x)           (1 << (x))

volatile int counter = 0;

typedef enum {
	red = 0,
	green = 1,
	blue = 2,
} color_t;

color_t led_mapping[3] = {red, green, blue};

void InitSwitch(void) {
	
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

void InitLED(void) {
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

void clearRGB(void){
	PTB->PDOR = MASK(RED_LED) | MASK(GREEN_LED);
	PTD->PDOR = MASK(BLUE_LED);
}

void ledControl(color_t color){
	clearRGB();
	switch (color){
		case red:
			PTB->PDOR = ~MASK(RED_LED);  //active low LED
			break;
		case green:
			PTB->PDOR = ~MASK(GREEN_LED);
			break;
		case blue:
			PTD->PDOR = ~MASK(BLUE_LED);
			break;
		default:
			clearRGB();
	}
}

static void delay(volatile uint32_t nof){
	while (nof!=0){
		__asm("NOP");
		nof--;
	}
}

void PORTD_IRQHandler() {
	//Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);  //Good practice or legacy code
	
	//Update Counter
	if ((PORTD->ISFR & MASK(SW_POS))) {
		counter++;
		if (counter > 0x02) {
			counter = 0;
		}
		delay(300000);
	}

	//Clear INT Flag in ISFR (remains set until logic 1 is manually written)
	PORTD->ISFR |= MASK(SW_POS); //Write 1 to clear interrupt on PORTD Pin 6
}

int main(void) {
	SystemCoreClockUpdate();
	
	InitSwitch();
	InitLED();
	while(1) {
		ledControl(led_mapping[counter]);
	}
}


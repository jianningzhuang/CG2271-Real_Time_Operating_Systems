#include "MKL25Z4.h"                    // Device header

#define RED_LED     18 // PortB Pin 18
#define GREEN_LED   19 // PortB Pin 19
#define BLUE_LED    1 // PortD Pin 1
#define MASK(x)     (1 << (x))

typedef enum {
	red = 0,
	green = 1,
	blue = 2,
} color_t;

unsigned int counter = 0;
color_t led_mapping[3] = {red, green, blue};


void InitGPIO(void){
	
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

//Control LED based on counter

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

//Delay Function

static void delay(volatile uint32_t nof){
	while (nof!=0){
		__asm("NOP");
		nof--;
	}
}

int main(void){
	SystemCoreClockUpdate();
	InitGPIO();
	while(1){
		ledControl(led_mapping[counter]);
		counter++;
		if (counter > 0x02){
			counter = 0;
		}
		delay(0x400000);
	}
}







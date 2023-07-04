/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define RED_LED     18 // PortB Pin 18
#define GREEN_LED   19 // PortB Pin 19
#define BLUE_LED    1 // PortD Pin 1
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

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
  // ...
  for (;;) {
		ledControl(red, led_on);
		osDelay(1000);
		ledControl(red, led_off);
		osDelay(1000);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initLED();
	offRGB();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}

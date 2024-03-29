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

#define MSG_COUNT   1


osThreadId_t green_id;
osThreadId_t red_id;
osSemaphoreId_t mySem;


osEventFlagsId_t evt_id;

typedef struct {
	uint8_t cmd;
	uint8_t data;
}myDataPkt;

osMessageQueueId_t redMsg, greenMsg, blueMsg;

osMutexId_t myMutex;

osStatus_t myStatus;


const osThreadAttr_t thread_attr = {
	.priority = osPriorityNormal1
};

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

void led_red_thread(void *argument) {
	for (;;) {
//		osSemaphoreAcquire(mySem, osWaitForever);
//		
//		ledControl(red, led_on);
//		osDelay(1000);
//		ledControl(red, led_off);
//		osDelay(1000);
		
//		osMutexAcquire(myMutex, osWaitForever);
//		ledControl(red, led_on);
//		osDelay(1000);
//		ledControl(red, led_off);
//		osDelay(1000);
		
		osEventFlagsWait(evt_id, 0x0010, osFlagsWaitAny, osWaitForever);
		osSemaphoreAcquire(mySem, 0);
		ledControl(red, led_on);

	}
}

void led_green_thread(void *argument) {
	static int init = 1;
	for (;;) {
//		osSemaphoreAcquire(mySem, osWaitForever);
//		
//		ledControl(green, led_on);
//		osDelay(1000);
//		ledControl(green, led_off);
//		osDelay(1000);
//    
//		if (init == 1){
//			osThreadSetPriority(red_id, osPriorityAboveNormal);
//			init = 0;
//		}
		
//		osMutexAcquire(myMutex, osWaitForever);
//		ledControl(green, led_on);
//		osDelay(1000);
//		ledControl(blue, led_off);
//		osDelay(1000);
		
		osEventFlagsWait(evt_id, 0x0001, osFlagsWaitAny, osWaitForever);
		osSemaphoreAcquire(mySem, osWaitForever);
		ledControl(green, led_on);
		osSemaphoreRelease(mySem);
	}
}

void led_blue_thread(void *argument) {
	for (;;) {
//		osMutexAcquire(myMutex, osWaitForever);
//		ledControl(blue, led_on);
//		osDelay(1000);
//		ledControl(blue, led_off);
//		osDelay(1000);
		
		osEventFlagsWait(evt_id, 0x0001, osFlagsWaitAny, osWaitForever);
		osSemaphoreAcquire(mySem, osWaitForever);
		ledControl(blue, led_on);
		osSemaphoreRelease(mySem);
	}
}

void control_thread(void *argument) {
	for (;;) {
		osEventFlagsSet(evt_id, 0x0001);
		osDelay(1000);
		osEventFlagsSet(evt_id, 0x0010);
		osDelay(1000);
		offRGB();
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
	mySem = osSemaphoreNew(1,1,NULL);
	myMutex = osMutexNew(NULL);
	evt_id = osEventFlagsNew(NULL);
	osThreadNew(led_red_thread, NULL, NULL);
	osThreadNew(led_green_thread, NULL, NULL);
	osThreadNew(led_blue_thread, NULL, NULL);
	osThreadNew(control_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}



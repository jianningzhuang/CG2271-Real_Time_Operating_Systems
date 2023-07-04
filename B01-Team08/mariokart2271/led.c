#include "led.h"

uint8_t greenLEDPins[] = {8, 9, 10, 11, 2, 3, 4, 5, 20, 21};
uint8_t redLEDPins[] = {9};
uint8_t portBPins[] = {8, 9, 10, 11};
uint8_t portCPins[] = {9};
uint8_t portEPins[] = {2, 3, 4, 5, 20, 21};


void initLEDModules() {
  InitGPIOAll(PORTB, portBPins, 4);
  InitGPIOAll(PORTE, portEPins, 6);
  InitGPIOAll(PORTC, portCPins, 1);
}

void offRedLED() {
	setPin(PORTC, portCPins[0], LOW);
}

void connectLEDSequence() {
	onGreenLED();
	delay(1000000);
	offGreenLED();
	delay(1000000);
	onGreenLED();
	delay(1000000);
	offGreenLED();
	delay(1000000);
}

void offGreenLED() {
	for (uint8_t i = 0; i < 4; i++) 
		setPin(PORTB, greenLEDPins[i], LOW);
	for (uint8_t i = 4; i < 10; i++) 
		setPin(PORTE, greenLEDPins[i], LOW);
}

void offBothLED() {
	offRedLED();
	offGreenLED();
}

void runningLEDHelper(PORT_Type *port, uint8_t pin) {
	setPin(port, pin, HIGH);
	 osDelay(100);
  setPin(port, pin, LOW);
}

void runningLED() {
	offGreenLED();
	for (uint8_t i = 0; i < 4; i++)
		runningLEDHelper(PORTB, greenLEDPins[i]);
	for (uint8_t i = 4; i < 10; i++)
		runningLEDHelper(PORTE, greenLEDPins[i]);
}

void onGreenLED() {
		for (uint8_t i = 0; i < 4; i++) 
		setPin(PORTB, greenLEDPins[i], HIGH);
	for (uint8_t i = 4; i < 10; i++) 
		setPin(PORTE, greenLEDPins[i], HIGH);
}

void flashRedLEDStationary() {
  setPin(PORTC, 9, HIGH);
  osDelay(250);
  setPin(PORTC, 9, LOW);
  osDelay(250);
}

void flashRedLEDMoving() {
	setPin(PORTC, 9, HIGH);
  osDelay(500);
  setPin(PORTC, 9, LOW);
  osDelay(500);
}


	

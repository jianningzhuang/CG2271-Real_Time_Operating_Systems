#include "GPIO.h"

void initGPIOPort(PORT_Type *port, uint8_t pin) {
  port->PCR[pin] &= ~PORT_PCR_MUX_MASK;
  port->PCR[pin] |= PORT_PCR_MUX(1);  
}

GPIO_Type* getGPIO(PORT_Type* port) {
  if (port == PORTA)
    return PTA;
  else if (port == PORTB)
    return PTB;
  else if (port == PORTC)
    return PTC;
  else if (port == PORTD)
    return PTD;  
  else
    return PTE;
}


void InitGPIOAll(PORT_Type *port, uint8_t pins[], uint8_t numPins)
{
  uint32_t mask = 0;

  SIM->SCGC5 |= getMaskPort(port);

  for (uint8_t i = 0; i < numPins; ++i) {
    initGPIOPort(port, pins[i]);
    mask |= MASK(pins[i]);
  }
  
  getGPIO(port)->PDDR |= mask;
  
}

void setPin(PORT_Type *port, uint8_t pin, value_t value) {
  GPIO_Type *gpio = getGPIO(port);
  if (value == HIGH)
    gpio->PSOR |= MASK(pin);
  else
    gpio->PCOR |= MASK(pin);
}

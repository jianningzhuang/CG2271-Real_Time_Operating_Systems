#include "common.h"

typedef enum {LOW, HIGH} value_t;

void initGPIOPort(PORT_Type *port, uint8_t pin);

void setPin(PORT_Type *port, uint8_t pin, value_t value);

void InitGPIOAll(PORT_Type *port, uint8_t pins[], uint8_t numPins);

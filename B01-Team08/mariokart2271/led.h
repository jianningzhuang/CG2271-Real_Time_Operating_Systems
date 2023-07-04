#include "GPIO.h"

void initLEDModules(void);

void offRedLED(void);
	
void offGreenLED(void);

void offBothLED(void);

void runningLEDHelper(PORT_Type *port, uint8_t pin);

void runningLED(void);

void onGreenLED(void);

void flashRedLEDStationary(void);

void flashRedLEDMoving(void);
	
void connectLEDSequence(void);


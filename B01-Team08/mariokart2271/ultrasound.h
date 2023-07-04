#include "common.h"
#define ECHO_POS 1
#define TRIG_POS 5
#define VCC_POS 12

void initUltrasonic(void);
void sendTrigPulse(void);
int measureDistance(void);
int readUltrasonic(void);
void TPM2_IRQHandler (void);

#ifndef MOTORS_H
#define MOTORS_H
#include "common.h"

//Front Left Wheel
//#define PTA1_Pin  1  //TPM2_CH0
//#define PTA2_Pin  2  //TPM2_CH1
#define PTD1_Pin  1  //TPM0_CH1
#define PTD3_Pin  3  //TPM0_CH3

//Front Right Wheel
#define PTD4_Pin  4  //TPM0_CH4
#define PTD5_Pin  5  //TPM0_CH5

//Back Left Wheel
#define PTD0_Pin  0  //TPM0_CH0
//#define PTD1_Pin  1  //TPM0_CH1

//Back Right Wheel
#define PTD2_Pin  2  //TPM0_CH2
//#define PTD3_Pin  3  //TPM0_CH3

extern volatile uint8_t ai_moving_flag;

typedef enum {
	FLF = 0, //Front Left Forward
	FLR = 1, //Front Left Reverse
	FRF = 2, //Front Right Forward
	FRR = 3, //Front Right Reverse
} wheel_t;



void initMotorsPWM(void);

void controlWheelSpeed(wheel_t, uint16_t);

void stop(void);

void forty_five_left(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);

void ninety_left(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);

void forty_five_right(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t); 

void ninety_right(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t); 

void forward(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t); 

void reverse(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t); 

void move(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

#endif


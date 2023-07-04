#include "motors.h"

volatile uint8_t ai_moving_flag = 0;

void initMotorsPWM(void) {
  
  //Enable Clock Gating for PORTD
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
  
  
  
  //Alt 4 for TPM0_CH0-5
  PORTD->PCR[PTD0_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD0_Pin] |= PORT_PCR_MUX(4);
  
  PORTD->PCR[PTD1_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD1_Pin] |= PORT_PCR_MUX(4);
  
  PORTD->PCR[PTD2_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD2_Pin] |= PORT_PCR_MUX(4);
  
  PORTD->PCR[PTD3_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD3_Pin] |= PORT_PCR_MUX(4);
  
  
  //Enable Clock Gating for Timer0 and Timer 2
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
  

  
  //Select clock for TPM Module
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= ~SIM_SOPT2_TPMSRC(1); //MCGFLLCLK clock or MCGPLLCLK/2 (built in module clock)
  
  
  //Set Modulo Value
  TPM0->MOD = 7500;
  
  
  /*Edge-Aligned PWM*/
  
  //Configure TPMx_SC  (Status and Control)
  //PS = 111 (128), CMOD = 01 (counter increments on every LPTPM counter clock, CPWMS = 0 (up counting)
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK);

  
  
  //Configure TPMx_CnSC (Channel Configuration)
  //TPM0_CH0 -> PTD0   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
  TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
  TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
  
  //TPM0_CH1 -> PTD1   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
  TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
  TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
  
  //TPM0_CH2 -> PTD2   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
  TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
  TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
  
  //TPM0_CH3 -> PTD3   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
  TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
  TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));


}

void controlWheelSpeed(wheel_t wheel, uint16_t speed) {
  //MOD = 7500, speed from 0 to 100 => 0 to 7500 => 0% to 100% duty cycle
  //e.g speed = 50 => 3750 = 50% duty cycle
  uint16_t duty = speed * 75;
  switch (wheel) {
  case FLF:
    TPM0_C2V = duty;
    break;
  case FLR:
    TPM0_C1V = duty;
    break;
  case FRF:
    TPM0_C0V = duty;
    break;
  case FRR:
    TPM0_C3V = duty;
    break;
  }
}

void stop() {
  controlWheelSpeed(FLF,0);
  controlWheelSpeed(FLR,0);
  controlWheelSpeed(FRF,0);
  controlWheelSpeed(FRR,0);
}

void forty_five_left(uint16_t delay, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
  controlWheelSpeed(FLF,0);
  controlWheelSpeed(FLR,35);
  controlWheelSpeed(FRF,35);
  controlWheelSpeed(FRR,0);
	ai_moving_flag = 1;
  osDelay(delay);
  stop();
	ai_moving_flag = 0;
	osDelay(1100);
}

void ninety_left(uint16_t delay, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
  controlWheelSpeed(FLF,0);
  controlWheelSpeed(FLR,35);
  controlWheelSpeed(FRF,35);
  controlWheelSpeed(FRR,0);
  ai_moving_flag = 1;
  osDelay(delay);
  stop();
	ai_moving_flag = 0;
	osDelay(1100);
}

void forty_five_right(uint16_t delay, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
  controlWheelSpeed(FLF,35);
  controlWheelSpeed(FLR,0);
  controlWheelSpeed(FRF,0);
  controlWheelSpeed(FRR,35);
  osDelay(delay);
  stop();
}

void ninety_right(uint16_t delay, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
  controlWheelSpeed(FLF,35);
  controlWheelSpeed(FLR,0);
  controlWheelSpeed(FRF,0);
  controlWheelSpeed(FRR,35);
	ai_moving_flag = 1;
  osDelay(delay);
  stop();
	ai_moving_flag = 0;
	osDelay(1100);
}

void forward(uint16_t delay, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
  controlWheelSpeed(FLF,35);
  controlWheelSpeed(FLR,0);
  controlWheelSpeed(FRF,35);
  controlWheelSpeed(FRR,0);
	ai_moving_flag = 1;
  osDelay(delay);
  stop();
	ai_moving_flag = 0;
	osDelay(1100);
}

void reverse(uint16_t delay, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
  controlWheelSpeed(FLF,0);
  controlWheelSpeed(FLR,FLspeed);
  controlWheelSpeed(FRF,0);
  controlWheelSpeed(FRR,FRspeed);
  osDelay(delay);
  stop();
}

void move(uint8_t move_data, uint8_t FLspeed, uint8_t FRspeed, uint8_t BLspeed, uint8_t BRspeed) {
	switch(move_data) {
	case 0x20: //no flags
		stop();
		break;
	case 0x10: //forward flag only
		controlWheelSpeed(FLF,FLspeed);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,FRspeed);
		controlWheelSpeed(FRR,0);
		break;
	case 0x11: //reverse flag only
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,30);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,30);
		break;
	case 0x12: //left flag only
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,30);
		controlWheelSpeed(FRF,30);
		controlWheelSpeed(FRR,0);
		break;
	case 0x13: //right flag only
		controlWheelSpeed(FLF,30);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,30);
		break;
	case 0x14: //hard left flag only
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,45);
		controlWheelSpeed(FRF,45);
		controlWheelSpeed(FRR,0);
		break;
	case 0x15: //hard right flag only
		controlWheelSpeed(FLF,45);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,45);
		break;
	case 0x16: //boost flag only
		controlWheelSpeed(FLF,90);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,90);
		controlWheelSpeed(FRR,0);
		break;
	case 0x17: //slow flag only
		controlWheelSpeed(FLF,30);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,30);
		controlWheelSpeed(FRR,0);
		break;
	case 0x18: //forward and left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,10);
		controlWheelSpeed(FRF,30);
		controlWheelSpeed(FRR,0);
	case 0x19: //forward and right
		controlWheelSpeed(FLF,30);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,10);
		break;
	case 0x1A: //forward and hard left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,20);
		controlWheelSpeed(FRF,35);
		controlWheelSpeed(FRR,0);
		break;
	case 0x1B: //forward and hard right
		controlWheelSpeed(FLF,35);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,20);
		break;
	case 0x1C: //reverse and left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,10);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,40);
		break;
	case 0x1D: //reverse and right
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,40);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,10);
		break;
	case 0x1E: //reverse and hard left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,20);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,50);
		break;
	case 0x1F: //reverse and hard right
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,50);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,20);
		break;
	case 0x21: //boost and left
		controlWheelSpeed(FLF,00);
		controlWheelSpeed(FLR,20);
		controlWheelSpeed(FRF,60);
		controlWheelSpeed(FRR,0);
		break;
	case 0x22: //boost and right
		controlWheelSpeed(FLF,60);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,20);
		controlWheelSpeed(FRR,0);
		break;
	case 0x23: //boost and hard left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,20);
		controlWheelSpeed(FRF,35);
		controlWheelSpeed(FRR,0);
		break;
	case 0x24: //boost and hard right
		controlWheelSpeed(FLF,35);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,20);
		break;
	case 0x25: //slow and left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,20);
		controlWheelSpeed(FRF,35);
		controlWheelSpeed(FRR,0);
		break;
	case 0x26: //slow and right
		controlWheelSpeed(FLF,35);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,20);
		break;
	case 0x27: //slow and hard left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,20);
		controlWheelSpeed(FRF,35);
		controlWheelSpeed(FRR,0);
		break;
	case 0x28: //slow and hard right
		controlWheelSpeed(FLF,35);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,20);
		break;
	default:
		stop();
		break;
	}
}

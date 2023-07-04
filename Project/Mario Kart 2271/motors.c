#include "motors.h"

void initMotorsPWM(void) {
	
	//Enable Clock Gating for PORTA and PORTD
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK);
	
	
	//Alt 3 for TPM2_CH0 and TPM2_CH1
	PORTA->PCR[PTA1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA1_Pin] |= PORT_PCR_MUX(3);
	
	PORTA->PCR[PTA2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA2_Pin] |= PORT_PCR_MUX(3);
	
	//Alt 4 for TPM0_CH0-5
	PORTD->PCR[PTD0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD0_Pin] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[PTD1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD1_Pin] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[PTD2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD2_Pin] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[PTD3_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD3_Pin] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[PTD4_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD4_Pin] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[PTD5_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD5_Pin] |= PORT_PCR_MUX(4);
	
	
	//Enable Clock Gating for Timer0 and Timer 2
	SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK);
	

	
	//Select clock for TPM Module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= ~SIM_SOPT2_TPMSRC(1); //MCGFLLCLK clock or MCGPLLCLK/2 (built in module clock)
	
	
	//Set Modulo Value
	TPM0->MOD = 7500;
	TPM2->MOD = 7500;
	
	
	/*Edge-Aligned PWM*/
	
	//Configure TPMx_SC  (Status and Control)
	//PS = 111 (128), CMOD = 01 (counter increments on every LPTPM counter clock, CPWMS = 0 (up counting)
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	
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
	
	//TPM0_CH4 -> PTD4   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
	TPM0_C4SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//TPM0_CH5 -> PTD5   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//TPM2_CH0 -> PTA1   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//TPM2_CH1 -> PTA2   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

}

void controlWheelSpeed(wheel_t wheel, uint16_t speed) {
	//MOD = 7500, speed from 0 to 100 => 0 to 7500 => 0% to 100% duty cycle
	//e.g speed = 50 => 3750 = 50% duty cycle
  uint16_t duty = speed * 75;
  switch (wheel) {
  case FLF:
    TPM2_C0V = duty;
    break;
	case FLR:
    TPM2_C1V = duty;
    break;
  case FRF:
    TPM0_C4V = duty;
    break;
  case FRR:
    TPM0_C5V = duty;
    break;
  case BLF:
    TPM0_C0V = duty;
    break;
  case BLR:
    TPM0_C1V = duty;
    break;
  case BRF:
    TPM0_C2V = duty;
    break;
  case BRR:
    TPM0_C3V = duty;    
    break;
  }
}

void stop() {
  controlWheelSpeed(FLF,0);
  controlWheelSpeed(FLR,0);
  controlWheelSpeed(FRF,0);
  controlWheelSpeed(FRR,0);
  controlWheelSpeed(BLF,0);
  controlWheelSpeed(BLR,0);
  controlWheelSpeed(BRF,0);
  controlWheelSpeed(BRR,0);
}

void move(uint8_t rx_data) {
	switch(rx_data) {
	case 0x00: //Stop
		stop();
		break;
	case 0x01: //Forward
		controlWheelSpeed(FLF,50);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,50);
		controlWheelSpeed(FRR,0);
		controlWheelSpeed(BLF,50);
		controlWheelSpeed(BLR,0);
		controlWheelSpeed(BRF,50);
		controlWheelSpeed(BRR,0);
		break;
	case 0x02: //Forward Left
		controlWheelSpeed(FLF,25);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,75);
		controlWheelSpeed(FRR,0);
		controlWheelSpeed(BLF,50);
		controlWheelSpeed(BLR,0);
		controlWheelSpeed(BRF,50);
		controlWheelSpeed(BRR,0);
		break;
	case 0x03: //Left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,50);
		controlWheelSpeed(FRF,50);
		controlWheelSpeed(FRR,0);
		controlWheelSpeed(BLF,0);
		controlWheelSpeed(BLR,50);
		controlWheelSpeed(BRF,50);
		controlWheelSpeed(BRR,0);
		break;
	case 0x04: //Forward Right
		controlWheelSpeed(FLF,75);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,25);
		controlWheelSpeed(FRR,0);
		controlWheelSpeed(BLF,50);
		controlWheelSpeed(BLR,0);
		controlWheelSpeed(BRF,50);
		controlWheelSpeed(BRR,0);
		controlWheelSpeed(FLF, 75);
		break;
	case 0x05: //Right
		controlWheelSpeed(FLF,50);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,50);
		controlWheelSpeed(BLF,50);
		controlWheelSpeed(BLR,0);
		controlWheelSpeed(BRF,0);
		controlWheelSpeed(BRR,50);
		break;
	case 0x06: //Reverse
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,50);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,50);
		controlWheelSpeed(BLF,0);
		controlWheelSpeed(BLR,50);
		controlWheelSpeed(BRF,0);
		controlWheelSpeed(BRR,50);
		break;
	case 0x07: //Reverse Left
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,50);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,50);
		controlWheelSpeed(BLF,0);
		controlWheelSpeed(BLR,25);
		controlWheelSpeed(BRF,0);
		controlWheelSpeed(BRR,75);
		break;
	case 0x08: //Reverse Right
		controlWheelSpeed(FLF,0);
		controlWheelSpeed(FLR,50);
		controlWheelSpeed(FRF,0);
		controlWheelSpeed(FRR,50);
		controlWheelSpeed(BLF,0);
		controlWheelSpeed(BLR,75);
		controlWheelSpeed(BRF,0);
		controlWheelSpeed(BRR,25);
		break;
	default:
		stop();
		break;
	}
}

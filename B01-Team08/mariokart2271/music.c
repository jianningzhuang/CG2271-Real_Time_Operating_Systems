#include "music.h" 

uint32_t tempo = 40;

uint32_t spiderman_melody[] = {C4, REST, DS4, REST, G4, REST, F4, REST, DS4, REST, C4, REST, 
															C4, REST, DS4, REST, G4, REST, GS4, REST, G4, REST, F4, REST, DS4, REST, C4, REST,
															F4, REST, GS4, REST, C5, REST, AS4, REST, GS4, REST, F4, REST, 
															C4, REST, DS4, REST, G4, REST, F4, REST, DS4, REST, C4, REST,
															GS4, REST, G4, REST, F4, REST, F4, REST, DS4, REST, F4, REST, DS4, REST, C4, REST,};
uint8_t spiderman_durations[] = {14, 2, 6, 2, 22, 18, 14, 2, 6, 2, 22, 18, 
																14, 2, 6, 2, 14, 2, 6, 2, 14, 2, 14, 2, 6, 2, 22, 18,
																14, 2, 6, 2, 22, 18, 14, 2, 6, 2, 22, 18, 
																14, 2, 6, 2, 22, 18, 14, 2, 6, 2, 22, 2,
																6, 2, 22, 50, 6, 2, 14, 2, 6, 2, 14, 2, 14, 2, 22, 18};

uint32_t spideman_length = sizeof(spiderman_melody)/4;
																
uint32_t mariokart_melody[] = {REST, D6, REST, DS6, REST, E6, REST, D6, REST, D6, REST, E6, REST, F6, REST, C7, REST,
															D7, REST, C7, REST, D7, REST, C7, REST, AS6, REST, A6, REST, G6, REST, F6, REST, C6, REST,
															D6, REST, DS6, REST, E6, REST, D6, REST, D6, REST, E6, REST, F6, REST, C7, REST,
															F7, REST, D7, REST, C7, REST, AS6, REST, G6, REST, F6, REST, C6, REST, CS6, REST,
															D6, REST, DS6, REST, E6, REST, D6, REST, D6, REST, E6, REST, F6, REST, C7, REST,
															D7, REST, C7, REST, D7, REST, C7, REST, AS6, REST, A6, REST, G6, REST, F6, REST, C7, REST,
															F7, REST, D7, REST, C7, REST, AS6, REST, A6, REST, AS6, REST, B6, REST, C7, REST, D7, REST, F6, REST,
															A6, REST, F6, REST, D6, REST, E6, REST, F6, REST, C6, REST, CS6, REST, D6, REST,
															DS6, REST, E6, REST, F6, REST, DS6, REST, DS6, REST, F6, REST, FS6, REST, CS7, REST,
															DS7, REST, CS7, REST, DS7, REST, CS7, REST, B6, REST, AS6, REST, GS6, REST, FS6, REST, CS6, REST,
															DS6, REST, E6, REST, F6, REST, DS6, REST, DS6, REST, F6, REST, FS6, REST, CS7, REST,
															FS7, REST, DS7, REST, CS7, REST, B6, REST, GS6, REST, FS6, REST, GS5, REST, FS5, REST, F5, REST,
															E5, REST, GS5, REST, B5, REST, DS6, REST, FS6, REST, DS6, REST, B5, REST, GS5, REST,
															E5, REST, GS5, REST, B5, REST, DS6, REST, FS6, REST, DS6, REST, DS6, REST, FS6, REST, GS6, REST,
															A6, REST, GS6, REST, FS6, REST, D6, REST, CS6, REST, D6, REST, DS6, REST, E6, REST, CS6, REST, D6, REST, CS6, REST,
															D6, REST, CS6, REST, B5, REST, A5, REST, GS5, REST, E5, REST, E5, REST, FS5, REST, GS5, REST, G5, REST, FS5, REST,
															F5, REST, A5, REST, C6, REST, E6, REST, G6, REST, E6, REST, C6, REST, A5, REST, GS5, REST, G5, REST,
															F5, REST, A5, REST, C6, REST, E6, REST, G6, REST, E6, REST, D6, REST, B5, REST, G6, REST, A6, REST,
															AS6, REST, A6, REST, G6, REST, F6, REST, D6, REST, DS6, REST, E6, REST, F6, REST, AS5, REST, F6, REST,
															DS6, REST, F6, REST, DS6, REST, C6, REST, G6, REST, AS6, REST, E6, REST, AS6, REST, G6, REST, AS6, REST, C7, REST, CS7, REST, D7, REST, DS7, REST, E7, REST,
															F7, REST,
															B5, REST, B5, REST, C6, REST, CS6, REST,
															D6, REST, AS5, REST, D6, REST, 
															D6, REST, C6, REST, D6, REST, DS6, REST, 
															E6, REST, C6, REST, E6, REST, 
															E6, REST, D6, REST, E6, REST, F6, REST, 
															A6, REST, A6, REST, G6, REST, G6, REST,
															F6, REST, F6, REST, E6, REST, E6, REST, 
															C7, REST, C7, REST, AS6, REST, AS6, REST,
															GS6, REST, GS6, REST, FS6, REST, FS6, REST, 
															AS6, REST, AS6, REST, F6, REST, AS6, REST, AS6, REST, AS6, REST, F6, REST, F6, REST, 
															C5, REST, CS5, REST, D5, REST, CS5, REST, C5, REST, CS5, REST, D5, REST, CS5, REST, C5, REST, CS5, REST, D5, REST, DS5, REST, E5, REST, F5, REST, FS5, REST, G5, REST, GS5, REST, A5, REST, AS5, REST, B5, REST,};
																
uint8_t mariokart_durations[] = {6, 4, 2, 4, 2, 7, 2, 4, 2, 4, 2, 1, 2, 4, 2, 4, 2, 
																3, 3, 3, 3, 3, 3, 3, 2, 1, 2, 1, 2, 1, 2, 7, 2, 4, 2,
																4, 2, 4, 2, 7, 2, 4, 2, 4, 2, 1, 2, 4, 2, 4, 2, 
																4, 2, 4, 2, 1, 2, 1, 2, 1, 2, 1, 20, 1, 2, 1, 2,
																4, 2, 4, 2, 7, 2, 4, 2, 4, 2, 1, 2, 4, 2, 4, 2, 
																3, 3, 3, 3, 3, 3, 3, 3, 1, 2, 1, 2, 1, 2, 7, 2, 4, 2,
																4, 2, 4, 2, 4, 2, 4, 2, 1, 2, 1, 2, 1, 2, 4, 2, 4, 2, 1, 2,
																4, 2, 4, 2, 1, 2, 4, 2, 1, 20, 1, 1, 1, 1, 1, 1,
																3, 3, 3, 3, 6, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 
																3, 3, 3, 3, 3, 3, 3, 3, 1, 2, 1, 2, 1, 2, 7, 2, 4, 2,
																3, 3, 3, 3, 6, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3,
																3, 3, 3, 3, 1, 2, 1, 2, 1, 2, 1, 20, 1, 1, 1, 1, 1, 1,
																2, 4, 1, 2, 2, 4, 2, 4, 2, 4, 2, 4, 7, 2, 2, 4,
																2, 4, 1, 2, 2, 4, 2, 4, 2, 4, 2, 4, 1, 2, 2, 4, 2, 4,
																2, 4, 2, 4, 2, 4, 2, 4, 1, 2, 1, 2, 1, 2, 1, 5, 1, 2, 1, 2, 1, 2,
																2, 4, 2, 4, 2, 4, 2, 4, 1, 2, 2, 4, 1, 2, 2, 4, 1, 1, 1, 1, 1, 1,
																2, 4, 1, 2, 1, 5, 2, 4, 2, 4, 2, 4, 2, 4, 1, 1, 1, 1, 1, 1,
																2, 4, 1, 2, 1, 5, 2, 4, 2, 4, 1, 2, 1, 2, 1, 2, 2, 4, 2, 4,
																2, 4, 2, 4, 2, 4, 2, 4, 1, 2, 1, 2, 1, 2, 1, 5, 4, 2, 7, 2,
																1, 1, 1, 1, 1, 1, 4, 2, 1, 2, 4, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
																58, 2,
																10, 2, 7, 2, 7, 2, 4, 2, 
																16, 2, 4, 2, 40, 2,
																4, 2, 7, 2, 7, 2, 4, 2, 
																16, 2, 4, 2, 40, 2,
																4, 2, 7, 2, 7, 2, 4, 2, 
																3, 15, 2, 4, 3, 6, 1, 14,
																3, 15, 2, 4, 3, 6, 1, 14,
																3, 9, 2, 7, 3, 12, 3, 9,
																3, 9, 2, 7, 3, 12, 3, 9,
																2, 4, 2, 4, 2, 7, 2, 4, 2, 4, 1, 2, 2, 4, 2, 4,
																2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,};

uint32_t mariokart_length = sizeof(mariokart_melody)/4;

uint32_t startup_melody[] = {REST, E7, REST, C7, REST, G6, REST, A6, REST, C7, REST, C7, REST};

uint8_t startup_durations[] = {6, 2, 4, 2, 4, 2, 4, 2, 1, 2, 7, 4, 22};

uint32_t startup_length = sizeof(startup_melody)/4;
																
void initMusicPWM(void) {
	
	//Enable Clock Gating for PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	/*Q1
	PTB0 and PTB1 are being used for Timer, so need to enable power to PORTB
	*/
	
	//Alternative 3 for TPM1_CH0 and TPM1_CH1
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	//PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	//PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	/*Q2
	Configure MUX bits to choose alternative 3
	Based on table in page 161, timer fucntionality is tied to MUX 3
	*/
	
	//Enable Clock Gating for Timer1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	/*Q4
	Enable Clock Gating for Timer Module
	When dealing with peripherals, enable power to peripheral subsystem you are planning to use
	SCGC6 (page 207) has clock gating bits for all 3 timer modules TPM0-2
	*/
	
	//Select clock for TPM Module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= ~SIM_SOPT2_TPMSRC(1); //MCGFLLCLK clock or MCGPLLCLK/2 (built in module clock)
	
	/*Q5
	SOPT2 (page 196)
	TPMSRC selects the clock source for the TPM counter clock
	We are using a built in module clock for this lab
	*/
	
	//Set Modulo Value
	TPM1->MOD = 7500;
	
	/*Q8
	#include CLOCK_SETUP 1 in <system_MKL2574.h> to change core clk freq to 48MHz
	48MHz = 48000000 Prescalar = 128 => 48000000/128 = 375000Hz
	To get 50Hz => MOD = 375000/50 = 7500
	*/
	
	
	/*Edge-Aligned PWM*/
	
	//Configure TPMx_SC  (Status and Control)
	//PS = 111 (128), CMOD = 01 (counter increments on every LPTPM counter clock, CPWMS = 0 (up counting)
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	/*Q6
	Configure the Status and Control register
	Set PS, CMOD and CPWMS bits
	*/
	
	//Configure TPMx_CnSC (Channel Configuration)
	//Channel 0 -> PTB0   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Channel 1 -> PTB1   ELSB:ELSA = 10 (High True pulses) MSB:MSA = 10 (Edge aligned PWM)
	//TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	//TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	/*Q7
	Configure the Channel Status and COntrol for Channel 0 and Channel 1
	*/

}


void playMelodyNote(uint32_t freq){
	if (freq == REST){
		TPM1->MOD = 0;
		TPM1_C0V = 0;
	}
	else {
		TPM1->MOD = FREQ_2_MOD(freq/1.5);
		TPM1_C0V = (FREQ_2_MOD(freq/1.5))/2;
	}
}

//void playAccompanimentNote(uint32_t freq){
//	if (freq == REST){
//		TPM2->MOD = 0;
//		TPM2_C0V = 0;
//	}
//	else {
//		TPM2->MOD = FREQ_2_MOD(freq/1.5);
//		TPM2_C0V = (FREQ_2_MOD(freq/1.5))/2;
//	}
//}



void playMelody(uint32_t melody[], uint8_t durations[], uint32_t length){
	for(int i = 0; i < length; i++){
		osSemaphoreAcquire(musicSem, osWaitForever);
		playMelodyNote(melody[i]);
		osDelay(tempo*durations[i]);
		osSemaphoreRelease(musicSem);
	}
}

//void playAccompaniment(uint32_t melody[], uint8_t durations[], uint32_t length){
//	for(int i = 0; i < length; i++){
//		playAccompanimentNote(melody[i]);
//		osDelay(tempo*durations[i]);
//	}
//}

void playStartMelody(uint32_t melody[], uint8_t durations[], uint32_t length){
	for(int i = 0; i < length; i++){
		playMelodyNote(melody[i]);
		delay(200000*durations[i]);
	}
}

void offMusic(void) {
	TPM1->MOD = 0;
	TPM1_C0V = 0;
}

void playSpiderman(void) {
	playMelody(spiderman_melody, spiderman_durations, spideman_length);
}

void playMariokart(void) {
	playMelody(mariokart_melody, mariokart_durations, mariokart_length);
}

void playStartUpMusic(void) {
	playStartMelody(startup_melody, startup_durations, startup_length);
}


#include "MKL25Z4.h"                    // Device header
#include "notes.h" 


#define RED_LED           18     // PortB Pin 18
#define GREEN_LED         19     // PortB Pin 19
#define BLUE_LED          1      // PortD Pin 1
#define MASK(x)           (1 << (x))

#define PTB0_Pin  0
#define PTB1_Pin  1
#define MUSICAL_NOTE_COUNT  12
#define FREQ_2_MOD(x) (375000/x)

typedef enum {
	red = 0,
	white = 1,
	blue = 2,
} color_t;

color_t led_mapping[3] = {red, white, blue};

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

void clearRGB(void){
	PTB->PDOR = MASK(RED_LED) | MASK(GREEN_LED);
	PTD->PDOR = MASK(BLUE_LED);
}

void ledControl(color_t color){
	clearRGB();
	switch (color){
		case red:
			PTB->PDOR = ~MASK(RED_LED);  //active low LED
			break;
		case blue:
			PTD->PDOR = ~MASK(BLUE_LED);
			break;
		case white:
			PTB->PDOR = ~(MASK(RED_LED) | MASK(GREEN_LED));
			PTD->PDOR = ~MASK(BLUE_LED);
			break;
		default:
			clearRGB();
	}
}


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
uint32_t tempo = 135000;

void initPWM(void) {
	
	//Enable Clock Gating for PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	/*Q1
	PTB0 and PTB1 are being used for Timer, so need to enable power to PORTB
	*/
	
	//Alternative 3 for TPM1_CH0 and TPM1_CH1
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
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
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) |(TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	/*Q7
	Configure the Channel Status and COntrol for Channel 0 and Channel 1
	*/

}

static void delay(volatile uint32_t nof){
	while (nof!=0){
		__asm("NOP");
		nof--;
	}
}

void delay100(volatile uint32_t nof){
	for(int i = 0; i < 100; i++){
		delay(nof);
	}
}

void playNote(uint32_t freq, int i){
	if (freq == REST){
		clearRGB();
		TPM1->MOD = 0;
		TPM1_C0V = 0;
	}
	else {
		ledControl(led_mapping[i%3]);
		TPM1->MOD = FREQ_2_MOD(freq/1.4142);
		TPM1_C0V = (FREQ_2_MOD(freq/1.4142))/2;
	}
}

void playSong(uint32_t melody[], uint8_t durations[], uint32_t length){
	for(int i = 0; i < length; i++){
		playNote(melody[i], i);
		delay(tempo*durations[i]);
	}
}


int main(void) {
	
	SystemCoreClockUpdate();
	initLED();
	initPWM();
	
	//CnV sets duty cycle
	TPM1_C0V = 0x0EA6; //14*16^2 + 10*16 + 6 = 3750 (half of 7500) => 50% duty cycle
	TPM1_C1V = 0x753;  //7*16^2 + 5*16 + 3 = 1875 => 25% duty cycle
	
	/*Q8
	CnV/MOD = duty cycle
	*/
	
	
	while(1) {
		playSong(spiderman_melody, spiderman_durations, spideman_length);
	}
}


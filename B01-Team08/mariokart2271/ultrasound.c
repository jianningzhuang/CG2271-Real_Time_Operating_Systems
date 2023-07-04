#include "GPIO.h"
#include "ultrasound.h"
#include "common.h"

volatile uint32_t time;
volatile uint32_t flag;
volatile uint32_t timeout;
volatile uint32_t count = 0;

volatile int ready = 0;
volatile uint16_t start = 0;
volatile uint16_t startRecord = 0;
volatile uint16_t endRecord = 0;
volatile uint16_t valueRecord = 0;


volatile uint16_t end = 0;
volatile uint16_t counter = 0;
volatile uint16_t read = 0;
volatile uint16_t num = 0;

volatile uint16_t maincounter = 0;


void initUltrasonic() {
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK);
	initGPIOPort(PORTA, TRIG_POS);
	PTA->PDDR |= MASK(VCC_POS) | MASK(TRIG_POS); // Set to output
	
	//Set PTA1 pin to TPM2_CH0 mode and pin to input
	PORTA->PCR[ECHO_POS] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[ECHO_POS] |= (PORT_PCR_MUX(3)); //Set to Alt3 (TMP2_CH0)
	PTA->PDDR &= ~(MASK(ECHO_POS)); //Set to input
	
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= ~SIM_SOPT2_TPMSRC(1);
	TPM2->MOD = 7500;
	
	//Configure channel 0 from TPM2 as input capture for both edges and enabling interrupt
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_CHIE_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(1) | TPM_CnSC_CHIE(1));
	
	TPM2_SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM2_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //CMOD = 1 => LPTPM counter increments on every LPTPM counter clock
  TPM2_SC &= ~(TPM_SC_CPWMS_MASK); //count up by default (0)
	
	//PTA->PDOR = MASK(VCC_POS); //Turn VCC on
	//Enable & Port A TPM2 interrupt
	__enable_irq();
	NVIC_SetPriority(TPM2_IRQn, 2);
	NVIC_ClearPendingIRQ(TPM2_IRQn); 
	NVIC_EnableIRQ(TPM2_IRQn); 
	
	
	//Disable Clk to avoid interrupt triggers
	SIM->SCGC6 &= ~SIM_SCGC6_TPM2_MASK;
}

void sendTrigPulse() {
	PTA->PDOR |= MASK(TRIG_POS);
	delay(0x11); //10us
	PTA->PDOR &= ~MASK(TRIG_POS);
	delay(0x18e70); //60ms
}

int readUltrasonic(void) {
	float value = 0;
	
	//Enable Clk Source to TPM0 module
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	sendTrigPulse();
	
	while(!ready){}; //measuring when echo comesback
	value = end - startRecord;
	start = 0;
	value = (value * 2.6666 * 0.01715 * 1.5) - 337 ;  // determine speed of ultrasonic from frequency and speedy of sound = 34300cm/s /2 =17150;
	
	//Disable Clk to avoid interrupt triggers
	SIM->SCGC6 &= ~SIM_SCGC6_TPM2_MASK;
	counter = 0;
		
	return end;
}

void TPM2_IRQHandler (void) // what is the interrupt logic
{
	NVIC_ClearPendingIRQ(TPM2_IRQn); //clear queue before begin;
		
	if(TPM2_C0SC & TPM_CnSC_CHF_MASK){  // If CHF = channel is 1 when an event occur- cleared by writing 1 
		TPM2_C0SC |= TPM_CnSC_CHF_MASK;
		if(!start){
			/*Clear the TPM0 counter register to start with ~0 */
			TPM2_CNT = 0x00000000000;
			startRecord = TPM2_CNT; 
			start = 1;
		} else {
			end = TPM2_C0V + counter * 7500; //tpm2_c0V is counter value
			ready = 1;
		}
	}
	if(TPM2_SC & TPM_SC_TOF_MASK) { //checking for overflow so that it resets when hit and counter = 1 cycle.
		TPM2_SC |= TPM_SC_TOF_MASK;
		counter++; 
	}
}




int measureDistance() {
	start = 0;
	ready = 0;
	
	maincounter++;
	read = readUltrasonic();
	delay(0x18e70); //1ms
	return read;
}

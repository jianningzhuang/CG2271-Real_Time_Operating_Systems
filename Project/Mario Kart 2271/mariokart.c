#include "UART.h"
#include "motors.h"
#include "music.h"                    

int main(void) {
	
	SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	initMotorsPWM();
	initMusicPWM();
	
	
	while(1) {
		if (rx_data == 0x11) {
			playMariokart();
		}
		else if (rx_data == 0x10) {
			offMusic();
		}
		else {
			move(rx_data);
		}
	}
}


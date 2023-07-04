/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "UART.h"
#include "motors.h"
#include "music.h"
#include "led.h"

#define CONNECT 0x55


const osThreadAttr_t priorityHigh = {
	.priority = osPriorityHigh
};

const osThreadAttr_t priorityAboveNormal = {
	.priority = osPriorityAboveNormal
};

const osThreadAttr_t priorityNormal = {
	.priority = osPriorityNormal
};


uint8_t FLspeed = 51;
uint8_t FRspeed = 50;
uint8_t BLspeed = 51;
uint8_t BRspeed = 50;

uint16_t forward_delay = 3000;
uint16_t forty_five_delay = 400;
uint16_t ninety_delay = 800;

uint8_t forward_flag = 0;
uint8_t reverse_flag = 0;
uint8_t left_flag = 0;
uint8_t right_flag = 0;
uint8_t hardleft_flag = 0;
uint8_t hardright_flag = 0;
uint8_t boost_flag = 0;
uint8_t slow_flag = 0;
uint8_t move_data = 0x00;

void connect_sequence() {
	playStartUpMusic();
	
}

 
/*----------------------------------------------------------------------------
 * Application melody thread
 *---------------------------------------------------------------------------*/
void melody_thread (void *argument) {
 
  // ...
  for (;;) {
		playMariokart();
	}
}

/*----------------------------------------------------------------------------
 * Application accompaniment thread
 *---------------------------------------------------------------------------*/
void accompaniment_thread (void *argument) {
 
  // ...
  for (;;) {
		//playSpiderman();
	}
}

/*----------------------------------------------------------------------------
 * Application motor thread
 *---------------------------------------------------------------------------*/
void motor_thread (void *argument) {
 
  // ...
  for (;;) {
		osSemaphoreAcquire(motorSem, osWaitForever);
		
		move(rx_data, FLspeed, FRspeed, BLspeed, BRspeed);

	}
}

/*----------------------------------------------------------------------------
 * Application motor thread
 *---------------------------------------------------------------------------*/
void debug_thread (void *argument) {
 
  // ...
  for (;;) {
		osSemaphoreAcquire(debugSem, osWaitForever);
		
		if (FUNCTION_MASK(rx_data) == 0x30) {
			if (ACTION_MASK(rx_data) == 0x01) {
				FLspeed += 1;
			} else if (ACTION_MASK(rx_data) == 0x02) {
				FLspeed -= 1;
			} else if (ACTION_MASK(rx_data) == 0x03) {
				FRspeed += 1;
			} else if (ACTION_MASK(rx_data) == 0x04) {
				FRspeed -= 1;
			} else if (ACTION_MASK(rx_data) == 0x05) {
				BLspeed += 1;
			} else if (ACTION_MASK(rx_data) == 0x06) {
				BLspeed -= 1;
			} else if (ACTION_MASK(rx_data) == 0x07) {
				BRspeed += 1;
			} else if (ACTION_MASK(rx_data) == 0x08) {
				BRspeed -= 1;
			} else if (ACTION_MASK(rx_data) == 0x09) {
				forward_delay += 5;
			} else if (ACTION_MASK(rx_data) == 0x0A) {
				forward_delay -= 5;
			} else if (ACTION_MASK(rx_data) == 0x0B) {
				forty_five_delay += 2;
			} else if (ACTION_MASK(rx_data) == 0x0C) {
				forty_five_delay -= 2;
			} else if (ACTION_MASK(rx_data) == 0x0D) {
				ninety_delay += 2;
			} else if (ACTION_MASK(rx_data) == 0x0E) {
				ninety_delay -= 2;
			} 
		} else if (FUNCTION_MASK(rx_data) == 0x40) {
				if (ACTION_MASK(rx_data) == 0x01) {
					forward(forty_five_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				} else if (ACTION_MASK(rx_data) == 0x02) {
					forty_five_left(forty_five_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				} else if (ACTION_MASK(rx_data) == 0x03) {
					forty_five_right(forty_five_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				} else if (ACTION_MASK(rx_data) == 0x04) {
					ninety_left(ninety_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				} else if (ACTION_MASK(rx_data) == 0x05) {
					ninety_right(ninety_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				} else if (ACTION_MASK(rx_data) == 0x06) {
					reverse(forty_five_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				} else {
					stop();
				}
		}
		
	}
}

/*----------------------------------------------------------------------------
 * Application brain thread
 *---------------------------------------------------------------------------*/
void brain_thread (void *argument) {
 
  // ...
  for (;;) {
		osSemaphoreAcquire(brainSem, osWaitForever);
		
		
		if (FUNCTION_MASK(rx_data) == 0x10 || FUNCTION_MASK(rx_data) == 0x20) {
			osSemaphoreRelease(motorSem);
		}
		else if (FUNCTION_MASK(rx_data) == 0x30 || FUNCTION_MASK(rx_data) == 0x40) {
			osSemaphoreRelease(debugSem);
		}
		
	}
}

/*----------------------------------------------------------------------------
 * Application led thread
 *---------------------------------------------------------------------------*/
void led_thread (void *argument) {
 
  // ...
  for (;;) {
		//osSemaphoreAcquire(ledSem, osWaitForever);
		switch(move_data) {
		case 0x00: //Stationary
			onGreenLED();
			flashRedLEDStationary();
			break;
		default:
			runningLED();
			flashRedLEDMoving();
			break;
		
		/* if (move_data == 0x00) {
		onGreenLED();
		flashRedLEDStationary();
	} else {
		runningLED();
		flashRedLEDMoving();
	} */
		
	}
}
	}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	
  initUART2(BAUD_RATE);
	initMotorsPWM();
	initMusicPWM();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	brainSem = osSemaphoreNew(1, 0, NULL);
	motorSem = osSemaphoreNew(1, 0, NULL);
	musicSem = osSemaphoreNew(1, 1, NULL);
	debugSem = osSemaphoreNew(1, 0, NULL);
	ledSem = osSemaphoreNew(1, 0, NULL);
	
	while (rx_data != CONNECT);
	connect_sequence();
	
	osThreadNew(brain_thread, NULL, &priorityHigh);
	osThreadNew(debug_thread, NULL, &priorityNormal);
  osThreadNew(melody_thread, NULL, &priorityNormal);
	//osThreadNew(accompaniment_thread, NULL, NULL);
	osThreadNew(motor_thread, NULL, &priorityAboveNormal);
	osThreadNew(led_thread, NULL, &priorityNormal);
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}

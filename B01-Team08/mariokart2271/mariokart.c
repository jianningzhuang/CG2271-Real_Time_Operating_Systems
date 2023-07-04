/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "UART.h"
#include "motors.h"
#include "music.h"
#include "led.h"
#include "ultrasound.h"

osMessageQueueId_t ultrasonicMessage;

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


uint8_t FLspeed = 50;
uint8_t FRspeed = 50;
uint8_t BLspeed = 50;
uint8_t BRspeed = 50;


uint16_t forward_delay = 750;
uint16_t forty_five_delay = 425;
uint16_t ninety_delay = 850;

uint8_t obstacle_flag = 0;
uint32_t sensor_threshold = 110;
uint32_t sensor_data = 0;


void connect_sequence() {
	playStartUpMusic();
	connectLEDSequence();
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
					forward(forward_delay, FLspeed, FRspeed, BLspeed, BRspeed);
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
 * Application self-drive thread
 *---------------------------------------------------------------------------*/
void auto_thread (void *argument) {
 
  // ...
  for (;;) {
		osSemaphoreAcquire(autoSem, osWaitForever);
		sensor_data = measureDistance();
		while(sensor_data <= sensor_threshold) {
			sensor_data = measureDistance();
		}
		controlWheelSpeed(FLF,32);
		controlWheelSpeed(FLR,0);
		controlWheelSpeed(FRF,30);
		controlWheelSpeed(FRR,0);
		ai_moving_flag = 1;
		while (1) {
			//while(1) {
		    //sensor_data = measureDistance();
	    //}
			sensor_data = measureDistance();
			if (sensor_data == 0) { // Faulty reading
				continue;
			}
			if (sensor_data > sensor_threshold) {
				continue;
			} else if (sensor_data <= sensor_threshold && obstacle_flag == 0) {
				stop();
				ai_moving_flag = 0;
				osDelay(1100);
				obstacle_flag = 1;
				forty_five_left(forty_five_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				forward_delay = 850;
				forward(forward_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				ninety_right(ninety_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				forward(forward_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				ninety_right(ninety_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				forward_delay = 1000;
				forward(forward_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				ninety_right(ninety_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				forward(forward_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				forty_five_left(forty_five_delay, FLspeed, FRspeed, BLspeed, BRspeed);
				controlWheelSpeed(FLF,32);
				controlWheelSpeed(FLR,0);
				controlWheelSpeed(FRF,30);
				controlWheelSpeed(FRR,0);
				ai_moving_flag = 1;
			} else if (sensor_data <= sensor_threshold && obstacle_flag == 1) {
				stop();
				ai_moving_flag = 0;
				connect_sequence();
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
		else if (rx_data == 0x51) {
			stop();
			connect_sequence();
			osSemaphoreAcquire(musicSem, osWaitForever);
		}
		else if (FUNCTION_MASK(rx_data) == 0x60) {
			osSemaphoreRelease(autoSem);
			osSemaphoreRelease(musicSem);
		}
		
	}
}

/*----------------------------------------------------------------------------
 * Application green led thread
 *---------------------------------------------------------------------------*/
void led_green_thread (void *argument) {
 
  // ...
  for (;;) {
		if (rx_data == 0x20 || (rx_data == 0x61 && ai_moving_flag == 0)) {
			onGreenLED();
		} else {
			runningLED();
		}
	}
}


	
	/*----------------------------------------------------------------------------
 * Application red led thread
 *---------------------------------------------------------------------------*/
void led_red_thread (void *argument) {
 
  // ...
  for (;;) {
		if (rx_data == 0x20 || (rx_data == 0x61 && ai_moving_flag == 0)) {
			flashRedLEDStationary();
		} else {
			flashRedLEDMoving();
		}
	}
}

 
int main (void) {
 
  // System Initialization

  SystemCoreClockUpdate();
	initLEDModules();
  initUART2(BAUD_RATE);
	initMotorsPWM();
	initMusicPWM();
	initUltrasonic();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	autoSem = osSemaphoreNew(1, 0, NULL);
	brainSem = osSemaphoreNew(1, 0, NULL);
	motorSem = osSemaphoreNew(1, 0, NULL);
	musicSem = osSemaphoreNew(1, 1, NULL);
	debugSem = osSemaphoreNew(1, 0, NULL);
	//ledSem = osSemaphoreNew(1, 0, NULL);
	
	
	//while(1) {
		//sensor_data = measureDistance();
	//}
	
	while (rx_data != CONNECT);
	connect_sequence();
	

	osThreadNew(auto_thread, NULL, &priorityAboveNormal);
	osThreadNew(brain_thread, NULL, &priorityHigh);
	osThreadNew(debug_thread, NULL, &priorityAboveNormal);
  osThreadNew(melody_thread, NULL, &priorityNormal);
	//osThreadNew(accompaniment_thread, NULL, NULL);
	osThreadNew(motor_thread, NULL, &priorityAboveNormal);
	osThreadNew(led_green_thread, NULL, &priorityNormal);
	osThreadNew(led_red_thread, NULL, &priorityNormal);
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}

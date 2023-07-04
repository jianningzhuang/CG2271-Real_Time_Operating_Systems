#ifndef SOUND_H
#define SOUND_H

#include "common.h"                    
#include "notes.h"
#include "motors.h"

#define PTB0_Pin  0  //main melody
#define PTB1_Pin  1  //2nd buzzer accompaniment?
#define FREQ_2_MOD(x) (375000/x)



void initMusicPWM(void);

void playNote(uint32_t freq);

void playSong(uint32_t melody[], uint8_t durations[], uint32_t length);

void offMusic(void);

void playSpiderman(void);

void playMariokart(void);

#endif


#ifndef MYMAIN
#define MYMAIN

#include <stdint.h>
#include <stm32l432xx.h>

void playNote(int duration, int freq);

void initSPI(int br, int cpol, int cpha);
char spiSendReceive(char send);

void lights_Sensing();
void lights_Target();
void lights_Lose();

void lights_Win1();
void lights_Win2();
void lights_Win3();

void lights_Tunnel1();
void lights_Tunnel2();
void lights_Tunnel3();

void start_frame();
void end_frame();
void led_frame(uint8_t bright, uint8_t r, uint8_t g, uint8_t b);

void delay(int ms);

#endif

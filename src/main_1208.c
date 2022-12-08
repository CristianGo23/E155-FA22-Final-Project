/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : MCU code for pinball (MicroPs)
Authors : Cristian Gonzalez, cgonzalez@g.hmc.edu
          Alessandro Maiuolo, amaiuolo@g.hmc.edu

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include <stm32l4xx.h>

// notes to freqs
#define B3 247
#define C4 262
#define C4_S 277
#define D4 294
#define D4_S 311
#define E4 330
#define F4 349
#define F4_S 370
#define G4 392
#define G4_S 392
#define A4 440
#define A4_S 466
#define B4 494
#define C5 523
#define D5 587
#define E5 659

#define BEAT 50


//GAME LIGHTING FUNCTIONS

void lights_Sensing() {
  // alternating red and green for sensing state
  start_frame();
  for (int i = 0; i< 30; i++) {
    led_frame(2, 255, 0, 0);
    led_frame(2, 0, 255, 0);
  }
  end_frame();
}

void lights_Target() {
  // 10-LED blue segment that travels along the strip when a target is hit
  for (int i = 0; i < 61; i++){
    start_frame();
    for (int j = 0; j < i; j++){
      led_frame(2,0xe6,0xb4,0x00);
    }
    for (int k = 0; k < 10; k++){
      led_frame(2,0,0,255);
    }
    end_frame();
    delay(10);
  }
}

// 3 alternating lighting configurations when win condition is met
// creates a cycling effect of redd, green, and blue LEDs
void lights_Win1() {
  start_frame();
  for (int i = 0; i < 20; i++){
    led_frame(2,255,0,0);
    led_frame(2,0,255,0);
    led_frame(2,0,0,255);
  }
  end_frame();
}
void lights_Win2() {
  start_frame();
  for (int i = 0; i < 20; i++){
    led_frame(2,0,0,255);
    led_frame(2,255,0,0);
    led_frame(2,0,255,0);
  }
  end_frame();
}
void lights_Win3() {
  start_frame();
  for (int i = 0; i < 20; i++){
    led_frame(2,0,255,0);
    led_frame(2,0,0,255);
    led_frame(2,255,0,0);
  }
  end_frame();
}

// all purple on loss
void lights_Lose(){
  start_frame();
  for (int i = 0; i < 60; i++){
    led_frame(2,255,o,30);
  }
  end_frame();
}

// number of active lights correspond to distance ball traveled into tunnel
void lights_Tunnel1(){
  start_frame();
  for (int i = 0; i < 20; i++){
    led_frame(2,255, 150, 0);
  }
  for (int i = 0; i < 40; i++){
    // led_frames need to be specified even for lights that are off (brightness 0)
    // this is because the end_frame sets the last LED to bright white (which we assign to the 61st light,
    // which is off the end of the strip)
    led_frame(0,255, 255, 255);
  }
  end_frame();
}
void lights_Tunnel2(){
  start_frame();
  for (int i = 0; i < 40; i++){
    led_frame(2,255, 150, 0);
  }
  for (int i = 0; i < 20; i++){
    led_frame(0,255, 255, 255);
  }
  end_frame();
}
void lights_Tunnel3(){
  start_frame();
  for (int i = 0; i < 60; i++){
    led_frame(2,255, 150, 0);
  }
  end_frame();
}


// start and end frames based on specifications
void start_frame(){
  for(int i = 0; i<4; i++){
    spiSendReceive(0x00);
  }
}

void end_frame(){
  for(int i = 0; i<4; i++){
    spiSendReceive(0xFF);
  }
}

// led_frame functioin definition make code more readable as spi and led specification are encapsulated
void led_frame(uint8_t bright, uint8_t r, uint8_t g, uint8_t b){
  spiSendReceive(0b11100000 | bright);
  spiSendReceive(b);
  spiSendReceive(g);
  spiSendReceive(r);
}



// APA102C LED Portion - set up MCU to communicate with LED strip
void initSPI(int br, int cpol, int cpha) {
  RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_SPI1EN, 1); 

  GPIOB->MODER &= ~_VAL2FLD(GPIO_MODER_MODE3, 0b01);
  GPIOB->MODER |= _VAL2FLD(GPIO_MODER_MODE3, 0b10);

  GPIOB->MODER &= ~_VAL2FLD(GPIO_MODER_MODE5, 0b01);
  GPIOB->MODER |= _VAL2FLD(GPIO_MODER_MODE5, 0b10);

  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 5);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);

  SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 1);

  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);

  SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 1);
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 1);
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 7);

  SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 1); // enable
}

// only using SPI1
char spiSendReceive(char send){
  while(_FLD2VAL(SPI_SR_TXE,SPI1->SR)==0);
  *(volatile char *) (&SPI1->DR) = send;
  while(_FLD2VAL(SPI_SR_RXNE,SPI1->SR)==0);
  return (char) SPI1->DR;
}

// delay function useful for playing lights and sounds
void delay(int ms){
  TIM15->CNT &= ~_VAL2FLD(TIM_CNT_CNT, 0xFFFF);
  while((_FLD2VAL(TIM_CNT_CNT, TIM15->CNT)) < ms);
}

// main method
int main(void) {
  // SETUP ----------------------------------

  // Using MSI at default 4MHz
  // sysclock: don't need to turn it on because on by default

  // enable tim15 and tim16
  RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_TIM15EN, 1);
  RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_TIM16EN, 1);

  // enable GPIOA and GPIOB
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOBEN, 1);

  // set up TIM15 (timing)
  TIM15->CR1 |= _VAL2FLD(TIM_CR1_CEN, 1);
  TIM15->CCER |= _VAL2FLD(TIM_CCER_CC1E, 1);
  TIM15->PSC |= _VAL2FLD(TIM_PSC_PSC, 0xFA0);
  TIM15->BDTR |= _VAL2FLD(TIM_BDTR_MOE, 1);
  TIM15->EGR |= _VAL2FLD(TIM_EGR_UG, 1);

  //set up TIM16 (pwm)
  GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE6, 0b01);
  GPIOA->MODER |= _VAL2FLD(GPIO_MODER_MODE6, 0b10);
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 14);
  TIM16->CR1 |= _VAL2FLD(TIM_CR1_ARPE, 1);
  TIM16->CCMR1 |= _VAL2FLD(TIM_CCMR1_OC1PE, 1);
  //TIM16->CCMR1 |= _VAL2FLD(TIM_CCMR1_CC1S, 0b00); (not necessarily - default value)
  TIM16->CCMR1 |= _VAL2FLD(TIM_CCMR1_OC1M, 0b0110);
  TIM16->CCMR1 &= ~_VAL2FLD(TIM_CCMR1_OC1M, 0b1001);
  TIM16->CCER |= _VAL2FLD(TIM_CCER_CC1E, 1);
  TIM16->BDTR |= _VAL2FLD(TIM_BDTR_MOE, 1);
  TIM16->PSC |= _VAL2FLD(TIM_PSC_PSC, 3); // set PSC to 3. gives min freq at 15Hz
  TIM16->EGR |= _VAL2FLD(TIM_EGR_UG, 1); // set UG to 1 (update shadows)

  // set up input pins for alert_MCU
  GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE5, 0b11);
  GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE9, 0b11);
  GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE10, 0b11);
  //GPIOA->MODER &= ~_VAL2FLD(GPIO_MODER_MODE11, 0b11);

  // spi led
  initSPI(1,0,0); // baud rate, clock polarity, and clock phase from LED strip specifications

  // initialize variables for readng from alert_MCU pins
  volatile uint16_t alert_MCU;
  volatile uint16_t alert_MCU0;
  volatile uint16_t alert_MCU1;
  volatile uint16_t alert_MCU2;

  // END SETUP ----------------------------------

  while(1) {
    // determine state of the game using alert_MCU
    alert_MCU0 = _FLD2VAL(GPIO_IDR_ID5, GPIOA->IDR);
    alert_MCU1 = _FLD2VAL(GPIO_IDR_ID9, GPIOA->IDR);
    alert_MCU2 = _FLD2VAL(GPIO_IDR_ID10, GPIOA->IDR);
    alert_MCU = alert_MCU0 + 2*alert_MCU1 + 4*alert_MCU2;

    // based on alert_MCU play the appropriate light and sound sequence
    switch (alert_MCU) {
      case 0:
        // no sound
        lights_Sensing();
        break;
      // for tunnels, short 10ms sound is played to allow for continuous sound
      // to be played that lasts the duration of the respective wait states
      case 1: //tunnel 1
        lights_Tunnel1();
        playNote(10, D4);
        break;
      case 2: //tunnel 2
        lights_Tunnel2();
        playNote(10, A4);
        break;
      case 3: //tunnel 3
        lights_Tunnel3();
        playNote(10, E5);
        break;
      case 4: //target
        playNote(80, D4);
        playNote(120, C5);
        lights_Target();
        break;
      case 5: //win
        playNote(350, F4);
        playNote(0, BEAT);
        lights_Win1();
        playNote(200, D4_S);
        playNote(0, BEAT);
        lights_Win2();
        playNote(200, D4_S);
        playNote(0, BEAT);
        lights_Win3();
        playNote(400, A4);
        playNote(0, BEAT);
        lights_Win1();
        playNote(500, A4_S);
        lights_Win2();
        playNote(500, A4_S);
        lights_Win3();
        playNote(500, A4_S);
        playNote(0, BEAT);
        break;
      case 6: //lose
        lights_Lose();
        playNote(140, A4_S);
        playNote(140, A4);
        playNote(140, G4_S);
        playNote(140, G4);
        playNote(140, F4_S);
        playNote(140, F4);
        playNote(140, E4);
        playNote(600, D4_S);
        playNote(0, BEAT);
        break;
      case 7: // target wait
        // notification that button is stuck in pressed position
        lights_Target();
        playNote(10, C5);
    }
  }
}

// function for playing frequency for a duration
void playNote(int duration, int freq) {
  // duration can only be 16 bit, in ms
  // freq should be above 15Hz and below 1MHz
  // freq 15Hz or less implies a rest
  if(freq>15 && duration>0) {
    // configure pwm frequency
    TIM16->ARR &= ~_VAL2FLD(TIM_ARR_ARR, 0xFFFF); // clear autoreload register
    TIM16->ARR |= _VAL2FLD(TIM_ARR_ARR, 1000000/freq); // set autoreload register
    TIM16->CCR1 &= ~_VAL2FLD(TIM_CCR1_CCR1, 0xFFFF); // clear capture compare registter
    TIM16->CCR1 |= _VAL2FLD(TIM_CCR1_CCR1, 500000/freq); // set capture compare register to give 50% duty cycle
    TIM16->CR1 |= _VAL2FLD(TIM_CR1_CEN, 1);
    TIM16->EGR |= _VAL2FLD(TIM_EGR_UG, 1);
  }

  TIM15->CNT &= ~_VAL2FLD(TIM_CNT_CNT, 0xFFFF); // reset delay counter (cnt = 0), no UG required since counter not shadowed
  while((_FLD2VAL(TIM_CNT_CNT, TIM15->CNT)) < duration); // delay for the duration

  if(freq>15 && duration>0) {
    // disable tim16 to stop playing the sound
    TIM16->CR1 &= ~_VAL2FLD(TIM_CR1_CEN, 1);
    TIM16 -> EGR |= _VAL2FLD(TIM_EGR_UG, 1);
  }
}


/*************************** End of file ****************************/

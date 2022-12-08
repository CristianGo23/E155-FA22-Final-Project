---
layout: page
title: Design
permalink: /design/
---

# MCU Design
<div>
   The MCU was designed to control the light and sound of our system. We're using the microcontroller's PWM (pulse width modulation) and timer functionality to specify pitches and durations of sounds to play. We use TIM15 to specify durations with 1ms precision. We use TIM16 to play sounds within the typical hearing range of a person (20Hz to 20kHz).
</div>
<div>
   
   In order to play a note, we have a playNote function that takes in a pitch and duration as inputs. First, TIM16 is set up to do 50% PWM at the specified frequency. TIM16 is enabled to play the sound. Then, the MCU reads TIM15 until the duration is reached. Once this happens, TIM16 is disabled – stopping the sound.
</div>
<div>
   
   The MCU communicates with the LED light strip via SPI (serial peripheral interface). The light strip's communication protocol only requires two lines: sclk (serial clock) and sdi (serial data in). Chip select is not utilized in the communication protocol because the light strip is designed to always respond when the sclk is activated. Serial data out (sdo) is not utilized because there is no useful information to pass back to the controller.
</div>

<div>
   
   In the main method, we first initialize the registers we need for TIM15, TIM16, and GPIO input pins required to read alert_MCU from the FPGA. After this, we enter a while(1) loop where the MCU reads the alert_MCU signal and uses it to determine the sound sequence to play. Right now, sequences with a duration of 10ms are designated that way because the FPGA stays in those states for a variable amount of time. Sequences with a duration of 500ms are representative of states reached where we do not need to be worried about being super responsive to the next signal: win, lose, hitting a target, and reaching the last gate of the tunnel.
</div>
<div>
   
   The microcontroller uses SPI to communicate with the Addressable LED RGB Strip (APA102). The LED strip only requires 3.3V of power, a ground connection, Serial Clock, and a Data Input signal. Although there is an output signal, we are not looking to gather data from the LED strip, and we only wish for the microcontroller to send the light intensity, and RGB combination to produce various light patterns for the different events in our project.
</div>

## MCU Block Diagram

# FPGA Design
<div>
   The FPGA's main function is to define the game logic using an FSM (finite state machine). On reset, or (re)starting the game, the game is in a sensing state. This is the state of the game for the most part. It senses: target hit, tunnel entered, win, and lose. If a target is hit, the game transitions to that state for a cycle in order to increment the score, and then it goes into a waiting state to wait for the button to be released to prevent multiple points from being rewarded from a single, prolonged button press. 
</div>
<div>
</div>
   The tunnel is considered entered when the second point in the tunnel array is obscured by the ball (tunnel[1]). From here, similar to hitting a target, the FSM goes into the Tunnel_1 state and increments the score for one cycle. Then, it goes into a waiting state. In this waiting state, the FSM is sensing for movement in the tunnel: whether the ball moves to the next gate (tunnel[2]) or falls out from the entrance (tunnel[0], the buffer). If it falls out from the entrance, the FSM returns to the sensing state. If it progresses to the next gate, it repeats the pattern of moving to Tunnel_2 and incrementing the score for one cycle, and then it moves to the corresponding waiting state. In this waiting state, the FSM similarly waits for the ball to progress to the third and final gate or for the ball to fall out from the entrance. If the ball falls out from the entrance, the FSM returns to the sensing state. If it crosses the third gate, it goes into Tunnel_3 state and increments the score for one cycle before returning to the sensing state.
</div>
<div>
   
   While in the sensing state, the FSM also checks the score to see if the threshold has been crossed. If yes, then the FSM transitions to the win state, and the game is over. Similarly, if the sensing state detects that the ball passed the flippers (signaled by another IR LED/sensor unit), then the FSM goes into the lose state and the game is over. The FSM outputs are enable_score and alert_MCU. Alert_MCU is a signal that communicates to the MCU what state the FSM is in and therefore what light/sound sequence to play.
</div>
<div>
   
   The scoring module consists of a time-multiplexedd two-digit hex 7-segment display. The score is driven by the score enable signal from the game FSM as well as an adder that translates the score from hex to decimal.
</div>
<div>
   
   We used test benches to verify the FSM as well as the pinball game as a whole (in FPGA). As we started implementing the hardware, we did more testing in hardware rather than in ModelSim.
</div>

## FPGA Block Diagram
<div style="text-align: left">
  <img src="../assets/schematics/Game FSM Logic.jpg" alt="logo" width="500" />
</div>

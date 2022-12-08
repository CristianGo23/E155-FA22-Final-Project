---
layout: page
title: Design
permalink: /design/
---

# MCU Design
<div>
   The MCU was designed to control the light and sound of our system. We're using the microcontroller's PWM and timer functionality to specify pitches and durations of sounds to play. We use TIM15 to specify durations with 1ms precision. We use TIM16 to play sounds within the typical hearing range of a person (20Hz to 20kHz).
</div>
<div>
   
   In order to play a note, we have a playNote function that takes in a pitch and duration (from lab 5). It starts the timer, starts the PWM at a specified frequency, and then reads the timer until the duration is reached. Once this happens, the PWM signal is turned off, stopping the sound. (Description minimal here since we did this in lab 5).
</div>
<div>
   
   In the main method, we first initialize the registers we need for TIM15, TIM16, and GPIO input pins required to read alert_MCU from the FPGA. After this, we enter a while(1) loop where the MCU reads the alert_MCU signal and uses it to determine the sound sequence to play. Right now, sequences with a duration of 10ms are designated that way because the FPGA stays in those states for a variable amount of time. Sequences with a duration of 500ms are representative of states reached where we do not need to be worried about being super responsive to the next signal: win, lose, hitting a target, and reaching the last gate of the tunnel.
</div>
<div>
   
   The microcontroller uses SPI to communicate with the Addressable LED RGB Strip (APA102). The LED strip only requires 3.3V of power, a ground connection, Serial Clock, and a Data Input signal. Although there is an output signal, we are not looking to gather data from the LED strip, and we only wish for the microcontroller to send the light intensity, and RGB combination to produce various light patterns for the different events in our project.
</div>

## MCU Block Diagram

# FPGA Design

## FPGA Block Diagram

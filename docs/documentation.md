---
layout: page
title: Documentation
permalink: /doc/
---

# New Hardware
The Infrared Emitter LEDs (IR1503) are currently being powered through the 3.3V power supplied by the micro USB port on the Development Board. In order for the Photodiode (OP950) to recognize the IR light, it has to be facing directly toward the LED shown in the figure below.

<div style="text-align: left">
  <img src="../assets/img/Photodiode.jpg" alt="logo" width="500" />
</div>

At a default state, the circuit involving the photodiode provides a high signal of around 3.3V when no blockage is present between the LED and the photodiode. When there is a blockage, the circuit outputs around 0V, which is enough for the FPGA to read as the input pin being driven low. This allows us to line up the 4 LEDs and photodiodes inside a CAD model of a tunnel, and track when a marble passes through each sensor, in order to signal the number of points they receive from passing however much distance through the tunnel.

<div style="text-align: left">
  <img src="../assets/img/APA102.jpg" alt="logo" width="500" />
</div>

The APA102 LED RGB Strip is a one meter strip of 60 LEDs that accepts instructions from the Microcontroller. The strip itself has four pins: VCC, GND, SCK, & DATA. VCC was connected to our 3.3V pin, and GND was connected to GND.

<div>
</div>

As for the rest of the communicaiton, we used SPI protocol and the pins P3B (SCK) and P5B (DATA) to send the bits of data to each of the 60 LEDs. The first 32 bits of the strip need to be set to all 0s, which is what we'll call the Start Frame. The remaining 60 LED 32 bit chunks will be formatted as shown in the Figure below. The End Frame is similar to the Start Frame, in that all 32 bits will need to be set to 1.

<div style="text-align: left">
  <img src="../assets/img/SPI_Interface.jpg" alt="logo" width="500" />
</div>



# Schematics
<!-- Include images of the schematics for your system. They should follow best practices for schematic drawings with all parts and pins clearly labeled. You may draw your schematics either with a software tool or neatly by hand. -->

<div style="text-align: left">
  <img src="../assets/schematics/Tunnel & Lose.jpg" alt="logo" width="500" />
</div>

<span class="bolded">IR Photodiode Sensors & IR LED Circuit (Tunnel & Lose)</span>

<div style="text-align: left">
  <img src="../assets/schematics/FSM Reset.jpg" alt="logo" width="300" />
</div>

<span class="bolded">FSM Reset & Enable Circuit</span>

<div style="text-align: left">
  <img src="../assets/schematics/Speaker.jpg" alt="logo" width="500" />
</div>

<span class="bolded">Speaker & Multiplexed Display Circuit</span>

<div style="text-align: left">
  <img src="../assets/schematics/FPGA.jpg" alt="logo" width="300" />
</div>

<span class="bolded">Field-Programmable Gate Array (FPGA)</span>

<div style="text-align: left">
  <img src="../assets/schematics/MCU.jpg" alt="logo" width="300" />
</div>

<span class="bolded">Microcontroller (MCU)</span>

<div style="text-align: left">
  <img src="../assets/schematics/Target.jpg" alt="logo" width="200" />
</div>

<span class="bolded">Target Buttons Circuit</span>

<div style="text-align: left">
  <img src="../assets/schematics/SPI.jpg" alt="logo" width="200" />
</div>

<span class="bolded">Addressable SPI Peripheral LED Strip Circuit</span>

# Source Code Overview
<!-- This section should include information to describe the organization of the code base and highlight how the code connects. -->

The source code for the project is located in the Github repository [here](https://github.com/CristianGo23/E155-FA22-Final-Project/tree/main/src).

---
layout: page
title: Project Overview
permalink: /
exclude: true
---

# Project Abstract

<div>
  This project is a pinball machine, loosely defined as a game where a player uses flippers to hit various targets with a ball to score points. We used IR sensing and buttons as detectors for scoring and losing. An LED light strip and speaker were implemented in order to generate the ambiance of the game. The FPGA is largely in charge of the game logic and scoring, while the MCU is in charge of light and sound to give the game an arcade feel. The enclosure is a near-rectangular box with openings at the front, bottom, and back to give access to the electronics boards housed within. Various ports exist in the enclosure to allow for the discreet harnessing of components to the boards. Flippers, their actuators, and IR LED/sensor housing are 3D printed. Altogether, the final product is a simple, fun, and surprisingly challenging game of pinball.
</div>

<div style="text-align: left">
  <img src="./assets/img/Logo.png" alt="logo" width="100" />
</div>


# Project Intro and Motivation
<div>
  We are building a pinball machine. Our motivation was to create a fun, interactive game that anyone who came at Projects Day can enjoy. It's a short, sweet, simple pinball machine made for anyone to use. Our pinball design consists of two different scoring opportunities: hitting any of the targets (buttons) and passing through a long tunnel (IR LED/sensor array) – the farther along the tunnel the ball travels, the more points the player scores. Light and sound are also important parts of pinball, and they serve to generate a fun arcade atmosphere. Specifically, an LED strip, as well as a speaker, will work together to play recognizable sequences to notify the user of events in the game (scoring, winning, losing, etc.). 
In this version of pinball, there is a win condition dictated by a score threshold. There is no concept of multiple lives – if the ball passes beyond the flippers (IR LED/sensor pair), the game is lost. The game needs to be reset when either win or lose triggers. 
</div>


# System Block Diagram

# Car Game "Galle Road" Using STM32F429 Discovery Board
![CAR GAME DEMONSTRATION](media/car_game.gif)

This assignment is a recreation of the popular Brick Car Racing Game using STM32F429 Discovery board by ST Microelectronics. The core processor in this development board is STM32F103 is an ARM Cortex M4 processor. This has been done using the Kiel Microvision IDE.

## Features
- Pause Game
- Abort Game
- High Score History
- Level Ups
- Car Color Can be Selected
- Speeding up increases the rate of score

## How to use
Import the project into Keil Microvision and upload it to the board. The main is in **GalleRoad.c** file. All the helper functions that are required to draw on LCD are in **DisplayElements.c** file.

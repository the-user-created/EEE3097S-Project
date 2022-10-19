# EEE3097S Group 20

This GitHub repository was used for storing the code and related files for the EEE3097S project.

## Description

This project was for the EEE3097S: Engineering Design: Electrical and Computer Engineering course. The goal of the project was to compress and encrypt motion data collected from a sensor on the SHARC buoy in Antartica. This compressed-encrypted data is then meant to be sent over a satellite link to researchers at UCT. At which point they can decrypt and decompress the data to retrieve the original sensor data. However, the students for this project only had to implement the system locally (i.e., not in Antartica and without the satellite link).

This repository was used by project group #20.

## Getting Started

### Dependencies

* STM32CubeIDE
* CLion
* C
* Python3
* STM32F0 Discovery
* SparkFun 9DoF IMU Breakout
* UART to USB Chip
* Cables

### Installing

The final code can be found in the folder `EEE3097S_Project`. The code used for the Simulation Testing can be found in `FPR_Code`. The code used for the Practical Testing can be found in `SPR_Code`. 

NOTE: the practical testing also used the STM code found in `EEE3097S_Project`.

The folder `SPR_Code` also contains the Python STM handler scripts. The script `stm_handler.py` can be used for a single STM setup for handling the transmission of data between the user and STM. The script `demo.py` was used for demonstrating the overall functionality of the STM for the demo video.

### Usage

Create a new STM32Cube project by importing the `EEE3097S_Project` folder. Connect the STM to the PC, UART converter chip, and SparkFun IMU. Run the STM32Cube project and when the "Downloading to STM" prompt appears, run the stm_handler.py script.

## Help

If any errors are encountered please use the 'Issues' tab of this repository.  

## Authors

* David Young
* Caide Spriestersbach

## License

This project is licensed under the MIT License - see the LICENSE.md file for details




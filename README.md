# Tronics Evaluation Kit Library 2.2
Tronics Microsystems Evaluation Board library for Arduino M0 and Yùn Rev2 boards with GYPRO® or AXO® MEMS Sensors.

![alt tag](https://preview.ibb.co/bYgYRd/Maquette_fond_blanc.png)

# Arduino-Firmware
Arduino (M0 and Yùn models) Firmware for Tronic's Microsytems Evaluation Kit

## Installation

1.	Connect the Arduino (M0 or Yùn) board to your computer using the USB cable and the Native USB Port. If this is the first time you are connecting an Arduino board to your computer, please wait a few minutes before going on, as Windows needs to install all the necessary drivers.

2.	Open Arduino IDE.
3.	Open the “Boards Manager” in the Tools > Board menu. 


4.	If you use the Arduino M0 Board, Install latest version of "Arduino SAMD Boards (32-Bits ARM Cortex M0+)" 

5.	Select “Arduino M0” or "Arduino Yùn" in the Tools > Board menu. 
 
6.	Select the serial port number corresponding to the Arduino board in the Tools > Port menu. The name “Arduino M0” or "Arduino Yùn" should be associated with the correct COM port. 

7.	Now, you need install Tronics EVB Library : Click on “Add .ZIP Library…” in the Sketch > Include Library menu.
 

8.	Select “TronicsEvaluationKit_Library” in the “Arduino Firmware & IDE” folder into the USB flash drive.
9.	Now, you can open “EvaluationTool” firmware” in the Examples menu. 

10.	Upload the program: Simply click on the "Upload" button in the environment. Wait a few seconds, and you should see the RX and TX LEDs on the board flashing. If the upload is successful, the message "Done uploading." will appear in the status bar at the bottom of the window. 

11.	Close Arduino IDE

> NOW YOUR ARDUINO M0 BOARD IS PROGRAMMED AND READY TO BE USED WITH TRONICS EVALUATION TOOL.

## Usage
This Firmware is optimised forEvaluation Tool 2.2 software.

## History
28/09/2017 - Creation of the Tronic's Microsystems GitHub repository.
03/09/2018 - Update to have a compatiblity with the Arduino Yùn Rev 2 board

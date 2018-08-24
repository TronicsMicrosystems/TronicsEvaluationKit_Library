/****************************************************************************

               EXAMPLE FIRMWARE for ARDUINO M0       //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
            This Firmware shows how to read the      //  //      //
              sensor outputs  (GYPRO® or AXO®)       //  //////////
             (Rate + Temperature + Self-Test) 

     Copyright (C) 2017 by Tronics Microsystems

     This file is part of Evaluation Tool 2.2.

     Evaluation Tool 2.0 is free software: you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public License as
     published by the Free Software Foundation, either version 3 of the
     License, or (at your option) any later version.

     Evaluation Tool 2.0 is distributed in the hope that it will be
     useful, but WITHOUT ANY WARRANTY; without even the implied warranty
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
     of GNU Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with Box.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
   @file AXO_ReadOutput.ino
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 23 August 2018
   @version : 2.2
   @brief File containing firmware to program Arduino M0 Board for Evaluation Tool 2.2 software developped by Tronics Microsystems.
   @see https://github.com/TronicsMicrosystems/TronicsEvaluationKit_Library
*/

#include <EVB.h>                    // Required to use AXO Features
#define ARDUINO_OUTPUT SerialUSB     // Define Output Port name (SerialUSB for the USB Port, or Serial1 for the RS422 Port (Only on EVB 2.1 or 3.0))

#define DATARATE 100

byte sensorOutputBuffer[6] = {};    // Define AXO Output Buffer
long Acceleration;                  // Define Rate 32-bits variable
unsigned int Temperature;           // Define Temperature 16-bits variable
byte ST;                            // Define Self Test 8-bits variable

void setup() {
  EVB.Init(ARDUINO_OUTPUT);          // Initialisation of GYPRO-EVB 2.0, 2.1 or 3.0 (GPIO Pins + SPI + Data Outputs)
}

void loop() {
  EVB.ReadOutput(sensorOutputBuffer, 6);        //  Reading of the AXO Output (fill the 6 bytes buffer)

  sensorOutputBuffer[0] &= 0b01111111;       // Bitmask to have only bits 6 to 0 of the first byte
  ST = sensorOutputBuffer[3] &= 0b00000001;  // Bitmask to have the self test value
  sensorOutputBuffer[3] &= 0b10000000;       // Bitmask to have only bits 7 of the fourth byte

  Acceleration = (long)sensorOutputBuffer[0];              // Bitmask and bitshift to determine Acceleration
  Acceleration = Acceleration * 256 + (long)sensorOutputBuffer[1];
  Acceleration = Acceleration * 256 + (long)sensorOutputBuffer[2];
  Acceleration = Acceleration * 256 + (long)sensorOutputBuffer[3];
  Acceleration = Acceleration * 2;

  Temperature = (unsigned int)sensorOutputBuffer[4];  // Bitmask and bitshift to determine temperature of the sensor
  Temperature = Temperature * 256 + (unsigned int)sensorOutputBuffer[5];

  ARDUINO_OUTPUT.print("Acceleration : ");             // Send to the value to the Arduino Output Port
  ARDUINO_OUTPUT.print(float(Acceleration / 256) / 500000, 3);
  ARDUINO_OUTPUT.print("\t\t Temperature : ");
  ARDUINO_OUTPUT.print(Temperature);
  ARDUINO_OUTPUT.print("\t\t Self-Test : ");
  ARDUINO_OUTPUT.println(ST);

  delay(1000 / DATARATE);
}
/****************************************************************************

               EXAMPLE FIRMWARE for ARDUINO M0       //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
            This Firmware shows how to read the      //  //      //
              sensor outputs  (GYPRO® or AXO®)       //  //////////
             (Rate + Temperature + Self-Test) 

     Copyright (C) 2017 by Tronics Microsystems

     This file is part of Evaluation Tool 2.1.

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
   @file Sen_ReadOutput.ino
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 12 Oct 2017
   @version : 2.1
   @brief File containing firmware to program Arduino M0 Board for Evaluation Tool 2.0 software developped by Tronics Microsystems.
   @see https://github.com/TronicsMicrosystems/Firmware-2.1
*/

#include <EVB.h>                    // Required to use GYPRO Features
#define ArduinoOutput SerialUSB     // Define Output Port name (SerialUSB for the USB Port, or Serial1 for the RS422 Port (Only on EVB 2.1 or 3.0))

#define DataRate 100

byte sensorOutputBuffer[6];               // Define GYPRO Output Buffer
long Rate;                          // Define Rate 32-bits variable
unsigned int Temperature;           // Define Temperature 16-bits variable
byte ST;                            // Define Self Test 8-bits variable

void setup() {
  EVB.Init(ArduinoOutput);          // Initialisation of GYPRO-EVB 2.0, 2.1 or 3.0 (GPIO Pins + SPI + Data Outputs)
}

void loop() {
  EVB.ReadOutput(sensorOutputBuffer, 6);        //  Reading of the GYPRO Output (fill the 6 bytes buffer)

  sensorOutputBuffer[0] &= 0b01111111;       // Bitmask to have only bits 6 to 0 of the first byte
  ST = sensorOutputBuffer[3] &= 0b00000001;  // Bitmask to have the self test value
  sensorOutputBuffer[3] &= 0b10000000;       // Bitmask to have only bits 7 of the fourth byte

  Rate = sensorOutputBuffer[0];              // Bitmask and bitshift to determine angular rate
  Rate = Rate * 256 + sensorOutputBuffer[1];
  Rate = Rate * 256 + sensorOutputBuffer[2];
  Rate = Rate * 256 + sensorOutputBuffer[3];
  Rate = Rate << 1;

  Temperature = (sensorOutputBuffer[4] << 8) | sensorOutputBuffer[5]; // Bitmask and bitshift to determine temperature of the sensor
  Temperature = Temperature >> 2;

  ArduinoOutput.print("Angular Rate : ");             // Send to the value to the Arduino Output Port
  ArduinoOutput.print(float(Rate / 256) / 10000, 3);
  ArduinoOutput.print("\t\t Temperature : ");
  ArduinoOutput.print(Temperature);
  ArduinoOutput.print("\t\t Self-Test : ");
  ArduinoOutput.println(ST);

  delay(1000 / DataRate);
}
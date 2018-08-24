/****************************************************************************

                FIRMWARE 2.2 for ARDUINO M0          //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
               This Firmware is optimised            //  //      //
                 for Evaluation Tool 2.2             //  //////////

     Copyright (C) 2018 by Tronics Microsystems

     This file is part of Tronics Evaluation Tool.

     Tronics Evaluation Tool is free software: you can redistribute it and
     /or modify it under the terms of the GNU Lesser General Public License
     as published by the Free Software Foundation, either version 3 of the
     License, or (at your option) any later version.

     Tronics Evaluation Tool is distributed in the hope that it will be
     useful, but WITHOUT ANY WARRANTY; without even the implied warranty
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
     of GNU Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with Box.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
   @file Common_Features.ino
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 23 August 2018
   @version : 2.2
   @brief File containing common features for EvalutationTool file.
   @see https://github.com/TronicsMicrosystems/TronicsEvaluationKit_Library
*/

/////////////////////////////////////////////////////////////
//                                                         //
//                      MAIN PROGRAM 1                     //
//                                                         //
/////////////////////////////////////////////////////////////
//                                                         //
//                    sensorOutputRead                     //
//                    sensorStartupTime                    //
//                      serialRead                         //
//                 tronicsFirmwareVersion                  //
//                                                         //
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//                                                         //
//       ANGULAR RATE or ACCELERATION AND TEMPERATUE       //
//             OUTPUT READING (RDGO / RDTO)                //
//                   AND SELF TEST                         //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorOutputRead(unsigned int DataRate, unsigned int AcquisitionTime) {   // Two arguments : Data Rate in hertz (Typically 1800 Hz) + Time Acquisition in seconds
  unsigned int DataRatePeriod = 1000000 / DataRate;              // Define Data Rate period in microseconds (µs)
  unsigned long NbSamples = (unsigned long)DataRate * (unsigned long)AcquisitionTime;
  byte sensorOutputBuffer[6] = {};                             // Define bytes Buffer of the Sensor (will contains the first 6 bytes of the SPI register)
  String Stop_Acquisition = "";                           // Variable to check if a stop of the acquisition is asked

  for (unsigned long i = 0; i < NbSamples; i++) {         // TimeAcquisition Loop (One loop = One Second)
    delayMicroseconds(DataRatePeriod - Transfert_Time);    // Wait/Synchronize the correct period between each measure (one measure = 76 or 77 µs). Need to anticipate this transfert to have correct sampling.

    EVB.ReadOutput(sensorOutputBuffer, 6);                // Read the 6 bytes of the Sensor Output (Angular Rate + Temperature + Selft Test)
    ARDUINO_OUTPUT.write(sensorOutputBuffer, 6);           // Send to computer the buffer with the 6 bytes
    
    if (ARDUINO_OUTPUT.available()) {                      // Check if an acquisition stop is asked
      Stop_Acquisition = ARDUINO_OUTPUT.read();
      break;      
    }
  }
}

/////////////////////////////////////////////////////////////
//                                                         //
//      STARTUP TIME FUNCTION (ONLY ON EVB 2.1 AND 3.0)    //
//          WORK IDENTICALLY THAN RDGO/RDTO FUNCTION       //
//                WITH ENABLE PIN MANAGEMENT               //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorStartupTime() {
  unsigned long DataRatePeriod = 1000;  // Define Data Rate period in microseconds (µs)
  unsigned long NbSamples = 3000;       // Define number samples to measure during the acquisition time
  byte sensorOutputBuffer[6];           // Define bytes Buffer of the Sensor or Axo (will contains the first 6 bytes of the SPI register)

  digitalWrite(EN_Pin, LOW);    // Set Enable Pin at
  digitalWrite(LED_Pin, LOW);   // Turn off the LED
  delay(10);

  for (unsigned long i = 0; i < NbSamples; i++) {       // AcquisitionTime Loop (One loop = One Measure)
    delayMicroseconds(DataRatePeriod - Transfert_Time);  // Wait/Synchronize the correct period between each measure (one measure = 77 µs). Need to anticipate this transfert to have correct sampling.

    if (i == DataRatePeriod) {
      digitalWrite(EN_Pin, HIGH);
      digitalWrite(LED_Pin, HIGH);
    }

    EVB.ReadOutput(sensorOutputBuffer, 6);           // Read the 6 bytes Sensor Output
    ARDUINO_OUTPUT.write(sensorOutputBuffer, 6);   // Send to computer the buffer with the 6 bytes

  }
}

/////////////////////////////////////////////////////////////
//                                                         //
//             READ INCOMING DATA USB FUNCTION             //
//                                                         //
/////////////////////////////////////////////////////////////

unsigned long serialRead() {
  int ByteReceived = 0;    // Declare ByteReceived variable
  int Compt = 0;           // Declare Compt variable
  boolean Signe = true;    // Declare Signe variable
  long NumberReceived = 0; // Declare NumberReceived variable

  while (NumberReceived == 0) {
    while (ARDUINO_OUTPUT.available() > 0) { // While loop available byte on the InBuffer of Arduino
      ByteReceived = ARDUINO_OUTPUT.read();  // Read byte received

      if ((ByteReceived == '-') && (Compt == 0))Signe = false;  // Case if byte is a signe for a negative number
      Compt += 1;  // Increment compteur of byte

      ByteReceived = ByteReceived - 48; // ASCII table conversion

      if ((ByteReceived >= 0) && (ByteReceived <= 9)) NumberReceived = (NumberReceived * 10) + ByteReceived; // Concatenation of all byte received in one single number
      delay(1);
    }

    if (Signe == false) NumberReceived = NumberReceived * (-1);  // If negative number, convert it
  }

  if (NumberReceived == -1) {         // Bypass method if Gain should be set at 0 (User or application should send -1 and for a 0 value)
    NumberReceived = 0;
  }

  return (NumberReceived); // Return number received
}

/////////////////////////////////////////////////////////////
//                                                         //
//               READ COMPLETE SPI REGISTER                //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorReadSPI() {
  byte sensorOutputBuffer[8];           // Define bytes Buffer of the Sensor or Axo (will contains the first 6 bytes of the SPI register)

  EVB.ReadOutput(sensorOutputBuffer, 8);           // Read the 6 bytes Sensor Output
  ARDUINO_OUTPUT.write(sensorOutputBuffer, 8);   // Send to computer the buffer with the 6 bytes
}

/////////////////////////////////////////////////////////////
//                                                         //
//                   CHECK SKETCH VERSION                  //
//                                                         //
/////////////////////////////////////////////////////////////

void tronicsFirmwareVersion() {
  ARDUINO_OUTPUT.println(TronicsFirmwareVersion);
}


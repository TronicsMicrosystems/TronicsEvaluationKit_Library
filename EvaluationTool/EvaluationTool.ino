/****************************************************************************

                 FIRMWARE 2.3 for ARDUINO            //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
               This Firmware is optimised            //  //      //
                 for Evaluation Tool 2.3             //  //////////

     Copyright (C) 2021 by Tronics Microsystems

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
   @file EvaluationTool.ino
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 29 March 2021
   @version : 2.3
   @brief File containing firmware to program Arduino Board for Evaluation Tool software developped by Tronics Microsystems.
   @see https://github.com/TronicsMicrosystems/TronicsEvaluationKit_Library
*/

#define ARDUINO_OUTPUT SerialUSB  // <== Define Ouput Port name (USB Port = SerialUSB ; RS422 Port = Serial1)
#define EVB_VERSION 30   // <== Define EVB Version (21 for EVB 2.1, 30 for EVB 3.0)


/////////////////////////////////////////////////////////////
//                                                         //
//                 GLOBAL INITIALISATIONS                  //
//                                                         //
/////////////////////////////////////////////////////////////

#include <EVB.h>  // Required for EVB use in library

void setup() {
  EVB.Init(ARDUINO_OUTPUT, EVB_VERSION);  // Initialisation of EVB 2.0, 2.1 or 3.0 (GPIO Pins + SPI + Data Outputs)
}

/////////////////////////////////////////////////////////////
//                                                         //
//                        MAIN PROGRAM                     //
//                                                         //
/////////////////////////////////////////////////////////////

void loop() {
  if (ASIC_Version == 1) { // Case ASIC V1 ==> GYPRO® 2300
    mainLoop1();
  }

  else {  // Case ASIC V2 ==> GYPRO® 3300, 4100 or AXO® 315.
    mainLoop2();
  }
}

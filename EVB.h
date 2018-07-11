/****************************************************************************

    		Tronics EVB Library for ARDUINO M0       //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
               This Firmware is optimised            //  //      //
                 for Evaluation Tool 2.2             //  //////////

     Copyright (C) 2017 by Tronics Microsystems

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
   @file EVB.h
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 29 Sept 2017
   @brief File containing header code for EvalutationTool_2_2 library.
   @see https://github.com/TronicsMicrosystems/Firmware-2.2
*/

#include <Arduino.h>

#ifndef EVB_h
#define EVB_h

#ifndef ARDUINO_ARCH_SAMD              // Be sure that you choose Arduino M0, M0 Pro or Zero board in Tools => Board
	#error "You must choose Arduino M0, M0 Pro and Zero in Tools menu => Board"
#endif

#define TronicsFirmwareVersion 22

extern uint16_t  ASIC_Version;
extern uint16_t  EVB_Version;

extern uint16_t  LED_Pin;
extern uint16_t  ST_Pin;
extern uint16_t  EN_Pin;
extern uint16_t  FLCK_Pin;
extern uint16_t  VDDIO_Pin;
extern uint16_t  CSB_Pin;
extern uint16_t  DRDY_Pin;
extern uint16_t  TransfertTime;

class EVBClass {
public:	
	static void Init(Serial_ ArduinoOutput);
	static void Init(Uart ArduinoOutput);
	
	static void ReadOutput(uint8_t Buffer_Sensor[], uint8_t Buffer_Size);
	
	static uint32_t ReadSR(uint32_t Address);
	static void WriteSR(uint32_t Data, uint32_t Address);
	
	static uint32_t ReadMTP(uint32_t Address);	
	static void ProgMTP(uint32_t Address);
	static void CopyMTP(uint32_t Address);
	
	static float ReadFCLK(uint32_t sampleTime);	
};

extern EVBClass EVB;

#endif

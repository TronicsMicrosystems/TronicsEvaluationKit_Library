/****************************************************************************

                FIRMWARE 2.3 for ARDUINO M0          //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
               This Firmware is optimised            //  //      //
                 for Evaluation Tool 2.3             //  //////////

     Copyright (C) 2020 by Tronics Microsystems

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
   @file EVB.cpp
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 02 July 2020
   @version : 2.3
   @brief File containing source code for EvalutationTool library.
   @see https://github.com/TronicsMicrosystems/TronicsEvaluationKit_Library
*/

#include "EVB.h"
#include "SPI.h"

uint16_t ASIC_Version = 0;

uint16_t LED_Pin = 13;
uint16_t ST_Pin = 0;
uint16_t EN_Pin = 0;
uint16_t FLCK_Pin = 0;
uint16_t VDDIO_Pin = 0;
uint16_t CSB_Pin = 0;
uint16_t DRDY_Pin = 0;

uint16_t Transfert_Time = 77;

/////////////////////////////////////////////////////////////
//                                                         //
//                 GLOBAL INITIALISATIONS                  //
//                                                         //
/////////////////////////////////////////////////////////////

void EVBClass::Init(Serial_ ArduinoOutput, uint16_t EvbVersion)	{	
	Startup_Initialization(EvbVersion);
	Transfert_Time = 100; //M0 = 76
	ArduinoOutput.begin(115200);          // Initialisation of USB communication
	while(!ArduinoOutput);
}

/*void EVBClass::Init(Uart ArduinoOutput)	{ // Uncomment to use RS422 port with Arduino M0
	Startup_Initialization();
	Transfert_Time = 77;
	ArduinoOutput.begin(921600);          // Initialisation of Serial communication
	while(!ArduinoOutput);
}*/

void EVBClass::Startup_Initialization(uint16_t EVB_Version) {
	// Hardware test to know if the board is an EVB 2.0, 2.1 or 3.0
	SPI.begin();                          // Initialisation of SPI communication
	SPI.setDataMode(SPI_MODE0);           // Set SPI communication MODE0 : CPOL=0 CPHA=0
	SPI.setClockDivider(SPI_CLOCK_DIV16); // Set SPI Frequency at 1 MHz
	SPI.setBitOrder(MSBFIRST);            // Set bit order : Most Significant Bit First 	

	if (EVB_Version == 20)
	{
		ST_Pin = 26;                      // Define ST (Self Test) Pin number (26)
		EN_Pin = 13;                      // Define En (Enable) Pin number (13 = LED)
		FLCK_Pin = 4;                     // Define FLCK (Clock frequency) Pin number (4)
		VDDIO_Pin = 27;                   // Define VVDIO Pin number (27)
		CSB_Pin = 25;                     // Define CSB (SPI Chip select) number (25)	
	}
	else if (EVB_Version == 21)
	{
		ST_Pin = 12;                      // Define ST (Self Test) Pin number (12)
		EN_Pin = 11;                      // Define En (Enable) Pin number (11)
		FLCK_Pin = 10;                    // Define FLCK (Clock frequency) Pin number (10)
		VDDIO_Pin = 9;                    // Define VVDIO Pin number (9)
		CSB_Pin = 8;                      // Define CSB (SPI Chip select) number (8)
	}

	else if (EVB_Version == 30)
	{
		EN_Pin = 8;                     // Define En (Enable) Pin number (8)
		ST_Pin = 9;                     // Define ST (Self Test) Pin number (9)
		FLCK_Pin = 10;                  // Define FLCK (Clock frequency) Pin number (10)
		CSB_Pin = 11;                   // Define CSB (SPI Chip select) Pin number (11)
		DRDY_Pin = 12;					// Define DRDY (Data Ready) Pin number (12)
		VDDIO_Pin = 13;                 // Define VDDIO Pin number (9)
	}

	pinMode(ST_Pin, INPUT);               // Set ST (Self Test) Pin as Input
	pinMode(FLCK_Pin, INPUT);             // Set FCLK (Clock Frequency) Pin as Input
	pinMode(DRDY_Pin, INPUT);             // Set DRDY (Data Ready) Pin as Input
	pinMode(EN_Pin, OUTPUT);              // Set EN (Enable) Pin as Output
	pinMode(VDDIO_Pin, OUTPUT);           // Set VDDIO Pin as Output
	pinMode(CSB_Pin, OUTPUT);             // Set CSB (SPI Chip select) Pin as Output
	pinMode(LED_Pin, OUTPUT);             // Set LED Pin as Output

	digitalWrite(EN_Pin, LOW);           // Set EN (Enable) pin to Low Level (Active Low) (To Reset the sensor)
	digitalWrite(CSB_Pin, HIGH);          // Set CSB (SPI Chip select) pin to High Level (Active Low)
    digitalWrite(VDDIO_Pin, LOW);         // Set VDDIO pin to High Level (Active Low)

	delay(1000);

	digitalWrite(EN_Pin, HIGH);           // Set EN (Enable) pin to High Level (Active Low)

	delay(1000);

	uint32_t TestASIC1 = ReadSR(0x00);
    uint32_t TestASIC2 = ReadSR(0x40);

    if ((TestASIC1 == TestASIC2) && (TestASIC1 != 0)) {
      ASIC_Version = 1;
      digitalWrite(VDDIO_Pin, LOW);       // Set VDDIO pin to Low Level (Active High)
    }
    else {
      ASIC_Version = 2;
      digitalWrite(VDDIO_Pin, HIGH);      // Set VDDIO pin to High Level (Active High)
	}		
}

/////////////////////////////////////////////////////////////
//                                                         //
//        ANGULAR RATE / ACCELERATION AND TEMPERATUE       //
//               OUTPUT READING (RDGO / RDTO)              //
//                     AND SELF TEST                       //
//                                                         //
/////////////////////////////////////////////////////////////

void EVBClass::ReadOutput(uint8_t Buffer_Sensor[], uint8_t Buffer_Size) {	
	digitalWrite(CSB_Pin, LOW);             // Set CS to Low Level (Active)
    SPI.transfer(0x50);                     // reading command : 0x5 + start address : 0x0 --> 0x50

	for (uint8_t i=0 ; i < Buffer_Size ; i++) {
    Buffer_Sensor[i] = SPI.transfer(0);
  }
	digitalWrite(CSB_Pin, HIGH);            // Set CS to High Level (Inactive)
}

/////////////////////////////////////////////////////////////
//                                                         //
//                     RSYST FUNCTION :                    //
//              READ 32-BIT DATA FROM THE SPI              //
//              REGISTER AT A CERTAIN ADDRESS              //
//                                                         //
/////////////////////////////////////////////////////////////

uint32_t EVBClass::ReadSR(uint32_t Address) {
	digitalWrite(CSB_Pin, LOW);		// Set CS to Low Level (Active)
	SPI.transfer(0x7C);        	 	// Send 0x7 (write command) and 0xC (SPI register address) -> 0x7C
	SPI.transfer(Address);  		// Send The system register adress
	SPI.transfer(0x01);				// Send System register command (0x01 : read from system register)
	digitalWrite(CSB_Pin, HIGH);	// Set CS to high Level (Inactive)

	delay(4);

	uint8_t rspi[4] = {};

	digitalWrite(CSB_Pin, LOW);           // Set CS to Low Level (Active)
	SPI.transfer(0x58);                   // Send 0x5 (read command) and 0x8 (SPI register address) -> 0x58
	rspi[0] = SPI.transfer(0x00);     // Read first incoming byte
	rspi[1] = SPI.transfer(0x00);     // Read second incoming byte
	rspi[2] = SPI.transfer(0x00);     // Read third incoming byte
	rspi[3] = SPI.transfer(0x00);     // Read last incoming byte
	digitalWrite(CSB_Pin, HIGH);          // Set CS to high Level (Inactive)

	uint32_t RSYST = (unsigned long)rspi[0] << 24 
                	| (unsigned long)rspi[1] << 16
                    | (unsigned long)rspi[2] << 8
                    | (unsigned long)rspi[3];

	return (RSYST);          // Return the data
}

/////////////////////////////////////////////////////////////
//                                                         //
//                     WSYST FUNCTION :                    //
//              WRITE 32-BIT DATA FROM THE SPI             //
//              REGISTER AT A CERTAIN ADDRESS              //
//                                                         //
/////////////////////////////////////////////////////////////

void EVBClass::WriteSR(uint32_t Data, uint32_t Address) {
	uint8_t wspi[4] = {};

	wspi[0] = (Data & 0xff000000) >> 24;  // Detachment of the data into 4 bytes
	wspi[1] = (Data & 0x00ff0000) >> 16;
	wspi[2] = (Data & 0x0000ff00) >>  8;
	wspi[3] = (Data & 0x000000ff)      ;

	digitalWrite(CSB_Pin, LOW);   	// Set CS to Low Level (Active)
	SPI.transfer(0x78);           	// Send 0x7 (write command) and 0x8 (SPI register address) -> 0x78
	SPI.transfer(wspi[0]);         	// Send first byte of the data
	SPI.transfer(wspi[1]);         	// Send second byte of the data
	SPI.transfer(wspi[2]);         	// Send third byte of the data
	SPI.transfer(wspi[3]);         	// Send last byte of the data
	SPI.transfer(Address);  		// Send The system register adress
	SPI.transfer(0x02);           	// Send System register command (0x02 : write to system register)
	digitalWrite(CSB_Pin, HIGH);  	// Set CS to high Level (Inactive)
}

/////////////////////////////////////////////////////////////
//                                                         //
//           READ DATA FROM OTP COMMAND (RMTP)             //
//                                                         //
/////////////////////////////////////////////////////////////

uint32_t EVBClass::ReadMTP(uint32_t Address) {
	digitalWrite(CSB_Pin, LOW);			// Set CS to Low Level (Active)
	SPI.transfer(0x7C);                 // Send 0x7 (Write command) and 0xC (SPI register address) -> 0x7C
	SPI.transfer(Address);				// Send OTP Address
	SPI.transfer(0x08);                 // Send 0x8 (Read a certain address from OTP command)
	digitalWrite(CSB_Pin, HIGH);		// Set CS to high Level (Inactive)

	delay(4);                          	// 4  ms of delay

	uint8_t rspi[4] = {};

	digitalWrite(CSB_Pin, LOW);        	// Set CS to Low Level (Active)
	SPI.transfer(0x58);                 // Send 0x5 (read command) and 0x8 (SPI register address) -> 0x58
	rspi[0] = SPI.transfer(0x00);     	// Read first incoming byte
	rspi[1] = SPI.transfer(0x00);     	// Read second incoming byte
	rspi[2] = SPI.transfer(0x00);     	// Read third incoming byte
	rspi[3] = SPI.transfer(0x00);     	// Read last incoming byte
	digitalWrite(CSB_Pin, HIGH);        // Set CS to high Level (Inactive)

	uint32_t RMTP = (unsigned long)rspi[0] << 24 
                	| (unsigned long)rspi[1] << 16
                    | (unsigned long)rspi[2] << 8
					| (unsigned long)rspi[3];					

  return (RMTP);                        // Return the Read Data from OTP
}

/////////////////////////////////////////////////////////////
//                                                         //
//        PROGRAM DATA IN THE OTP COMMAND (PMTP)           //
//                                                         //
/////////////////////////////////////////////////////////////

void EVBClass::ProgMTP(uint32_t Address) {
	digitalWrite(CSB_Pin, LOW);		// Set CS to Low Level (Active)
	SPI.transfer(0x7C);        		// Send 0x7 (write command) and 0xC (SPI register address) -> 0x7C
	SPI.transfer(Address);    		// Send The OTP address (Address OxC of the SPI register) - Exemple : 0x3E for B0 coefficient
	SPI.transfer(0x06);         	// Send the 8 bits commands : 0x06 = Program a certain address in the OTP
	digitalWrite(CSB_Pin, HIGH);	// Set CS to high Level (Inactive)
}

/////////////////////////////////////////////////////////////
//                                                         //
//              COPY DATA IN THE SYTEM REGISTER            //
//                 FROM THE OTP COMMAND (CMTP)             //
//                                                         //
/////////////////////////////////////////////////////////////

void EVBClass::CopyMTP(uint32_t Address) {
	digitalWrite(CSB_Pin, LOW);		// Set CS to Low Level (Active)
	SPI.transfer(0x7C);				// Send 0x7 (write command) and 0xC (SPI register address) -> 0x7C
	SPI.transfer(Address);			// Send The OTP address (Address OxC of the SPI register) - Exemple : 0x3E for B0 coefficient
	SPI.transfer(0x07);				// Send the 8 bits commands : 0x06 = Program a certain address in the OTP
	digitalWrite(CSB_Pin, HIGH);	// Set CS to high Level (Inactive)
}

EVBClass EVB;

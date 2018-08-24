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
   @file G3300S_A215_Features.ino
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 23 August 2018
   @version : 2.2
   @brief File containing GYPRO® 3300S and AXO® 215 features for EvalutationTool file.
   @see https://github.com/TronicsMicrosystems/TronicsEvaluationKit_Library
*/

/////////////////////////////////////////////////////////////
//                                                         //
//                      MAIN PROGRAM 2                     //
//                                                         //
/////////////////////////////////////////////////////////////
//                                                         //
//  I - Basic Features                                     //
//            1 : sensorOutputRead                         //
//            2 : sensorGoutSel                            //
//            3 : sensorWriteTempCompensation              //
//            4 : sensorReadTempCompensation               //
//            5 : sensorToutSel                            //
//            6 : sensorWriteTempCalibration               //
//            7 : sensorReadTempCalibration                //
//                                                         //
//  II - Others Features                                   //
//            9 : sensorHardwareSelfTest                   //
//           10 : sensorStartupTime                        //
//                                                         //
//  III - OTP Features                                     //
//           11 : sensorMTPSlotsNumber                     //
//           12 : sensorWriteTempCompensation on MTP       //
//           13 : sensorWriteTempCalibration on MTP        //
//           14 : sensorUpdateMTPSlotNumber                //
//                                                         //
//  IV - ID Features                                       //
//           15 : sensorUID                                //
//           16 : sensorProductName                        //
//           17 : EVBVersion                               //
//                                                         //
//  V - Developper Features (Private)                      //
//           18 : sensorReadAllSystemRegister              //
//           19 : sensorWriteAllSystemRegister             //
//           20 : sensorReadSystemRegister                 //
//           21 : sensorWriteSystemRegister                //
//           22 : sensorReadMTP                            //
//           23 : sensorReadSPI                            //
//           24 : tronicsFirmwareVersion                   //
//           25 : sensorWriteSystemRegister
//                                                         //
/////////////////////////////////////////////////////////////

void mainLoop2() {
  switch (serialRead()) {
    /* RDGO + RDTO + ST : Return Rate, Temperature and Self Test values */
    case 1 : {
        unsigned int _DataRate = serialRead();  // Wait for an incoming Data Rate value (typical 0 to 2000)
        unsigned int _TimeAcq = serialRead();   // Wait for an incoming Acquisition Time value in seconds (0 to 4294967295)
        sensorOutputRead(_DataRate, _TimeAcq);  // Run RDGO_RDTO Function with 2 last arguments
        break;
      }

    /* GOUT SEL : Select Raw / Temperature Compensated gyro output (1 = Temperature Compensated ; 2 = Raw) */
    case 2 : {
        unsigned int _GoutSel =  serialRead();  // Declare _GoutSel value

        if (_GoutSel == 1) {        // Case 1 = Raw
          sensorGoutSel0_2();       // Set gyro output sel at 0
        }
        else if (_GoutSel == 2) {   // Case 2 = Temperature Compensated
          sensorGoutSel1_2();       // Set gyro output sel at 1
        }
        break;
      }

    /* TEMPERATURE COMPENSATION */
    case 3 : {
        unsigned long _B0 = serialRead();   // Declare B0 received
        unsigned long _SF0 = serialRead();  // Declare SF0 received
        unsigned long _TMID = serialRead(); // Declare TMID received
        unsigned long _B1 = serialRead();   // Declare B1 received
        unsigned long _SF1 = serialRead();  // Declare SF1 received
        unsigned long _B2 = serialRead();   // Declare B2 received
        unsigned long _SF2 = serialRead();  // Declare SF2 received
        unsigned long _B3 = serialRead();   // Declare B3 received
        unsigned long _SF3 = serialRead();  // Declare SF3 received
        unsigned long _B4 = serialRead();   // Declare B4 received
        unsigned long _SF4 = serialRead();  // Declare SF4 received

        sensorWriteTempCompensation_2(_B0, _SF0, _TMID, _B1, _SF1, _B2, _SF2, _B3, _SF3, _B4, _SF4); // sensorWriteTempCompensation_2 : Write all the compensation coefficients into the System Register
        break;
      }

    /* READ TEMPERATURE COMPENSATION COEFF */
    case 4 :
      sensorReadTempCompensation_2(); // Read and display temperature compensation coefficients
      break;

    /* TOUT SEL : Select Raw / Calibrated temperature output (1 = Raw ; 2 = Calibrated) */
    case 5 : {
        unsigned int _ToutSel = (unsigned int)serialRead();  // Declare _ToutSel value

        if (_ToutSel == 1) {    // Case 1 = Raw
          sensorToutSel0_2();   // Set temperature output sel at 0 (Raw Output)
        }
        else if (_ToutSel == 2) { // Case 2 = Calibrated
          sensorToutSel1_2();     // Set temperature output sel at 0 (Calibrated Output)
        }
        break;
      }

    /*  TEMPERATURE CALIBRATION : Write temperature sensor calibration coefficients in the system register */
    case 6 : {
        unsigned int _Gain = (unsigned int)serialRead();          // Declare Gain received
        unsigned int _Offset = (unsigned int)serialRead();        // Declare Offset received
        sensorWriteTempCalibration_2(_Gain, _Offset);  // Write Gain and Offset into the System Register
        break;                                      // End of this case
      }

    /* READ TEMPERATURE CALIBRATION COEFF */
    case 7 :
      sensorReadTempCalibration_2();  // Read and display temperature sensor calibration coefficients
      break;

    /* ANALOG SELF TEST : Test GYPRO ST Pin */
    case 9 : {
        unsigned int hardwareST = digitalRead(ST_Pin);  // Test Self Test analog with Arduino ADC on the ST pin
        ARDUINO_OUTPUT.println(hardwareST);              // Display ST Analog test result (0 = Failed, 1 = Passed)
        break;
      }

    /* CHECKING CONFIGURATION : STANDARD (Config = 0) OR LOW DELAY (Config = 1) */
    case 10:
      sensorStartupTime();     // Run RDGO_RDTO Function with 2 last arguments
      break;

    /* CHECKING MTP SLOTS STATUS */
    case 11 : {
        unsigned int MTP_Slot_Value = sensorMTPSlotNumber_2(); // Check MTP Slot value
        ARDUINO_OUTPUT.println(MTP_Slot_Value);                // Display MTP Slot value
        break;
      }

    /* PROGRAMMING THE COMPENSATION COEFFICIENTS IN THE OTP */
    case 12 : {
        unsigned long _B0_MTP = serialRead();   // Declare B0 received
        unsigned long _SF0_MTP = serialRead();  // Declare SF0 received
        unsigned long _TMID_MTP = serialRead(); // Declare TMID received
        unsigned long _B1_MTP = serialRead();   // Declare B1 received
        unsigned long _SF1_MTP = serialRead();  // Declare SF1 received
        unsigned long _B2_MTP = serialRead();   // Declare B2 received
        unsigned long _SF2_MTP = serialRead();  // Declare SF2 received
        unsigned long _B3_MTP = serialRead();   // Declare B3 received
        unsigned long _SF3_MTP = serialRead();  // Declare SF3 received
        unsigned long _B4_MTP = serialRead();   // Declare B4 received
        unsigned long _SF4_MTP = serialRead();  // Declare SF4 received

        delay(10);

        // STEP 1 : Write the coefficients in the System Register (TEMP_COMPENSATION function)
        sensorWriteTempCompensation_2(_B0_MTP, _SF0_MTP, _TMID_MTP, _B1_MTP, _SF1_MTP, _B2_MTP, _SF2_MTP, _B3_MTP, _SF3_MTP, _B4_MTP, _SF4_MTP);
        delay(100);

        // STEP 2 : Program the coefficients one by one in the OTP (PROG_OTP) with 100 ms delay between each
        EVB.ProgMTP(0x3E);  // Programm B0
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x3F);  // Programm SF0
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x40);  // Programm TMID
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x41);  // Programm B1
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x42);  // Programm SF1
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x43);  // Programm B2
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x44);  // Programm SF2
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x45);  // Programm B3
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x46);  // Programm SF3
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x47);  // Programm B4
        delay(100);         // Wait 100 ms

        EVB.ProgMTP(0x48);  // Programm BSCALE
        delay(100);         // Wait 100 ms
        break;
      }

    /* PROGRAMMING THE CALIBRATION COEFFICIENTS IN THE OTP */
    case 13: {
        unsigned int _Gain_MTP = serialRead();    // Declare Gain received
        unsigned int _Offset_MTP = serialRead();  // Declare Offset received

        // STEP 1 : Write the coefficients in the System Register (sensorTempCalibration function)
        sensorWriteTempCalibration_2(_Gain_MTP, _Offset_MTP);
        delay(100); // Wait 100 ms

        // STEP 2 : Program the coefficients in the OTP (ProgMTP function)
        EVB.ProgMTP(0x04);
        delay(100);   // Wait 100 ms

        break;
      }

    /* UPDATE MTP SLOTS STATUS : Update new MTP slot value and copy new coefficients into the System Register (avoid restart the GYPRO) */
    case 14 : {
        // STEP 1a : Read data from register at Address 0x02 of the MTP and current MTP slots status
        unsigned long MTPRegister_02 = EVB.ReadMTP(0x3D);         // Read complete register in the MTP at address 0x3D (ReadMTP)
        unsigned int Current_MTPSLOTNB = sensorMTPSlotNumber_2(); // Read current MTP slots Value

        // STEP 1b : Calculate the new MTP slots status (ex : 0b0111 -> 0b1111)
        unsigned long New_MTPSLOTNB = Current_MTPSLOTNB << 1;
        New_MTPSLOTNB += 1;
        New_MTPSLOTNB = New_MTPSLOTNB << 8;

        // STEP 1c : Bitmask between the complete register content (address 0x02) and the new MTP slots status previously calculated
        MTPRegister_02 &= 0xFFFFE0FF;
        MTPRegister_02 |= New_MTPSLOTNB; // Updated register with the new MTP slots Value

        // STEP 2 : Write the new register updated at address in the System Register 0x02
        EVB.WriteSR(MTPRegister_02, 0x3D);  // WSYST function
        delay(100);              // Delay of 100 ms

        // STEP 3 : Program the modified register in MTP at address 0x02
        EVB.ProgMTP(0x3D);  // PROG_OTP function
        delay(100);         // Delay of 100 ms

        // STEP 4 : Copy the data from address 0x02 in the OTP into the corresponding address in system register
        EVB.CopyMTP(0x3D);  // COPY_OTP function
        delay(100);         // Delay of 100 ms

        // STEP 5 : Copy the coefficients from the OTP to the System Register
        EVB.CopyMTP(0x04);  // Copy gain and offset coefficients into the System Register from the OTP
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x3E);  // Copy V0 into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x3F);  // Copy S0 into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x40);  // Copy VTEMP0 into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x41);  // Copy ANULL into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x42);  // Copy ASCALE into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x43);  // Copy BNULL into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x44);  // Copy BSCALE into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x45);  // Copy CNULL into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x46);  // Copy CSCALE into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x47);  // Copy DNULL into the System Register from the OTP
        delay(100);         // Wait 100 ms
        EVB.CopyMTP(0x48);  // Copy DSCALE into the System Register from the OTP
        delay(100);         // Wait 100 ms
        break;
      }

    /* GYPRO UID : Return GYPRO UID */
    case 15 : {
        String SensorUID = sensorUID_2(); // Check UID of the Sensor
        ARDUINO_OUTPUT.println(SensorUID); // Send it to USB serial
        break;
      }

    /* CHECKING PRODUCT NAME */
    case 16: {
        String ProductName = sensorProductName_2();  // Declare and read Sensor type (GYPRO_TYPE function)
        ARDUINO_OUTPUT.println(ProductName);
        break;
      }

    /* CHECK EVB VERSION */
    case 17:
      ARDUINO_OUTPUT.println(EVB_Version);
      break;

    /* READ ALL SYSTEM REGISTER */
    case 18: {
        unsigned long SensorAllSystemRegister[82]; // Declare Array of 82 lines for all the System Register

        for (int i = 0; i <= 82; i++) {                       // For loop to complete the array with one register by line
          SensorAllSystemRegister[i] = EVB.ReadSR(i);         // Copy content of the system register (address i) in the i line of SYSTEM_REGISTER array
          ARDUINO_OUTPUT.println(SensorAllSystemRegister[i]);  // Display i system register value
        }
        break;
      }

    /* WRITE ALL SYSTEM REGISTER */
    case 19: {
        unsigned long _SystemRegister_i = 0;

        for (int i = 0; i <= 82; i++) {        // For loop to complete the array with one register by line
          _SystemRegister_i = serialRead();    // Copy content of the system register (address i) in the i line of SYSTEM_REGISTER array
          EVB.WriteSR(_SystemRegister_i, i);
          delay(10);
        }

        digitalWrite(LED_Pin, HIGH);
        delay(10);
        digitalWrite(LED_Pin, LOW);

        break;
      }

    /* READ SYSTEM REGISTER */
    case 20: {
        unsigned long _SystemRegisterAddress = serialRead();
        unsigned long SystemRegisterData = EVB.ReadSR(_SystemRegisterAddress);
        ARDUINO_OUTPUT.println(SystemRegisterData);
        break;
      }

    /* WRITE SYSTEM REGISTER */
    case 21: {
        unsigned long _SystemRegisterAddress = serialRead();
        unsigned long _SystemRegisterData = serialRead();
        EVB.WriteSR(_SystemRegisterData, _SystemRegisterAddress);
        break;
      }

    /* READ MTP */
    case 22: {
        unsigned long _MTPAddress = serialRead();
        unsigned long MTPData = EVB.ReadMTP(_MTPAddress);
        ARDUINO_OUTPUT.println(MTPData);
        break;
      }

    /* READ SPI REGISTER */
    case 23: {
        sensorReadSPI();
        break;
      }

    /* CHECK SKETCH VERSION */
    case 24: {
        tronicsFirmwareVersion();
        break;
      }

    default :     // Default case
      break;
  }
}

/////////////////////////////////////////////////////////////
//                                                         //
//                     READ THE UID OF                     //
//                     THE GYPRO MEMS                      //
//                                                         //
/////////////////////////////////////////////////////////////

String sensorUID_2() {
  String ProductName = sensorProductName_2();
  unsigned long Sensor_UID_Reg = EVB.ReadSR(0x03);  // Read UID stocked at the address 0x03 of the system register
  String Sensor_UID_full = "";

  if (ProductName == "AXO215") {        // Case if the sensor is an AXO215
    Sensor_UID_Reg &= 0x00FFFFFE;
    Sensor_UID_Reg = Sensor_UID_Reg >> 1;
    Sensor_UID_full = Sensor_UID_Reg;
  }
  else {                                // Case if the sensor is a GYPRO3300S
    Sensor_UID_Reg = Sensor_UID_Reg >> 1;

    unsigned long Sensor_a = Sensor_UID_Reg & 0x7C000000; // Bitmask and bitshift series to determine the GYPRO UID
    Sensor_a = Sensor_a >> 26;

    unsigned long Sensor_b = Sensor_UID_Reg & 0x03F00000;
    Sensor_b = Sensor_b >> 20;

    unsigned long Sensor_c = Sensor_UID_Reg & 0x000FC000;
    Sensor_c = Sensor_c >> 14;

    unsigned long Sensor_d = Sensor_UID_Reg & 0x00003C00;
    Sensor_d = Sensor_d >> 10;

    unsigned long Sensor_e = Sensor_UID_Reg & 0x000003E0;
    Sensor_e = Sensor_e >> 5;
    Sensor_e += 64;

    unsigned long Sensor_f = Sensor_UID_Reg & 0x0000001F;

    if (Sensor_a < 10) {
      Sensor_UID_full.concat("0");
    }
    Sensor_UID_full.concat(Sensor_a);

    if (Sensor_b < 10) {
      Sensor_UID_full.concat("0");
    }
    Sensor_UID_full.concat(Sensor_b);

    if (Sensor_c < 10) {
      Sensor_UID_full.concat("0");
    }
    Sensor_UID_full.concat(Sensor_c);

    if (Sensor_d < 10) {
      Sensor_UID_full.concat("0");
    }
    Sensor_UID_full.concat(Sensor_d);

    char Sensor_ec;
    if (Sensor_e > 90) {
      Sensor_UID_full.concat("A");
      char Sensor_ec = Sensor_e - 26;
      Sensor_UID_full.concat(Sensor_ec);
    }
    else {
      Sensor_ec = Sensor_e;
      Sensor_UID_full.concat(Sensor_ec);
    }

    if (Sensor_f < 10) {
      Sensor_UID_full.concat("0");
    }
    Sensor_UID_full.concat(Sensor_f);
  }

  return (Sensor_UID_full);                 // Return UID of the Sensor (The same written on the sensor)
}

/////////////////////////////////////////////////////////////
//                                                         //
//            READ THE CONFIGURATION OF THE GYPRO          //
//                 (STANDARD OR LOW DELAY)                 //
//                                                         //
/////////////////////////////////////////////////////////////

unsigned long SENSOR_TYPE() {
  unsigned long Sensor_Type = EVB.ReadSR(0x27);       // Read System Register at Address 0x27

  if (Sensor_Type == 12) {
    Sensor_Type = 0;
  }
  else {
    Sensor_Type = 1;
  }
  return (Sensor_Type);     // return the configuration (0 = AXO215 ; 1 = GYPRO3300S)
}

/////////////////////////////////////////////////////////////
//                                                         //
//              READ THE UID (SERIAL NUMBER)               //
//                   OF THE GYPRO MEMS                     //
//                                                         //
/////////////////////////////////////////////////////////////

String sensorProductName_2() {
  String Product_Name = "";
  unsigned long Sensor_Type = SENSOR_TYPE();
  if (Sensor_Type == 0) {
    Product_Name = "AXO215";
  }
  else {
    Product_Name = "GYPRO3300S";
  }

  return (Product_Name);
}

/////////////////////////////////////////////////////////////
//                                                         //
//               SELECTION OF THE GYRO OUTPUT              //
//      RAW / TEMPERATURE COMPENSATED DATA (GOUT_SEL)      //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorGoutSel0_2() {                        // Set GOUT SEL at 0 (Raw)
  unsigned long SR_GOUT_SEL = EVB.ReadSR(0x3D);  // Read System Register at address 0x3D
  SR_GOUT_SEL &= 0x7FFFFFFF;                     // Bitmask to change only bit 31 (low)
  EVB.WriteSR(SR_GOUT_SEL, 0x3D);                // Write the modified register
}

void sensorGoutSel1_2() {                        // Set GOUT SEL at 1 (Temperature Compensated)
  unsigned long SR_GOUT_SEL = EVB.ReadSR(0x3D);  // Read System Register at Address 0x3D
  SR_GOUT_SEL |= 0x80000000;                     // Bitmask to change only bit 31 (high)
  EVB.WriteSR(SR_GOUT_SEL, 0x3D);                // Write the modified register
}

/////////////////////////////////////////////////////////////
//                                                         //
//            SELECTION OF THE TEMPERATURE OUTPUT          //
//              RAW / CALIBRATED DATA (GOUT_SEL)           //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorToutSel0_2() {                        // Set TOUT SEL at 0 (Raw)
  unsigned long SR_TOUT_SEL = EVB.ReadSR(0x04);  // Read System Register at address 0x04
  SR_TOUT_SEL &= 0xFFFFFFF7;                     // Bitmask to change only bit 3 (low)
  EVB.WriteSR(SR_TOUT_SEL, 0x04);                // Write the modified register
}

void sensorToutSel1_2() {                        // Set TOUT SEL at 1 (Calibrated)
  unsigned long SR_TOUT_SEL = EVB.ReadSR(0x04);  // Read System Register at address 0x04
  SR_TOUT_SEL |= 0x00000008;                     // Bitmask to change only bit 3 (high)
  EVB.WriteSR(SR_TOUT_SEL, 0x04);                // Write the modified register
}

/////////////////////////////////////////////////////////////
//                                                         //
//                  WRITE COMMAND OF THE                   //
//              TEMPERATURE SENSOR CALIBRATION             //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorWriteTempCalibration_2(unsigned int Temp_Gain, unsigned int Temp_Offset) {
  Temp_Gain = Temp_Gain << 4;                   // 4 Bitshift
  unsigned long Temp_Gain_plus_Offset = 0;      // Create long 32-bits data 0000 0000 0000 00GG GGGG GGGG GGGG XXXX (O = Offset ; G = Gain)
  Temp_Gain_plus_Offset += (unsigned long)Temp_Offset << 18;   // Write Offset at correct place
  Temp_Gain_plus_Offset += (unsigned long)Temp_Gain;           // Write Gain at correct place

  unsigned long SR_Gain_Offset = EVB.ReadSR(0x04);  // Read System register at address
  SR_Gain_Offset &= 0x0000000F;                     // 0000 OOOO OOOO OOO0 0000 0000 0000 1111 = 0x0000000F (Offset and Gain bits place at 0)
  SR_Gain_Offset |= (unsigned long)Temp_Gain_plus_Offset;          // Bitmaks to change only bits where Gain and Offset should be written
  EVB.WriteSR(SR_Gain_Offset, 0x04);                // Write the modified register
}

/////////////////////////////////////////////////////////////
//                                                         //
//             READ COMMAND OF THE TEMPERATURE             //
//             SENSOR CALIBRATION COEFFICIENTS             //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorReadTempCalibration_2(void) {
  unsigned long SR_Gain_Offset = EVB.ReadSR(0x04);  // Read System Register at address 0x04

  unsigned long Read_Gain = SR_Gain_Offset;
  Read_Gain &= 0x0003FFF0;            // Bitmask and bitshift to determine Gain value
  Read_Gain = Read_Gain >> 4;
  ARDUINO_OUTPUT.println(Read_Gain);   // Write the Gain on the USB

  unsigned long Read_Offset = SR_Gain_Offset;
  Read_Offset &= 0xFFFC0000;          // Bitmask and bitshift to determine Offset value
  Read_Offset = Read_Offset >> 18;
  ARDUINO_OUTPUT.println(Read_Offset); // Write the Offset on the USB
}

/////////////////////////////////////////////////////////////
//                                                         //
//             WRITE COMMAND OF THE TEMPERATURE            //
//                COMPENSATION COEFFICIENTS                //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorWriteTempCompensation_2(unsigned long Coeff_B0, unsigned long Coeff_SF0, unsigned long Coeff_TMID, unsigned long Coeff_B1, unsigned long Coeff_SF1, unsigned long Coeff_B2, unsigned long Coeff_SF2, unsigned long Coeff_B3, unsigned long Coeff_SF3, unsigned long Coeff_B4, unsigned long Coeff_SF4)  {
  /* Write Coeff_B0 Function */
  Coeff_B0 &= 0x00FFFFFF;         // Bitmask to have only 24 bits data
  EVB.WriteSR(Coeff_B0, 0x3E);    // Write B0 coefficient in the System Register at Address 0x3E

  /* Write Coeff_SF0 Function */
  Coeff_SF0 &= 0x7FFFFFFF;        // Bitmask to have only 31 bits data
  EVB.WriteSR(Coeff_SF0, 0x3F);   // Write SF0 coefficient in the System Register at Address 0x3F

  /* Write Coeff_TMID Function */
  Coeff_TMID &= 0x000FFFFF;       // Bitmask to have only 20 bits data
  EVB.WriteSR(Coeff_TMID, 0x40);  // Write TMID coefficient in the System Register at Address 0x40

  /* Write Coeff_B1 Function */
  Coeff_B1 &= 0x3FFFFFFF;         // Bitmask to have only 30 bits data
  EVB.WriteSR(Coeff_B1, 0x41);    // Write B1 coefficient in the System Register at Address 0x41

  /* Write Coeff_SF1 Function */
  Coeff_SF1 &= 0x3FFFFFFF;        // Bitmask to have only 30 bits data
  EVB.WriteSR(Coeff_SF1, 0x42);   // Write SF1 coefficient in the System Register at Address 0x42

  /* Write Coeff_B2 Function */
  Coeff_B2 &= 0x000FFFFF;         // Bitmask to have only 20 bits data
  EVB.WriteSR(Coeff_B2, 0x43);    // Write B2 coefficient in the System Register at Address 0x43

  /* Write Coeff_SF2 Function */
  Coeff_SF2 &= 0x001FFFFF;        // Bitmask to have only 21 bits data
  EVB.WriteSR(Coeff_SF2, 0x44);   // Write SF2 coefficient in the System Register at Address 0x44

  /* Write Coeff_B3 Function */
  Coeff_B3 &= 0x000FFFFF;         // Bitmask to have only 20 bits data
  EVB.WriteSR(Coeff_B3, 0x45);    // Write B3 coefficient in the System Register at Address 0x45

  /* Write Coeff_SF3 Function */
  Coeff_SF3 &= 0x000FFFFF;        // Bitmask to have only 20 bits data
  EVB.WriteSR(Coeff_SF3, 0x46);   // Write SF3 coefficient in the System Register at Address 0x46

  /* Write Coeff_B4 Function */
  Coeff_B4 &= 0x0007FFFF;         // Bitmask to have only 19 bits data
  EVB.WriteSR(Coeff_B4, 0x47);    // Write B4 coefficient in the System Register at Address 0x47

  /* Write Coeff_SF4 Function */
  Coeff_SF4 &= 0x0007FFFF;        // Bitmask to have only 19 bits data
  EVB.WriteSR(Coeff_SF4, 0x48);   // Write SF4 coefficient in the System Register at Address 0x48
}

/////////////////////////////////////////////////////////////
//                                                         //
//             READ COMMAND OF THE TEMPERATURE             //
//                COMPENSATION COEFFICIENTS                //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorReadTempCompensation_2(void) {
  unsigned long READ_Coeff_B0 = EVB.ReadSR(0x3E);     // Read B0 coefficient register in the System Register at address 0x3E
  unsigned long READ_Coeff_SF0 = EVB.ReadSR(0x3F);    // Read SF0 coefficient register in the System Register at address 0x3F
  unsigned long READ_Coeff_TMID = EVB.ReadSR(0x40);   // Read TMID coefficient register in the System Register at address 0x40
  unsigned long READ_Coeff_B1 = EVB.ReadSR(0x41);     // Read B1 coefficient register in the System Register at address 0x41
  unsigned long READ_Coeff_SF1 = EVB.ReadSR(0x42);    // Read SF1 coefficient register in the System Register at address 0x42
  unsigned long READ_Coeff_B2 = EVB.ReadSR(0x43);     // Read B2 coefficient register in the System Register at address 0x43
  unsigned long READ_Coeff_SF2 = EVB.ReadSR(0x44);    // Read SF2 coefficient register in the System Register at address 0x44
  unsigned long READ_Coeff_B3 = EVB.ReadSR(0x45);     // Read B3 coefficient register in the System Register at address 0x45
  unsigned long READ_Coeff_SF3 = EVB.ReadSR(0x46);    // Read SF3 coefficient register in the System Register at address 0x46
  unsigned long READ_Coeff_B4 = EVB.ReadSR(0x47);     // Read B4 coefficient register in the System Register at address 0x47
  unsigned long READ_Coeff_SF4 = EVB.ReadSR(0x48);    // Read SF4 coefficient register in the System Register at address 0x48

  READ_Coeff_B0 &= 0x00FFFFFF;    // Bitmask to extract only B0 coefficient
  READ_Coeff_SF0 &= 0x7FFFFFFF;   // Bitmask to extract only SF0 coefficient
  READ_Coeff_TMID &= 0x000FFFFF;  // Bitmask to extract only TMID coefficient
  READ_Coeff_B1 &= 0x3FFFFFFF;    // Bitmask to extract only B1 coefficient
  READ_Coeff_SF1 &= 0x3FFFFFFF;   // Bitmask to extract only SF1 coefficient
  READ_Coeff_B2 &= 0x000FFFFF;    // Bitmask to extract only B2 coefficient
  READ_Coeff_SF2 &= 0x001FFFFF;   // Bitmask to extract only SF2 coefficient
  READ_Coeff_B3 &= 0x000FFFFF;    // Bitmask to extract only B3 coefficient
  READ_Coeff_SF3 &= 0x000FFFFF;   // Bitmask to extract only SF3 coefficient
  READ_Coeff_B4 &= 0x0007FFFF;    // Bitmask to extract only B4 coefficient
  READ_Coeff_SF4 &= 0x0007FFFF;   // Bitmask to extract only SF4 coefficient

  ARDUINO_OUTPUT.println(READ_Coeff_B0);   // Write on the USB B0 value
  ARDUINO_OUTPUT.println(READ_Coeff_SF0);  // Write on the USB SF0 value
  ARDUINO_OUTPUT.println(READ_Coeff_TMID); // Write on the USB TMID value
  ARDUINO_OUTPUT.println(READ_Coeff_B1);   // Write on the USB B1 value
  ARDUINO_OUTPUT.println(READ_Coeff_SF1);  // Write on the USB SF1 value
  ARDUINO_OUTPUT.println(READ_Coeff_B2);   // Write on the USB B2 value
  ARDUINO_OUTPUT.println(READ_Coeff_SF2);  // Write on the USB SF2 value
  ARDUINO_OUTPUT.println(READ_Coeff_B3);   // Write on the USB B3 value
  ARDUINO_OUTPUT.println(READ_Coeff_SF3);  // Write on the USB SF3 value
  ARDUINO_OUTPUT.println(READ_Coeff_B4);   // Write on the USB B4 value
  ARDUINO_OUTPUT.println(READ_Coeff_SF4);  // Write on the USB SF4 value
}

/////////////////////////////////////////////////////////////
//                                                         //
//         CHECK COMMAND MTP SLOT VALUE (MTPSLOTNB)        //
//                                                         //
/////////////////////////////////////////////////////////////

unsigned long sensorMTPSlotNumber_2() {
  unsigned long numberslots = EVB.ReadMTP(0x3D);  // Read MTP at address 0x3D
  numberslots = (numberslots & 0x00001F00) >> 8;  // Bitmask to determinate numberslot value
  return (numberslots);                           // Return numberslot value
}



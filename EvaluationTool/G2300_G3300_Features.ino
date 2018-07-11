/****************************************************************************

                FIRMWARE 2.1 for ARDUINO M0          //////////  //
                   EVB 2.0, 2.1 and 3.0              //      //  //
                  TRONIC'S MICROSYSTEMS              //  //  //  //
               http://www.tronicsgroup.com/          //  //  //  //
               This Firmware is optimised            //  //      //
                 for Evaluation Tool 2.1             //  //////////

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
   @file G2300_G3300_Features.ino
   @author Loïc Blanchard (loic.blanchard@tronicsgroup.com)
   @date 19 Sept 2017
   @version : 2.1
   @brief File containing GYPRO® 2300 and 3300 features for EvalutationTool_2_1 file.
   @see https://github.com/TronicsMicrosystems/Firmware-2.1
*/

/////////////////////////////////////////////////////////////
//                                                         //
//                      MAIN PROGRAM 1                     //
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
//            8 : sensorReadFCLK                           //
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
//                                                         //
/////////////////////////////////////////////////////////////

void mainLoop1() {
  switch (serialRead()) {
    /* RDGO + RDTO + ST : Return Rate, Temperature and Self Test values */
    case 1 : {
        unsigned int _DataRate = serialRead();  // Wait for an incoming Data Rate value (typical 0 to 2000)
        unsigned int _TimeAcq = serialRead();   // Wait for an incoming Acquisition Time value in seconds (0 to 4294967295)
        sensorOutputRead(_DataRate, _TimeAcq);  // Run RDGO_RDTO Function with 2 last arguments
        break;
      }

    /* GOUT SEL : Select Raw / Temperature Compensated gyro output (1 = Temperature Compensated ; 2 = Raw) */
    case 2: {
        unsigned int _GoutSel =  serialRead();  // Declare _GoutSel value

        if (_GoutSel == 1) {        // Case 1 = Temperature Compensated
          sensorGoutSel0_1();       // Set gyro output sel at 0
        }
        else if (_GoutSel == 2) {   // Case 2 = Raw
          sensorGoutSel1_1();       // Set gyro output sel at 1
        }
        break;
      }

    /* TEMPERATURE COMPENSATION */
    case 3 : {
        unsigned int _SF2 = serialRead();    // Declare SF2 received
        unsigned int _B2 = serialRead();     // Declare B2 received
        unsigned long _B1 = serialRead();    // Declare B1 received
        unsigned long _B0 = serialRead();    // Declare B0 received
        unsigned long _SF1 = serialRead();   // Declare SF1 received
        unsigned long _SF0 = serialRead();   // Declare SF0 received
        unsigned long _TMID = serialRead();  // Declare TMID received

        sensorWriteTempCompensation_1(_SF2, _B2, _B1, _B0, _SF1, _SF0, _TMID); // Write all the compensation coefficients into the System Register
        break;
      }

    /* READ TEMPERATURE COMPENSATION COEFF */
    case 4 :
      sensorReadTempCompensation_1(); // Read and display temperature compensation coefficients
      break;


    /* TOUT SEL : Select Raw / Calibrated temperature output (1 = Raw ; 2 = Calibrated) */
    case 5 : {
        unsigned int _ToutSel = serialRead();  // Declare _ToutSel value

        if (_ToutSel == 1) {        // Case 1 = Raw
          sensorToutSel0_1();       // Set temperature output sel at 0 (Raw Output)
        }
        else if (_ToutSel == 2) {   // Case 2 = Calibrated
          sensorToutSel1_1();       // Set temperature output sel at 0 (Calibrated Output)
        }
        break;
      }

    /*  TEMPERATURE CALIBRATION : Write temperature sensor calibration coefficients in the system register */
    case 6 : {
        unsigned int _Gain = serialRead();            // Declare Gain received
        unsigned int _Offset = serialRead();          // Declare Offset received
        sensorWriteTempCalibration_1(_Gain, _Offset); // Write Gain and Offset into the System Register
        break;                                        // End of this case
      }


    /* READ TEMPERATURE CALIBRATION COEFF */
    case 7 :
      sensorReadTempCalibration_1();  // Read and display temperature sensor calibration coefficients
      break;


    /* READ DRIVE FREQUENCY */
    case 8 : {
        float sensorFCLK = EVB.ReadFCLK(1000);   // Read Frequency
        long Sensor_Type_Fclk = sensorType();  // Check Sensor Type (GYPRO 2300 or 3300) (sensorType function)

        if (Sensor_Type_Fclk == 1) {           // Case Gypro 3300 -> Read the operating frequency of the ASIC
          sensorFCLK = (sensorFCLK * 4) / 40;  // Divide by 40 and multiply by 4 due to oversampling
        }

        else {                                 // Case Gypro 2300 -> Read the operating frequency of the ASIC
          sensorFCLK = (sensorFCLK * 4) / 122; // Divide by 122 and multiply by 4 due to oversampling
        }

        ArduinoOutput.println(sensorFCLK);  // Display Drive Frequency
        break;
      }

    /* HARDWARE SELF TEST : Test GYPRO ST Pin */
    case 9 : {
        int _HardwareST = digitalRead(ST_Pin);    // Test Self Test analog with Arduino ADC on the ST pin
        ArduinoOutput.println(_HardwareST);       // Display ST Analog test result (0 = Failed, 1 = Passed)
        break;
      }

    /* CHECKING CONFIGURATION : STANDARD (Config = 0) OR LOW DELAY (Config = 1) */
    case 10:
      sensorStartupTime();     // Run RDGO_RDTO Function with 2 last arguments
      break;

    /* CHECKING MTP SLOTS STATUS */
    case 11 : {
        unsigned int MTP_Slot_Value = sensorMTPSlotNumber_1(); // Check MTP Slot value
        ArduinoOutput.println(MTP_Slot_Value);                 // Display MTP Slot value
        break;
      }

    /* PROGRAMMING THE COMPENSATION COEFFICIENTS IN THE OTP */
    case 12 : {
        unsigned int _SF2_MTP = serialRead();    // Declare SF2 received
        unsigned int _B2_MTP = serialRead();     // Declare B2 received
        unsigned long _B1_MTP = serialRead();    // Declare B1 received
        unsigned long _B0_MTP = serialRead();    // Declare B0 received
        unsigned long _SF1_MTP = serialRead();   // Declare SF1 received
        unsigned long _SF0_MTP = serialRead();   // Declare SF0 received
        unsigned long _TMID_MTP = serialRead();  // Declare TMID received

        delay(10);

        // STEP 1 : Write the coefficients in the System Register (sensorTempCompensation function)
        sensorWriteTempCompensation_1(_SF2_MTP, _B2_MTP, _B1_MTP, _B0_MTP, _SF1_MTP, _SF0_MTP, _TMID_MTP);
        delay(100);

        // STEP 2 : Program the coefficients one by one in the OTP (PROG_OTP) with 100 ms delay between each
        EVB.ProgMTP(0x2E);  // Programm SF2 and B2
        delay(100);         // Wait 100 ms
        EVB.ProgMTP(0x2F);  // Programm B1
        delay(100);         // Wait 100 ms
        EVB.ProgMTP(0x30);  // Programm B0
        delay(100);         // Wait 100 ms
        EVB.ProgMTP(0x31);  // Programm SF1
        delay(100);         // Wait 100 ms
        EVB.ProgMTP(0x32);  // Programm SF0
        delay(100);         // Wait 100 ms
        EVB.ProgMTP(0x33);  // Programm TMID
        delay(100);         // Wait 100 ms
        break;              // End of this case
      }

    /* PROGRAMMING THE CALIBRATION COEFFICIENTS IN THE OTP */
    case 13: {
        unsigned int _Gain_MTP = serialRead();         // Declare Gain received
        unsigned int _Offset_MTP = serialRead();       // Declare Offset received

        // STEP 1 : Write the coefficients in the System Register (sensorTempCalibration function)
        sensorWriteTempCalibration_1(_Gain_MTP, _Offset_MTP);
        delay(100);         // Wait 100 ms

        // STEP 2 : Program the coefficients in the OTP (PROG_OTP function)
        EVB.ProgMTP(0x04);
        delay(100);         // Wait 100 ms

        break;
      }

    /* UPDATE MTP SLOTS STATUS : Update new MTP slot value and copy new coefficients into the System Register (avoid restart the GYPRO) */
    case 14 : {
        // STEP 1a : Read data from register at Address 0x02 of the MTP and current MTP slots status
        unsigned long MTPRegister_02 = EVB.ReadMTP(0x02);           // Read complete register in the MTP at address 0x02 (READ_MTP)
        unsigned int Current_MTPSLOTNB = sensorMTPSlotNumber_1();   // Read current MTP slots Value

        // STEP 1b : Calculate the new MTP slots status (ex : 0b111 -> 0b1111)
        unsigned long New_MTPSLOTNB = Current_MTPSLOTNB << 1;
        New_MTPSLOTNB += 1;
        New_MTPSLOTNB = New_MTPSLOTNB << 8;

        // STEP 1c : Bitmask between the complete register content (address 0x02) and the new MTP slots status previously calculated
        MTPRegister_02 &= 0xFFFF00FF;
        MTPRegister_02 |= New_MTPSLOTNB; // Updated register with the new MTP slots Value

        // STEP 2 : Write the new register updated at address in the System Register 0x02
        EVB.WriteSR(MTPRegister_02, 0x02);  // WSYST function
        delay(100);   // Delay of 100 ms

        // STEP 3 : Program the modified register in MTP at address 0x02
        EVB.ProgMTP(0x02);  // PROG_OTP function
        delay(100);         // Delay of 100 ms

        // STEP 4 : Copy the data from address 0x02 in the OTP into the corresponding address in system register
        EVB.CopyMTP(0x02);  // COPY_OTP function
        delay(100);         // Delay of 100 ms

        // STEP 5 : Copy the coefficients from the OTP to the System Register
        EVB.CopyMTP(0x04);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x2E);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x2F);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x30);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x31);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x32);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        EVB.CopyMTP(0x33);  // COPY_OTP function
        delay(100);         // Delay of 100 ms
        break;
      }

    /* GYPRO UID : Return GYPRO UID */
    case 15 : {
        String Sensor_UID = sensorUID_1();  // Check UID of the Sensor
        ArduinoOutput.println(Sensor_UID);  // Send it to USB serial
        break;
      }

    /* CHECKING PRODUCT NAME */
    case 16: {
        String ProductName = sensorProductName_1();  // Declare and read Gypro type (GYPRO_TYPE function)
        ArduinoOutput.println(ProductName);
        break;
      }

    /* CHECK EVB VERSION */
    case 17:
      ArduinoOutput.println(EVB_Version);
      break;

    /* READ ALL SYSTEM REGISTER */
    case 18: {
        unsigned long SensorAllSystemRegister[60];      // Declare Array of 82 lines for all the System Register
        for (int i = 0; i <= 60; i++) {                 // For loop to complete the array with one register by line
          SensorAllSystemRegister[i] = EVB.ReadSR(i);         // Copy content of the system register (address i) in the i line of SYSTEM_REGISTER array
          ArduinoOutput.println(SensorAllSystemRegister[i]);  // Display i system register value
        }
        break;
      }

    /* WRITE ALL SYSTEM REGISTER */
    case 19: {
        unsigned long _SystemRegister_i = 0;

        for (int i = 0; i <= 60; i++) {        // For loop to complete the array with one register by line
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
        ArduinoOutput.println(SystemRegisterData);
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
        ArduinoOutput.println(MTPData);
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
//              READ THE UID (SERIAL NUMBER)               //
//                   OF THE GYPRO MEMS                     //
//                                                         //
/////////////////////////////////////////////////////////////

String sensorUID_1() {
  unsigned long SystemRegister_00 = EVB.ReadSR(0x00);     // Read System Register at Address 0x00
  SystemRegister_00 &= 0x7FFFFFFF;                        // Bitmask to set bit 31 at low-level

  unsigned long Sensor_a = SystemRegister_00 & 0x7C000000; // Bitmask and bitshift series to determine the Sensor UID
  Sensor_a = Sensor_a >> 26;

  unsigned long Sensor_b = SystemRegister_00 & 0x03F00000;
  Sensor_b = Sensor_b >> 20;

  unsigned long Sensor_c = SystemRegister_00 & 0x000FC000;
  Sensor_c = Sensor_c >> 14;

  unsigned long Sensor_d = SystemRegister_00 & 0x00003C00;
  Sensor_d = Sensor_d >> 10;

  unsigned long Sensor_e = SystemRegister_00 & 0x000003E0;
  Sensor_e = Sensor_e >> 5;
  Sensor_e += 64;

  unsigned long Sensor_f = SystemRegister_00 & 0x0000001F;

  String Sensor_UID_full = "";

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

  return (Sensor_UID_full);                 // Return UID of the GYPRO (The same written on the sensor)
}


/////////////////////////////////////////////////////////////
//                                                         //
//                READ THE TYPE OF THE GYPRO               //
//                     (2300 OR 3300)                      //
//                                                         //
/////////////////////////////////////////////////////////////

unsigned long sensorType() {
  unsigned long SystemRegister_0A = EVB.ReadSR(0x0A); // Read System Register at Address 0x0A
  SystemRegister_0A &= 0xFC000000;                    // Bitmask to keep bit 26 to 31
  SystemRegister_0A = SystemRegister_0A >> 26;        // Bitshift

  unsigned long Sensor_Config = 0;                    // Declare config variable
  if (SystemRegister_0A == 19) {                      // If the register is equal to 19 => Gypro 2300, else if equal to 60 => Gypro 3300
    Sensor_Config = 1;
  }
  return (Sensor_Config);                              // Return Sensor_config (0 = 2300 ; 1 = 3300)
}

/////////////////////////////////////////////////////////////
//                                                         //
//            READ THE CONFIGURATION OF THE GYPRO          //
//                 (STANDARD OR LOW DELAY)                 //
//                                                         //
/////////////////////////////////////////////////////////////

unsigned long sensorConfig() {
  unsigned long SystemRegister_2B = EVB.ReadSR(0x2B); // Read System Register at Address 0x2B
  SystemRegister_2B &= 0x00400000;                    // Bitmask to conserve only bit 22
  SystemRegister_2B = SystemRegister_2B >> 22;        // Bitshift
  return (SystemRegister_2B);                         // return the configuration (0 = Standard ; 1 = Low-Delay)
}

/////////////////////////////////////////////////////////////
//                                                         //
//              READ THE UID (SERIAL NUMBER)               //
//                   OF THE GYPRO MEMS                     //
//                                                         //
/////////////////////////////////////////////////////////////

String sensorProductName_1() {
  String Product_Name = "";

  unsigned long Sensor_Type = sensorType();
  if (Sensor_Type == 0) {
    unsigned long Sensor_Config = sensorConfig();
    if (Sensor_Config == 0) {
      Product_Name = "GYPRO2300";
    }
    else {
      Product_Name = "GYPRO2300LD";
    }
  }
  else {
    Product_Name = "GYPRO3300";
  }

  return (Product_Name);
}

/////////////////////////////////////////////////////////////
//                                                         //
//               SELECTION OF THE GYRO OUTPUT              //
//      RAW / TEMPERATURE COMPENSATED DATA (GOUT_SEL)      //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorGoutSel0_1() {                       // Set GOUT SEL at 0 (Temperature Compensated)
  unsigned long SR_GOUT_SEL = EVB.ReadSR(0x02); // Read System Register at address 0x02
  SR_GOUT_SEL &= 0xF7FFFFFF;                    // Bitmask to change only bit 27 (low-level)
  EVB.WriteSR(SR_GOUT_SEL, 0x02);               // Write the modified register
}

void sensorGoutSel1_1() {                       // Set GOUT SEL at 1 (Raw)
  unsigned long SR_GOUT_SEL = EVB.ReadSR(0x02); // Read System Register at Address 0x02
  SR_GOUT_SEL |= 0x08000000;                    // Bitmask to change only bit 27 (high-level)
  EVB.WriteSR(SR_GOUT_SEL, 0x02);               // Write the modified register
}

/////////////////////////////////////////////////////////////
//                                                         //
//        SELECTION OF THE TEMPERATURE SENSOR OUTPUT       //
//              RAW / CALIBRATED DATA (TOUT_SEL)           //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorToutSel0_1() {                       // Set TOUT SEL at 0 (Raw)
  unsigned long SR_TOUT_SEL = EVB.ReadSR(0x09); // Read System Register at address 0x09
  SR_TOUT_SEL &= 0xFFFFFFFB;                    // Bitmask to change only bit 2 (low-level)
  EVB.WriteSR(SR_TOUT_SEL, 0x09);               // Write the modified register
}

void sensorToutSel1_1() {                       // Set TOUT SEL at 1 (Calibrated)
  unsigned long SR_TOUT_SEL = EVB.ReadSR(0x09); // Read System Register at address 0x09
  SR_TOUT_SEL |= 0x00000004;                    // Bitmask to change only bit 2 (high-level)
  EVB.WriteSR(SR_TOUT_SEL, 0x09);               // Write the modified register
}

/////////////////////////////////////////////////////////////
//                                                         //
//                  WRITE COMMAND OF THE                   //
//              TEMPERATURE SENSOR CALIBRATION             //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorWriteTempCalibration_1(unsigned int Temp_Gain, unsigned int Temp_Offset) { // Twor Arguments : The desired Gain + The desired Offset
  Temp_Gain = Temp_Gain << 2;                   // 2 Bitshift
  unsigned long Temp_Gain_plus_Offset = 0;      // Create long 32-bits data XXXX OOOO OOOO OOOO  XXGG GGGG GGGG GGXX (O = Offset ; G = Gain)
  Temp_Gain_plus_Offset += Temp_Offset << 16;   // Write Offset at correct place
  Temp_Gain_plus_Offset += Temp_Gain;           // Write Gain at correct place

  unsigned long SR_Gain_Offset = EVB.ReadSR(0x04);  // Read System register at address
  SR_Gain_Offset &= 0xF000C003;                     // 1111 0000 0000 0000 1100 0000 0000 0011 = 0xF000C003 (Offset and Gain bits place at 0)
  SR_Gain_Offset |= Temp_Gain_plus_Offset;          // Bitmaks to change only bits where Gain and Offset should be written
  EVB.WriteSR(SR_Gain_Offset, 0x04);                // Write the modified register
}

/////////////////////////////////////////////////////////////
//                                                         //
//             READ COMMAND OF THE TEMPERATURE             //
//             SENSOR CALIBRATION COEFFICIENTS             //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorReadTempCalibration_1() {
  unsigned long SR_Gain_Offset = EVB.ReadSR(0x04);  // Read System Register at address 0x04

  unsigned long Read_Gain = SR_Gain_Offset;
  Read_Gain &= 0x00003FFC;            // Bitmask and bitshift to determine Gain value
  Read_Gain = Read_Gain >> 2;
  ArduinoOutput.println(Read_Gain);   // Write the Gain on the USB

  unsigned long Read_Offset = SR_Gain_Offset;
  Read_Offset &= 0x0FFF0000;          // Bitmask and bitshift to determine Offset value
  Read_Offset = Read_Offset >> 16;
  ArduinoOutput.println(Read_Offset); // Write the Offset on the USB
}

/////////////////////////////////////////////////////////////
//                                                         //
//             WRITE COMMAND OF THE TEMPERATURE            //
//                COMPENSATION COEFFICIENTS                //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorWriteTempCompensation_1(unsigned int Coeff_SF2, unsigned int Coeff_B2, unsigned long Coeff_B1, unsigned long Coeff_B0, unsigned long Coeff_SF1, unsigned long Coeff_SF0, unsigned long Coeff_TMID) {
  /* WRSF2B2 Function */
  unsigned long Coeff_SF2_plus_B2 = 0;
  Coeff_SF2_plus_B2 += Coeff_SF2 << 16;   // SF2 and B2 coefficients on one byte
  Coeff_SF2_plus_B2 += Coeff_B2;
  EVB.WriteSR(Coeff_SF2_plus_B2, 0x2E);   // Write SF2 and B2 coefficients in the System Register at Address 0x2E

  /* WRB1 Function */
  Coeff_B1 &= 0x3FFFFFFF;         // Bitmask to have only 30 bits data
  EVB.WriteSR(Coeff_B1, 0x2F);    // Write B1 coefficient in the System Register at Address 0x2F

  /* WRB0 Function */
  Coeff_B0 &= 0x3FFFFFFF;         // Bitmask to have only 30 bits data
  EVB.WriteSR(Coeff_B0, 0x30);    // Write B0 coefficient in the System Register at Address 0x30

  /* WRSF1 Function */
  Coeff_SF1 &= 0x3FFFFFFF;        // Bitmask to have only 30 bits data
  EVB.WriteSR(Coeff_SF1, 0x31);   // Write SF1 coefficient in the System Register at Address 0x31

  /* WRSF0 Function */
  Coeff_SF0 &= 0x3FFFFFFF;        // Bitmask to have only 30 bits data
  EVB.WriteSR(Coeff_SF0, 0x32);   // Write SF0 coefficient in the System Register at Address 0x32

  /* WRTMID Function */
  Coeff_TMID &= 0x000FFFFF;       // Bitmask to have only 20 bits data
  EVB.WriteSR(Coeff_TMID, 0x33);  // Write TMID coefficient in the System Register at Address 0x33
}

/////////////////////////////////////////////////////////////
//                                                         //
//             READ COMMAND OF THE TEMPERATURE             //
//                COMPENSATION COEFFICIENTS                //
//                                                         //
/////////////////////////////////////////////////////////////

void sensorReadTempCompensation_1(void) {
  unsigned long READ_SF2_B2 = EVB.ReadSR(0x2E);  // Read SF2 and B2 coefficients in the System Register at address 0x2E
  unsigned long READ_B1 = EVB.ReadSR(0x2F);      // Read B1 coefficient register in the System Register at address 0x2F
  unsigned long READ_B0 = EVB.ReadSR(0x30);      // Read B0 coefficient register in the System Register at address 0x30
  unsigned long READ_SF1 = EVB.ReadSR(0x31);     // Read SF1 coefficient register in the System Register at address 0x31
  unsigned long READ_SF0 = EVB.ReadSR(0x32);     // Read SF0 coefficient register in the System Register at address 0x32
  unsigned long READ_TMID = EVB.ReadSR(0x33);    // Read TMID coefficients register in the System Register at address 0x33
  unsigned int READ_SF2 = (READ_SF2_B2 & 0xFFFF0000) >> 16; // Bitmask/Bitshift to collect only SF2 coefficient
  unsigned int READ_B2 = READ_SF2_B2 & 0x0000FFFF; // Bitmask to collect B2

  READ_B1 &= 0x3FFFFFFF;    // Bitmask to extract only B1 coefficient
  READ_B0 &= 0x3FFFFFFF;    // Bitmask to extract only B0 coefficient
  READ_SF1 &= 0x3FFFFFFF;   // Bitmask to extract only SF1 coefficient
  READ_SF0 &= 0x3FFFFFFF;   // Bitmask to extract only SF0 coefficient
  READ_TMID &= 0x000FFFFF;  // Bitmask to extract only TMID coefficient

  ArduinoOutput.println(READ_SF2);  // Write on the USB SF2 value
  ArduinoOutput.println(READ_B2);   // Write on the USB B2 value
  ArduinoOutput.println(READ_B1);   // Write on the USB B1 value
  ArduinoOutput.println(READ_B0);   // Write on the USB B0 value
  ArduinoOutput.println(READ_SF1);  // Write on the USB SF1 value
  ArduinoOutput.println(READ_SF0);  // Write on the USB SF0 value
  ArduinoOutput.println(READ_TMID); // Write on the USB TMID value
}

/////////////////////////////////////////////////////////////
//                                                         //
//         CHECK COMMAND MTP SLOT VALUE (MTPSLOTNB)        //
//                                                         //
/////////////////////////////////////////////////////////////

unsigned long sensorMTPSlotNumber_1() {
  unsigned long numberslots = EVB.ReadMTP(0x02);  // Read OTP at address 0x02
  numberslots = (numberslots & 0x0000FF00) >> 8;  // Bitmask to determinate numberslot value
  return (numberslots);                           // Return numberslot value
}



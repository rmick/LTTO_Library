
////---------------------------------------------------------------------------------------------------------
//    Add the required libraries

#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>                //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include "LTTO.h"                           //  This is the LTTO library

////---------------------------------------------------------------------------------------------------------
//    Create an instance of the LTTO library object to use in your code.
//    You can add up to 4 different instances into one sketch on a UNO, more on a MEGA

LTTO ltto1(13, 11);                          //  This creates an Object called ltto1 using Pin 13 to Tx and Pin 11 to Rx
//LTTO ltto2(10 ,9);                          //  This creates an Object called ltto2 using Pin 10 to Tx and Pin  9 to Rx
//LTTO ltto3( 8, 7);
//LTTO lttoLeonardo(9, 0);

long int beaconCount1 = 1;
byte     tagCount1    = 0;
long int beaconCount2 = 1;
byte     tagCount2    = 0;
long int beaconCount3 = 1;
byte     tagCount3    = 0;

////---------------------------------------------------------------------------------------------------------
//    void setup()

void setup() {
    // put your setup code here, to run once:

    Serial.begin(250000);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning
}

////---------------------------------------------------------------------------------------------------------
//    void loop()

void loop() {
    // put your main code here, to run repeatedly:
    
    if (ltto1.DecodeIR() )
    {  
        switch (ltto1.decodedIRmessage.type)
        {
            case 'T':
            //ProcessRxTag(decodedIRmessage.TeamID, decodedIRmessage.PlayerID, decodedIRmessage.ShotStrength);
              Serial.print(F("\nTag1 - "));
              Serial.print(ltto1.decodedIRmessage.rawDataPacket, BIN);
              tagCount1++;
              Serial.print(F(" : "));
              Serial.print(tagCount1);
              break;
            
            case 'B':
              Serial.print(F("\n\tBeacon1 - "));
              Serial.print(ltto1.decodedIRmessage.rawDataPacket, BIN);
              Serial.print(F(" : "));
              Serial.print(beaconCount1);
              beaconCount1++;
              break;

            case 'P':
              Serial.print(F("\n\t\t------------------\n\t\tPacket1 - 0x"));
              Serial.print(ltto1.decodedIRmessage.PacketByte, HEX);
              //ProcessRxPacket();
              break;

            case 'D':
              Serial.print(F("\n\t\t\tData1 - 0x"));
              Serial.print(ltto1.decodedIRmessage.DataByte, HEX);
              //ProcessRxDataByte();
              break;

            case 'C':
              Serial.print(F("\n\t\tCheckSum1 - 0x"));
              Serial.print(ltto1.decodedIRmessage.rawDataPacket & B11111111, HEX); 
              //ProcessRxCheckSum();
              break;   
        }
        ltto1.decodedIRmessage.newMessage = false;
 
    }


//    if (ltto2.DecodeIR() )
//    {  
//        switch (ltto2.decodedIRmessage.type)
//        {
//            case 'T':
//            //ProcessRxTag(decodedIRmessage.TeamID, decodedIRmessage.PlayerID, decodedIRmessage.ShotStrength);
//              Serial.print(F("\n\Tag2 - "));
//              Serial.print(ltto1.decodedIRmessage.rawDataPacket, BIN);
//              tagCount2++;
//              Serial.print(F(" : "));
//              Serial.print(tagCount2);
//              break;
//            
//            case 'B':
//              Serial.print(F("\n\tBeacon2 - "));
//              Serial.print(ltto2.decodedIRmessage.rawDataPacket, BIN);
//              Serial.print(F(" : "));
//              Serial.print(beaconCount2);
//              beaconCount2++;
//              break;
//
//            case 'P':
//              Serial.print(F("\n\t\t------------------\n\t\tPacket2 - 0x"));
//              Serial.print(ltto2.decodedIRmessage.PacketByte, HEX);
//              //ProcessRxPacket();
//              break;
//
//            case 'D':
//              Serial.print(F("\n\t\t\tData2 - 0x"));
//              Serial.print(ltto2.decodedIRmessage.DataByte, HEX);
//              //ProcessRxDataByte();
//              break;
//
//            case 'C':
//              Serial.print(F("\n\t\tCheckSum2 - 0x"));
//              Serial.print(ltto2.decodedIRmessage.rawDataPacket & B11111111, HEX); 
//              //ProcessRxCheckSum();
//              break;   
//        }
//        ltto2.decodedIRmessage.newMessage = false;
// 
//    }
//
//
//
//
//    if (ltto3.DecodeIR() )
//    {  
//        switch (ltto3.decodedIRmessage.type)
//        {
//            case 'T':
//            //ProcessRxTag(decodedIRmessage.TeamID, decodedIRmessage.PlayerID, decodedIRmessage.ShotStrength);
//              Serial.print(F("\n\Tag3 - "));
//              Serial.print(ltto1.decodedIRmessage.rawDataPacket, BIN);
//              tagCount3++;
//              Serial.print(F(" : "));
//              Serial.print(tagCount3);
//              break;
//            
//            case 'B':
//              Serial.print(F("\n\tBeacon3 - "));
//              Serial.print(ltto3.decodedIRmessage.rawDataPacket, BIN);
//              Serial.print(F(" : "));
//              Serial.print(beaconCount3);
//              beaconCount3++;
//              break;
//
//            case 'P':
//              Serial.print(F("\n\t\t------------------\n\t\tPacket3 - 0x"));
//              Serial.print(ltto3.decodedIRmessage.PacketByte, HEX);
//              //ProcessRxPacket();
//              break;
//
//            case 'D':
//              Serial.print(F("\n\t\t\tData3 - 0x"));
//              Serial.print(ltto3.decodedIRmessage.DataByte, HEX);
//              //ProcessRxDataByte();
//              break;
//
//            case 'C':
//              Serial.print(F("\n\t\tCheckSum3 - 0x"));
//              Serial.print(ltto3.decodedIRmessage.rawDataPacket & B11111111, HEX); 
//              //ProcessRxCheckSum();
//              break;   
//        }
//        ltto3.decodedIRmessage.newMessage = false;
// 
//    }


    if (Serial.available() !=0)
    {
        char keyIn = Serial.read();
        switch (keyIn)
        {
            case 'e':
                ltto1.GetErrorCounts();
                break;
        }
    }        






    return;


    
  //  ltto1.SendBeacon(IFF_BEACON,1,4);                   //  Send an IFF (Identify Friend or Foe) Beacon with the TeamID = 1 and PlayerID = 4
    
  //  ltto1.SendBeacon(HIT_RESPONSE_BEACON,4,2);          //  Send a Beacon saying you werre just hit and your TeamID = 2 and PlayerID = 2
    
    //ltto1.SendZoneBeacon(MEDIC_BEACON,?,?);           //  Send a MEDIC beacon to un-neutraise a player on ANY team
    
  //  ltto1.SendTag(0,8,1);                               //  Send a TAG from Team 0, Player 8, with a Mega Strength of 1 (Normal)
    
  //  delay (2500);
}

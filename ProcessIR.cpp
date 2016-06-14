
#include <Arduino.h>
#include "LTTO.h"

///---------------------------------------------------------------------------------------------------------
//    Private : ProcessRxTag

void LTTO::ProcessTag()
{
    // Find TeamID of tagger
    decodedIRmessage.teamID = (decodedIRmessage.rawDataPacket & B01100000);             // TeamID = 1 thru 3  (0 = NoTeams)
    decodedIRmessage.teamID = decodedIRmessage.teamID >> 5;
    // Find PlayerID
    decodedIRmessage.playerID = (decodedIRmessage.rawDataPacket & B00011100);         // PlayerID = 1 thru 8
    decodedIRmessage.playerID = (decodedIRmessage.playerID >> 2) + 1;
    // Find tag Power
    decodedIRmessage.shotStrength = (decodedIRmessage.rawDataPacket & B00000011)+1;   // Tag strength = 1 to 4
}

void LTTO::ProcessBeacon()
{
    // Find TeamID of Beacon
    decodedIRmessage.teamID = (decodedIRmessage.rawDataPacket & B11000);                // TeamID = 1 thru 3  (0 = NoTeams)
    decodedIRmessage.teamID = decodedIRmessage.teamID >> 3;
    // Check the TagBeacon flag
    decodedIRmessage.tagReceivedBeacon = (decodedIRmessage.rawDataPacket & B00100);   // Sets flag for whether the beacon was sent because of receiving a tag
    //decodedIRmessage.tagReceivedBeacon = (decodedIRmessage.tagReceivedBeacon >> 2);
    // Find tag Power
    decodedIRmessage.shotStrength = (decodedIRmessage.rawDataPacket & B00011)+1;      // Tag strength = 1 to 4  - MUST be Zero if TagBeacon flag above is false

    //Serial.print(decodedIRmessage.tagReceivedBeacon);
    if      (decodedIRmessage.tagReceivedBeacon == 1)                                       decodedIRmessage.beaconType = HIT_RESPONSE_BEACON;
    else if (decodedIRmessage.tagReceivedBeacon == 0 && decodedIRmessage.shotStrength == 1) decodedIRmessage.beaconType = IFF_BEACON;
    else if (decodedIRmessage.tagReceivedBeacon == 0 && decodedIRmessage.shotStrength == 2) decodedIRmessage.beaconType = ZONE_BEACON;
    else if (decodedIRmessage.tagReceivedBeacon == 0 && decodedIRmessage.shotStrength == 3) decodedIRmessage.beaconType = CONTESTED_ZONE;
    else if (decodedIRmessage.tagReceivedBeacon == 0 && decodedIRmessage.shotStrength == 4) decodedIRmessage.beaconType = SUPPLY_ZONE;
}

void LTTO::ProcessPacket()
{
    _byteCount = 0;
    decodedIRmessage.packetByte = decodedIRmessage.rawDataPacket & B11111111;
    _checkSumRx = 0;
    decodedIRmessage.checkSumOK = false;
    decodedIRmessage.checkSumCalc = 0;
}

void LTTO::ProcessDataByte()
{
    _byteCount++;
    decodedIRmessage.dataByte = decodedIRmessage.rawDataPacket & B11111111;
    decodedIRmessage.checkSumCalc = decodedIRmessage.checkSumCalc + decodedIRmessage.rawDataPacket;
}

void LTTO::ProcessCheckSum()
{
    decodedIRmessage.checkSumRxByte = decodedIRmessage.rawDataPacket & B11111111;
    if (decodedIRmessage.checkSumCalc == decodedIRmessage.checkSumRxByte) decodedIRmessage.checkSumOK = true;
}

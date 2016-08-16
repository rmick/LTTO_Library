
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
    decodedIRmessage.byteCount = 0;
    decodedIRmessage.packetByte = decodedIRmessage.rawDataPacket & B11111111;
    decodedIRmessage.checkSumOK = false;
    decodedIRmessage.checkSumCalc = decodedIRmessage.packetByte;

    switch (decodedIRmessage.packetByte)
    {
        case ANNOUNCE_CUSTOM_GAME:
            decodedIRmessage.packetName = "ANNOUNCE_CUSTOM_GAME";
            break;
        case ANNOUNCE_CUSTOM_GAME_2_TEAMS:
            decodedIRmessage.packetName = "ANNOUNCE_CUSTOM_GAME_2_TEAMS";
            break;
        case ANNOUNCE_CUSTOM_GAME_3_TEAMS:
            decodedIRmessage.packetName = "ANNOUNCE_CUSTOM_GAME_3_TEAMS";
            break;
        case ANNOUNCE_HIDE_AND_SEEK:
            decodedIRmessage.packetName = "ANNOUNCE_HIDE_AND_SEEK";
            break;
        case ANNOUNCE_HUNT_THE_PREY:
            decodedIRmessage.packetName = "ANNOUNCE_HUNT_THE_PREY";
            break;
        case ANOUNCE_KINGS_2_TEAMS:
            decodedIRmessage.packetName = "ANOUNCE_KINGS_2_TEAMS";
            break;
        case ANOUNCE_KINGS_3_TEAMS:
            decodedIRmessage.packetName = "ANOUNCE_KINGS_3_TEAMS";
            break;
        case ANNOUNCE_OWN_THE_ZONE:
            decodedIRmessage.packetName = "ANNOUNCE_OWN_THE_ZONE";
            break;
        case ANNOUNCE_OWN_THE_ZONE_2_TEAMS:
            decodedIRmessage.packetName = "ANNOUNCE_OWN_THE_ZONE_2_TEAMS";
            break;
        case ANNOUNCE_OWN_THE_ZONE_3_TEAMS:
            decodedIRmessage.packetName = "ANNOUNCE_OWN_THE_ZONE_3_TEAMS";
            break;
        case ANNOUNCE_SPECIAL_GAME:
            decodedIRmessage.packetName = "ANNOUNCE_SPECIAL_GAME";
            break;
        case ASSIGN_PLAYER_FAIL:
            decodedIRmessage.packetName = "ASSIGN_PLAYER_FAIL";
            break;
        case REQUEST_JOIN_GAME:
            decodedIRmessage.packetName = "REQUEST_JOIN_GAME";
            break;
        case ACK_PLAYER_ASSIGN:
            decodedIRmessage.packetName = "ACK_PLAYER_ASSIGN";
            break;
        case REQUEST_ASSISTANCE:
            decodedIRmessage.packetName = "REQUEST_ASSISTANCE";
            break;
        case SEND_ASSISTANCE:
            decodedIRmessage.packetName = "SEND_ASSISTANCE";
            break;
        case REQUEST_TAG_REPORT:
            decodedIRmessage.packetName = "REQUEST_TAG_REPORT";
            break;
        case RANK_REPORT:
            decodedIRmessage.packetName = "RANK_REPORT";
            break;
        case TAG_SUMMARY:
            decodedIRmessage.packetName = "TAG_SUMMARY";
            break;
        case TEAM_1_TAG_REPORT:
            decodedIRmessage.packetName = "TEAM_1_TAG_REPORT";
            break;
        case TEAM_2_TAG_REPORT:
            decodedIRmessage.packetName = "TEAM_2_TAG_REPORT";
            break;
        case TEAM_3_TAG_REPORT:
            decodedIRmessage.packetName = "TEAM_3_TAG_REPORT";
            break;
        case SINGLE_TAG_REPORT:
            decodedIRmessage.packetName = "SINGLE_TAG_REPORT";
            break;
        case TEXT_MESSAGE:
            decodedIRmessage.packetName = "TEXT_MESSAGE";
            break;
        case SPECIAL_ATTACK:
            decodedIRmessage.packetName = "SPECIAL_ATTACK";
            break;
    }
}

void LTTO::ProcessDataByte()
{
    decodedIRmessage.byteCount++;
    decodedIRmessage.dataByte = decodedIRmessage.rawDataPacket & B11111111;
    decodedIRmessage.checkSumCalc = decodedIRmessage.checkSumCalc + decodedIRmessage.rawDataPacket;
    //    if (decodedIRmessage.packetName == "REQUEST_JOIN_GAME")   ProcessRequestJoinGameDataByte(decodedIRmessage.byteCount);
    //    if (decodedIRmessage.packetName == "ACK_PLAYER_ASSIGN")   ProcessAckPlayerAssignDataByte(decodedIRmessage.byteCount);
}

void LTTO::ProcessCheckSum()
{
    decodedIRmessage.checkSumRxByte = decodedIRmessage.rawDataPacket & B11111111;
    if (decodedIRmessage.checkSumCalc == decodedIRmessage.checkSumRxByte) decodedIRmessage.checkSumOK = true;
    else                                                                  decodedIRmessage.checkSumOK = false;
}

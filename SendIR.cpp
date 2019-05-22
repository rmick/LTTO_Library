
#include <Arduino.h>
#include "LTTO.h"

///---------------------------------------------------------------------------------------------------------
//    Public : SendIR

void LTTO::sendIR(char type, uint16_t message)
{
    int _msgLength = 0;
    int _interDelay = 25;

    //Send Header
    switch (type)
    {
    // if Type = BEACON       Header is 366, length is 5 bits
    // if Type = TAG          Header is 363, length is 7 bits
    // if Type = DATA         Header is 363, length is 8 bits
    // if Type = PACKET       Header is 363, length is 9 bits, first bit must be 0
    // if Type = CHECKSUM     Header is 363, length is 9 bits, first bit must be 1

    case PACKET:
        _msgLength = 9;
        _interDelay = 25;
        _checkSumCalc = message;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;

    case DATA:
        _msgLength = 8;
        _interDelay = 25;
        _checkSumCalc = _checkSumCalc + message;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;

    case CHECKSUM:
        _msgLength = 9;
        _interDelay = 25;
        message = _checkSumCalc;           // Overwrite the message with the calculated checksum
		    message = message | 256;          //  Set the required 9th MSB bit to 1 to indicate it is a checksum
		    PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;

    case TAG:
        _msgLength = 7;
        _interDelay = 5;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;

    case BEACON:
        _msgLength = 5;
        _interDelay = 25;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(6);
        break;

    case LTAR_BEACON:
        _msgLength = 9;
        _interDelay = 25;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(6);
        break;  
    }

    //Send message
    for (int bitCount = _msgLength-1; bitCount >=0; bitCount--)
    {
        delayMicroseconds (2000);
        PulseIR(bitRead(message, bitCount)+1);        // the +1 is to convert 0/1 data into 1/2mS pulses.
    }

    delay(_interDelay);



	//#ifdef DEBUG
//	Serial.print(F("\nSending IR- "));
//	Serial.print(type);
//	Serial.print(F(": "));
//	printBinary(message, 8);
//	Serial.print(F(" - "));
//	Serial.print(message, HEX);
	// #endif
}


                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///---------------------------------------------------------------------------------------------------------
//    Private : PulseIR


//#define TIMER555

void LTTO::PulseIR(byte _mSec)
{
    unsigned long _pulseStartTime = micros();
    unsigned long _pulseLength = _mSec*1000;
    unsigned long _pulseEndTime = _pulseStartTime + _pulseLength - 24;

   #ifndef TIMER555
    while (_pulseEndTime >micros() )
    {
        digitalWrite(_txPin, HIGH);
        delayMicroseconds(12);
        digitalWrite(_txPin, LOW);
        delayMicroseconds(12);
    }
    #endif

    #ifdef TIMER555
         pinMode(_txPin, OUTPUT);
		 digitalWrite(_txPin, HIGH);		//TODO: Debug for Logic Analyzer
         delayMicroseconds(_pulseLength);
         pinMode(_txPin, INPUT);
    #endif

}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------------------------------------------
//    Public : SendTag - Send a tag in a hosted game. This takes the simple data, translates it into Binary and sends it using the SendIR class

bool LTTO::sendTag(byte teamID, byte playerID, byte tagPower)
{
    uint16_t _fireMessage = 0;

    if (playerID < 1 || playerID > 8) return false;
    if (teamID > 3)                   return false;
    if (tagPower <1  || tagPower > 4) return false;

    //Assemble the fireMessage
    _fireMessage = _fireMessage << 2;
    _fireMessage = teamID;
    _fireMessage = _fireMessage << 3;
    _fireMessage = _fireMessage + (playerID-1);
    _fireMessage = _fireMessage << 2;
    _fireMessage = _fireMessage + (tagPower-1);
    sendIR( TAG, _fireMessage);
    return true;
}
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------------------------------------------
//    Public : SendTag - Send a tag in a non-hosted game. This takes the simple data, translates it into Binary and sends it using the SendIR class

bool LTTO::sendLTAG(byte tagPower)
{
    uint16_t _fireMessage = 0;

    if (tagPower <1  || tagPower > 4) return false;

    _fireMessage = _fireMessage + (tagPower-1);
    sendIR( TAG, _fireMessage);
    return true;
}
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///---------------------------------------------------------------------------------------------------------
//    Public : SendBeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

bool LTTO::sendBeacon(bool tagReceived, byte teamID, byte tagPower)
{
    uint16_t _beaconMessage;

    if (teamID > 3)                   return false;
    if (tagPower <1  || tagPower > 4) return false;
    //Assemble the fireMessage
    _beaconMessage = teamID;
    _beaconMessage = _beaconMessage << 1;
    _beaconMessage = _beaconMessage + tagReceived;
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + (tagPower-1);

    this->sendIR( BEACON, _beaconMessage);
    return true;
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------------------------------------------
//    Public : SendZoneBeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

bool LTTO::sendZoneBeacon(byte zoneType, byte teamID)
{
    uint16_t _beaconMessage;

    if (teamID > 3)   return false;
    //Assemble the fireMessage
    _beaconMessage = teamID;
    _beaconMessage = _beaconMessage << 1;
    _beaconMessage = _beaconMessage + 0;
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + zoneType;                   // 00-Invalid, 01-Reserved, 10-Contested Zone, 11-Supply Zone

    this->sendIR( BEACON, _beaconMessage);
    return true;
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ///---------------------------------------------------------------------------------------------------------
// //    Public : SendLTARbeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

bool LTTO::sendLTARbeacon(bool tagReceived, bool shieldsActive, byte tagsRemaining, byte unKnown, byte teamID)
{
    uint16_t _beaconMessage;

    if (teamID > 3)   return false;
    //Assemble the fireMessage
    _beaconMessage = tagReceived;
    _beaconMessage = _beaconMessage << 1;
    _beaconMessage = _beaconMessage + shieldsActive;
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + tagsRemaining;                    //00-None, 01 - 1 to 25%, 10 - 25 to 50%, 11 - 51 to 100%
    _beaconMessage = _beaconMessage << 3;
    _beaconMessage = _beaconMessage + unKnown;
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + teamID;

    this->sendIR( LTAR_BEACON, _beaconMessage);
    return true;
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

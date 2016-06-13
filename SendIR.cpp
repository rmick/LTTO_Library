
#include <Arduino.h>
#include "LTTO.h"

///---------------------------------------------------------------------------------------------------------
//    Public : SendIR

void LTTO::SendIR(char type, uint16_t message)
{
    int _msgLength = 0;
    int _interDelay = 25;
  
    #ifdef DEBUG
        Serial.print(F("\nSending IR- "));
        Serial.print(type);
        Serial.print(F(": "));
        PrintBinary(message, 10);
    #endif
  
    //Send Header
    switch (type)
    {
    // if Type = B then Beacon,   Header is 366, length is 5 bits
    // if Type = T then Tag,      Header is 363, length is 7 bits
    // if Type = D the data byte, Header is 363, length is 8 bits
    // if Type = P then Packet,   Header is 363, length is 9 bits, first bit must be 0
    // if Type = C then CheckSum, Header is 363, length is 9 bits, first bit must be 1
    
    case 'P':
        _msgLength = 9;
        _interDelay = 25;
        _checkSumCalc = 0;
        _checkSumCalc = _checkSumCalc + message;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;

    case 'D':
        _msgLength = 8;
        _interDelay = 25;
        _checkSumCalc = _checkSumCalc + message;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;

    case 'C':
        _msgLength = 9;
        _interDelay = 25;
        message = _checkSumCalc;           //Overwrite the message with the calculated checksum
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        message = message | 256;          //  Set the required 9th MSB bit to 1 to indicate it is a checksum
        break;
         
    case 'T':
        _msgLength = 7;
        _interDelay = 5;
        PulseIR(3);
        delayMicroseconds (6000);
        PulseIR(3);
        break;
    
    case 'B':
        _msgLength = 5;
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
}


                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///---------------------------------------------------------------------------------------------------------
//    Private : PulseIR

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
         delayMicroseconds(_pulseLength);
         pinMode(_txPin, INPUT);
    #endif
        
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            
                            
///---------------------------------------------------------------------------------------------------------
//    Public : SendLTAG - Send an LTAG (non hosted game) tag. This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendLTAG(byte teamID, byte playerID, byte tagPower)
{
    uint16_t _fireMessage;
      
    //Assemble the fireMessage
    _fireMessage = teamID;
    _fireMessage = _fireMessage << 3;
    _fireMessage = _fireMessage + (playerID-1);
    _fireMessage = _fireMessage << 2;
    _fireMessage = _fireMessage + (tagPower-1);
    SendIR( TAG, _fireMessage);
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            
                            
///---------------------------------------------------------------------------------------------------------
//    Public : SendTag - Send a tag in a hosted game. This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendTag(byte teamID, byte playerID, byte tagPower)
{
    uint16_t _fireMessage;
      
    //Assemble the fireMessage
    _fireMessage = 136;                                                // Adds 10001000 to the start of a standard tag message, to indicate it is in a hosted game,
    _fireMessage = _fireMessage << 2;
    _fireMessage = teamID;
    _fireMessage = _fireMessage << 3;
    _fireMessage = _fireMessage + (playerID-1);
    _fireMessage = _fireMessage << 2;
    _fireMessage = _fireMessage + (tagPower-1);
    SendIR( TAG, _fireMessage);
}
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            
///---------------------------------------------------------------------------------------------------------
//    Public : SendBeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendBeacon(bool tagReceived, byte teamID, byte tagPower)
{
    uint16_t _beaconMessage;
      
    //Assemble the fireMessage
    _beaconMessage = teamID;
    _beaconMessage = _beaconMessage << 1;
    _beaconMessage = _beaconMessage + tagReceived;
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + (tagPower-1);

    this->SendIR( BEACON, _beaconMessage);
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------------------------------------------
//    Public : SendZoneBeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendZoneBeacon(byte zoneType, byte teamID)
{
    uint16_t _beaconMessage;
      
    //Assemble the fireMessage
    _beaconMessage = teamID;
    _beaconMessage = _beaconMessage << 1;
    _beaconMessage = _beaconMessage + 0;
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + zoneType;                   // 00-Invalid, 01-Reserved, 10-Contested Zone, 11-Supply Zone

    this->SendIR( BEACON, _beaconMessage);
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///---------------------------------------------------------------------------------------------------------
//    Public : SendLTARbeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendLTARbeacon(bool tagReceived, bool shieldsActive, byte tagsRemaining, byte unKnown, byte teamID)
{
    uint16_t _beaconMessage;
      
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

    this->SendIR( BEACON, _beaconMessage);
}

                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


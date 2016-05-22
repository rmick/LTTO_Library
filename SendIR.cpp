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
        Serial.print(F(" - "));
        Serial.print(message, HEX);
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
  
    #ifdef DEBUG
        Serial.print(F("\nIR Sent! "));
    #endif
}

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

///---------------------------------------------------------------------------------------------------------
//    Public : SendTag - This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendLtagTag(byte teamID, byte playerID, byte tagPower)
{
    uint16_t _fireMessage;
      
    //Assemble the fireMessage
    _fireMessage = teamID;
    _fireMessage = _fireMessage << 3;
    _fireMessage = _fireMessage + (playerID-1);
    _fireMessage = _fireMessage << 2;
    _fireMessage = _fireMessage + (tagPower-1);
    SendIR( 'T', _fireMessage);
}

///---------------------------------------------------------------------------------------------------------
//    Public : SendStandardBeacon - This takes the simple data, translates it into Binary and sends it using the SendIR class

void LTTO::SendStandardBeacon(bool TagReceived, byte teamID, byte tagPower)
{
    uint16_t _beaconMessage;
      
    //Assemble the fireMessage
    _beaconMessage = teamID;
    _beaconMessage = _beaconMessage << 1;
    _beaconMessage = _beaconMessage + (TagReceived);
    _beaconMessage = _beaconMessage << 2;
    _beaconMessage = _beaconMessage + (tagPower-1);

    this->SendIR( 'B', _beaconMessage);
}




void LTTO::PrintBinary(int v, int num_places)
{
//#ifdef DEBUG
  int mask=0, n;
  for (n=1; n<=num_places; n++)
  {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask;  // truncate v to specified number of places
  
  while(num_places)
  {
    if (v & (0x0001 << (num_places-1)))
    {
      Serial.print(F("1"));
    }
    else
    {
      Serial.print(F("0"));
    }
    
    --num_places;
    if(((num_places%4) == 0) && (num_places != 0))
    {
      Serial.print(F("_"));
    }
  }
//  #endif
}


#include "LTTO.h"
#define LIBCALL_ENABLEINTERRUPT
#include <EnableInterrupt.h>

///---------------------------------------------------------------------------------------------------------
//    The LTTO Constructor

LTTO::LTTO()
{

};

void LTTO::begin(byte txPin, byte rxPin)
{
    _txPin = txPin;
    _rxPin = rxPin;
    pinMode (_txPin, OUTPUT);
    pinMode (_rxPin, INPUT_PULLUP);

#ifdef DEBUG
    _deBugPin = DE_BUG_TIMING_PIN;
    pinMode (DE_BUG_TIMING_PIN, OUTPUT);         // This is my DeBug timing pin, fed to the Logic Analyser
    digitalWrite(DE_BUG_TIMING_PIN, LOW);
#endif

    receiveMilliTimer = 32767;
    irPacketLength = 0;
    countISR = 0;

    _shortPulseLengthError = 0;
    _arrayOverLengthError = 0;
    _badMessage_CountISRshortPacket = 0;
    _badMessage_InvalidPacketType = 0;
    _badMessage_non3_6Header = 0;

    _fifoPushPointer = 0;
    _fifoPopPointer = 0;

    ////----------------------------------------------------------------------------------------------
    //    Set Timer0 interrupt
    //    Timer0 is used for millis(), so this routine piggybacks on that by using a mid-time interupt

    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);

    ////----------------------------------------------------------------------------------------------
    //    Add this instance to the Interrupt array

    SetUpPinChangeInterupt(_rxPin, this );
}

///---------------------------------------------------------------------------------------------------------
//    LTTO Destructor

// TODO: Add a Destructor to remove the Interrupt.



///---------------------------------------------------------------------------------------------------------
//    Public: This routine Serial.prints the error counts.

void LTTO::readErrors()
{
    Serial.print(F("\n----------------"));
    Serial.print(F("\nShortPulse: "));
    Serial.print(_shortPulseLengthError);
    Serial.print(F("\tArrayOverRun: "));
    Serial.print(_arrayOverLengthError);
    Serial.print(F("\nShortPacketLength: "));
    Serial.print(_badMessage_CountISRshortPacket);
    Serial.print(F("\tInvalidPacketType: "));
    Serial.print(_badMessage_InvalidPacketType);
    Serial.print(F("\tBadMessage_non3_6Header: "));
    Serial.print(_badMessage_non3_6Header);
    Serial.print(F("\n----------------"));
}


///---------------------------------------------------------------------------------------------------------
//    Public : read/writeXXXXX - Returns the decoded variables of the last IR message

void LTTO::clearMessageOverwrittenCount()
{
    _messageOverwrittenCount = 0;
}

byte LTTO::readMessageOverwrittenCount()
{
    return _messageOverwrittenCount;
}

char LTTO::readMessageType()
{
    return decodedIRmessage.type;
}

unsigned int LTTO::readRawDataPacket()
{
    return decodedIRmessage.rawDataPacket;
}

byte LTTO::readTeamID()
{
    return decodedIRmessage.teamID;
}

byte LTTO::readPlayerID()
{
    return decodedIRmessage.playerID;
}

byte LTTO::readShotStrength()
{
    return decodedIRmessage.shotStrength;
}

char LTTO::readBeaconType()
{
    return decodedIRmessage.beaconType;
}

bool LTTO::readTagReceivedBeacon()
{
    return decodedIRmessage.tagReceivedBeacon;
}
byte LTTO::readPacketByte()
{
    return decodedIRmessage.packetByte;
}

String LTTO::readPacketName()
{
    return decodedIRmessage.packetName;
}

String LTTO::readDataType()
{
    return decodedIRmessage.dataType;
}

long int LTTO::readDataByte()
{
    return decodedIRmessage.dataByte;
}

uint8_t LTTO::readCheckSumRxByte()
{
    return decodedIRmessage.checkSumRxByte;
}

bool LTTO::readCheckSumOK()
{
    return decodedIRmessage.checkSumOK;
}


///---------------------------------------------------------------------------------------------------------
//    Public : PrintBinary - Prints out any number in Binary, including lead zeros, the size specified.
//                           Very useful for debugging

void LTTO::printBinary(uint16_t number, byte numberOfDigits)
{
    uint16_t _number = number;
    byte _numberOfDigits = numberOfDigits;
    #ifdef DEBUG
        int _mask = 0;
        for (byte _n = 1; _n <= _numberOfDigits; _n++)
        {
            _mask = (_mask << 1) | 0x0001;
        }
        _number = _number & _mask;  // truncate v to specified number of places

        while(_numberOfDigits)
        {
            if (_number & (0x0001 << (_numberOfDigits - 1) ) )
            {
                Serial.print(F("1"));
            }
            else
            {
                Serial.print(F("0"));
            }

            --_numberOfDigits;

            if( ( (_numberOfDigits % 4) == 0) && (_numberOfDigits != 0) )
            {
                Serial.print(F("_"));
            }
        }
    #endif
}

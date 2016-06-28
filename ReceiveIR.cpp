
#include <Arduino.h>
#include "LTTO.h"

///---------------------------------------------------------------------------------------------------------
//    Public : getXXXXX - Returns the decoded variables of the last IR message

bool LTTO::readNewMessageAvailable()
{
    return newMessageWaiting;
}

void LTTO::writeMessageProcessed()
{
    newMessageWaiting = false;
}

char LTTO::readMessageType()
{
    return decodedIRmessage.type;
}

unsigned int LTTO::readRawDataPacket()
{
    return decodedIRmessage.rawDataPacket;
}

byte LTTO::readMessageOverwrittenCount()
{
    return messageOverwrittenCount;
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
//    Private-Public : Required to be Public for the ISR, but not advertised - DO NOT USE OUTSIDE THE CLASS !!!!!

void LTTO::IncrementMessageOverwrittenCount()
{
    messageOverwrittenCount++;
}

void LTTO::PinChange()
{
    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, HIGH);
    #endif

    ///---------------------------------------------------------------------------------------------------------
    //  Set up the variables
    int16_t _pulseLength;
    static uint16_t _lastEdge;
    uint16_t _pinChangeTime;

    ////---------------------------------------------------------------------------------------------------------
    //  Action the Interrupt
    _pinChangeTime = micros();                      // Store the time that the pin changes
    _pulseLength = _pinChangeTime - _lastEdge;      // Measure the elapsed time since last lastEdge

    ////---------------------------------------------------------------------------------------------------------
    //  Look for short pulses (noise) and discard
    if (_pulseLength < 500 && _pulseLength > 0)
    {
        _shortPulseLengthError++;
        #ifdef DEBUG
            digitalWrite(DE_BUG_TIMING_PIN, LOW);
        #endif
        return;              // exit as the pulse is too short, so probably noise
    }

    ////---------------------------------------------------------------------------------------------------------
    // Pulse length is good. Continue actioning the Interrupt
    receiveMilliTimer = IR_TIME_OUT;                        // Set the timer for a ??mS break - the expiry of which indicates an end of packet.
    _lastEdge = _pinChangeTime;                             // Reset the lastEdge to now
    int8_t _bitLength = (_pulseLength+500)/1000;            // Round up and divide by 1,000 to give mS
    int _pinState = digitalRead(_rxPin);                    // Check state of pin (High/Low) for mark or space.
    if (_pinState == LOW) _bitLength = 0 - _bitLength;      // Set a Mark as Positive and a Break as Negative

    ////---------------------------------------------------------------------------------------------------------
    //  Store the data to the message array
    if (countISR < ARRAY_LENGTH)
    {
        if      (countISR  < 4)                     _messageIR[countISR] = _bitLength;         // Store the raw +3/-6/+X data to the first bits to keep it obvious
        else if (countISR >= 4 && _bitLength > 0)   _messageIR[countISR] = _bitLength - 1;     // Store + data bits as 0/1 instead of the raw +1/+2 to make it simple
        else                                        _messageIR[countISR] = _bitLength;         // Leave the - spaces as raw 2mS
    }
    else
    {
        _arrayOverLengthError++;
        #ifdef DEBUG
            digitalWrite(DE_BUG_TIMING_PIN, LOW);
        #endif
        countISR = 0;
        return;
    }
    ////---------------------------------------------------------------------------------------------------------
    //  increment the ISR counter pack your bags.
    countISR++;
    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, LOW);
    #endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////---------------------------------------------------------------------------------------------------------
//    Private CreateIRmessage is called from the SIGNAL Timer0 when an end of packet break is detected

void LTTO::CreateIRmessage()                              //  TODO: Currently not checking for valid -2mS breaks !!!!
{
    if (irPacketLength < 6)                           //  The message was incomplete.
    {
        incomingIRmessage.type = SHORT_PACKET;
        PushToFifo();
        return;
    }

    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, HIGH);
    #endif

    if (_messageIR[1] == 3 && _messageIR[2] == -6);     //  We have a good header.
    else
    {
        incomingIRmessage.type = NON_36_HEADER;
        PushToFifo();
    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, HIGH);
    #endif
        return;
    }

    ////---------------------------------------------------------------------------------------------------------
    //  Set the message type via the number of bits received (and 1/9th bit of Packet/Checksum)

    if      (irPacketLength > 20 && _messageIR[3] == 3 && _messageIR[5] == 0)   incomingIRmessage.type = 'P';          // Packet
    else if (irPacketLength > 20 && _messageIR[3] == 3 && _messageIR[5] == 1)   incomingIRmessage.type = 'C';          // Checksum
    else if (irPacketLength > 18 && _messageIR[3] == 3 && irPacketLength < 21)  incomingIRmessage.type = 'D';          // Data
    else if (irPacketLength > 16 && _messageIR[3] == 3 && irPacketLength < 19)  incomingIRmessage.type = 'T';          // Tag
    else if (irPacketLength > 12 && _messageIR[3] == 6 && irPacketLength < 15)  incomingIRmessage.type = 'B';          // Beacon - only beacons have 3/6/6 header !!
    else
    {
        incomingIRmessage.type = INVALID_TYPE;
        PushToFifo();
        #ifdef DEBUG
            digitalWrite(DE_BUG_TIMING_PIN, HIGH);
        #endif
        return;
    }

    ////---------------------------------------------------------------------------------------------------------
    // Set the message length based on the type

    byte messageLength = 0;
    if      (incomingIRmessage.type == 'T') messageLength = 17;           // Long Break [0] + 3 header [1,2,3] + break [4] + 7 bits,breaks [5,7,9,11,13,15,17]
    else if (incomingIRmessage.type == 'B') messageLength = 13;           // Long Break [0] + 3 header [1,2,3] + break [4] + 5 bits,breaks [5,7,9,11,13]
    else if (incomingIRmessage.type == 'P') messageLength = 21;           // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]
    else if (incomingIRmessage.type == 'D') messageLength = 19;           // Long Break [0] + 3 header [1,2,3] + break [4] + 8 bits,breaks [5,7,9,11,13,15,17,19]
    else if (incomingIRmessage.type == 'C') messageLength = 21;           // Long Break [0] + 3 header [1,2,3] + break [4] + 9 bits,breaks [5,7,9,11,13,15,17,19,21]

    ////---------------------------------------------------------------------------------------------------------
    //  Push the data into the dataPacket

    for (int i = 5; i<=messageLength; i+=2)
    {
        incomingIRmessage.rawDataPacket = incomingIRmessage.rawDataPacket << 1;
        incomingIRmessage.rawDataPacket = incomingIRmessage.rawDataPacket + (_messageIR [i]);
    }

    ////---------------------------------------------------------------------------------------------------------
    //  Tidy up and go home

    PushToFifo();

    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, LOW);
    #endif

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////---------------------------------------------------------------------------------------------------------
//  Public : bool DecodeIR() is called from the host application and returns true if there is a message waiting.
//              It also fills in all the fields of the struct dataPacket
bool LTTO::available()
{
    if (newMessageWaiting == false)
    {
        return false;
    }

    PopFromFifo();

    if      (decodedIRmessage.type == TAG)      ProcessTag();
    //TODO: Check for a bad 3/6 Tag packet and then flag as a near miss !!
    else if (decodedIRmessage.type == BEACON)   ProcessBeacon();
    else if (decodedIRmessage.type == PACKET)   ProcessPacket();
    else if (decodedIRmessage.type == DATA)     ProcessDataByte();
    else if (decodedIRmessage.type == CHECKSUM) ProcessCheckSum();
    else if (decodedIRmessage.type == SHORT_PACKET)   { _badMessage_CountISRshortPacket++;   decodedIRmessage.type = BAD_MESSAGE; }
    else if (decodedIRmessage.type == INVALID_TYPE)   { _badMessage_InvalidPacketType++;     decodedIRmessage.type = BAD_MESSAGE; }
    else if (decodedIRmessage.type == NON_36_HEADER)  { _badMessage_non3_6Header++;          decodedIRmessage.type = BAD_MESSAGE; }

    newMessageWaiting = false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LTTO::printIR(char mode)
{
    if (mode == 'X')
    {
        Serial.print(F("\nPrintIR(ext): "));
        for (int i = 0; i<= ARRAY_LENGTH; i++)
        {
            if (_messageIR[i] != 42)                  //TODO This 'if' statement can go away as we no longer fill the array with 42 as blanks.
            {
                Serial.print(_messageIR [i]);
                Serial.print(F(", "));
            }
        }
    }

    else if (mode == 'S')
    {
        Serial.print(F("\nPrintIR: "));
        Serial.print(decodedIRmessage.type);
        Serial.print(F(", "));
        Serial.print(decodedIRmessage.rawDataPacket, BIN);
        Serial.println();
    }

    if (decodedIRmessage.type == 'C') Serial.println(F("-------------------"));

}

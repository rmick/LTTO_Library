
#include "LTTO.h"
#define LIBCALL_ENABLEINTERRUPT
#include <EnableInterrupt.h>

///---------------------------------------------------------------------------------------------------------
//    The LTTO Constructor

LTTO::LTTO(byte txPin, byte rxPin)
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
    
    _byteCount =  0;
    _checkSumRx = 0;
    
    _shortPulseLengthError = 0;
    _arrayOverLengthError = 0;
    _badMessage_CountISRshortPacket = 0;
    _badMessage_InvalidPacketType = 0;
    _badMessage_non3_6Header = 0;
    
    ////----------------------------------------------------------------------------------------------
    //    Set Timer0 interrupt
    //    Timer0 is used for millis(), so this routine piggybacks on that by using a mid-time interupt
    
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    
    ////----------------------------------------------------------------------------------------------
    //    Add this instance to the Interrupt array
    
    SetUpPinChangeInterupt(_rxPin, this );
};

///---------------------------------------------------------------------------------------------------------
//    LTTO Destructor

// TODO: Add a Destructor to remove the Interrupt.



///---------------------------------------------------------------------------------------------------------
//    Public: This routine Serial.prints the error counts.

void LTTO::GetErrorCounts()
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
//    Public : PrintBinary - Prints out any number in Binary, including lead zeros, the size specified.
//                           Very useful for debugging



void LTTO::PrintBinary(int number, int numberOfDigits)
{
#ifdef DEBUG
    int mask = 0;
    for (byte n = 1; n <= numberOfDigits; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    number = number & mask;  // truncate v to specified number of places
    
    while(numberOfDigits)
    {
        if (number & (0x0001 << (numberOfDigits - 1) ) )
        {
            Serial.print(F("1"));
        }
        else
        {
            Serial.print(F("0"));
        }
        
        --numberOfDigits;
        
        if( ( (numberOfDigits % 4) == 0) && (numberOfDigits != 0) )
        {
            Serial.print(F("_"));
        }
    }
#endif
}
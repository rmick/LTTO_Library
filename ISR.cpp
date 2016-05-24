#include <Arduino.h>
#define EI_ARDUINO_INTERRUPTED_PIN
#define LIBCALL_ENABLEINTERRUPT
#include <EnableInterrupt.h>
#include "LTTO.h"

const byte isrArrayLength = 16;
LTTO* isrArray[isrArrayLength];

////---------------------------------------------------------------------------------------------------------
//    ISRchange is a global function used to capture an edge change interrupt and pass it to the correct object

void ISRchange()
{
    for (byte arrayIndex = 0; arrayIndex <= isrArrayLength; arrayIndex++)
    {
        if (arduinoInterruptedPin == arrayIndex)
        {
            isrArray[arrayIndex]->PinChange();

            Serial.print( arduinoInterruptedPin );
            Serial.print(F("-"));
            uint32_t cast = uint32_t(&isrArray[arrayIndex] );
            Serial.println(cast, HEX);
        }
    }
}

////---------------------------------------------------------------------------------------------------------
//    SetUpPinChange Interrupt is used to create an array of pointers to which LTTO instance needs to get the interrupt

void SetUpPinChangeInterupt(byte interruptPin, LTTO* lttoInstance )
{
     enableInterrupt(interruptPin, ISRchange, CHANGE);
     isrArray[interruptPin] = lttoInstance; 
}


void LTTO::PrintPointer(unsigned long index)
{
    Serial.print(F("\nArray index pin "));
    Serial.print(index);
    Serial.print(F(" = \t"));
    uint32_t cast = uint32_t(&isrArray[index] );
    Serial.print(cast, HEX);
}


////---------------------------------------------------------------------------------------------------------
//    Timer 0 interrupt. Decrements the receiveMilliTimmer variable every 1mS.

SIGNAL(TIMER0_COMPA_vect)
{
  
//    ////---------------------------------------------------------------------------------------------------------
//    //  Action the 1mS timer. Look for End of Packet >6mS Break. 
//    ltto1.receiveMilliTimer--;
//  
//    if (receiveMilliTimer == 0)
//    {
//      #ifdef DEBUG
//        digitalWrite(DE_BUG_TIMING_PIN, HIGH);
//      #endif
//      if (( decodedIRmessage.newMessage = FALSE) ) decodedIRmessage.messageOverwritten++;
//      
//      receiveMilliTimer = 32767;
//      irPacketLength = countISR;
//      countISR = 0;
//      CreateIRmessage();
//      #ifdef DEBUG
//        digitalWrite(DE_BUG_TIMING_PIN, LOW);
//      #endif
//    }
//
//    ////---------------------------------------------------------------------------------------------------------
//    //  Prevent rollover into the 0 trigger value
//    if (receiveMilliTimer == 100) receiveMilliTimer = 32767;      // Prevents rollover into the 25mS zone
}

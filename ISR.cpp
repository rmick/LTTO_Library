
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
    for (byte arrayIndex = 0; arrayIndex < isrArrayLength; arrayIndex++)
    {
        if (arduinoInterruptedPin == arrayIndex && isrArray[arrayIndex] != 0)           // !=0 is to stop noise on floating pins causing false triggering.
        {
            isrArray[arrayIndex]->PinChange();
        }
    }
}

////---------------------------------------------------------------------------------------------------------
//    SetUpPinChangeInterrupt is used to create an array of pointers to LTTO instances in order for ISRchange to know which object to use.
//      The array is indexed to Arduino pin numbers for ease of use (but is a tad wasteful of RAM).

void SetUpPinChangeInterupt(byte interruptPin, LTTO* lttoInstance )
{
    enableInterrupt(interruptPin, ISRchange, CHANGE);
    isrArray[interruptPin] = lttoInstance;
}

////---------------------------------------------------------------------------------------------------------
//    Timer 0 interrupt. Decrements the receiveMilliTimmer variable every 1mS.

SIGNAL(TIMER0_COMPA_vect)
{

    ////---------------------------------------------------------------------------------------------------------
    //  Action the 1mS timer. Look for End of Packet >6mS Break.

    for (byte arrayIndex = 0; arrayIndex < isrArrayLength; arrayIndex++)
    {

        if (isrArray[arrayIndex] != 0)                  // Indicates that there is a pointer for that PIN.

        {
            isrArray[arrayIndex]->receiveMilliTimer--;

            if (isrArray[arrayIndex]->receiveMilliTimer == 0)
            {
#ifdef DEBUG
                digitalWrite(DE_BUG_TIMING_PIN, HIGH);
#endif
                //TODO : this is no longer valid now that we have a FIFO !!!!

                isrArray[arrayIndex]->receiveMilliTimer = 32767;
                isrArray[arrayIndex]->irPacketLength    = isrArray[arrayIndex]->countISR;
                isrArray[arrayIndex]->countISR          = 0;
                isrArray[arrayIndex]->CreateIRmessage();
#ifdef DEBUG
                digitalWrite(DE_BUG_TIMING_PIN, LOW);
#endif
            }

            ////---------------------------------------------------------------------------------------------------------
            //  Prevent rollover into the 0 trigger value
            if (isrArray[arrayIndex]->receiveMilliTimer == 100) isrArray[arrayIndex]->receiveMilliTimer = 32767;      // Prevents rollover into the 25mS zone
        }
    }
}


////---------------------------------------------------------------------------------------------------------
//    Include Libraries

#include <Arduino.h>
#include "LTTO.h"

byte fifoPushPointer = 0;
byte fifoPopPointer = 0;

void LTTO::PushToFifo()          // Push latest message onto FIFO
{
    incomingIRmessageFIFO[fifoPopPointer].type = incomingIRmessage.type;
    incomingIRmessageFIFO[fifoPopPointer].rawDataPacket = incomingIRmessage.rawDataPacket;

    newMessageWaiting = true;
    return;

    fifoPushPointer++;
    if (fifoPushPointer == 10)
    {
        if (fifoPopPointer == 0) messageOverwrittenCount++;
        fifoPushPointer = 0;
    }

}

void LTTO::PopFromFifo()                                    // Pop latest message from the FIFO
{
    // Is there a new message to collect?

    decodedIRmessage.type = incomingIRmessageFIFO[fifoPopPointer].type;
    decodedIRmessage.rawDataPacket = incomingIRmessageFIFO[fifoPopPointer].rawDataPacket;
    newMessageWaiting = false;

    return;

    fifoPopPointer++;
    if (fifoPopPointer == 10 && fifoPushPointer == 0) messageOverwrittenCount++;
    if (fifoPopPointer == 10) fifoPopPointer = 0;
}

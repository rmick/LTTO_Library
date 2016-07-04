
////---------------------------------------------------------------------------------------------------------
//    Include Libraries

#include <Arduino.h>
#include "LTTO.h"


void LTTO::PushToFifo(char type, uint16_t message)          // Push latest message onto FIFO
{
    if (_incomingIRmessageFIFO[_fifoPushPointer].processed == false)
    {
        _messageOverwrittenCount++;
        for (byte i = 0; i < FIFO_SIZE; i++)
        {
            _incomingIRmessageFIFO[i].processed = true;
        }
    }
    _incomingIRmessageFIFO[_fifoPushPointer].type = type;
    _incomingIRmessageFIFO[_fifoPushPointer].rawDataPacket = message;
    _incomingIRmessageFIFO[_fifoPushPointer].processed = false;
    #ifdef DEBUG
        Serial.print(F("\nPush : "));
        Serial.print(_fifoPushPointer);
    #endif
    _fifoPushPointer++;
    if (_fifoPushPointer == FIFO_SIZE) _fifoPushPointer = 0;
}

void LTTO::PopFromFifo()                                    // Pop latest message from the FIFO
{
    // Is there a new message to collect?
    if (_incomingIRmessageFIFO[_fifoPopPointer].processed == true)  return;

    decodedIRmessage.type = _incomingIRmessageFIFO[_fifoPopPointer].type;
    decodedIRmessage.rawDataPacket = _incomingIRmessageFIFO[_fifoPopPointer].rawDataPacket;
    decodedIRmessage.newMessage = true;
    _incomingIRmessageFIFO[_fifoPopPointer].processed = true;
    #ifdef DEBUG
        Serial.print(F("\t Pop : "));
        Serial.print(_fifoPopPointer);
    #endif

    _fifoPopPointer++;
    if (_fifoPopPointer == FIFO_SIZE) _fifoPopPointer = 0;
}

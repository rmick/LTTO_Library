
////---------------------------------------------------------------------------------------------------------
//    Include Libraries

#include <Arduino.h>
#include "LTTO.h"


void LTTO::PushToFifo(char type, uint16_t message)          // Push latest message onto FIFO
{
    _incomingIRmessageFIFO[_fifoPushPointer].type = type;
    _incomingIRmessageFIFO[_fifoPushPointer].rawDataPacket = message;
    _incomingIRmessageFIFO[_fifoPushPointer].processed = false;
    Serial.print(F("\nPush : "));
    Serial.print(_fifoPushPointer);
    // Serial.print(F(" : "));
    // Serial.print(type);
    // Serial.print(F(" : "));
    // Serial.print(message);
    // Serial.print(F(" : "));
    // Serial.print(_incomingIRmessageFIFO[_fifoPushPointer].processed);

    _fifoPushPointer++;
    if (_fifoPushPointer == FIFO_SIZE) _fifoPushPointer = 0;
    if (_incomingIRmessageFIFO[_fifoPushPointer].processed == false)
    {
        _incomingIRmessageFIFO[_fifoPushPointer].processed == true;
        _messageOverwrittenCount++;
        Serial.print(F("\n  Missed Pointer: "));
        Serial.print(_fifoPushPointer);

    }
}

void LTTO::PopFromFifo()                                    // Pop latest message from the FIFO
{
    // Is there a new message to collect?
    if (_incomingIRmessageFIFO[_fifoPopPointer].processed == true) return;

    decodedIRmessage.type = _incomingIRmessageFIFO[_fifoPopPointer].type;
    decodedIRmessage.rawDataPacket = _incomingIRmessageFIFO[_fifoPopPointer].rawDataPacket;
    decodedIRmessage.newMessage = true;
    _incomingIRmessageFIFO[_fifoPopPointer].processed = true;
    Serial.print(F("\t Pop : "));
    Serial.print(_fifoPopPointer);
    // Serial.print(F(" : "));
    // Serial.print(decodedIRmessage.type);
    // Serial.print(F(" : "));
    // Serial.print(decodedIRmessage.rawDataPacket);

    _fifoPopPointer++;
    if (_fifoPopPointer == FIFO_SIZE) _fifoPopPointer = 0;
}

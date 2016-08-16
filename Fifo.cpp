
////---------------------------------------------------------------------------------------------------------
//    Include Libraries

#include <Arduino.h>
#include "LTTO.h"


void LTTO::PushToFifo(char type, uint16_t message)          // Push latest message onto FIFO
{
	if (_incomingIRmessageFIFO[_fifoPushPointer].type != ' ')
    {
        _messageOverwrittenCount++;
                Serial.print(F("Missed Pointer: "));
                Serial.print(_fifoPushPointer);
				return;
    }

    _incomingIRmessageFIFO[_fifoPushPointer].type = type;
    _incomingIRmessageFIFO[_fifoPushPointer].rawDataPacket = message;
    _incomingIRmessageFIFO[_fifoPushPointer].processed = false;
    #ifdef DEBUG
        Serial.print(F("\nPush:"));
        Serial.print(_fifoPushPointer);
    #endif
    _fifoPushPointer++;
    if (_fifoPushPointer == FIFO_SIZE) _fifoPushPointer = 0;
}

void LTTO::PopFromFifo()                                    // Pop latest message from the FIFO
{
    // Is there a new message to collect?
    if (_incomingIRmessageFIFO[_fifoPopPointer].type == ' ') return;

    decodedIRmessage.type			= _incomingIRmessageFIFO[_fifoPopPointer].type;
    decodedIRmessage.rawDataPacket	= _incomingIRmessageFIFO[_fifoPopPointer].rawDataPacket;
    decodedIRmessage.newMessage = true;
	_incomingIRmessageFIFO[_fifoPopPointer].type = ' ';		// Sets the message as empty/processed
	#ifdef DEBUG
        Serial.print(F("\tPop:"));
        Serial.print(_fifoPopPointer);
    #endif
    _fifoPopPointer++;
    if (_fifoPopPointer == FIFO_SIZE) _fifoPopPointer = 0;
}

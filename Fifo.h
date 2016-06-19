#ifndef FIFO_h
#define FIFO_h

////---------------------------------------------------------------------------------------------------------
//    Include Libraries

#include <Arduino.h>

////---------------------------------------------------------------------------------------------------------
//    Declare Global Variables

#define DEBUG

////---------------------------------------------------------------------------------------------------------
//    Declare The class

class FIFO
{
public:
    ////---------------------------------------------------------------------------------------------------------
    //  Public member functions

    FIFO (String nameOfFIFO, byte sizeOfStack);
    void PushToStack(char type, uint16_t message);
    void PopFromStack(byte teamID, byte playerID, byte tagPower);
    void ClearStack();

private:
    byte _pushPointer;
    byte _popPointer;
};

    #endif

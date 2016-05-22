
////---------------------------------------------------------------------------------------------------------
//    Include Libraries
#ifndef LTTO.h

#include <Arduino.h>

#define LIBCALL_ENABLEINTERRUPT
#include <EnableInterrupt.h>


////---------------------------------------------------------------------------------------------------------
//    Declare Global Variables

//#include "HashDefines.h"
const byte         ARRAY_LENGTH = 28;

typedef void (*isrFunctionPointer) (const byte isrRxPin);

#define DEBUG
#define DE_BUG_TIMING_PIN   12

////---------------------------------------------------------------------------------------------------------
//    Declare The class

class LTTO
{
    public:
        LTTO (byte txPin, byte rxPin);
        void PinChange(void);
        void SendIR(char type, uint16_t message);
        void SendLtagTag(byte teamID, byte playerID, byte tagPower);
        void SendHostedTag(byte teamID, byte playerID, byte tagPower);
        void SendStandardBeacon(bool TagReceived, byte teamID, byte tagPower);
        void SendZoneBeacon(char beaconType, byte teamID, byte playerID);
        void PrintBinary(int v, int num_places);

    private:
        void PulseIR(byte mSec);
        void ISRchange(void);


        byte _txPin = 9;                    // The Pin that the IR LED is attached to (via a current limiting resistor!)
        byte _rxPin = 3;                    // The Pin that the IR Receiver output is connected to.
        byte _checkSumCalc;               // The Checkcum data for sending Packet/Data messages.
        byte _deBugPin;
};


#endif


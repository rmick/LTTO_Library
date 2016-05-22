#define LIBCALL_ENABLEINTERRUPT
#include <EnableInterrupt.h>


////---------------------------------------------------------------------------------------------------------
//    ISRchange is a global function used to capture an edge change interrupt and pass it to the correct object

void ISRchange()
{
    // Check which pin was interrupted
    byte pinThatChanged = 13;
    
    //TODO: Call instance of LTTO:PinChange
    
    
}

void SetUpPinChangeInterupt(byte interruptPin) //, point* lttoInstance )
{
//     enableInterrupt(interruptPin, ltto1.ISRchange, CHANGE);
     //TODO: Add the pointer to an array with the pinNumber
}


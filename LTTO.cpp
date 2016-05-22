#include "LTTO.h"
//#inlcude "HashDefines"
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

        ////----------------------------------------------------------------------------------------------
        //    Set Timer0 interrupt
        //    Timer0 is used for millis(), so this routine piggybacks on that by using a mid-time interupt
        
        //OCR0A = 0xAF;
        //TIMSK0 |= _BV(OCIE0A);

        ////----------------------------------------------------------------------------------------------
        //    Add this instance to the Interrupt array
        // SetUpPinChangeInterupt(_txPin);
        //enableInterrupt(_rxPin, this->ISRchange, CHANGE);  
                           
    };

///---------------------------------------------------------------------------------------------------------
//    LTTO Destructor

    // TODO: Add a Destructor to remove the Interrupt.
    

///---------------------------------------------------------------------------------------------------------
//    Private : ISRchange - This is the ISR routine called by the EnableInterrupt library

void LTTO::PinChange()
{
    
}



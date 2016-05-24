
#include "LTTO.h"
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
        
        OCR0A = 0xAF;
        TIMSK0 |= _BV(OCIE0A);

        ////----------------------------------------------------------------------------------------------
        //    Add this instance to the Interrupt array

        SetUpPinChangeInterupt(_rxPin, this );
        //lttoISR isr;
        //isr.isrRxPin = _rxPin;
        //isr.lttoInstance = (this);
        //SetUpPinChangeInterupt(isr.isrRxPin, isr.lttoInstance);
        
        
                           
    };

///---------------------------------------------------------------------------------------------------------
//    LTTO Destructor

    // TODO: Add a Destructor to remove the Interrupt.
    

///---------------------------------------------------------------------------------------------------------
//    Public : Pinchange - This is the ISR routine called by the EnableInterrupt library

void LTTO::PinChange()
{
    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, HIGH);
    #endif
  
    ///---------------------------------------------------------------------------------------------------------
    //  Set up the variables
    int16_t _pulseLength;
    static uint16_t _lastEdge;
    uint16_t _pinChangeTime;
        
    ////---------------------------------------------------------------------------------------------------------
    //  Action the Interrupt
    _pinChangeTime = micros();                      // Store the time that the pin changes
    _pulseLength = _pinChangeTime - _lastEdge;      // Measure the elapsed time since last lastEdge
  
    ////---------------------------------------------------------------------------------------------------------
    //  Look for short pulses (noise) and discard
    if (_pulseLength < 500 && _pulseLength > 0)
    {
        shortPulseLengthError++;
        #ifdef DEBUG
            digitalWrite(DE_BUG_TIMING_PIN, LOW);
        #endif
        return;              // exit as the pulse is too short, so probably noise
    }

    ////---------------------------------------------------------------------------------------------------------
    // Pulse length is good. Continue actioning the Interrupt
    receiveMilliTimer = IR_TIME_OUT;                        // Set the timer for a ??mS break - the expiry of which indicates an end of packet.
    _lastEdge = _pinChangeTime;                             // Reset the lastEdge to now
    int8_t _bitLength = (_pulseLength+500)/1000;            // Round up and divide by 1,000 to give mS
    int _pinState = digitalRead(_rxPin);                    // Check state of pin (High/Low) for mark or space.
    if (_pinState == LOW) _bitLength = 0 - _bitLength;      // Set a Mark as Positive and a Break as Negative






// TODO: Get rid of this once it is proven.
// DEBUG STUFF FOR LOGIC ANALYSER //                    //  This is to prove whether the correct instance of the class is receiving
                                                        //  the IRQ instruction, by flashing it's TX pin high.
if (countISR == 1)
{
    digitalWrite(_txPin,HIGH);
    delayMicroseconds(20);
    digitalWrite(_txPin, LOW);
}





  
    ////---------------------------------------------------------------------------------------------------------
    //  Store the data to the message array
    if (countISR < ARRAY_LENGTH) 
    {
        if      (countISR  < 4)                   messageIR[countISR] = _bitLength;         // Store the raw +3/-6/+X data to the first bits to keep it obvious
        else if (countISR >= 4 && _bitLength > 0) messageIR[countISR] = _bitLength - 1;     // Store + data bits as 0/1 instead of the raw +1/+2 to make it simple
        else                                      messageIR[countISR] = _bitLength;         // Leave the - spaces as raw 2mS
    }
    else
    {
        //arrayOverLengthError++;
        #ifdef DEBUG
            digitalWrite(DE_BUG_TIMING_PIN, LOW);
        #endif
        countISR = 0;
        return;
    }
    ////---------------------------------------------------------------------------------------------------------
    //  increment the ISR counter pack your bags.
    countISR++;
    #ifdef DEBUG
        digitalWrite(DE_BUG_TIMING_PIN, LOW);
    #endif
}







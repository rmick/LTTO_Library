
///---------------------------------------------------------------------------------------------------------
//    LTTO set up
#define DEBUG
#define EI_ARDUINO_INTERRUPTED_PIN                      //  This tells the ISR routine which was the last Pin that changed
#include <EnableInterrupt.h>                            //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include <LTTO.h>                                       //  This adds the LTTO library to the sketch

LTTO ltto;                                              //  This creates an instance of the LTTO object, called ltto

////---------------------------------------------------------------------------------------------------------

const bool  RELAY_ACTIVE_LOW = true;                    //  Set this to false if the relays need a +5v output to turn on
const byte  RELAY_HOLD_TIME  = 10;                      //  Number of seconds to close the relay for when a hit is received.
const byte  RELAY_PIN        =  2;                      //  This is the digital pin that the relay is connected to.


void setup()
{
    Serial.begin(115200);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning
    ltto.begin(13,11);                                  //  Start the ltto IR comms , using Pin 13 to Tx and Pin 11 to Rx.
                                                        //  N.B. - You can use ANY pins on the Arduino, not just 13 and 11.
    pinMode(RELAY_PIN, OUTPUT);
    if      (RELAY_ACTIVE_LOW == true) digitalWrite(RELAY_PIN, HIGH);
    else                               digitalWrite(RELAY_PIN, LOW);
}

void loop()
{
    if (ltto.available() )                                      // available() checks if there is a message to receive. (1 = true, 0 = false)
    {
        if (ltto.readMessageType() == TAG)                      // if readMessageType() returns the type of Message as TAG then......
        {
            digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));   //  read the current state of the the RELAY_PIN and invert it.
            delay(RELAY_HOLD_TIME * 1000);                      //  wait for RELAY_HOLD_TIME in seconds
            digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));   //  and set the pin back to the inactive state.
        }
    } 
}

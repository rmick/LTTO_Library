#include <Arduino.h>

////---------------------------------------------------------------------------------------------------------
//    LTTO set up

#define EI_ARDUINO_INTERRUPTED_PIN          //  This tells the ISR routine which was the last Pin that changed
#include <EnableInterrupt.h>                //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include <LTTO.h>                           //  This is the LTTO library

LTTO ltto(13, 11);                          //  This creates an instance of the LTTO object, called ltto, using Pin 13 to Tx and Pin 11 to Rx.
//  N.B. - You can use ANY pins on the Arduino, not just 13 and 11.

////---------------------------------------------------------------------------------------------------------

void setup() {
// put your setup code here, to run once:

    Serial.begin(250000);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning

}

void loop() {
// put your main code here, to run repeatedly:

    ltto.SendIR(PACKET, TEXT_MESSAGE);                  // Send a Packet header of the type TEXT_MESSAGE
    ltto.SendIR(DATA,   0x48);                          // The letter 'H' in ASCII hex
    ltto.SendIR(DATA,   0x45);                          // The letter 'E' in ASCII hex
    ltto.SendIR(DATA,   0x4C);                          // The letter 'L' in ASCII hex
    ltto.SendIR(DATA,   0x4C);                          // The letter 'L' in ASCII hex
    ltto.SendIR(DATA,   0x4F);                          // The letter 'O' in ASCII hex
    ltto.SendIR(CHECKSUM, 0x00);                        // Send a CheckSum Packet - if you use 0x00 then the checksum is calculated for you.

    delay (10000);                                       // Wait 10 seconds and then repeat

}

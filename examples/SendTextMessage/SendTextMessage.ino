
////---------------------------------------------------------------------------------------------------------
//    LTTO set up

#define EI_ARDUINO_INTERRUPTED_PIN                      //  This tells the ISR routine which was the last Pin that changed
#include <EnableInterrupt.h>                            //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include <LTTO.h>                                       //  This is the LTTO library

LTTO ltto;                                              //  This creates an instance of the LTTO object, called ltto

////---------------------------------------------------------------------------------------------------------
//    Define variables

const int buttonPin = 2;                                // the number of the pushbutton pin
bool buttonState = 0;                                   // variable for reading the pushbutton status

////---------------------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning
    pinMode(buttonPin, INPUT);                          // initialize the pushbutton pin as an input:
    ltto.begin(13,11);                                  //  Start the ltto IR comms , using Pin 13 to Tx and Pin 11 to Rx.
}                                                       //  N.B. - You can use ANY pins on the Arduino, not just 13 and 11.



void loop()
{
    buttonState = digitalRead(buttonPin);               // read the state of the pushbutton value:

    if (buttonState == HIGH)                            // check if the pushbutton is pressed - if it is, the buttonState is HIGH:
    {
        ltto.sendIR(PACKET, TEXT_MESSAGE);                  // Send a Packet header of the type TEXT_MESSAGE
        ltto.sendIR(DATA,   0x48);                          // The letter 'H' in ASCII hex
        ltto.sendIR(DATA,   0x45);                          // The letter 'E' in ASCII hex
        ltto.sendIR(DATA,   0x4C);                          // The letter 'L' in ASCII hex
        ltto.sendIR(DATA,   0x4C);                          // The letter 'L' in ASCII hex
        ltto.sendIR(DATA,   0x4F);                          // The letter 'O' in ASCII hex
        ltto.sendIR(CHECKSUM, 0x00);                        // Send a CheckSum Packet - if you use 0x00 then the checksum is calculated for you.
    }
}


////---------------------------------------------------------------------------------------------------------
//    LTTO set up

#define EI_ARDUINO_INTERRUPTED_PIN                      //  This tells the ISR routine which was the last Pin that changed
#include <EnableInterrupt.h>                            //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include <LTTO.h>                                       //  This is the LTTO library

LTTO ltto;                                              //  This creates an instance of the LTTO object, called ltto

////---------------------------------------------------------------------------------------------------------
//    Define variables

const int buttonPin = 2;                                // the number of the pushbutton pin

////---------------------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning
    pinMode(buttonPin, INPUT);                          // initialize the pushbutton pin as an input:
    ltto.begin(13,11);                                  //  Start the ltto IR comms , using Pin 13 to Tx and Pin 11 to Rx.
}                                                       //  N.B. - You can use ANY pins on the Arduino, not just 13 and 11.

int buttonState = 0;                                    // variable for reading the pushbutton status
int shotStrength = 1;                                   // Create a variable called shotStrenght, set the value to 1,2,3,4 (1 = no-Mega)

void loop()
{
    buttonState = digitalRead(buttonPin);               // read the state of the pushbutton value:

    if (buttonState == HIGH)                            // check if the pushbutton is pressed - if it is, the buttonState is HIGH:
    {
        ltto.sendLTAG(shotStrength);                    // Send a Tag from player 1, strength 1 (non Mega)
        delay (200);                                    // Add a delay for button debounce
    }
}

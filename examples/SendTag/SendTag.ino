////---------------------------------------------------------------------------------------------------------
//    LTTO set up

#define EI_ARDUINO_INTERRUPTED_PIN          //  This tells the ISR routine which was the last Pin that changed
#include <EnableInterrupt.h>                //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include <LTTO.h>                           //  This is the LTTO library

LTTO ltto(13, 11);                          //  This creates an instance of the LTTO object, called ltto, using Pin 13 to Tx and Pin 11 to Rx.
                                            //  N.B. - You can use ANY pins on the Arduino, not just 13 and 11.

const int buttonPin = 2;                    // the number of the pushbutton pin

bool buttonState = 0;                                   // variable for reading the pushbutton status
byte teamID = 1;                                        // Create a variable called teamID, set to 1,2,3 for hosted games
byte playerID = 4;                                      // Create a variable called playerID, set the player # to a value between 1 and 8
byte shotStrength = 1;                                  // Create a variable called shotStrenght, set the value to 1,2,3,4 (1 = no-Mega)

////---------------------------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning

    pinMode(buttonPin, INPUT);                          // initialize the pushbutton pin as an input:
}

void loop()
{
    buttonState = digitalRead(buttonPin);               // read the state of the pushbutton value:

    if (buttonState == HIGH)                            // check if the pushbutton is pressed - if it is, the buttonState is HIGH:
    {
        ltto.sendTag(teamID, playerID, shotStrength);   // Send a Tag from team 1, player 4, strength 1)
        delay (200);                                    // Add a short delay for debounce.
    }
}

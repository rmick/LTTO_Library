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

    Serial.begin(115000);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning

}

byte teamID = 1;                                        // Create a variable called teamID, set the team # to 1
byte playerID = 4;                                      // Create a variable called playerID, set the player # to 4
byte shotStrength = 1;                                  // Create a variable called shotStrenght, set the value to 1 (same as no-Mega)

void loop() {
// put your main code here, to run repeatedly:

    ltto.sendTag(teamID, playerID, shotStrength);    // Send a Tag from team 1, player 4, strength 1)

    delay (5000);                                               // Wait 5 seconds and then repeat

}


////---------------------------------------------------------------------------------------------------------
//    Add the required libraries

#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>                //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include "LTTO.h"                           //  This is the LTTO library

////---------------------------------------------------------------------------------------------------------
//    Create an instance of the LTTO library object to use in your code.
//    You can add up to 4 different instances into one sketch on a UNO, more on a MEGA

LTTO ltto1(13,11);                          //  This creates an Object called ltto1 using Pin 13 to Tx and Pin 11 to Rx
LTTO ltto2(1 ,10);                          //  This creates an Object called ltto2 using Pin 12 to Tx and Pin 15 to Rx
LTTO ltto3(5 , 9);


////---------------------------------------------------------------------------------------------------------
//    void setup()

void setup() {
    // put your setup code here, to run once:

    Serial.begin(250000);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning

    Serial.print (F("\nisrArray :"));
    for (byte x = 0; x <= 16; x++)
    {
        ltto1.PrintPointer(x);
    }
}

////---------------------------------------------------------------------------------------------------------
//    void loop()

void loop() {
    // put your main code here, to run repeatedly:

    return;
    
    ltto1.SendBeacon(IFF_BEACON,1,4);                   //  Send an IFF (Identify Friend or Foe) Beacon with the TeamID = 1 and PlayerID = 4
    
    ltto1.SendBeacon(HIT_RESPONSE_BEACON,4,2);          //  Send a Beacon saying you werre just hit and your TeamID = 2 and PlayerID = 2
    
    //ltto1.SendZoneBeacon(MEDIC_BEACON,?,?);           //  Send a MEDIC beacon to un-neutraise a player on ANY team
    
    ltto1.SendTag(0,8,1);                               //  Send a TAG from Team 0, Player 8, with a Mega Strength of 1 (Normal)
    
    delay (2500);
}


#include <EnableInterrupt.h>
//#include "HashDefines.h"
#include "LTTO.h"



LTTO ltto1(13,11);
LTTO ltto2(12,15);


void setup() {
    // put your setup code here, to run once:

    Serial.begin(250000);
    Serial.println(F("\nHere we go boys....."));
}

void loop() {
    // put your main code here, to run repeatedly:

    ltto1.SendLtagTag(0,1,1);
    ltto2.SendLtagTag(1,1,1);
    //Serial.print("\nTag Sent");
    delay (2500);
}

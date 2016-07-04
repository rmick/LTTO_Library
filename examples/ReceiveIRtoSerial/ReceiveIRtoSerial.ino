#include <Arduino.h>

///---------------------------------------------------------------------------------------------------------
//    LTTO set up
#define DEBUG
#define EI_ARDUINO_INTERRUPTED_PIN          //  This tells the ISR routine which was the last Pin that changed
#include <EnableInterrupt.h>                //  This is required for the LTTO libary to work.  https://github.com/GreyGnome/EnableInterrupt
#include <LTTO.h>

LTTO ltto(13, 9);                          //  This creates an instance of the LTTO object, called ltto, using Pin 13 to Tx and Pin 11 to Rx.
                                            //  N.B. - You can use ANY pins on the Arduino, not just 13 and 11.

////---------------------------------------------------------------------------------------------------------



void setup() {
// put your setup code here, to run once:

    Serial.begin(115200);                               //  Starts the Serial port for debugging
    Serial.println(F("\nHere we go boys....."));        //  A short message so that we know something is happenning
}




void loop() {
// put your main code here, to run repeatedly:

    if (ltto.available() )                                      // available() checks if there is a message to receive. (1 = true, 0 = false)
    {
        switch (ltto.readMessageType() )                         // readMessageType() returns the type of Message (TAG, BEACON, PACKET, DATA, CHECKSUM)
        {
            case TAG:
                Serial.print(F("\nTag - "));
                Serial.print("Team #: ");
                Serial.print(ltto.readTeamID() );                // readTeamID() returns the teamID (0 = None, 1 = team1, 2 = team2, 3 = team3)
                Serial.print(F("\tPlayer #: "));
                Serial.print(ltto.readPlayerID() );              // readPlayerID() returns the playerID (Player # 1 through 8)
                Serial.print(F("\tShotStrength: "));
                Serial.print(ltto.readShotStrength() );          // readShotStrength() returns the shotStrength (value 1 through 4, where 1 is a normal tag - non mega)
            break;

            case BEACON:
                if      (ltto.readBeaconType() == IFF_BEACON)       // readBeaconType() returns the beacon type. IFF_BEACON is an 'Identify Friend or Foe' beacon, sent by all taggers twice a second.
                {                                                   // It includes a TeamID, in order to work out if the sending tagger is on the same team as the receiver.
                    Serial.print(F("\n\tIFF Beacon - "));
                    Serial.print(F("TeamID #: "));
                    Serial.print(ltto.readTeamID() );
                }
                else if (ltto.readBeaconType() == HIT_RESPONSE_BEACON)  // readBeaconType() returns the beacon type. HIT_RESPONSE_BEACON is sent once when a tagger is hit.
                {                                                       // It returns the TeamID and the strength of the tag received.
                    Serial.print(F("\n\t Hit Response Beacon - "));
                    Serial.print(F("TeamID #: "));
                    Serial.print(ltto.readTeamID() );
                    Serial.print(F("\tTagStrength: "));
                    Serial.print(ltto.readShotStrength() );
                }
                else if (ltto.readBeaconType() == ZONE_BEACON)           // GetBeaconType() returns the beacon type. ZONE_BEACON is not currently used in LTTO taggers.
                {
                    Serial.print(F("\n\t  Zone Beacon"));
                }
                else if (ltto.readBeaconType() == CONTESTED_ZONE)       // readBeaconType() returns the beacon type. CONTESTED_ZONE is used in some Zone based games
                {
                    Serial.print(F("\n\t   Contested Zone"));
                }
                else if (ltto.readBeaconType() == SUPPLY_ZONE)          // readBeaconType() returns the beacon type. SUPPLY ZONE is a beacon that can Respawn (un-neutralise) players
                {                                                       // It can also replenish Tags and Sheilds. Supply zones can be Neutral or Team based. See wiki.lazerswarm.com for more details
                    Serial.print(F("\n\t    Supply Zone Beacon - "));
                    if (ltto.readTeamID() == 0)                          // If teamID = 0 then it is a Neutral zone (works for any team)
                    {
                        Serial.print(F("Neutral"));
                    }
                    else
                    {
                        Serial.print(F("TeamID #: "));                  // If teamID is 1,2 or 3 - then the supply base only works for that team
                        Serial.print(ltto.readTeamID() );
                    }
                }
            break;

            case PACKET:                                                // A Packet byte a Header that announces what follows. Used to Host Games and send special messages
                Serial.print(F("\n\t\t------------------"));
                Serial.print(F("\n\t\tPacket1 - 0x"));
                Serial.print(ltto.readPacketByte(), HEX);
                Serial.print(F(" - "));
                Serial.print(ltto.readPacketName() );
            break;

            case DATA:                                                  // A Data byte contains information related to the Packet type. There are usually multiple data bytes in a message
                Serial.print(F("\n\t\t\tData - 0x"));
                Serial.print(ltto.readDataByte(), HEX);
            break;

            case CHECKSUM:                                                          // The CheckSum byte is the 8bit sum of the Packet byte + all the data bytes
                Serial.print(F("\n\t\tCheckSum - "));
                if      (ltto.readCheckSumOK() == true)  Serial.print(F("OK."));    // If readCheckSumOK = 1 (true), then checkSum is correct.
                else if (ltto.readCheckSumOK() == false) Serial.print(F("BAD !"));  // If readCheckSumOK = 0 (false), there is a data error, and the entire message should be discarded
            break;

            case BAD_MESSAGE:
                Serial.print(F("\n\t\t\tBad IR data"));
            break;
        }

        if(ltto.readMessageOverwrittenCount() > 0 )                     // readMessageOverwrittenCount() returns a count of how many messages have been missed. Must be ZERO !
        {
            Serial.print(F("\n\nSorry but the main loop is too slow - "));
            Serial.print(ltto.readMessageOverwrittenCount() );
            Serial.println(F(" message/s missed\n"));
            ltto.clearMessageOverwrittenCount();
        }
    }


    if (Serial.available() !=0)
     {
         char keyIn = Serial.read();
         switch (keyIn)
         {
             case '1':
                 delay (1000);
                 break;
             case '2':
                 delay (2000);
                 break;
             case '5':
                 delay (5000);
                 break;
        }

    }
}

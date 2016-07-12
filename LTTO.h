
#ifndef LTTO_h
#define LTTO_h

////---------------------------------------------------------------------------------------------------------
//    Include Libraries

#include <Arduino.h>
#include "HashDefines.h"

#define LIBCALL_ENABLEINTERRUPT
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>



////---------------------------------------------------------------------------------------------------------
//    Declare Global Variables

const byte  ARRAY_LENGTH = 28;
const byte  IR_TIME_OUT  = 9;
const byte  FIFO_SIZE    = 20;

//#define DEBUG
//#define DE_BUG_TIMING_PIN   12

////---------------------------------------------------------------------------------------------------------
//    Declare The class

class LTTO
{
public:
    ////---------------------------------------------------------------------------------------------------------
    //  Public member functions

    LTTO();
    void        begin(byte txPin, byte rxPin);
    void        sendIR(char type, uint16_t message);
    bool        sendLTAG(byte tagPower);
    bool        sendTag(byte teamID, byte playerID, byte tagPower);
    bool        sendBeacon(bool tagReceived, byte teamID, byte tagPower);
    bool        sendZoneBeacon(byte zoneType, byte teamID);
    bool        sendLTARbeacon(bool tagReceived, bool shieldsActive, byte tagsRemaining, byte unKnown, byte teamID);

    bool        available();
    void        clearMessageOverwrittenCount();
    byte        readMessageOverwrittenCount();
    char        readMessageType();
    uint16_t    readRawDataPacket();
    byte        readTeamID();
    byte        readPlayerID();
    byte        readShotStrength();
    char        readBeaconType();
    bool        readTagReceivedBeacon();
    byte        readPacketByte();
    String      readPacketName();
    String      readDataType();
    long int    readDataByte();
    uint8_t     readCheckSumRxByte();
    bool        readCheckSumOK();


    void        printBinary(uint16_t number, byte numberOfDigits);
    void        readErrors();
    void        printIR(char mode);

    //Do NOT call these public functions, they are public solely to allow the ISR routines to access them
    void        PinChange(void);
    void        CreateIRmessage();
    //void        IncrementMessageOverwrittenCount();

    ////---------------------------------------------------------------------------------------------------------
    //  Public member variables

    //ISR routine variables
    volatile uint16_t   receiveMilliTimer;          // This is public so that Timer0 SIGNAL can change it.
    volatile byte       irPacketLength;
    volatile byte       countISR;

private:
    ////---------------------------------------------------------------------------------------------------------
    //  Private member functions

    void PulseIR(byte mSec);
    void ProcessTag();
    void ProcessBeacon();
    void ProcessPacket();
    void ProcessDataByte();
    void ProcessCheckSum();
    void PushToFifo(char type, uint16_t message);
    void PopFromFifo();

    ////---------------------------------------------------------------------------------------------------------
    //  Private member variables

    //Hardware variables
    byte _txPin;                        // The Pin that the IR LED is attached to (via a current limiting resistor!)
    byte _rxPin;                        // The Pin that the IR Receiver output is connected to.
    byte _deBugPin;                     // The Pin used for debugging (via a Logic Analyser or Oscilloscope)

    //Other variables
    int8_t  _messageIR [ARRAY_LENGTH];      // Array of IR data bits, populated by PinChange
    byte    _checkSumCalc;                  // Used to calculate the SendIR CheckSum

    // Error checking and debug counters
    volatile byte   _shortPulseLengthError;
    volatile byte   _arrayOverLengthError;
    int             _badMessage_CountISRshortPacket;
    int             _badMessage_InvalidPacketType;
    int             _badMessage_non3_6Header;

    struct irMessage
    {
        volatile bool           newMessage;               //  true = Yes this is a new message waiting
        volatile char           type;                     //  T, B, P, D, C
        volatile unsigned int   rawDataPacket;            //  The undecoded datapacket
        //volatile byte           messageOverwrittenCount;  //  This counts messages that are overwritten by a new packet before being read by the main loop 'if(DecodeIR() )' call
        byte                    teamID;                   //  Team 0 = No Team, then 1,2,3
        byte                    playerID;                 //  Player 1 thru 8
        byte                    shotStrength;             //  Mega = 1 thru 4
        char                    beaconType;               //  '0' = IFF, '1' = TagReceived, 'Z' = ZONE,
        bool                    tagReceivedBeacon;        //  true = Beacon sent due to Tagger being tagged (confirmation)
        byte                    packetByte;               //  The undecoded Packet number in Hex
        String                  packetName;               //  The simple name for the packet (e.g. Announce Game)  - source   wiki.lazerswarm.com
        String                  dataType;                 //  What the current DataByte contains (e.g. GameID, TaggerID, ShieldTime, PackedByte1, etc)
        long int                dataByte;                 //  Data Byte (in Hex)
        byte                    checkSumCalc ;            //  The calculated checkSum - each data byte adds to this value
        uint8_t                 checkSumRxByte;           //  CheckSum value (in Hex)
        bool                    checkSumOK;               //  true = The Checksum matches the data packets
        //Packet & Data variables
        byte                    byteCount;                //  Counter of what byte number the current byte is.
        bool                    gameIDmatch;
        byte                    taggerID;
        bool                    ackPlayerAssignOK;

    };

    irMessage decodedIRmessage;

    struct irRxRaw
    {
        volatile char           type;                     //  T, B, P, D, C
        volatile unsigned int   rawDataPacket;            //  The undecoded datapacket
        volatile bool           processed;                //  Tracks whether this message has been processed.
    };

    irRxRaw _incomingIRmessageFIFO[FIFO_SIZE];

    volatile int8_t _fifoPushPointer;
    volatile int8_t _fifoPopPointer;
    volatile byte   _messageOverwrittenCount;
    volatile int8_t _numberOfMessagesWaiting;
};

void SetUpPinChangeInterupt(byte interruptPin, LTTO* lttoInstance );           // Need to pre-declare it here, otherwise the Compiler barks !

#endif

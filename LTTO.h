
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

#define DEBUG
#define DE_BUG_TIMING_PIN   12

////---------------------------------------------------------------------------------------------------------
//    Declare The class

class LTTO
{
public:
    ////---------------------------------------------------------------------------------------------------------
    //  Public member functions

    LTTO (byte txPin, byte rxPin);
    void SendIR(char type, uint16_t message);
    void SendLTAG(byte teamID, byte playerID, byte tagPower);
    void SendTag(byte teamID, byte playerID, byte tagPower);
    void SendBeacon(bool tagReceived, byte teamID, byte tagPower);
    void SendZoneBeacon(byte zoneType, byte teamID);
    void SendLTARbeacon(bool tagReceived, bool shieldsActive, byte tagsRemaining, byte unKnown, byte teamID);
    bool Available();

    bool    GetNewMessage();
    void    SetMessageProcessed();
    char    GetMessageType();
    unsigned int GetRawDataPacket();
    byte    GetMessageOverwrittenCount();
    void    IncrementMessageOverwrittenCount();
    byte    GetTeamID();
    byte    GetPlayerID();
    byte    GetShotStrength();
    char    GetBeaconType();
    bool    GetTagReceivedBeacon();
    byte    GetPacketByte();
    String  GetPacketName();
    String  GetDataType();
    long int GetDataByte();
    uint8_t GetCheckSumRxByte();
    bool    GetCheckSumOK();


    void PrintBinary(int v, int num_places);
    void GetErrorCounts();
    void PrintIR(char mode);

    //Do NOT call these public functions, they are public solely to allow the ISR routines to access them
    void PinChange(void);
    void CreateIRmessage();

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

    ////---------------------------------------------------------------------------------------------------------
    //  Private member variables

    //Hardware variables
    byte _txPin;                        // The Pin that the IR LED is attached to (via a current limiting resistor!)
    byte _rxPin;                        // The Pin that the IR Receiver output is connected to.
    byte _deBugPin;                     // The Pin used for debugging (via a Logic Analyser or Oscilloscope)

    //Other variables
    byte        _checkSumCalc;                  // The Checkcum data for sending Packet/Data messages.
    int8_t      _messageIR [ARRAY_LENGTH];      // Array of data bits, populated by PinChange
    uint8_t     _byteCount;                     // Counter for data packet bytes to identify which is which
    uint8_t     _checkSumRx;                    // Received CheckSum (calculated)

    //Packet & Data variables
    bool _gameIDmatch;
    byte _taggerID;
    bool _ackPlayerAssignOK;

    // Error checking and debug counters
    volatile byte   _shortPulseLengthError;
    volatile byte   _arrayOverLengthError;
    int             _badMessage_CountISRshortPacket;
    int             _badMessage_InvalidPacketType;
    int             _badMessage_non3_6Header;

    struct irMessage
    {
        volatile bool           newMessage;               //  true = Yes there is a new message waiting
        volatile char           type;                     //  T, B, P, D, C
        volatile unsigned int   rawDataPacket;            //  The undecoded datapacket
        volatile byte           messageOverwrittenCount;  //  This counts messages that are overwritten by a new packet before being read by the main loop 'if(DecodeIR() )' call
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
    };

    irMessage decodedIRmessage;

};

void SetUpPinChangeInterupt(byte interruptPin, LTTO* lttoInstance );           // Need to pre-declare it here, otherwise the Compiler barks !

#endif

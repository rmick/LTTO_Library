
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
        void PinChange(void);
        void SendIR(char type, uint16_t message);
        void SendTag(byte teamID, byte playerID, byte tagPower);
        void SendHostedTag(byte teamID, byte playerID, byte tagPower);
        void SendBeacon(bool TagReceived, byte teamID, byte tagPower);
        void SendZoneBeacon(char beaconType, byte teamID, byte playerID);
        void PrintBinary(int v, int num_places);
        void CreateIRmessage();
        bool DecodeIR();
        void GetErrorCounts();

    ////---------------------------------------------------------------------------------------------------------
    //  Public member variables

        //ISR routine variables
        volatile uint16_t   receiveMilliTimer;          // This is public so that Timer0 SIGNAL can change it.
        volatile byte       irPacketLength;
        volatile byte       countISR;


        struct decodedIR
        {
              volatile bool           newMessage;           //  true = Yes there is a new message waiting
              volatile char           type;                 //  T, B, P, D, C
              volatile unsigned int   rawDataPacket;        //  The undecoded datapacket
              volatile unsigned int   messageOverwritten;   //  This counts messages that are overwritten by a new packet before being read by the main loop 'if(DecodeIR() )' call
              unsigned int            TeamID;               //  Team 0 = No Team, then 1,2,3
              unsigned int            PlayerID;             //  Player 1 thru 8
              unsigned int            ShotStrength;         //  Mega = 1 thru 4
              // TODO: These are yet to be implemented in DecodeIR()
              bool                    TagReceivedBeacon;    //  true = Beacon sent due to Tagger being tagged (confirmation)
              int                     PacketByte;           //  The undecoded Packet number in Hex
              String                  PacketName;           //  The simple name for the packet (e.g. Announce Game)  - source   wiki.lazerswarm.com
              String                  DataType;             //  What the current DataByte contains (e.g. GameID, TaggerID, ShieldTime, PackedByte1, etc)
              long int                DataByte;             //  Data Byte (in Hex)
              uint8_t                 CheckSumRxByte;       //  CheckSum value (in Hex)
              bool                    CheckSumOK;           //  true = The Checksum matches the data packets
        };
        decodedIR decodedIRmessage;

    private:
    ////---------------------------------------------------------------------------------------------------------
    //  Private member functions
    
        void PulseIR(byte mSec);

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
        uint8_t     _CheckSumRx;                    // Received CheckSum (calculated)

        // Error checking and debug counters
        volatile byte   _shortPulseLengthError;
        volatile byte   _arrayOverLengthError;
        int             _badMessage_CountISRshortPacket;
        int             _badMessage_InvalidPacketType;
        int             _badMessage_non3_6Header;
        
};

void SetUpPinChangeInterupt(byte interruptPin, LTTO* lttoInstance );           // Need to pre-declare it here, otherwise the Compiler barks ! 

#endif


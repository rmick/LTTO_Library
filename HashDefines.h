
////---------------------------------------------------------------------------------------------------------
//    SendIR stuff

#define     TAG             'T'
#define     BEACON          'B'
#define     PACKET          'P'
#define     DATA            'D'
#define     CHECKSUM        'C'
#define     BAD_MESSAGE     'X'

////---------------------------------------------------------------------------------------------------------
//    Beacon Stuff

#define     IFF_BEACON          '0'
#define     HIT_RESPONSE_BEACON '1'
#define     SUPPLY_ZONE         'S'
#define     CONTESTED_ZONE      'C'
#define     ZONE_BEACON         'Z'

////---------------------------------------------------------------------------------------------------------
//     PACKET TYPES

#define     COUNT_DOWN                      0x000
#define     ASSIGN_PLAYER                   0x001
#define     ANNOUNCE_CUSTOM_GAME            0x002
#define     ANNOUNCE_CUSTOM_GAME_2_TEAMS    0x003
#define     ANNOUNCE_CUSTOM_GAME_3_TEAMS    0x004
#define     ANNOUNCE_HIDE_AND_SEEK          0x005
#define     ANNOUNCE_HUNT_THE_PREY          0x006
#define     ANOUNCE_KINGS_2_TEAMS           0x007
#define     ANOUNCE_KINGS_3_TEAMS           0x008
#define     ANNOUNCE_OWN_THE_ZONE           0x009
#define     ANNOUNCE_OWN_THE_ZONE_2_TEAMS   0x00A
#define     ANNOUNCE_OWN_THE_ZONE_3_TEAMS   0x00B
#define     ANNOUNCE_SPECIAL_GAME           0x00C
#define     ASSIGN_PLAYER_FAIL              0x00F
#define     REQUEST_JOIN_GAME               0x010
#define     ACK_PLAYER_ASSIGN               0x011
#define     REQUEST_ASSISTANCE              0x020
#define     SEND_ASSISTANCE                 0x021

#define     TEXT_MESSAGE                    0x080

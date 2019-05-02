
////---------------------------------------------------------------------------------------------------------
//    SendIR stuff

#define     TAG             'T'
#define     BEACON          'B'
#define     LTAR_BEACON     'L'
#define     PACKET          'P'
#define     DATA            'D'
#define     CHECKSUM        'C'
#define     BAD_MESSAGE     'x'
#define     SHORT_PACKET    's'
#define     INVALID_TYPE    't'
#define     NON_36_HEADER   'h'

////---------------------------------------------------------------------------------------------------------
//    Beacon Stuff

#define     IFF_BEACON          '0'
#define     HIT_RESPONSE_BEACON '1'
#define     SUPPLY_ZONE         '3'
#define     CONTESTED_ZONE      '2'
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
#define     REQUEST_TAG_REPORT              0x031
#define     RANK_REPORT                     0x032
#define     TAG_SUMMARY                     0x040
#define     TEAM_1_TAG_REPORT               0x041
#define     TEAM_2_TAG_REPORT               0x042
#define     TEAM_3_TAG_REPORT               0x043
#define     SINGLE_TAG_REPORT               0x048
#define     TEXT_MESSAGE                    0x080
#define     SPECIAL_ATTACK                  0x090

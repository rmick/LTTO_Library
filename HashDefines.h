
////---------------------------------------------------------------------------------------------------------
//    SendIR stuff

#define     TAG             'h'
#define     BEACON          'i'
#define     LTAR_BEACON     'j'
#define     PACKET          'k'
#define     DATA            'l'
#define     CHECKSUM        'm'
#define     BAD_MESSAGE     'x'
#define     SHORT_PACKET    's'
#define     INVALID_TYPE    't'
#define     NON_36_HEADER   'h'

////---------------------------------------------------------------------------------------------------------
//    Beacon Stuff

#define     IFF_BEACON                  '0'
#define     HIT_RESPONSE_BEACON         '1'
#define     SUPPLY_ZONE                 '3'
#define     CONTESTED_ZONE              '2'
#define     ZONE_BEACON                 'Z'
#define     LTAR_IFF_BEACON             'f'
#define     LTAR_HIT_RESPONSE_BEACON    'g'

////---------------------------------------------------------------------------------------------------------
//     PACKET TYPES

#define     COUNT_DOWN                      0x000      // ASCII    Ctrl-@
#define     ASSIGN_PLAYER                   0x001      // ASCII    Ctrl-A
#define     ANNOUNCE_CUSTOM_GAME            0x002      // ASCII    Ctrl-B
#define     ANNOUNCE_CUSTOM_GAME_2_TEAMS    0x003      // ASCII    Ctrl-C
#define     ANNOUNCE_CUSTOM_GAME_3_TEAMS    0x004      // ASCII    Ctrl-D
#define     ANNOUNCE_HIDE_AND_SEEK          0x005      // ASCII    Ctrl-E
#define     ANNOUNCE_HUNT_THE_PREY          0x006      // ASCII    Ctrl-F
#define     ANOUNCE_KINGS_2_TEAMS           0x007      // ASCII    Ctrl-G
#define     ANOUNCE_KINGS_3_TEAMS           0x008      // ASCII    Ctrl-H
#define     ANNOUNCE_OWN_THE_ZONE           0x009      // ASCII    Ctrl-I
#define     ANNOUNCE_OWN_THE_ZONE_2_TEAMS   0x00A      // ASCII    Ctrl-J
#define     ANNOUNCE_OWN_THE_ZONE_3_TEAMS   0x00B      // ASCII    Ctrl-K
#define     ANNOUNCE_SPECIAL_GAME           0x00C      // ASCII    Ctrl-L
#define     ASSIGN_PLAYER_FAIL              0x00F      // ASCII    Ctrl-O
#define     REQUEST_JOIN_GAME               0x010      // ASCII    Ctrl-P
#define     ACK_PLAYER_ASSIGN               0x011      // ASCII    Ctrl-Q
#define     REQUEST_ASSISTANCE              0x020      // ASCII    Space
#define     SEND_ASSISTANCE                 0x021      // ASCII    !
#define     REQUEST_TAG_REPORT              0x031      // ASCII    1
#define     RANK_REPORT                     0x032      // ASCII    2
#define     TAG_SUMMARY                     0x040      // ASCII    @
#define     TEAM_1_TAG_REPORT               0x041      // ASCII    A
#define     TEAM_2_TAG_REPORT               0x042      // ASCII    B
#define     TEAM_3_TAG_REPORT               0x043      // ASCII    C
#define     SINGLE_TAG_REPORT               0x048      // ASCII    H
#define     TEXT_MESSAGE                    0x080      // ASCII    C+
#define     SPECIAL_ATTACK                  0x090      // ASCII    E+

#include "source/c/icepalace_maps.h"
#include "bank_helpers.h"
#include "globals.h"

CODE_BANK(ROOM_LOGIC_BANK);
const unsigned char palace_room_0[] = {
26, 26, 26, 26,
6, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 7, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
2, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 3, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
2, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 3, 
2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 9, 
128
};

const unsigned char* const palace_rooms[] = {
palace_room_0, 
};
CODE_BANK_POP();
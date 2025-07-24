#include "source/c/forest_maps.h"
#include "bank_helpers.h"
#include "globals.h"

CODE_BANK(ROOM_LOGIC_BANK);
const unsigned char shelterforest_room_0[] = {
1, 26, 26, 26,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
0, 7, 0, 0, 7, 0, 0, 1, 0, 0, 1, 0, 
0, 7, 0, 0, 7, 0, 0, 1, 0, 0, 1, 0, 
0, 7, 0, 0, 7, 0, 0, 1, 0, 0, 1, 0, 
0, 7, 0, 0, 7, 20, 0, 1, 0, 0, 1, 0, 
0, 7, 0, 0, 7, 0, 0, 1, 0, 0, 1, 0, 
5, 6, 0, 0, 7, 0, 20, 1, 0, 0, 2, 3, 
0, 0, 0, 0, 7, 0, 0, 1, 0, 0, 0, 0, 
128
};

const unsigned char shelterforest_room_1[] = {
26, 26, 0, 26,
3, 3, 3, 3, 6, 0, 0, 2, 5, 5, 5, 5, 
7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
7, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 1, 
7, 0, 0, 0, 8, 9, 10, 11, 0, 0, 0, 1, 
7, 0, 0, 0, 12, 13, 14, 15, 0, 0, 0, 1, 
7, 0, 20, 0, 16, 17, 18, 19, 20, 0, 0, 1, 
7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
128
};

const unsigned char* const shelterforest_rooms[] = {
shelterforest_room_0, 
shelterforest_room_1, 
};
CODE_BANK_POP();
#include "monster_anims.h"

// Walker metasprites
const unsigned char walkerWalk0[]={
    0, 0, 0x01, 4,
    8, 0, 0x02, 4,
    0, 8, 0x03, 4,
    8, 8, 0x04, 4,
    128
};
const unsigned char walkerWalk1[]={
    0, 0, 0x01, 4,
    8, 0, 0x02, 4,
    0, 8, 0x05, 4,
    8, 8, 0x06, 4,
    128
};


extern const unsigned char* monWalkerAnims[]={
    walkerWalk0,
    walkerWalk1
};

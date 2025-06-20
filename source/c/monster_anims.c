#include "monster_anims.h"
#include "neslib.h"

// Walker metasprites
const unsigned char walkerWalk0[]={
    0, 0, 0x1D, 4,
    8, 0, 0x1D, 4 | OAM_FLIP_H,
    0, 8, 0x2D, 4,
    8, 8, 0x2E, 4,
    128
};
const unsigned char walkerWalk1[]={
    0, 0, 0x1D, 4,
    8, 0, 0x1D, 4 | OAM_FLIP_H,
    0, 8, 0x2E, 4 | OAM_FLIP_H,
    8, 8, 0x2D, 4 | OAM_FLIP_H,
    128
};


extern const unsigned char* monWalkerAnims[]={
    walkerWalk0,
    walkerWalk1
};

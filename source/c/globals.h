#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include "bank_helpers.h"
#include "maps.h"
#include "actors.h"

enum GameState
{
    GS_INTRO,
    GS_GAMEPLAY,
    GS_SCREENTRANS,
    GS_ENVTRANS,
    GS_DEAD,
    GS_OUTRO
};

ZEROPAGE_EXTERN(enum GameState, currentState);
ZEROPAGE_EXTERN(unsigned char, i);
ZEROPAGE_EXTERN(unsigned char, x);
ZEROPAGE_EXTERN(unsigned char, y);
ZEROPAGE_EXTERN(unsigned int, attrib_addr);
ZEROPAGE_EXTERN(unsigned char, spr);
ZEROPAGE_EXTERN(unsigned char, framecount);
ZEROPAGE_EXTERN(unsigned char, pad_trig);
ZEROPAGE_EXTERN(unsigned char, pad);
ZEROPAGE_EXTERN(unsigned char, playerLevel);
ZEROPAGE_EXTERN(unsigned char, currentRoom);
ZEROPAGE_EXTERN(enum Environment, currentEnvironment);
ZEROPAGE_EXTERN(unsigned char, spawnedItems);

#define MAX_ITEMS 5
extern Item itemList[];

#define TOTAL_SPAWNABLES 255
extern unsigned char deadList[];

#endif

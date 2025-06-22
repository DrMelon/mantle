#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include "bank_helpers.h"
#include "maps.h"
#include "actors.h"
#include "items.h"

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
ZEROPAGE_EXTERN(unsigned char, i2);
ZEROPAGE_EXTERN(unsigned char, x);
ZEROPAGE_EXTERN(unsigned char, y);
ZEROPAGE_EXTERN(unsigned int, attrib_addr);
ZEROPAGE_EXTERN(unsigned char, spr);
ZEROPAGE_EXTERN(unsigned char, framecount);
ZEROPAGE_EXTERN(unsigned char, pad_trig);
ZEROPAGE_EXTERN(unsigned char, pad);
ZEROPAGE_EXTERN(unsigned char, playerHp);
ZEROPAGE_EXTERN(unsigned char, playerExp);
ZEROPAGE_EXTERN(unsigned char, playerLevel);
ZEROPAGE_EXTERN(unsigned char, currentRoom);
ZEROPAGE_EXTERN(enum Environment, currentEnvironment);
ZEROPAGE_EXTERN(unsigned char, spawnedItems);
ZEROPAGE_EXTERN(unsigned char, spawnedMonsters);
ZEROPAGE_EXTERN(unsigned char, soundTestNum);
ZEROPAGE_EXTERN(unsigned char, roomSwitchDir);
ZEROPAGE_EXTERN(unsigned char, writingVram);
ZEROPAGE_EXTERN(unsigned char, hudDirty);
extern unsigned char* roomPtr;
extern unsigned char* metatilesPtr;
extern unsigned char currentRoomColl[];

#define MAX_ITEMS 5
extern Item itemList[];

#define MAX_MONSTERS 16
extern Monster monsterList[];

#define TOTAL_SPAWNABLES 255
extern unsigned char deadList[];

extern unsigned char palmTreeBuffer[];

#endif

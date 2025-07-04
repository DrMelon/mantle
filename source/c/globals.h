#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include "bank_helpers.h"
#include "maps.h"
#include "actors.h"
#include "items.h"
#include "teleport.h"
#include "projectiles.h"
#include "monsters.h"

enum GameState
{
    GS_INTRO,
    GS_GAMEPLAY,
    GS_TEXTBOX,
    GS_SCREENTRANS,
    GS_SCREENTRANS_TELE,
    GS_ENVTRANS,
    GS_DEAD,
    GS_OUTRO
};

//
// Global Variables (zeropage)
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//

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
ZEROPAGE_EXTERN(unsigned char, prevRoom);
ZEROPAGE_EXTERN(enum Environment, currentEnvironment);
ZEROPAGE_EXTERN(unsigned char, spawnedItems);
ZEROPAGE_EXTERN(unsigned char, spawnedMonsters);
ZEROPAGE_EXTERN(unsigned char, spawnedTeles);
ZEROPAGE_EXTERN(unsigned char, spawnedProjectiles);
ZEROPAGE_EXTERN(unsigned char, soundTestNum);
ZEROPAGE_EXTERN(unsigned char, roomSwitchDir);
ZEROPAGE_EXTERN(unsigned char, writingVram);
ZEROPAGE_EXTERN(unsigned char, hudDirty);
ZEROPAGE_EXTERN(unsigned char, queueTele);
ZEROPAGE_EXTERN(unsigned char, textQueued);
ZEROPAGE_EXTERN(unsigned char, textLength);
ZEROPAGE_EXTERN(unsigned char, textColOffset);
ZEROPAGE_EXTERN(unsigned char, textLineOffset);
ZEROPAGE_EXTERN(unsigned char, textSeekChar);
ZEROPAGE_EXTERN(unsigned char, textDelay);
ZEROPAGE_EXTERN(unsigned char, monsterAggression);
ZEROPAGE_EXTERN(unsigned char, jumpArcs);

#pragma bss-name(push, "ZEROPAGE")
    extern WalkingCharacter kris;
#pragma bss-name(pop)

extern unsigned char* roomPtr;
extern unsigned char* metatilesPtr;
extern unsigned char currentRoomColl[];

#define MAX_ITEMS 5
extern Item itemList[];

#define MAX_MONSTERS 16
extern Monster monsterList[];

#define MAX_JUMP_ARCS 4
extern JumpArcState jumpArcList[];

#define MAX_TELEPORTERS 2
extern Teleporter teleList[];

#define TOTAL_SPAWNABLES 255
extern unsigned char deadList[];

#define MAX_PROJECTILES 8
extern Projectile projList[];

extern unsigned char palmTreeBuffer[];

#endif

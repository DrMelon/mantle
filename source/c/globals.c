#include "bank_helpers.h"
#include "globals.h"

ZEROPAGE_DEF(enum GameState, currentState);
ZEROPAGE_DEF(unsigned char, i);
ZEROPAGE_DEF(unsigned char, i2);
ZEROPAGE_DEF(unsigned char, x);
ZEROPAGE_DEF(unsigned char, x2);
ZEROPAGE_DEF(unsigned char, y);
ZEROPAGE_DEF(unsigned char, y2);
ZEROPAGE_DEF(unsigned int, attrib_addr);
ZEROPAGE_DEF(unsigned char, spr);
ZEROPAGE_DEF(unsigned char, framecount);
ZEROPAGE_DEF(unsigned char, pad_trig);
ZEROPAGE_DEF(unsigned char, pad);
ZEROPAGE_DEF(unsigned char, playerHp);
ZEROPAGE_DEF(unsigned char, playerExp);
ZEROPAGE_DEF(unsigned char, playerLevel);
ZEROPAGE_DEF(unsigned char, currentRoom);
ZEROPAGE_DEF(unsigned char, prevRoom);
ZEROPAGE_DEF(enum Environment, currentEnvironment);
ZEROPAGE_DEF(unsigned char, spawnedItems);
ZEROPAGE_DEF(unsigned char, spawnedMonsters);
ZEROPAGE_DEF(unsigned char, spawnedTeles);
ZEROPAGE_DEF(unsigned char, spawnedProjectiles);
ZEROPAGE_DEF(unsigned char, spawnedRafts);
ZEROPAGE_DEF(unsigned char, soundTestNum);
ZEROPAGE_DEF(unsigned char, roomSwitchDir);
ZEROPAGE_DEF(unsigned char, writingVram);
ZEROPAGE_DEF(unsigned char, hudDirty);
ZEROPAGE_DEF(unsigned char, queueTele);
ZEROPAGE_DEF(unsigned char, textQueued);
ZEROPAGE_DEF(unsigned char, textLength);
ZEROPAGE_DEF(unsigned char, textSeekChar);
ZEROPAGE_DEF(unsigned char, textColOffset);
ZEROPAGE_DEF(unsigned char, textLineOffset);
ZEROPAGE_DEF(unsigned char, textDelay);
ZEROPAGE_DEF(unsigned char, monsterAggression);
ZEROPAGE_DEF(unsigned char, jumpArcs);
ZEROPAGE_DEF(unsigned char, theatricActive);
ZEROPAGE_DEF(unsigned char, theatricIndex);
ZEROPAGE_DEF(unsigned char, theatricStage);
ZEROPAGE_DEF(unsigned char, theatricTimer);

#pragma bss-name(push, "ZEROPAGE")
    WalkingCharacter kris;
    Twisted twisted;
#pragma bss-name(pop)

const unsigned char* roomPtr = 0;
const unsigned char* metatilesPtr = 0;
unsigned char unpackedRoom[128];

Item itemList[MAX_ITEMS];
Monster monsterList[MAX_MONSTERS];
Teleporter teleList[MAX_TELEPORTERS];
Projectile projList[MAX_PROJECTILES];
unsigned char deadList[TOTAL_SPAWNABLES];
JumpArcState jumpArcList[MAX_JUMP_ARCS];
Raft raftList[MAX_RAFTS];

unsigned char palmTreeBuffer[64];

unsigned char treeRoomVisits = 0;

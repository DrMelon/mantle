#include "bank_helpers.h"
#include "globals.h"

ZEROPAGE_DEF(enum GameState, currentState);
ZEROPAGE_DEF(unsigned char, i);
ZEROPAGE_DEF(unsigned char, i2);
ZEROPAGE_DEF(unsigned char, x);
ZEROPAGE_DEF(unsigned char, y);
ZEROPAGE_DEF(unsigned int, attrib_addr);
ZEROPAGE_DEF(unsigned char, spr);
ZEROPAGE_DEF(unsigned char, framecount);
ZEROPAGE_DEF(unsigned char, pad_trig);
ZEROPAGE_DEF(unsigned char, pad);
ZEROPAGE_DEF(unsigned char, playerLevel);
ZEROPAGE_DEF(unsigned char, currentRoom);
ZEROPAGE_DEF(enum Environment, currentEnvironment);
ZEROPAGE_DEF(unsigned char, spawnedItems);
ZEROPAGE_DEF(unsigned char, spawnedMonsters);
ZEROPAGE_DEF(unsigned char, soundTestNum);
ZEROPAGE_DEF(unsigned char, roomSwitchDir);
ZEROPAGE_DEF(unsigned char, writingVram);
ZEROPAGE_DEF(unsigned char, hudDirty);

unsigned char* roomPtr = 0;
unsigned char* metatilesPtr = 0;
unsigned char currentRoomColl[96];

#pragma bss-name(push, "ZEROPAGE")
Item itemList[MAX_ITEMS];
#pragma bss-name(pop)

Monster monsterList[MAX_MONSTERS];
unsigned char deadList[TOTAL_SPAWNABLES];

unsigned char palmTreeBuffer[16];

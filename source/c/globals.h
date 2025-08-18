#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include "bank_helpers.h"
#include "maps.h"
#include "actors.h"
#include "items.h"
#include "teleport.h"
#include "projectiles.h"
#include "monsters.h"
#include "rafts.h"
#include "twisted.h"

enum GameState
{
    GS_GAMEPLAY,
    GS_SCREENTRANS,
    GS_SCREENTRANS_TELE,
    GS_ENVTRANS,
    GS_DEATH,
    GS_NOTHING
};

enum Theatric
{
    TH_INTRO,
    TH_GETSWORD,
    TH_GETICEKEY,
    TH_USEICEKEY,
    TH_GETSHELTERKEY,
    TH_USED_UP,
    TH_TWISTEDSPAWN,
    TH_TWISTEDSPAWNFAST,
    TH_OUTRO
};

// Bank Assignments
#define ROOM_LOGIC_BANK 4
#define MONSTER_PROJECTILES_BANK 1
#define ACTOR_LOGIC_BANK 3
#define JUMP_LUT_BANK 2
#define KRIS_ANIMS_BANK 2
#define CHEAT_CODES_BANK 6
#define TWISTED_BANK 6
#define INTRO_BANK 3
#define MUSIC_BANK 5
#define UI_BANK 0

// Music Tracks
#define MUSIC_INTRO 0
#define MUSIC_SWORD 1
#define MUSIC_SWORD_SLOW 2
#define MUSIC_NORTHERNLIGHT 3
#define MUSIC_GLACEIR 5

// SFX
#define SFX_SWORD_SWING 0
#define SFX_SWORD_DINK 1
#define SFX_KILL 2
#define SFX_COLLECT_ITEM 3
#define SFX_LEVELUP 4
#define SFX_ENTER_DUNGEON 5

// Narrative flags
#define NARFLAG_MET_TWISTED_EARLY      0b00000001
#define NARFLAG_KILLED_SONGCAT         0b00000010
#define NARFLAG_FOUGHT_TWISTED_ONCE    0b00000100
#define NARFLAG_FOUND_NOELLE           0b00001000
#define NARFLAG_KILLED_SUSIE           0b00010000
#define NARFLAG_KILLED_RALSEI          0b00100000

//
// Global Variables (zeropage)
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//

ZEROPAGE_EXTERN(enum GameState, currentState);
ZEROPAGE_EXTERN(unsigned char, i);
ZEROPAGE_EXTERN(unsigned char, i2);
ZEROPAGE_EXTERN(unsigned char, x);
ZEROPAGE_EXTERN(unsigned char, x2);
ZEROPAGE_EXTERN(unsigned char, y);
ZEROPAGE_EXTERN(unsigned char, y2);
ZEROPAGE_EXTERN(unsigned int, attrib_addr);
ZEROPAGE_EXTERN(unsigned char, spr);
ZEROPAGE_EXTERN(unsigned char, framecount);
ZEROPAGE_EXTERN(unsigned char, stripefc);
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
ZEROPAGE_EXTERN(unsigned char, spawnedRafts);
ZEROPAGE_EXTERN(unsigned char, spawnedButtons);
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
ZEROPAGE_EXTERN(unsigned char, theatricActive);
ZEROPAGE_EXTERN(unsigned char, theatricIndex);
ZEROPAGE_EXTERN(unsigned char, theatricStage);
ZEROPAGE_EXTERN(unsigned char, theatricTimer);
ZEROPAGE_EXTERN(unsigned char, narrativeFlags);
ZEROPAGE_EXTERN(unsigned char, roomLocked);

#pragma bss-name(push, "ZEROPAGE")
    extern WalkingCharacter kris;
    extern Twisted twisted;
#pragma bss-name(pop)

// For city & ice palace
extern Follower followerA;
extern Follower followerB;
extern unsigned char followPositions[];
extern unsigned char lastFollowPosIdx;

// Fast-access room pointers
extern const unsigned char* roomPtr;
extern const unsigned char* metatilesPtr;
// Buffer for unpacking room data after loading with RLE
extern unsigned char unpackedRoom[];

// Buffers for actors
#define MAX_ITEMS 8
extern Item itemList[];

#define MAX_MONSTERS 8
extern Monster monsterList[];

#define MAX_JUMP_ARCS 4
extern JumpArcState jumpArcList[];

#define MAX_TELEPORTERS 4
extern Telebutton teleList[];

#define TOTAL_SPAWNABLES 255
#define DEAD_LIST_LEN 32
extern unsigned char deadList[];

#define MAX_PROJECTILES 8
extern Projectile projList[];

#define MAX_RAFTS 2
extern Raft raftList[];


// Tile update buffer for killing trees (and more)
extern unsigned char palmTreeBuffer[];
extern unsigned char doorLockMemory[]; // 8 tiles must be stored for locking doors in ice palace

// Narrative/theatrics
extern unsigned char treeRoomVisits; // Number of times visited the Tree Room

// Music/sound data
extern unsigned char music_data_mantle[];

#endif

#include "bank_helpers.h"
#include "globals.h"

ZEROPAGE_DEF(enum GameState, currentState);
ZEROPAGE_DEF(unsigned char, i);
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

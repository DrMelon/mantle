#ifndef __CHEATS_H_
#define __CHEATS_H_

// Turn off this define to disable cheats.
#define CHEATS_ENABLED 1
#ifdef CHEATS_ENABLED

extern unsigned char cheatInputIdx;
extern unsigned char lastEightInputs[];
extern const unsigned char maxLevelCheat[];
extern const unsigned char refillHpCheat[];
extern const unsigned char skipToIslandCheat[];

extern void update_cheats();
extern unsigned char cheat_check(const unsigned char* cheat);

#endif
#endif

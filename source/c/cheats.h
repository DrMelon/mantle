#ifndef __CHEATS_H_
#define __CHEATS_H_

// Turn off this define to disable cheats.
#define CHEATS_ENABLED 1
#ifdef CHEATS_ENABLED

extern unsigned char cheatInputIdx;
extern unsigned char lastEightInputs[];
extern const unsigned char maxLevelCheat[];

extern void update_cheats();
extern unsigned char max_level_cheat_check();

#endif
#endif

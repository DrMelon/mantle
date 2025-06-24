#ifndef __UI_H_
#define __UI_H_

#include "actors.h"

extern const unsigned char hpText[];
extern const unsigned char lv1Text[];
extern const unsigned char lv2Text[];
extern const unsigned char lv3Text[];
extern const unsigned char lvMaxText[];

extern const unsigned char barBlocks[];

extern const unsigned char dialog_0[];

extern void draw_ui_borders();
extern void refresh_hud_bars(char hp, char lvl, char exp);
extern void summon_text(unsigned char* text, char exclusive_state);
extern void clear_text();

// RAM
extern unsigned char hudUpdateBuffer[];
#endif

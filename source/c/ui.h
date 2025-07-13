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
extern const unsigned char dialog_1[];
extern const unsigned char dialog_2[];
extern const unsigned char dialog_3[];

extern const unsigned char icekey_found_0[];

extern const unsigned char intro_0[];

extern const unsigned char instruct_0[];
extern const unsigned char instruct_1[];

extern void draw_ui_borders();
extern void init_hud_refresh_banked();
extern void refresh_hud_bars(char hp, char lvl, char exp);
extern void queue_text_banked();
extern void queue_text(const unsigned char* textLine, unsigned char mode);
extern void update_text();
extern void clear_text();

// RAM
extern unsigned char hudUpdateBuffer[];
extern unsigned char textBuffer[];
extern unsigned char textVRAMBuffer[];
#endif

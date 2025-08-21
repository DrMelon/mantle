#ifndef __UI_H_
#define __UI_H_

#include "actors.h"

extern const unsigned char hpText[];
extern const unsigned char lv1Text[];
extern const unsigned char lv2Text[];
extern const unsigned char lv3Text[];
extern const unsigned char lvMaxText[];

extern const unsigned char barBlocks[];

extern const unsigned char* const cave_dialogs[];

extern const unsigned char icekey_found_0[];
extern const unsigned char icekey_use_0[];
extern const unsigned char northernlight_0[];
extern const unsigned char secret_message_0[];
extern const unsigned char remember_path_0[];

extern const unsigned char icepalace_text_0[];
extern const unsigned char icepalace_text_1[];
extern const unsigned char icepalace_text_2[];

extern const unsigned char instruct_0[];
extern const unsigned char instruct_1[];

extern void draw_ui_borders();
extern void init_hud_refresh_banked();
extern void refresh_hud_bars(char hp, char lvl, char exp);
extern void queue_text_banked();
extern void queue_text(const unsigned char* textLine, unsigned char mode);
extern void update_text();
extern void clear_text();
extern void start_dialog(const unsigned char* const* dialog, unsigned char length);

// RAM
extern unsigned char hudUpdateBuffer[];
extern unsigned char textBuffer[];
extern unsigned char textVRAMBuffer[];
#endif

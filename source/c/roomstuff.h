#ifndef __ROOMSTUFF_H_
#define __ROOMSTUFF_H_

#include "maps.h"

extern void load_env_target_banked();
extern void load_environment(enum Environment env);
extern void load_room_intro();
extern void load_room();
extern void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettemask);
extern void switch_to_room();
extern void tele_to_room();
extern void skip_to_island();
extern void skip_to_ice_palace();
extern void skip_to_city();
extern void skip_to_forest();
extern void reload_area();

#endif

#ifndef __ROOMSTUFF_H_
#define __ROOMSTUFF_H_

#include "maps.h"

extern void load_environment(enum Environment env);
extern void load_room();
extern void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettemask);
extern void switch_to_room(unsigned char room);
extern void tele_to_room(unsigned char room, unsigned char telex, unsigned char teley);
extern void skip_to_island();

#endif

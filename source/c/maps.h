#ifndef __MAPS_H_
#define __MAPS_H_

enum Environment
{
   E_DESERT,
   E_ISLAND,
   E_CITY,
   E_ICEPALACE,
   E_SHELTER
};

#define ROOM_DATA_OFFSET 4;

extern const unsigned char* environment_metatiles[];

extern const unsigned char desert_metatiles[];
extern const unsigned char desert_room_0[];
extern const unsigned char* desert_rooms[];

extern const unsigned char** environment_rooms[];

int solidity_check(unsigned char px, unsigned char py);
int tilemap_solid(unsigned char tx, unsigned char ty);
int tile_solid(unsigned char tile);
void set_map_tile_in_room(unsigned char tx, unsigned char ty, unsigned char tile);


extern void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettemask);


#endif

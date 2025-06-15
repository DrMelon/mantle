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

extern const unsigned char* environment_metatiles[];

extern const unsigned char desert_metatiles[];
extern const unsigned char desert_room_0[];
extern const unsigned char* desert_rooms[];

extern const unsigned char** environment_rooms[];

int solidity_check(unsigned char px, unsigned char py);
int tilemap_solid(unsigned char tx, unsigned char ty);
int tile_solid(unsigned char tile);

#endif

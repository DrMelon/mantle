#ifndef __MAPS_H_
#define __MAPS_H_

#include "actors.h"

enum Environment
{
   E_DESERT,
   E_ISLAND,
   E_ICEPALACE,
   E_CITY,
   E_SHELTER
};

#define ROOM_DATA_OFFSET 4

#define TILE_D_SAND 0
#define TILE_D_TREE 1
#define TILE_D_FERN 2
#define TILE_D_PYRBRICK_L 3
#define TILE_D_PYRBRICK_R 4
#define TILE_D_DOORF_TL 5
#define TILE_D_DOORF_TR 6
#define TILE_D_DOORF_BL 7
#define TILE_D_DOORF_BR 8
#define TILE_D_BRICK_TL 9
#define TILE_D_BRICK_TR 10
#define TILE_D_BRICK_BL 11
#define TILE_D_BRICK_BR 12
#define TILE_D_BRICK_H 13
#define TILE_D_BRICK_V 14
#define TILE_D_WATER 15
#define TILE_D_WATER_S 16
#define TILE_D_WATER_E 17
#define TILE_D_WATER_N 18
#define TILE_D_WATER_W 19
#define TILE_D_WATER_TL 20
#define TILE_D_WATER_TR 21
#define TILE_D_WATER_BL 22
#define TILE_D_WATER_BR 23
#define TILE_D_ROCKS 24
#define TILE_D_DIRT 25
#define TILE_D_STAIRS 26
#define TILE_D_DUNGEONPLAT 27
#define TILE_D_CACTUS 28
#define TILE_D_EMPTYWALK 29
#define TILE_D_EMPTYFULL 30
#define TILE_D_PEBBLES 31
#define TILE_D_BRIDGE 32

extern const unsigned char* environment_metatiles[];

extern const unsigned char desert_metatiles[];
extern const unsigned char desert_room_0[];

extern const unsigned char* const* environment_rooms[];

int solidity_check(unsigned char px, unsigned char py);
int tilemap_solid(unsigned char tx, unsigned char ty);
int tile_solid(unsigned char tile);
int swim_check(unsigned char tx, unsigned char ty);
int tilemap_swimmable(unsigned char tx, unsigned char ty);
void set_map_tile_in_room(unsigned char tx, unsigned char ty, unsigned char tile);
void set_map_tile_on_character(WalkingCharacter* chara, unsigned char tile);

extern void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettemask);


#endif

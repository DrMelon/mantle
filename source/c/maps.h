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
#define TILE_D_BLACK 33
#define TILE_D_CHEST_CLOSED 34
#define TILE_D_CHEST_OPEN 35

#define TILE_I_SAND 0
#define TILE_I_TREE 1
#define TILE_I_FERN 2
#define TILE_I_WFERN 3
#define TILE_I_BRICK_H 4
#define TILE_I_BRICK_V 5
#define TILE_I_LADDER 6
#define TILE_I_ROCKS 7
#define TILE_I_DIRT 8
#define TILE_I_BRICKDOOR_TL 9
#define TILE_I_BRICKDOOR_TR 10
#define TILE_I_BRICKDOOR_BL 11
#define TILE_I_BRICKDOOR_BR 12
#define TILE_I_PEBBLES 13
#define TILE_I_BRIDGE_DOCK 14
#define TILE_I_WATER 15
#define TILE_I_WATER_S 16
#define TILE_I_WATER_E 17
#define TILE_I_WATER_N 18
#define TILE_I_WATER_W 19
#define TILE_I_WATER_TL 20
#define TILE_I_WATER_TR 21
#define TILE_I_WATER_BL 22
#define TILE_I_WATER_BR 23
#define TILE_I_STAIRS 24
#define TILE_I_DUNGEONPLAT 25
#define TILE_I_EMPTYWALK 26
#define TILE_I_EMPTYFULL 27
#define TILE_I_DELTDOOR_TL 28
#define TILE_I_DELTDOOR_TR 29
#define TILE_I_DELTDOOR_BL 30
#define TILE_I_DELTDOOR_BR 31
#define TILE_I_DUNGEONWALL_L 32
#define TILE_I_DUNGEONWALL_R 34
#define TILE_I_DUNGEONWALL_D 35
#define TILE_I_DUNGEONWALL_U 36
#define TILE_I_DUNGEONCORN_TL 37
#define TILE_I_DUNGEONCORN_TR 38
#define TILE_I_DUNGEONCORN_BL 39
#define TILE_I_DUNGEONCORN_BR 40
#define TILE_I_WATERFALL_TL 41
#define TILE_I_WATERFALL_TR 42
#define TILE_I_SMALLDOOR 43
#define TILE_I_WBRIDGE_L 44
#define TILE_I_WBRIDGE_R 45


extern const unsigned char* const environment_metatiles[];

extern const unsigned char desert_metatiles[];
extern const unsigned char island_metatiles[];

extern const unsigned char* const* environment_rooms[];

extern int solidity_check(unsigned char px, unsigned char py);
extern int solidity_check_nocactus(unsigned char px, unsigned char py);
extern int cactus_check(unsigned char px, unsigned char py);
extern int tilemap_solid(unsigned char tx, unsigned char ty);
extern int tilemap_solid_nocactus(unsigned char tx, unsigned char ty);
extern int tilemap_ouchie(unsigned char tx, unsigned char ty);
extern int tile_solid(unsigned char tile);
extern int swim_check(unsigned char tx, unsigned char ty);
extern int tilemap_swimmable(unsigned char tx, unsigned char ty);
extern void set_map_tile_in_room(unsigned char tx, unsigned char ty, unsigned char tile);
extern void set_map_tile_on_character(WalkingCharacter* chara, unsigned char tile);
extern void draw_black_tile_in_room(unsigned char tx, unsigned char ty);


#endif

#include "maps.h"
#include "globals.h"
#include "neslib.h"
#include "actors.h"
#include "desert_maps.h"
#include "island_maps.h"
#include "icepalace_maps.h"
#include "city_maps.h"
#include "dungeon_maps.h"
#include "forest_maps.h"
#include "shelter_maps.h"
#include "bank_helpers.h"

// Format: 4 8x8 tiles that make up this metatile, and palette mask for attrib (actual mask differs based on tile pos)
// then, tile solidity type (0 = walkable, 1 = not walkable, 2 = not walkable, hurts)
const unsigned char desert_metatiles[]={
    0x01, 0x02, 0x11, 0x12, 0b01010101, 0,
    0x03, 0x04, 0x13, 0x14, 0b01010101, 1,
    0x05, 0x06, 0x15, 0x16, 0b01010101, 1,
    0x29, 0x2A, 0x39, 0x3A, 0b01010101, 1,
    0x2B, 0x2C, 0x3B, 0x3C, 0b01010101, 1,
    0x41, 0x42, 0x51, 0x52, 0b01010101, 1,
    0x43, 0x44, 0x53, 0x54, 0b01010101, 1,
    0x61, 0x62, 0x71, 0x72, 0b01010101, 0,
    0x63, 0x64, 0x73, 0x74, 0b01010101, 0,
    0x49, 0x4A, 0x59, 0x5A, 0b01010101, 1,
    0x69, 0x6A, 0x79, 0x7A, 0b01010101, 1,
    0x8B, 0x8C, 0x9B, 0x9C, 0b01010101, 1,
    0x89, 0x8A, 0x99, 0x9A, 0b01010101, 1,
    0x4B, 0x4C, 0x5B, 0x5C, 0b01010101, 1,
    0x6B, 0x6C, 0x7B, 0x7C, 0b01010101, 1,
    0x31, 0x32, 0x32, 0x31, 0b11111111, 1,
    0x21, 0x22, 0x31, 0x32, 0b11111111, 1,
    0x22, 0x31, 0x21, 0x32, 0b11111111, 1,
    0x31, 0x32, 0x21, 0x22, 0b11111111, 1,
    0x31, 0x22, 0x32, 0x21, 0b11111111, 1,
    0x21, 0x22, 0x22, 0x32, 0b11111111, 1,
    0x21, 0x22, 0x31, 0x21, 0b11111111, 1,
    0x21, 0x32, 0x22, 0x21, 0b11111111, 1,
    0x31, 0x22, 0x21, 0x22, 0b11111111, 1,
    0x25, 0x26, 0x35, 0x36, 0b10101010, 1,
    0x01, 0x02, 0x11, 0x12, 0b10101010, 0,
    0x0B, 0x0C, 0x1B, 0x1C, 0b01010101, 0,
    0x0D, 0x0E, 0x1D, 0x1E, 0b01010101, 0,
    0x07, 0x08, 0x17, 0x18, 0b01010101, 2,
    0x00, 0x00, 0x00, 0x00, 0b01010101, 0,
    0x00, 0x00, 0x00, 0x00, 0b01010101, 1,
    0x23, 0x24, 0x33, 0x34, 0b01010101, 0,
    0x0F, 0x0F, 0x1F, 0x1F, 0b10101010, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x45, 0x46, 0x55, 0x56, 0b10101010, 1,
    0x47, 0x48, 0x57, 0x58, 0b10101010, 1
};

const unsigned char island_metatiles[]={
    0x01, 0x02, 0x11, 0x12, 0b01010101, 0,  /* TILE_I_SAND 0 */
    0x03, 0x04, 0x13, 0x14, 0b01010101, 1,  /* TILE_I_TREE 1 */
    0x05, 0x06, 0x15, 0x16, 0b01010101, 1,  /* TILE_I_FERN 2 */
    0x07, 0x08, 0x17, 0x18, 0b11111111, 1,  /* TILE_I_WFERN 3 */
    0x27, 0x28, 0x37, 0x38, 0b10101010, 1,  /* TILE_I_BRICK_H 4 */
    0x29, 0x2A, 0x39, 0x3A, 0b10101010, 1,  /* TILE_I_BRICK_V 5 */
    0x45, 0x46, 0x55, 0x56, 0b10101010, 0,  /* TILE_I_LADDER 6 */
    0x25, 0x26, 0x35, 0x36, 0b10101010, 1,  /* TILE_I_ROCKS 7 */
    0x01, 0x02, 0x11, 0x12, 0b10101010, 0,  /* TILE_I_DIRT 8 */
    0x41, 0x42, 0x51, 0x52, 0b10101010, 1,  /* TILE_I_BRICKDOOR_TL 9 */
    0x43, 0x44, 0x53, 0x54, 0b10101010, 1,  /* TILE_I_BRICKDOOR_TR 10 */
    0x61, 0x62, 0x71, 0x72, 0b10101010, 0,  /* TILE_I_BRICKDOOR_BL 11 */
    0x63, 0x64, 0x73, 0x74, 0b10101010, 0,  /* TILE_I_BRICKDOOR_BR 12 */
    0x23, 0x24, 0x33, 0x34, 0b01010101, 0,  /* TILE_I_PEBBLES 13 */
    0x0F, 0x0F, 0x1F, 0x1F, 0b10101010, 0,  /* TILE_I_BRIDGE_DOCK 14 */
    0x31, 0x32, 0x32, 0x31, 0b11111111, 1,  // Water, 15-23
    0x21, 0x22, 0x31, 0x32, 0b11111111, 1,
    0x22, 0x31, 0x21, 0x32, 0b11111111, 1,
    0x31, 0x32, 0x21, 0x22, 0b11111111, 1,
    0x31, 0x22, 0x32, 0x21, 0b11111111, 1,
    0x21, 0x22, 0x22, 0x32, 0b11111111, 1,
    0x21, 0x22, 0x31, 0x21, 0b11111111, 1,
    0x21, 0x32, 0x22, 0x21, 0b11111111, 1,
    0x31, 0x22, 0x21, 0x22, 0b11111111, 1,
    0x0B, 0x0C, 0x1B, 0x1C, 0b01010101, 0,  /* TILE_I_STAIRS 24 */
    0x0D, 0x0E, 0x1D, 0x1E, 0b01010101, 0,  /* TILE_I_DUNGEONPLAT 25 */
    0x10, 0x10, 0x10, 0x10, 0b10101010, 0,  /* TILE_I_EMPTYWALK 26 */
    0x10, 0x10, 0x10, 0x10, 0b10101010, 1,  /* TILE_I_EMPTYFULL 27 */
    0x81, 0x82, 0x91, 0x92, 0b10101010, 1,  /* TILE_I_DELTDOOR_TL 28 */
    0x83, 0x84, 0x93, 0x94, 0b10101010, 1,  /* TILE_I_DELTDOOR_TR 29 */
    0x85, 0x86, 0x95, 0x96, 0b10101010, 1,  /* TILE_I_DELTDOOR_BL 30 */
    0x87, 0x88, 0x97, 0x98, 0b10101010, 1,  /* TILE_I_DELTDOOR_BR 31 */
    0x10, 0x4B, 0x10, 0x5B, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_L 32 */
    0x4C, 0x10, 0x5C, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_R 33 */
    0x10, 0x10, 0x47, 0x48, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_D 34 */
    0x57, 0x58, 0x10, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_U 35 */
    0x10, 0x10, 0x10, 0x4A, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_TL 36 */
    0x10, 0x10, 0x49, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_TR 37 */
    0x10, 0x5A, 0x10, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_BL 38 */
    0x59, 0x10, 0x10, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_BR 39 */
    0x67, 0x68, 0x77, 0x78, 0b10101010, 0,  /* TILE_I_WATERFALL_TL 40 */
    0x69, 0x6A, 0x79, 0x7A, 0b10101010, 0,  /* TILE_I_WATERFALL_TR 41 */
    0x65, 0x66, 0x75, 0x76, 0b10101010, 0,  /* TILE_I_SMALLDOOR 42 */
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
};

const unsigned char icepalace_metatiles[]={
    0x2B, 0x2C, 0x3B, 0x3C, 0b01010101, 0, // floor
    0x09, 0x0A, 0x19, 0x1A, 0b01010101, 1, // water
    0x10, 0x4B, 0x10, 0x5B, 0b01010101, 1, // wall l
    0x4C, 0x10, 0x5C, 0x10, 0b01010101, 1, // wall r
    0x10, 0x10, 0x47, 0x48, 0b01010101, 1, // wall d
    0x57, 0x58, 0x10, 0x10, 0b01010101, 1, // wall u
    0x10, 0x10, 0x10, 0x4A, 0b01010101, 1, // wallcorn tl
    0x10, 0x10, 0x49, 0x10, 0b01010101, 1, // wallcorn tr
    0x10, 0x5A, 0x10, 0x10, 0b01010101, 1, // wallcorn bl
    0x59, 0x10, 0x10, 0x10, 0b01010101, 1, // wallcorn br
    0x00, 0x00, 0x00, 0x00, 0b01010101, 0,    //TILE_IP_EMPTY 10
    0x00, 0x00, 0x00, 0x00, 0b01010101, 1,    //TILE_IP_EMPTY_SOLID 11
    0x27, 0x28, 0x37, 0x38, 0b01010101, 1,    //TILE_IP_BRICKWALL 12
    0x81, 0x82, 0x91, 0x92, 0b01010101, 1,    //TILE_IP_DELTDOOR_TL 13
    0x83, 0x84, 0x93, 0x94, 0b01010101, 1,    //TILE_IP_DELTDOOR_TR 14
    0x85, 0x86, 0x95, 0x96, 0b01010101, 1, //TILE_IP_DELTDOOR_BL 15
    0x87, 0x88, 0x97, 0x98, 0b01010101, 1, //TILE_IP_DELTDOOR_BR 16
    0x4D, 0x4E, 0x5D, 0x5E, 0b01010101, 0,    //TILE_IP_BUTTON 17
    0x8D, 0x8E, 0x9D, 0x9E, 0b01010101, 0, //TILE_IP_BUTTON_DOWN 18
    0x6B, 0x6C, 0x7B, 0x7C, 0b01010101, 1,//TILE_IP_PUZDOOR_L 19
    0x6D, 0x6E, 0x7D, 0x7E, 0b01010101, 1,//TILE_IP_PUZDOOR_R 20
    0x8B, 0x8C, 0x9B, 0x9C, 0b11111111, 1,//TILE_IP_TREE 21

    0x10, 0x10, 0x10, 0x10, 0b00000000, 0, // black
};

const unsigned char city_metatiles[]={
    0x60, 0x60, 0x60, 0x60, 0b01010101, 0,  //TILE_CITY_FLOOR 0
    0x01, 0x02, 0x11, 0x12, 0b01010101, 0,    //TILE_CITY_FLOOR_DETAIL 1
    0x21, 0x21, 0x60, 0x60, 0b01010101, 0,    //TILE_CITY_FLOOR_SHADOW 2
    0x03, 0x04, 0x13, 0x14, 0b00000000, 1,    //TILE_CITY_WALL_DETAIL 3
    0x05, 0x06, 0x15, 0x16, 0b00000000, 1,    //TILE_CITY_WALL_SCREEN 4
    0x0B, 0x0C, 0x1B, 0x1C, 0b00000000, 1,    //TILE_CITY_WALL_ANGLEDETAIL_L 5
    0x0D, 0x0E, 0x1D, 0x1E, 0b00000000, 1,    //TILE_CITY_WALL_ANGLEDETAIL_R 6
    0x2B, 0x2C, 0x3B, 0x3C, 0b00000000, 1,    //TILE_CITY_WALL_ANGLEWINDOW_L 7
    0x2D, 0x2E, 0x3D, 0x3E, 0b00000000, 1,    //TILE_CITY_WALL_ANGLEWINDOW_R 8
    0x07, 0x08, 0x17, 0x18, 0b01010101, 0,    //TILE_CITY_WALL_ANGLEFLOOR_L 9
    0x09, 0x0A, 0x19, 0x1A, 0b01010101, 0,    //TILE_CITY_WALL_ANGLEFLOOR_R 10
    0x20, 0x20, 0x20, 0x20, 0b01010101, 0,    //TILE_CITY_UMBRA 11
    0x23, 0x24, 0x33, 0x34, 0b10101010, 1,    //TILE_CITY_BOLLARD 12
    0x43, 0x44, 0x53, 0x54, 0b10101010, 1,    //TILE_CITY_BOLLARD_V 13
    0x23, 0x44, 0x53, 0x54, 0b10101010, 1,    //TILE_CITY_BOLLARD_CORNER 14
    0x25, 0x26, 0x35, 0x36, 0b00000000, 1,    //TILE_CITY_WALL_WINDOW 15
    0x27, 0x28, 0x37, 0x38, 0b00000000, 0,    //TILE_CITY_MANHOLE 16
    0x45, 0x46, 0x55, 0x56, 0b11111111, 0,    //TILE_CITY_STAIRS 17
    0x20, 0x20, 0x20, 0x20, 0b00000000, 1,    //TILE_CITY_WALLDARK 18
    0x00, 0x00, 0x00, 0x00, 0b00000000, 1,    //TILE_CITY_WALLLIGHT 19
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0, // TILE_CITY_BLACK 20
};

const unsigned char dungeon_metatiles[]={
    0x89, 0x8A, 0x99, 0x9A, 0b01010101, 0, //TILE_DUNGEON_FLOOR 0
    0x10, 0x4B, 0x10, 0x5B, 0b01010101, 1, //TILE_DUNGEON_WALL_L 1
    0x4C, 0x10, 0x5C, 0x10, 0b01010101, 1,  //TILE_DUNGEON_WALL_R 2
    0x10, 0x10, 0x47, 0x48, 0b01010101, 1, //TILE_DUNGEON_WALL_D 3
    0x57, 0x58, 0x10, 0x10, 0b01010101, 1, //TILE_DUNGEON_WALL_U 4
    0x10, 0x10, 0x10, 0x4A, 0b01010101, 1,  //TILE_DUNGEON_CORN_TL 5
    0x10, 0x10, 0x49, 0x10, 0b01010101, 1, //TILE_DUNGEON_CORN_TR 6
    0x10, 0x5A, 0x10, 0x10, 0b01010101, 1, //TILE_DUNGEON_CORN_BL 7
    0x59, 0x10, 0x10, 0x10, 0b01010101, 1, //TILE_DUNGEON_CORN_BR 8
    0x20, 0x20, 0x20, 0x20, 0b01010101, 0, //TILE_DUNGEON_PALE 9
    0x00, 0x00, 0x00, 0x00, 0b01010101, 0, //TILE_DUNGEON_EMPTY 10
    0x00, 0x00, 0x00, 0x00, 0b01010101, 1, //TILE_DUNGEON_SOLID 11
    0x8B, 0x8C, 0x9B, 0x9C, 0b01010101, 1, //TILE_DUNGEON_TREE 12
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0, //TILE_DUNGEON_BLACK 13
};

const unsigned char forest_metatiles[]={
    0x00, 0x00, 0x00, 0x00, 0b01010101, 0,    //TILE_FOREST_FLOOR 0
    0x62, 0x63, 0x72, 0x73, 0b01010101, 1,    //TILE_FOREST_TREES_E 1
    0x64, 0x65, 0x74, 0x75, 0b01010101, 1,    //TILE_FOREST_TREES_CORN_E 2
    0x66, 0x67, 0x76, 0x77, 0b01010101, 1,    //TILE_FOREST_TREES_N 3
    0x80, 0x81, 0x90, 0x91, 0b01010101, 1,    //TILE_FOREST_TREES_S 4
    0x82, 0x83, 0x92, 0x93, 0b01010101, 1,    //TILE_FOREST_TREES_N2 5
    0x84, 0x85, 0x94, 0x95, 0b01010101, 1,    //TILE_FOREST_TREES_CORN_W 6
    0x86, 0x87, 0x96, 0x97, 0b01010101, 1,    //TILE_FOREST_TREES_W 7
    0x00, 0x59, 0x68, 0x69, 0b01010101, 1,    //TILE_FOREST_SHELTER0 8
    0x5A, 0x5B, 0x6A, 0x6B, 0b01010101, 1,    //TILE_FOREST_SHELTER1 9
    0x5C, 0x5D, 0x6C, 0x6D, 0b01010101, 1,    //TILE_FOREST_SHELTER2 10
    0x00, 0x00, 0x6E, 0x00, 0b01010101, 1,    //TILE_FOREST_SHELTER3 11
    0x00, 0x79, 0x88, 0x89, 0b01010101, 1,    //TILE_FOREST_SHELTER4 12
    0x7A, 0x7B, 0x8A, 0x8B, 0b01010101, 1,    //TILE_FOREST_SHELTER5 13
    0x7C, 0x7D, 0x8C, 0x8D, 0b01010101, 1,    //TILE_FOREST_SHELTER6 14
    0x7E, 0x00, 0x8E, 0x00, 0b01010101, 1,    //TILE_FOREST_SHELTER7 15
    0x98, 0x99, 0x00, 0x00, 0b01010101, 0,    //TILE_FOREST_SHELTER8 16
    0x9A, 0x9B, 0x00, 0x00, 0b01010101, 0,    //TILE_FOREST_SHELTER9 17
    0x9C, 0x9D, 0x00, 0x00, 0b01010101, 0,    //TILE_FOREST_SHELTER10 18
    0x9E, 0x00, 0x00, 0x00, 0b01010101, 0,    //TILE_FOREST_SHELTER11 19
    0x00, 0x00, 0x70, 0x00, 0b01010101, 0,    //TILE_FOREST_GRASS 20
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,    //TILE_FOREST_BLACK 21
};

const unsigned char shelter_metatiles[]={
    0x47, 0x48, 0x57, 0x58, 0b01010101, 0, //TILE_SHELTER_FLOOR 0
    0x41, 0x42, 0x51, 0x52, 0b01010101, 1, //TILE_SHELTER_WALL 1
    0x29, 0x2A, 0x39, 0x3A, 0b01010101, 2, //TILE_SHELTER_SPIKES 2
    0x00, 0x00, 0x00, 0x00, 0b01010101, 0, //TILE_SHELTER_EMPTY 3
    0x00, 0x00, 0x00, 0x00, 0b01010101, 1, //TILE_SHELTER_SOLID 4
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0, //TILE_SHELTER_BLACK 5
};

// Format: S, E, N, W exits, then the map tile layout (12x8 metatiles),
// then a running list of entities for the room:
// first, an ID that says what kind of thing it is: 0 = monster, 1 = entrance/exit (like stairs), 2 = sword pickup, 3 = chest
// then an x and a y position
// then 2 subtype ids for monsters, tele locations etc
// then a "unique id" for monsters so we can track which ones are dead-dead
// list terminates if you reach an id of 128
const unsigned char* const environment_metatiles[]={
  desert_metatiles,
  island_metatiles,
  icepalace_metatiles,
  city_metatiles,
  dungeon_metatiles,
  forest_metatiles,
  shelter_metatiles
};

const unsigned char* const* environment_rooms[]={
  desert_rooms,
  island_rooms,
  palace_rooms,
  city_rooms,
  dungeon_rooms,
  shelterforest_rooms,
  shelterrooms,
};

unsigned char solidity_check(unsigned char px, unsigned char py)
{
    // TopLeft
    x = (px+2) >> 4;
    y = (py+2) >> 4;
    if(tilemap_solid(x, y)) return 0;

    // TopRight
    x = (px+14) >> 4;
    y = (py+2) >> 4;
    if(tilemap_solid(x, y)) return 0;

    // BottomLeft
    x = (px+2) >> 4;
    y = (py+14) >> 4;
    if(tilemap_solid(x, y)) return 0;

    // BottomRight
    x = (px+14) >> 4;
    y = (py+14) >> 4;
    if(tilemap_solid(x, y)) return 0;

    return 1;
}

unsigned char bridge_check(unsigned char px, unsigned char py)
{
    x = (px) >> 4;
    y = (py) >> 4;
    if(currentEnvironment == E_ISLAND && currentRoom != 15) // ignores bridges in waterfall area
    {
        return tile_at(x, y) == TILE_I_BRIDGE_DOCK;
    }
    else
    {
        return 0;
    }
}


unsigned char solidity_check_nocactus(unsigned char px, unsigned char py)
{
    // TopLeft
    x = (px+2) >> 4;
    y = (py+2) >> 4;
    if(tilemap_solid_nocactus(x, y)) return 0;

    // TopRight
    x = (px+14) >> 4;
    y = (py+2) >> 4;
    if(tilemap_solid_nocactus(x, y)) return 0;

    // BottomLeft
    x = (px+2) >> 4;
    y = (py+14) >> 4;
    if(tilemap_solid_nocactus(x, y)) return 0;

    // BottomRight
    x = (px+14) >> 4;
    y = (py+14) >> 4;
    if(tilemap_solid_nocactus(x, y)) return 0;

    return 1;
}

unsigned char cactus_check(unsigned char px, unsigned char py)
{
    // TopLeft
    x = (px+4) >> 4;
    y = (py+4) >> 4;
    if(tilemap_ouchie(x, y)) return 0;

    // TopRight
    x = (px+12) >> 4;
    y = (py+4) >> 4;
    if(tilemap_ouchie(x, y)) return 0;

    // BottomLeft
    x = (px+4) >> 4;
    y = (py+12) >> 4;
    if(tilemap_ouchie(x, y)) return 0;

    // BottomRight
    x = (px+14) >> 4;
    y = (py+14) >> 4;
    if(tilemap_ouchie(x, y)) return 0;

    return 1;
}

unsigned char swim_check(unsigned char px, unsigned char py)
{
    // TopLeft
    x = (px+2) >> 4;
    y = (py+2) >> 4;
    if(tilemap_swimmable(x, y)) return 0;

    // TopRight
    x = (px+14) >> 4;
    y = (py+2) >> 4;
    if(tilemap_swimmable(x, y)) return 0;

    // BottomLeft
    x = (px+2) >> 4;
    y = (py+14) >> 4;
    if(tilemap_swimmable(x, y)) return 0;

    // BottomRight
    x = (px+14) >> 4;
    y = (py+14) >> 4;
    if(tilemap_swimmable(x, y)) return 0;

    return 1;
}

unsigned char tile_at(unsigned char tx, unsigned char ty)
{
    x = tx - 2;
    y = ty - 3;
    if(x < 0 || x >= 12) return 255;
    if(y < 0 || y >= 8) return 255;
    i = (x + (y*12)) + 4;
    return unpackedRoom[i];
}

unsigned char tilemap_swimmable(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12)) + 4;
    i = unpackedRoom[i];
    if(currentEnvironment == E_DESERT)
        return i < 15 || i > 23;
    if(currentEnvironment == E_ISLAND)
        return i != TILE_I_WATER && i != TILE_I_WATERFALL_TL && i != TILE_I_WATERFALL_TR;
    return 0;
}

unsigned char tilemap_ouchie(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12)) + 4;
    return metatilesPtr[unpackedRoom[i]*6 + 5] == 2;
}

unsigned char tilemap_solid_nocactus(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12)) + 4;
    return metatilesPtr[unpackedRoom[i]*6 + 5] == 1;
}

unsigned char tilemap_solid(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12)) + 4;
    return metatilesPtr[unpackedRoom[i]*6 + 5];
}

void set_map_tile_on_character(WalkingCharacter* chara, unsigned char tile)
{
    unsigned short ntrAdr = 0;
    if(currentEnvironment != E_DESERT)
    {
        return;
    }
    x = (chara->xpos + 7 >> 4) - 2;
    y = (chara->ypos + 7 >> 4) - 3;
    i = x + (y*12) + 4;
    // Only update tile collisions if the tile to replace was a tree or fern
    if(currentEnvironment == E_DESERT)
    {
        if(unpackedRoom[i] != TILE_D_TREE && unpackedRoom[i] != TILE_D_FERN && unpackedRoom[i] != TILE_D_CACTUS) return;
    }
    unpackedRoom[i] = tile; // UPDATE TILE COLLISIONS
    ntrAdr = NTADR_A((x+2)*2,(y+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr);
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = metatilesPtr[tile*6];
    palmTreeBuffer[3] = MSB(ntrAdr+1);
    palmTreeBuffer[4] = LSB(ntrAdr+1);
    palmTreeBuffer[5] = metatilesPtr[tile*6+1];
    ntrAdr = NTADR_A((x+2)*2,((y+3)*2)+1);
    palmTreeBuffer[6] = MSB(ntrAdr);
    palmTreeBuffer[7] = LSB(ntrAdr);
    palmTreeBuffer[8] = metatilesPtr[tile*6+2];
    palmTreeBuffer[9] = MSB(ntrAdr+1);
    palmTreeBuffer[10] = LSB(ntrAdr+1);
    palmTreeBuffer[11] = metatilesPtr[tile*6+3];
    palmTreeBuffer[12] = NT_UPD_EOF;
    set_vram_update(palmTreeBuffer);
    writingVram = 1;

}

void set_map_tile_in_room(unsigned char tx, unsigned char ty, unsigned char tile)
{
    unsigned short ntrAdr = 0;
    unpackedRoom[tx + (ty*12) + 4] = tile; // UPDATE TILE COLLISIONS
    ntrAdr = NTADR_A((tx+2)*2,(ty+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr);
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = metatilesPtr[tile*6];
    palmTreeBuffer[3] = MSB(ntrAdr+1);
    palmTreeBuffer[4] = LSB(ntrAdr+1);
    palmTreeBuffer[5] = metatilesPtr[tile*6+1];
    ntrAdr = NTADR_A((tx+2)*2,((ty+3)*2)+1);
    palmTreeBuffer[6] = MSB(ntrAdr);
    palmTreeBuffer[7] = LSB(ntrAdr);
    palmTreeBuffer[8] = metatilesPtr[tile*6+2];
    palmTreeBuffer[9] = MSB(ntrAdr+1);
    palmTreeBuffer[10] = LSB(ntrAdr+1);
    palmTreeBuffer[11] = metatilesPtr[tile*6+3];
    palmTreeBuffer[12] = NT_UPD_EOF;
    set_vram_update(palmTreeBuffer);
    writingVram = 1;
}

void draw_black_tile_in_room(unsigned char tx, unsigned char ty)
{
    tx += 2;
    ty += 3;

    if(currentEnvironment == E_DESERT)
    {
        set_map_tile_in_room(tx-2, ty-3, TILE_D_BLACK);
    }
    else if(currentEnvironment == E_ISLAND)
    {
        set_map_tile_in_room(tx-2, ty-3, TILE_I_BLACK);
    }
    attrib_addr = 0x23C0 + ((ty)/2) * 8 + ((tx)/2);
    palmTreeBuffer[12] = MSB(attrib_addr);
    palmTreeBuffer[13] = LSB(attrib_addr);
    palmTreeBuffer[14] = 0b00000000;
    palmTreeBuffer[15] = NT_UPD_EOF;
    set_vram_update(palmTreeBuffer);
}

// Ice palace room locking and unlocking;
// simply replace the entrances with appropriate door tiles.
void lock_room_doors()
{
    // Some rooms in the ice palace lock their doors until the player completes a puzzle or kill all monsters
    unsigned short ntrAdr = 0;
    unsigned char shouldLock = 0;
    if(currentEnvironment == E_ICEPALACE)
    {
        if(currentRoom == 1 && spawnedMonsters > 0)
        {
            shouldLock = 1;
        }
        if(spawnedButtons > 0 && spawnedMonsters > 0)
        {
            shouldLock = 1;
        }
    }

    if(shouldLock == 0) return;


    // first store the tiles before we replace them
    doorLockMemory[0] = tile_at(0+2, 3+3);
    doorLockMemory[1] = tile_at(0+2, 4+3);
    doorLockMemory[2] = tile_at(5+2, 0+3);
    doorLockMemory[3] = tile_at(6+2, 0+3);
    doorLockMemory[4] = tile_at(5+2, 7+3);
    doorLockMemory[5] = tile_at(6+2, 7+3);
    doorLockMemory[6] = tile_at(11+2, 3+3);
    doorLockMemory[7] = tile_at(11+2, 4+3);

    // Left side (right-facing doors, vert)
    ntrAdr = NTADR_A((0+2)*2, (3+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[TILE_IP_PUZDOOR_L*6];
    palmTreeBuffer[4] = metatilesPtr[TILE_IP_PUZDOOR_L*6+2];
    palmTreeBuffer[5] = metatilesPtr[TILE_IP_PUZDOOR_L*6];
    palmTreeBuffer[6] = metatilesPtr[TILE_IP_PUZDOOR_L*6+2];
    ntrAdr = NTADR_A((0+2)*2 + 1, ((3+3)*2));
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[TILE_IP_PUZDOOR_L*6+1];
    palmTreeBuffer[11] = metatilesPtr[TILE_IP_PUZDOOR_L*6+3];
    palmTreeBuffer[12] = metatilesPtr[TILE_IP_PUZDOOR_L*6+1];
    palmTreeBuffer[13] = metatilesPtr[TILE_IP_PUZDOOR_L*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi(); // wait a frame, then do the next bit


    // Top Side (door pair, horz)
    ntrAdr = NTADR_A((5+2)*2, (0+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[TILE_IP_PUZDOOR_L*6];
    palmTreeBuffer[4] = metatilesPtr[TILE_IP_PUZDOOR_L*6+1];
    palmTreeBuffer[5] = metatilesPtr[TILE_IP_PUZDOOR_R*6];
    palmTreeBuffer[6] = metatilesPtr[TILE_IP_PUZDOOR_R*6+1];
    ntrAdr = NTADR_A(((5+2)*2), (0+3)*2 + 1);
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[TILE_IP_PUZDOOR_L*6+2];
    palmTreeBuffer[11] = metatilesPtr[TILE_IP_PUZDOOR_L*6+3];
    palmTreeBuffer[12] = metatilesPtr[TILE_IP_PUZDOOR_R*6+2];
    palmTreeBuffer[13] = metatilesPtr[TILE_IP_PUZDOOR_R*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi(); // wait a frame, then do the next bit

    // Bottom Side (door pair, horz)
    ntrAdr = NTADR_A((5+2)*2, (7+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[TILE_IP_PUZDOOR_L*6];
    palmTreeBuffer[4] = metatilesPtr[TILE_IP_PUZDOOR_L*6+1];
    palmTreeBuffer[5] = metatilesPtr[TILE_IP_PUZDOOR_R*6];
    palmTreeBuffer[6] = metatilesPtr[TILE_IP_PUZDOOR_R*6+1];
    ntrAdr = NTADR_A(((5+2)*2), (7+3)*2 + 1);
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[TILE_IP_PUZDOOR_L*6+2];
    palmTreeBuffer[11] = metatilesPtr[TILE_IP_PUZDOOR_L*6+3];
    palmTreeBuffer[12] = metatilesPtr[TILE_IP_PUZDOOR_R*6+2];
    palmTreeBuffer[13] = metatilesPtr[TILE_IP_PUZDOOR_R*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi(); // wait a frame, then do the next bit

    // Right side (left-facing doors, vert)
    ntrAdr = NTADR_A((11+2)*2, (3+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[TILE_IP_PUZDOOR_R*6];
    palmTreeBuffer[4] = metatilesPtr[TILE_IP_PUZDOOR_R*6+2];
    palmTreeBuffer[5] = metatilesPtr[TILE_IP_PUZDOOR_R*6];
    palmTreeBuffer[6] = metatilesPtr[TILE_IP_PUZDOOR_R*6+2];
    ntrAdr = NTADR_A((11+2)*2 + 1, ((3+3)*2));
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[TILE_IP_PUZDOOR_R*6+1];
    palmTreeBuffer[11] = metatilesPtr[TILE_IP_PUZDOOR_R*6+3];
    palmTreeBuffer[12] = metatilesPtr[TILE_IP_PUZDOOR_R*6+1];
    palmTreeBuffer[13] = metatilesPtr[TILE_IP_PUZDOOR_R*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi();
    set_vram_update(NULL);

    roomLocked = 1; // set lock flag so that buttons/monster deaths can open it

    // set collisions
    unpackedRoom[0 + (3*12) + 4] = TILE_IP_PUZDOOR_L;
    unpackedRoom[0 + (4*12) + 4] = TILE_IP_PUZDOOR_L;
    unpackedRoom[5 + (0*12) + 4] = TILE_IP_PUZDOOR_L;
    unpackedRoom[6 + (0*12) + 4] = TILE_IP_PUZDOOR_R;
    unpackedRoom[5 + (7*12) + 4] = TILE_IP_PUZDOOR_L;
    unpackedRoom[6 + (7*12) + 4] = TILE_IP_PUZDOOR_R;
    unpackedRoom[11 + (3*12) + 4] = TILE_IP_PUZDOOR_R;
    unpackedRoom[11 + (4*12) + 4] = TILE_IP_PUZDOOR_R;

    // move kris so they won't get trapped in the doors
    if(roomSwitchDir == 0) kris.ypos += 16;
    else if(roomSwitchDir == 1) kris.xpos += 16;
    else if(roomSwitchDir == 2) kris.ypos -= 16;
    else if(roomSwitchDir == 3) kris.xpos -= 16;
}

void unlock_room_doors()
{
    unsigned short ntrAdr = 0;
    // Switch the doors back to what they are supposed to be, using doorLockMemory
    unpackedRoom[0 + (3*12) + 4] = doorLockMemory[0];
    unpackedRoom[0 + (4*12) + 4] = doorLockMemory[1];
    unpackedRoom[5 + (0*12) + 4] = doorLockMemory[2];
    unpackedRoom[6 + (0*12) + 4] = doorLockMemory[3];
    unpackedRoom[5 + (7*12) + 4] = doorLockMemory[4];
    unpackedRoom[6 + (7*12) + 4] = doorLockMemory[5];
    unpackedRoom[11 + (3*12) + 4] = doorLockMemory[6];
    unpackedRoom[11 + (4*12) + 4] = doorLockMemory[7];


   // Left side (right-facing doors, vert)
    ntrAdr = NTADR_A((0+2)*2, (3+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[doorLockMemory[0]*6];
    palmTreeBuffer[4] = metatilesPtr[doorLockMemory[0]*6+2];
    palmTreeBuffer[5] = metatilesPtr[doorLockMemory[1]*6];
    palmTreeBuffer[6] = metatilesPtr[doorLockMemory[1]*6+2];
    ntrAdr = NTADR_A((0+2)*2 + 1, ((3+3)*2));
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[doorLockMemory[0]*6+1];
    palmTreeBuffer[11] = metatilesPtr[doorLockMemory[0]*6+3];
    palmTreeBuffer[12] = metatilesPtr[doorLockMemory[1]*6+1];
    palmTreeBuffer[13] = metatilesPtr[doorLockMemory[1]*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi(); // wait a frame, then do the next bit


    // Top Side (door pair, horz)
    ntrAdr = NTADR_A((5+2)*2, (0+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[doorLockMemory[2]*6];
    palmTreeBuffer[4] = metatilesPtr[doorLockMemory[2]*6+1];
    palmTreeBuffer[5] = metatilesPtr[doorLockMemory[3]*6];
    palmTreeBuffer[6] = metatilesPtr[doorLockMemory[3]*6+1];
    ntrAdr = NTADR_A(((5+2)*2), (0+3)*2 + 1);
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[doorLockMemory[2]*6+2];
    palmTreeBuffer[11] = metatilesPtr[doorLockMemory[2]*6+3];
    palmTreeBuffer[12] = metatilesPtr[doorLockMemory[3]*6+2];
    palmTreeBuffer[13] = metatilesPtr[doorLockMemory[3]*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi(); // wait a frame, then do the next bit

    // Bottom Side (door pair, horz)
    ntrAdr = NTADR_A((5+2)*2, (7+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[doorLockMemory[4]*6];
    palmTreeBuffer[4] = metatilesPtr[doorLockMemory[4]*6+1];
    palmTreeBuffer[5] = metatilesPtr[doorLockMemory[5]*6];
    palmTreeBuffer[6] = metatilesPtr[doorLockMemory[5]*6+1];
    ntrAdr = NTADR_A(((5+2)*2), (7+3)*2 + 1);
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_HORZ;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[doorLockMemory[4]*6+2];
    palmTreeBuffer[11] = metatilesPtr[doorLockMemory[4]*6+3];
    palmTreeBuffer[12] = metatilesPtr[doorLockMemory[5]*6+2];
    palmTreeBuffer[13] = metatilesPtr[doorLockMemory[5]*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi(); // wait a frame, then do the next bit

    // Right side (left-facing doors, vert)
    ntrAdr = NTADR_A((11+2)*2, (3+3)*2);
    palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[1] = LSB(ntrAdr);
    palmTreeBuffer[2] = 4;
    palmTreeBuffer[3] = metatilesPtr[doorLockMemory[6]*6];
    palmTreeBuffer[4] = metatilesPtr[doorLockMemory[6]*6+2];
    palmTreeBuffer[5] = metatilesPtr[doorLockMemory[7]*6];
    palmTreeBuffer[6] = metatilesPtr[doorLockMemory[7]*6+2];
    ntrAdr = NTADR_A((11+2)*2 + 1, ((3+3)*2));
    palmTreeBuffer[7] = MSB(ntrAdr) | NT_UPD_VERT;
    palmTreeBuffer[8] = LSB(ntrAdr);
    palmTreeBuffer[9] = 4;
    palmTreeBuffer[10] = metatilesPtr[doorLockMemory[6]*6+1];
    palmTreeBuffer[11] = metatilesPtr[doorLockMemory[6]*6+3];
    palmTreeBuffer[12] = metatilesPtr[doorLockMemory[7]*6+1];
    palmTreeBuffer[13] = metatilesPtr[doorLockMemory[7]*6+3];
    palmTreeBuffer[14] = NT_UPD_EOF;

    set_vram_update(palmTreeBuffer);
    ppu_wait_nmi();
    set_vram_update(NULL);

    roomLocked = 0;
}

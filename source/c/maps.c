#include "maps.h"
#include "globals.h"
#include "neslib.h"
#include "actors.h"
#include "desert_maps.h"
#include "island_maps.h"
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
    0x00, 0x00, 0x00, 0x00, 0b01010101, 0,  /* TILE_I_EMPTYWALK 26 */
    0x00, 0x00, 0x00, 0x00, 0b01010101, 1,  /* TILE_I_EMPTYFULL 27 */
    0x81, 0x82, 0x91, 0x92, 0b10101010, 1,  /* TILE_I_DELTDOOR_TL 28 */
    0x83, 0x84, 0x93, 0x94, 0b10101010, 1,  /* TILE_I_DELTDOOR_TR 29 */
    0x85, 0x86, 0x95, 0x96, 0b10101010, 1,  /* TILE_I_DELTDOOR_BL 30 */
    0x87, 0x88, 0x97, 0x98, 0b10101010, 1,  /* TILE_I_DELTDOOR_BR 31 */
    0x4B, 0x10, 0x5B, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_L 32 */
    0x10, 0x4C, 0x10, 0x5C, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_R 33 */
    0x47, 0x48, 0x10, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_D 34 */
    0x10, 0x10, 0x57, 0x58, 0b10101010, 1,  /* TILE_I_DUNGEONWALL_U 35 */
    0x4A, 0x48, 0x5B, 0x10, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_TL 36 */
    0x47, 0x49, 0x10, 0x5C, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_TR 37 */
    0x4B, 0x10, 0x5A, 0x58, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_BL 38 */
    0x10, 0x4C, 0x59, 0x57, 0b10101010, 1,  /* TILE_I_DUNGEONCORN_BR 39 */
    0x67, 0x68, 0x77, 0x78, 0b10101010, 0,  /* TILE_I_WATERFALL_TL 40 */
    0x69, 0x6A, 0x79, 0x7A, 0b10101010, 0,  /* TILE_I_WATERFALL_TR 41 */
    0x65, 0x66, 0x75, 0x76, 0b10101010, 0,  /* TILE_I_SMALLDOOR 42 */
/* TILE_I_WBRIDGE_L 43 */
/* TILE_I_WBRIDGE_R 44 */
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
    0x10, 0x10, 0x10, 0x10, 0b00000000, 0,
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
  island_metatiles
};

const unsigned char* const* environment_rooms[]={
  desert_rooms,
  island_rooms
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
    if(currentEnvironment == E_ISLAND)
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
    x = (px+12) >> 4;
    y = (py+12) >> 4;
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
    i = (x + (y*12));
    return currentRoomColl[i];
}

unsigned char tilemap_swimmable(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12));
    i = currentRoomColl[i];
    return i < 15 || i > 23;
}

unsigned char tilemap_ouchie(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12));
    return metatilesPtr[currentRoomColl[i]*6 + 5] == 2;
}

unsigned char tilemap_solid_nocactus(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12));
    return metatilesPtr[currentRoomColl[i]*6 + 5] == 1;
}

unsigned char tilemap_solid(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 1;
    if(y < 0 || y >= 8) return 1;
    i = (x + (y*12));
    return metatilesPtr[currentRoomColl[i]*6 + 5];
}

void set_map_tile_on_character(WalkingCharacter* chara, unsigned char tile)
{
    unsigned short ntrAdr = 0;
    if(currentEnvironment == E_ISLAND)
    {
        return;
    }
    x = (chara->xpos + 7 >> 4) - 2;
    y = (chara->ypos + 7 >> 4) - 3;
    i = x + (y*12);
    // Only update tile collisions if the tile to replace was a tree or fern
    if(currentEnvironment == E_DESERT)
    {
        if(currentRoomColl[i] != TILE_D_TREE && currentRoomColl[i] != TILE_D_FERN && currentRoomColl[i] != TILE_D_CACTUS) return;
    }
    currentRoomColl[i] = tile; // UPDATE TILE COLLISIONS
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
    currentRoomColl[tx + (ty*12)] = tile; // UPDATE TILE COLLISIONS
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

}


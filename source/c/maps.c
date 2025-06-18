#include "maps.h"
#include "globals.h"
#include "neslib.h"

// Format: 4 8x8 tiles that make up this metatile, and palette mask for attrib (actual mask differs based on tile pos)
// then, tile solidity type (0 = walkable, 1 = not walkable)
const unsigned char desert_metatiles[]={
   0x01, 0x02, 0x11, 0x12, 0b01010101, 0,
   0x03, 0x04, 0x13, 0x14, 0b01010101, 1,
   0x0B, 0x0C, 0x1B, 0x1C, 0b01010101, 0,
   0x29, 0x2A, 0x39, 0x3A, 0b01010101, 1,
   0x2B, 0x2C, 0x3B, 0x3C, 0b01010101, 1
};

// Format: S, E, N, W exits, then the map tile layout (12x8 metatiles),
// then a running list of entities for the room:
// first, an ID that says what kind of thing it is: 0 = monster, 1 = entrance/exit (like stairs), 2 = sword pickup
// then an x and a y position
// then a subtype id for monsters etc
// then a "unique id" for monsters so we can track which ones are dead-dead
// list terminates if you reach an id of 128
const unsigned char desert_room_start[]={
  1, 0, 0, 0,
  3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
  4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
  128
};

const unsigned char desert_room_movetest[]={
  1, 0, 0, 0,
  1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
  2, 5, 6, 0, 0,
  128
};

const unsigned char* desert_rooms[]={
  desert_room_start,
  desert_room_movetest
};

const unsigned char* environment_metatiles[]={
  desert_metatiles
};

const unsigned char** environment_rooms[]={
  desert_rooms
};

int solidity_check(unsigned char px, unsigned char py)
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

int tilemap_solid(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 0;
    if(y < 0 || y >= 8) return 0;
    i = (x + (y*12)) + ROOM_DATA_OFFSET;
    return tile_solid(environment_rooms[currentEnvironment][currentRoom][i]);
}

int tile_solid(unsigned char tile)
{
    return environment_metatiles[currentEnvironment][tile*6 + 5];
}

void set_map_tile_in_room(unsigned char tx, unsigned char ty, unsigned char tile)
{
    // TODO: This makes the screen flicker every time you stab a tree which kind of Sucks; this should happen during vblank instead.
    // Need to use set_vram_update to DMA some memory over to the ppu instead, and clone the map data on room load to make it modifiable.
    //ppu_off();
    //i = tx + (ty*12) + ROOM_DATA_OFFSET;
    //environment_rooms[currentEnvironment][currentRoom][i] = tile;
    //vram_adr(NTADR_A((x+2)*2,(y+3)*2));
    //vram_put(environment_metatiles[currentEnvironment][(tile*6)]);
    //vram_put(environment_metatiles[currentEnvironment][(tile*6)+1]);
    //vram_adr(NTADR_A((x+2)*2,((y+3)*2+1)));
    //vram_put(environment_metatiles[currentEnvironment][(tile*6)+2]);
    //vram_put(environment_metatiles[currentEnvironment][(tile*6)+3]);

    //set_palette_for_bg_tile(x+2, y+3, environment_metatiles[currentEnvironment][(tile*6)+4]);
    //ppu_on_all();
}

#include "maps.h"
#include "globals.h"

// Format: 4 8x8 tiles that make up this metatile, and palette mask for attrib (actual mask differs based on tile pos)
const unsigned char desert_metatiles[]={
   0x01, 0x02, 0x11, 0x12, 0b01010101,
   0x03, 0x04, 0x13, 0x14, 0b01010101,
   0x0B, 0x0C, 0x1B, 0x1C, 0b01010101,
   0x29, 0x2A, 0x39, 0x3A, 0b01010101,
   0x2B, 0x2C, 0x3B, 0x3C, 0b01010101
};

// Format: S, E, N, W exits, then the map tile layout (12x8 metatiles)
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
    i = (x + (y*12)) + 4;
    return tile_solid(environment_rooms[currentEnvironment][currentRoom][i]);
}

int tile_solid(unsigned char tile)
{
    return tile == 1 || tile == 3 || tile == 4;
}

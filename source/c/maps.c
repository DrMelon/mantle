#include "maps.h"
#include "globals.h"
#include "neslib.h"
#include "actors.h"

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
  0, 3, 2, 0, 1,
  0, 4, 2, 0, 2,
  0, 8, 2, 0, 3,
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
    i = (x + (y*12));
    return metatilesPtr[currentRoomColl[i]*6 + 5];
}

void set_map_tile_on_character(WalkingCharacter* chara, unsigned char tile)
{
    unsigned short ntrAdr = 0;
    x = (chara->xpos + 7 >> 4) - 2;
    y = (chara->ypos + 7 >> 4) - 3;
    i = x + (y*12);
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
    i = tx + (ty*12);
    currentRoomColl[i] = tile; // UPDATE TILE COLLISIONS
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

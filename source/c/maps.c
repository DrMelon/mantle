#include "maps.h"
#include "globals.h"
#include "neslib.h"
#include "actors.h"
#include "desert_maps.h"

// Format: 4 8x8 tiles that make up this metatile, and palette mask for attrib (actual mask differs based on tile pos)
// then, tile solidity type (0 = walkable, 1 = not walkable)
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
   0x31, 0x32, 0x32, 0x31, 0b11111111, 0,
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
   0x07, 0x08, 0x17, 0x18, 0b01010101, 1,
   0x00, 0x00, 0x00, 0x00, 0b01010101, 0,
   0x00, 0x00, 0x00, 0x00, 0b01010101, 1,
   0x23, 0x24, 0x33, 0x34, 0b01010101, 0,
   0x0F, 0x0F, 0x1F, 0x1F, 0b10101010, 0,
   0x25, 0x26, 0x35, 0x36, 0b01010101, 1,
};

// Format: S, E, N, W exits, then the map tile layout (12x8 metatiles),
// then a running list of entities for the room:
// first, an ID that says what kind of thing it is: 0 = monster, 1 = entrance/exit (like stairs), 2 = sword pickup
// then an x and a y position
// then a subtype id for monsters etc
// then a "unique id" for monsters so we can track which ones are dead-dead
// list terminates if you reach an id of 128
const unsigned char desert_room_start_test[]={
  1, 0, 0, 0,
4,3,4,3,4,3,4,3,4,3,4,3,
3,4,3,4,3,5,6,4,3,4,3,4,
4,3,4,3,4,13,13,3,4,5,6,3,
3,4,3,4,3,13,13,4,3,7,8,4,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,0,0,1,1,1,1,1,
  0, 5, 5, 0, 1,
  0, 4, 5, 0, 2,
  0, 9, 5, 0, 3,
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

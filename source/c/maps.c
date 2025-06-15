#include "maps.h"

// Format: 4 8x8 tiles that make up this metatile, and palette mask for attrib (actual mask differs based on tile pos)
const unsigned char desert_metatiles[]={
   0x01, 0x02, 0x11, 0x12, 0b01010101,
   0x03, 0x04, 0x13, 0x14, 0b01010101,
   0x0B, 0x0C, 0x1B, 0x1C, 0b01010101,
   0x29, 0x2A, 0x39, 0x3A, 0b01010101,
   0x2B, 0x2C, 0x3B, 0x3C, 0b01010101
};

// Format: S, E, N, W exits, then the map tile layout (12x8 metatiles)
extern const unsigned char desert_room_0[]={
  0, 0, 0, 0,
  3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
  4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
};

#include "neslib.h";
#include "bank_helpers.h";

// Kris metasprites
const unsigned char krisWalkDown0[]={
    0, 0, 0x01, 4,
    8, 0, 0x02, 4,
    0, 8, 0x03, 4,
    8, 8, 0x04, 4,
    128
};
const unsigned char krisWalkDown1[]={
    0, 0, 0x01, 4,
    8, 0, 0x02, 4,
    0, 8, 0x05, 4,
    8, 8, 0x06, 4,
    128
};
const unsigned char krisWalkRight0[]={
    0, 0, 0x07, 4,
    8, 0, 0x08, 4,
    0, 8, 0x09, 4,
    8, 8, 0x0A, 4,
    128
};
const unsigned char krisWalkRight1[]={
    0, 1, 0x07, 4,
    8, 1, 0x08, 4,
    1, 9, 0x0B, 4,
    9, 9, 0x0C, 4,
    128
};
const unsigned char krisWalkUp0[]={
    0, 0, 0x11, 4,
    8, 0, 0x12, 4,
    0, 8, 0x13, 4,
    8, 8, 0x14, 4,
    128
};
const unsigned char krisWalkUp1[]={
    0, 0, 0x11, 4,
    8, 0, 0x12, 4,
    0, 8, 0x15, 4,
    8, 8, 0x16, 4,
    128
};
const unsigned char krisWalkLeft0[]={
    0, 0, 0x17, 4,
    8, 0, 0x18, 4,
    -1, 8, 0x19, 4,
    7, 8, 0x1A, 4,
    128
};
const unsigned char krisWalkLeft1[]={
    0, 1, 0x17, 4,
    8, 1, 0x18, 4,
    0, 9, 0x1B, 4,
    8, 9, 0x1C, 4,
    128
};


const unsigned char* const krisWalkAnims[]={
    krisWalkDown0,
    krisWalkDown1,
    krisWalkRight0,
    krisWalkRight1,
    krisWalkUp0,
    krisWalkUp1,
    krisWalkLeft0,
    krisWalkLeft1,
};

const unsigned char krisStrikeDown0[]={
   0, 1, 0x01, 4,
   8, 1, 0x02, 4,
   0, 9, 0x21, 4,
   8, 9, 0x22, 4,
   128
};

const unsigned char krisStrikeDown1[]={
   0, 1, 0x01, 4,
   8, 1, 0x02, 4,
   0, 9, 0x23, 4,
   8, 9, 0x22, 4,
   0, 17, 0x33, 4,
   0, 25, 0x43, 4,
   128
};

const unsigned char krisStrikeDown2[]={
   0, 1, 0x01, 4,
   8, 1, 0x02, 4,
   0, 9, 0x24, 4,
   8, 9, 0x22, 4,
   0, 17, 0x43, 4,
   128
};

const unsigned char krisStrikeRight0[]={
    0, 1, 0x29, 4,
    8, 1, 0x2A, 4,
    0, 9, 0x39, 4,
    8, 9, 0x3A, 4,
    128
};

const unsigned char krisStrikeRight1[]={
    0, 1, 0x29, 4,
    8, 1, 0x2A, 4,
    0, 9, 0x39, 4,
    8, 9, 0x3A, 4,
    13, 8, 0x2B, 4,
    21, 8, 0x2C, 4,
    128
};

const unsigned char krisStrikeRight2[]={
    3, 8, 0x2B, 4,
    11, 8, 0x2C, 4,
    0, 1, 0x29, 4,
    8, 1, 0x2A, 4,
    0, 9, 0x39, 4,
    8, 9, 0x3A, 4,
    128
};

const unsigned char krisStrikeUp0[]={
    0, 1, 0x25, 4,
    8, 1, 0x26, 4,
    0, 9, 0x35, 4,
    8, 9, 0x36, 4,
    128
};

const unsigned char krisStrikeUp1[]={
    0, 1, 0x25, 4,
    8, 1, 0x26, 4,
    0, 9, 0x35, 4,
    8, 9, 0x36, 4,
    8, -6, 0x37, 4,
    8, -14, 0x27, 4,
    128
};

const unsigned char krisStrikeUp2[]={
    0, 1, 0x25, 4,
    8, 1, 0x26, 4,
    0, 9, 0x35, 4,
    8, 9, 0x36, 4,
    8, -5, 0x28, 4,
    128
};

const unsigned char krisStrikeLeft0[]={
    8, 1, 0x29, 4 | OAM_FLIP_H,
    0, 1, 0x2A, 4 | OAM_FLIP_H,
    8, 9, 0x39, 4 | OAM_FLIP_H,
    0, 9, 0x3A, 4 | OAM_FLIP_H,
    128
};

const unsigned char krisStrikeLeft1[]={
    8, 1, 0x29, 4 | OAM_FLIP_H,
    0, 1, 0x2A, 4 | OAM_FLIP_H,
    8, 9, 0x39, 4 | OAM_FLIP_H,
    0, 9, 0x3A, 4 | OAM_FLIP_H,
    8-13, 8, 0x2B, 4 | OAM_FLIP_H,
    8-21, 8, 0x2C, 4 | OAM_FLIP_H,
    128
};

const unsigned char krisStrikeLeft2[]={
    8-3, 8, 0x2B, 4 | OAM_FLIP_H,
    8-11, 8, 0x2C, 4 | OAM_FLIP_H,
    8, 1, 0x29, 4 | OAM_FLIP_H,
    0, 1, 0x2A, 4 | OAM_FLIP_H,
    8, 9, 0x39, 4 | OAM_FLIP_H,
    0, 9, 0x3A, 4 | OAM_FLIP_H,
    128
};


const unsigned char* const krisStrikeAnims[]={
    krisStrikeDown0,
    krisStrikeDown1,
    krisStrikeDown2,
    krisStrikeRight0,
    krisStrikeRight1,
    krisStrikeRight2,
    krisStrikeUp0,
    krisStrikeUp1,
    krisStrikeUp2,
    krisStrikeLeft0,
    krisStrikeLeft1,
    krisStrikeLeft2
};

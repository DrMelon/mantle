
#include "neslib.h"
#include "ralsei_anims.h"
#include "bank_helpers.h"
#include "globals.h"

CODE_BANK(KRIS_ANIMS_BANK);
const unsigned char ralseiWalkDown0[]={
  0, 0, 0xC3, 2,
  8, 0, 0xC4, 2,
  0, 8, 0xD3, 2,
  8, 8, 0xD4, 2,
  128
};

const unsigned char ralseiWalkDown1[]={
  0, 0, 0xC3, 2,
  8, 0, 0xC4, 2,
  0, 8, 0xD5, 2,
  8, 8, 0xD6, 2,
  128
};

const unsigned char ralseiWalkRight0[]={
  0, 0, 0xCA, 2 | OAM_FLIP_H,
  8, 0, 0xC9, 2 | OAM_FLIP_H,
  0, 8, 0xDA, 2 | OAM_FLIP_H,
  8, 8, 0xD9, 2 | OAM_FLIP_H,
  128
};

const unsigned char ralseiWalkRight1[]={
  0, 0, 0xCC, 2 | OAM_FLIP_H,
  8, 0, 0xCB, 2 | OAM_FLIP_H,
  0, 8, 0xDC, 2 | OAM_FLIP_H,
  8, 8, 0xDB, 2 | OAM_FLIP_H,
  128
};

const unsigned char ralseiWalkUp0[]={
  0, 0, 0xC5, 2,
  8, 0, 0xC6, 2,
  0, 8, 0xC7, 2,
  8, 8, 0xC8, 2,
  128
};

const unsigned char ralseiWalkUp1[]={
  0, 0, 0xC5, 2,
  8, 0, 0xC6, 2,
  0, 8, 0xD7, 2,
  8, 8, 0xD8, 2,
  128
};

const unsigned char ralseiWalkLeft0[]={
  0, 0, 0xC9, 2,
  8, 0, 0xCA, 2,
  0, 8, 0xD9, 2,
  8, 8, 0xDA, 2,
  128
};

const unsigned char ralseiWalkLeft1[]={
  0, 0, 0xCB, 2,
  8, 0, 0xCC, 2,
  0, 8, 0xDB, 2,
  8, 8, 0xDC, 2,
  128
};

const unsigned char* const ralseiWalkAnims[]={
  ralseiWalkDown0,
  ralseiWalkDown1,
  ralseiWalkRight0,
  ralseiWalkRight1,
  ralseiWalkUp0,
  ralseiWalkUp1,
  ralseiWalkLeft0,
  ralseiWalkLeft1
};

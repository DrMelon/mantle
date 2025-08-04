
#include "neslib.h"
#include "susie_anims.h"
#include "bank_helpers.h"
#include "globals.h"

CODE_BANK(KRIS_ANIMS_BANK);
// it's kinda funny how susie uses the same palette index as noelle
const unsigned char susieWalkDown0[]={
  0, 0, 0xE0, 1,
  8, 0, 0xE1, 1,
  0, 8, 0xF0, 1,
  8, 8, 0xF1, 1,
  128
};

const unsigned char susieWalkDown1[]={
  0, 0, 0xE0, 1,
  8, 0, 0xE1, 1,
  0, 8, 0xF2, 1,
  8, 8, 0xF3, 1,
  128
};

const unsigned char susieWalkRight0[]={
  0, 0, 0xE7, 1 | OAM_FLIP_H,
  8, 0, 0xE6, 1 | OAM_FLIP_H,
  0, 8, 0xF7, 1 | OAM_FLIP_H,
  8, 8, 0xF6, 1 | OAM_FLIP_H,
  128
};

const unsigned char susieWalkRight1[]={
  0, 0, 0xE9, 1 | OAM_FLIP_H,
  8, 0, 0xE8, 1 | OAM_FLIP_H,
  0, 8, 0xF9, 1 | OAM_FLIP_H,
  8, 8, 0xF8, 1 | OAM_FLIP_H,
  128
};

const unsigned char susieWalkUp0[]={
  0, 0, 0xE4, 1,
  8, 0, 0xE5, 1,
  0, 8, 0xF4, 1,
  8, 8, 0xF5, 1,
  128
};

const unsigned char susieWalkUp1[]={
  0, 0, 0xE4, 1,
  8, 0, 0xE5, 1,
  0, 8, 0xF5, 1 | OAM_FLIP_H,
  8, 8, 0xF4, 1 | OAM_FLIP_H,
  128
};

const unsigned char susieWalkLeft0[]={
  0, 0, 0xE6, 1,
  8, 0, 0xE7, 1,
  0, 8, 0xF6, 1,
  8, 8, 0xF7, 1,
  128
};

const unsigned char susieWalkLeft1[]={
  0, 0, 0xE8, 1,
  8, 0, 0xE9, 1,
  0, 8, 0xF8, 1,
  8, 8, 0xF9, 1,
  128
};

const unsigned char* const susieWalkAnims[]={
  susieWalkDown0,
  susieWalkDown1,
  susieWalkRight0,
  susieWalkRight1,
  susieWalkUp0,
  susieWalkUp1,
  susieWalkLeft0,
  susieWalkLeft1
};

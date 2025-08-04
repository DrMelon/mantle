
#include "neslib.h"
#include "noelle_anims.h"
#include "bank_helpers.h"
#include "globals.h"

CODE_BANK(KRIS_ANIMS_BANK);
const unsigned char noelleWalkDown0[]={
  0, 0, 0x6E, 1,
  8, 0, 0x6F, 1,
  0, 8, 0x7E, 1,
  8, 8, 0x7F, 1,
  128
};

const unsigned char noelleWalkDown1[]={
  0, 0, 0x6F, 1 | OAM_FLIP_H,
  8, 0, 0x6E, 1 | OAM_FLIP_H,
  0, 8, 0x7F, 1 | OAM_FLIP_H,
  8, 8, 0x7E, 1 | OAM_FLIP_H,
  128
};

const unsigned char noelleWalkRight0[]={
  0, 0, 0x8F, 1 | OAM_FLIP_H,
  8, 0, 0x8E, 1 | OAM_FLIP_H,
  0, 8, 0x9F, 1 | OAM_FLIP_H,
  8, 8, 0x9E, 1 | OAM_FLIP_H,
  128
};

const unsigned char noelleWalkRight1[]={
  0, 2, 0x8F, 1 | OAM_FLIP_H,
  8, 2, 0x8E, 1 | OAM_FLIP_H,
  0, 9, 0x9F, 1 | OAM_FLIP_H,
  8, 9, 0x9E, 1 | OAM_FLIP_H,
  128
};

const unsigned char noelleWalkUp0[]={
  0, 0, 0xAE, 1,
  8, 0, 0xAF, 1,
  0, 8, 0xBE, 1,
  8, 8, 0xBF, 1,
  128
};

const unsigned char noelleWalkUp1[]={
  0, 0, 0xAF, 1 | OAM_FLIP_H,
  8, 0, 0xAE, 1 | OAM_FLIP_H,
  0, 8, 0xBF, 1 | OAM_FLIP_H,
  8, 8, 0xBE, 1 | OAM_FLIP_H,
  128
};

const unsigned char noelleWalkLeft0[]={
  0, 0, 0x8E, 1,
  8, 0, 0x8F, 1,
  0, 8, 0x9E, 1,
  8, 8, 0x9F, 1,
  128
};

const unsigned char noelleWalkLeft1[]={
  0, 2, 0x8E, 1,
  8, 2, 0x8F, 1,
  0, 9, 0x9E, 1,
  8, 9, 0x9F, 1,
  128
};

const unsigned char* const noelleWalkAnims[]={
  noelleWalkDown0,
  noelleWalkDown1,
  noelleWalkRight0,
  noelleWalkRight1,
  noelleWalkUp0,
  noelleWalkUp1,
  noelleWalkLeft0,
  noelleWalkLeft1
};

const unsigned char* const noelleHurtAnims[]={

};
CODE_BANK_POP();

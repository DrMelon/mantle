
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

// noelle's hurt anims are glitchier. no real reason, but it's fun cause of her glitch-finding blogs
const unsigned char noelleHurtDown[]={
  0, 0, 0x6E, 2,
  8, 0, 0x6F, 3,
  0, 8, 0x7E, 2,
  8, 8, 0x7F, 3,
  128
};

const unsigned char noelleHurtRight[]={
  0, 0, 0x8F, 3 | OAM_FLIP_H,
  8, 0, 0x8E, 0 | OAM_FLIP_H,
  0, 8, 0x9F, 3 | OAM_FLIP_H,
  8, 8, 0x9E, 0 | OAM_FLIP_H,
  128
};

const unsigned char noelleHurtUp[]={
  0, 0, 0xAE, 0,
  8, 0, 0xAF, 2,
  0, 8, 0xBE, 0,
  8, 8, 0xBF, 2,
  128
};

const unsigned char noelleHurtLeft[]={
  0, 0, 0x8E, 3,
  8, 0, 0x8F, 2,
  0, 8, 0x9E, 3,
  8, 8, 0x9F, 2,
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
  noelleWalkDown0,
  noelleHurtDown,
  noelleWalkRight0,
  noelleHurtRight,
  noelleWalkUp0,
  noelleHurtUp,
  noelleWalkLeft0,
  noelleHurtLeft
};


CODE_BANK_POP();

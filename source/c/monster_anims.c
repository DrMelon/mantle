#include "monster_anims.h"
#include "neslib.h"
#include "bank_helpers.h"

// REMINDER:
// To move jumptables to ROM instead of RAM,
// declare type like:
// const unsigned char* const tableName[];

// Walker metasprites
CODE_BANK(1);
const unsigned char walkerWalk0[]={
    0, 0, 0x1D, 7,
    8, 0, 0x1D, 7 | OAM_FLIP_H,
    0, 8, 0x2D, 7,
    8, 8, 0x2E, 7,
    128
};
const unsigned char walkerWalk1[]={
    1, 0, 0x1D, 7,
    9, 0, 0x1D, 7 | OAM_FLIP_H,
    1, 8, 0x2E, 7 | OAM_FLIP_H,
    9, 8, 0x2D, 7 | OAM_FLIP_H,
    128
};
const unsigned char walkerHurt0[]={
    0, 0, 0x3D, 7,
    8, 0, 0x3D, 7 | OAM_FLIP_H,
    0, 8, 0x4D, 7,
    8, 8, 0x4D, 7 | OAM_FLIP_H,
    128
};
const unsigned char walkerHurt1[]={
    0, 0, 0x3D, 5,
    8, 0, 0x3D, 5 | OAM_FLIP_H,
    0, 8, 0x4D, 5,
    8, 8, 0x4D, 5 | OAM_FLIP_H,
    128
};

// Shooter metasprites.
const unsigned char shooterWalk0[]={
    1, 0, 0x1D, 6,
    9, 0, 0x1D, 6 | OAM_FLIP_H,
    1, 8, 0x2E, 6 | OAM_FLIP_H,
    9, 8, 0x2D, 6 | OAM_FLIP_H,
    128
};
const unsigned char shooterWalk1[]={
    0, 0, 0x1D, 6,
    8, 0, 0x1D, 6 | OAM_FLIP_H,
    0, 8, 0x2D, 6,
    8, 8, 0x2E, 6,
    128
};
const unsigned char shooterPrepare0[]={
    0, 0, 0x1D, 6,
    8, 0, 0x1D, 6 | OAM_FLIP_H,
    0, 8, 0x2D, 6,
    8, 8, 0x2E, 6,
    128
};
const unsigned char shooterPrepare1[]={
    0, 0, 0x1D, 0,
    8, 0, 0x1D, 0 | OAM_FLIP_H,
    0, 8, 0x2D, 0,
    8, 8, 0x2E, 0,
    128
};
const unsigned char shooterShoot0[]={
    0, 0, 0x3D, 6,
    8, 0, 0x3D, 6 | OAM_FLIP_H,
    0, 8, 0x4D, 6,
    8, 8, 0x4E, 6,
    128
};
const unsigned char shooterShoot1[]={
    0, 0, 0x3D, 6,
    8, 0, 0x3D, 6 | OAM_FLIP_H,
    0, 8, 0x4D, 6,
    8, 8, 0x4E, 6,
    128
};
const unsigned char shooterHurt0[]={
    0, 0, 0x3D, 6,
    8, 0, 0x3D, 6 | OAM_FLIP_H,
    0, 8, 0x4D, 6,
    8, 8, 0x4D, 6 | OAM_FLIP_H,
    128
};
const unsigned char shooterHurt1[]={
    0, 0, 0x3D, 5,
    8, 0, 0x3D, 5 | OAM_FLIP_H,
    0, 8, 0x4D, 5,
    8, 8, 0x4D, 5 | OAM_FLIP_H,
    128
};

// Fish metasprites.
const unsigned char fishSwimLeft[]={
    0, 0, 0x4A, 7 | OAM_FLIP_H,
    8, 0, 0x49, 7 | OAM_FLIP_H,
    0, 3, 0x4A, 7 | OAM_FLIP_H | OAM_FLIP_V,
    8, 3, 0x49, 7 | OAM_FLIP_H | OAM_FLIP_V,
    128
};

const unsigned char fishSwimRight[]={
    0, 0, 0x49, 7,
    8, 0, 0x4A, 7,
    0, 3, 0x49, 7 | OAM_FLIP_V,
    8, 3, 0x4A, 7 | OAM_FLIP_V,
    128
};

const unsigned char fishSwimUp[]={
    0, 0, 0x5B, 7 | OAM_FLIP_V,
    0, 8, 0x4B, 7 | OAM_FLIP_V,
    3, 0, 0x5B, 7 | OAM_FLIP_V | OAM_FLIP_H,
    3, 8, 0x4B, 7 | OAM_FLIP_V | OAM_FLIP_H,
    128
};

const unsigned char fishSwimDown[]={
    0, 0, 0x4B, 7,
    0, 8, 0x5B, 7,
    3, 0, 0x4B, 7 | OAM_FLIP_H,
    3, 8, 0x5B, 7 | OAM_FLIP_H,
    128
};

const unsigned char fishHurtLeft0[]={
    0, 0, 0x5A, 7 | OAM_FLIP_H,
    8, 0, 0x59, 7 | OAM_FLIP_H,
    0, 3, 0x5A, 7 | OAM_FLIP_H | OAM_FLIP_V,
    8, 3, 0x59, 7 | OAM_FLIP_H | OAM_FLIP_V,
    128
};

const unsigned char fishHurtLeft1[]={
    0, 0, 0x5A, 5 | OAM_FLIP_H,
    8, 0, 0x59, 5 | OAM_FLIP_H,
    0, 3, 0x5A, 5 | OAM_FLIP_H | OAM_FLIP_V,
    8, 3, 0x59, 5 | OAM_FLIP_H | OAM_FLIP_V,
    128
};

const unsigned char fishHurtRight0[]={
    0, 0, 0x59, 7,
    8, 0, 0x5A, 7,
    0, 3, 0x59, 7 | OAM_FLIP_V,
    8, 3, 0x5A, 7 | OAM_FLIP_V,
    128
};

const unsigned char fishHurtRight1[]={
    0, 0, 0x59, 5,
    8, 0, 0x5A, 5,
    0, 3, 0x59, 5 | OAM_FLIP_V,
    8, 3, 0x5A, 5 | OAM_FLIP_V,
    128
};

const unsigned char fishHurtDown0[]={
    0, 0, 0x4C, 7,
    0, 8, 0x5C, 7,
    3, 0, 0x4C, 7 | OAM_FLIP_H,
    3, 8, 0x5C, 7 | OAM_FLIP_H,
    128
};

const unsigned char fishHurtDown1[]={
    0, 0, 0x4C, 5,
    0, 8, 0x5C, 5,
    3, 0, 0x4C, 5 | OAM_FLIP_H,
    3, 8, 0x5C, 5 | OAM_FLIP_H,
    128
};

const unsigned char fishHurtUp0[]={
    0, 0, 0x5C, 7 | OAM_FLIP_V,
    0, 8, 0x4C, 7 | OAM_FLIP_V,
    3, 0, 0x5C, 7 | OAM_FLIP_V | OAM_FLIP_H,
    3, 8, 0x4C, 7 | OAM_FLIP_V | OAM_FLIP_H,
    128
};

const unsigned char fishHurtUp1[]={
    0, 0, 0x5C, 5 | OAM_FLIP_V,
    0, 8, 0x4C, 5 | OAM_FLIP_V,
    3, 0, 0x5C, 5 | OAM_FLIP_V | OAM_FLIP_H,
    3, 8, 0x4C, 5 | OAM_FLIP_V | OAM_FLIP_H,
    128
};

// Flower metasprites.
const unsigned char flowerIdle[]={
    0, 0, 0x31, 6,
    8, 0, 0x31, 6 | OAM_FLIP_H,
    0, 8, 0x41, 6,
    8, 8, 0x41, 6 | OAM_FLIP_H,
    128
};

const unsigned char flowerPrepare0[]={
    0, 0, 0x31, 4,
    8, 0, 0x31, 4 | OAM_FLIP_H,
    0, 8, 0x41, 4,
    8, 8, 0x41, 4 | OAM_FLIP_H,
    128
};

const unsigned char flowerPrepare1[]={
    0, 0, 0x31, 6,
    8, 0, 0x31, 6 | OAM_FLIP_H,
    0, 8, 0x41, 6,
    8, 8, 0x41, 6 | OAM_FLIP_H,
    128
};

const unsigned char flowerHurt0[]={
    0, 0, 0x31, 0,
    8, 0, 0x31, 0 | OAM_FLIP_H,
    0, 8, 0x41, 0,
    8, 8, 0x41, 0 | OAM_FLIP_H,
    128
};

const unsigned char flowerHurt1[]={
    0, 0, 0x31, 6,
    8, 0, 0x31, 6 | OAM_FLIP_H,
    0, 8, 0x41, 6,
    8, 8, 0x41, 6 | OAM_FLIP_H,
    128
};

const unsigned char lizardIdleRight[]={
    8, 0, 0x84, 6 | OAM_FLIP_H,
    0, 0, 0x85, 6 | OAM_FLIP_H,
    8, 8, 0x94, 6 | OAM_FLIP_H,
    0, 8, 0x95, 6 | OAM_FLIP_H,
    128
};

const unsigned char lizardIdleLeft[]={
    0, 0, 0x84, 6,
    8, 0, 0x85, 6,
    0, 8, 0x94, 6,
    8, 8, 0x95, 6,
    128
};

const unsigned char lizardJumpRight[]={
    8, 0, 0x86, 6 | OAM_FLIP_H,
    0, 0, 0x87, 6 | OAM_FLIP_H,
    8, 8, 0x96, 6 | OAM_FLIP_H,
    0, 8, 0x97, 6 | OAM_FLIP_H,
    128
};

const unsigned char lizardJumpLeft[]={
    0, 0, 0x86, 6,
    8, 0, 0x87, 6,
    0, 8, 0x96, 6,
    8, 8, 0x97, 6,
    128
};

const unsigned char lizardPrepareLeft[]={
    0, 0, 0x84, 4,
    8, 0, 0x85, 4,
    0, 8, 0x94, 4,
    8, 8, 0x95, 4,
    128
};


const unsigned char lizardPrepareRight[]={
    8, 0, 0x84, 4 | OAM_FLIP_H,
    0, 0, 0x85, 4 | OAM_FLIP_H,
    8, 8, 0x94, 4 | OAM_FLIP_H,
    0, 8, 0x95, 4 | OAM_FLIP_H,
    128
};

const unsigned char lizardHurtLeft[]={
    0, 0, 0x86, 0,
    8, 0, 0x87, 0,
    0, 8, 0x96, 0,
    8, 8, 0x97, 0,
    128
};

const unsigned char lizardHurtRight[]={
    8, 0, 0x86, 0 | OAM_FLIP_H,
    0, 0, 0x87, 0 | OAM_FLIP_H,
    8, 8, 0x96, 0 | OAM_FLIP_H,
    0, 8, 0x97, 0 | OAM_FLIP_H,
    128
};

const unsigned char* const monWalkerAnims[]={
    walkerWalk0,
    walkerWalk1,
    walkerHurt0,
    walkerHurt1
};

const unsigned char* const monShooterAnims[]={
    shooterWalk0,
    shooterWalk1,
    shooterPrepare0,
    shooterPrepare1,
    shooterShoot0,
    shooterShoot1,
    shooterHurt0,
    shooterHurt1
};

// Directionality: 0 = down, 1 = right, 2 = up, 3 = left
const unsigned char* const fishSwimAnims[]={
    fishSwimDown,
    fishSwimRight,
    fishSwimUp,
    fishSwimLeft
};

const unsigned char* const fishHurtAnims[]={
    fishHurtDown0,
    fishHurtDown1,
    fishHurtRight0,
    fishHurtRight1,
    fishHurtUp0,
    fishHurtUp1,
    fishHurtLeft0,
    fishHurtLeft1
};

const unsigned char* const flowerAnims[]={
    flowerIdle,
    flowerPrepare0,
    flowerPrepare1,
    flowerHurt0,
    flowerHurt1
};

const unsigned char* const lizardIdleAnims[]={
    lizardIdleLeft,
    lizardIdleRight
};
const unsigned char* const lizardPrepareAnims[]={
    lizardIdleLeft,
    lizardPrepareLeft,
    lizardIdleRight,
    lizardPrepareRight
};
const unsigned char* const lizardJumpAnims[]={
    lizardJumpLeft,
    lizardJumpRight
};
const unsigned char* const lizardHurtAnims[]={
    lizardJumpLeft,
    lizardHurtLeft,
    lizardJumpRight,
    lizardHurtRight
};

CODE_BANK_POP();

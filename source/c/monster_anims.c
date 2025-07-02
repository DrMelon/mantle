#include "monster_anims.h"
#include "neslib.h"

// Walker metasprites
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

extern const unsigned char* monWalkerAnims[]={
    walkerWalk0,
    walkerWalk1,
    walkerHurt0,
    walkerHurt1
};

extern const unsigned char* monShooterAnims[]={
    shooterWalk0,
    shooterWalk1,
    shooterPrepare0,
    shooterPrepare1,
    shooterShoot0,
    shooterShoot1,
    shooterHurt0,
    shooterHurt1
};

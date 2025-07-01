#include "projectiles.h"
#include "globals.h"
#include "neslib.h"

const unsigned char pelletSprite[]={
    0, 0, 0xB4, 6,
    128
};


void update_projectile(Projectile* proj)
{
    unsigned char prevsubx;
    unsigned char prevsuby;
    // Type-specific logic goes here

    // Subpixel movement: add to subpixel counter. keep previous to do carry checks...
    // this completely sucks compared to doing this in assembly where i can just read the carry flag to see if the operation wrapped... but it's seemingly impossible just to do a carry branch in C?
    prevsubx = proj->subx;
    prevsuby = proj->suby;
    proj->subx += proj->xvel;
    proj->suby += proj->yvel;
    if(prevsubx > 128 && proj->subx < 127)
    {
        proj->xpos++;
    }
    else if(prevsubx < 127 && proj->subx > 128)
    {
        proj->xpos--;
    }
    if(prevsuby > 128 && proj->suby < 127)
    {
        proj->ypos++;
    }
    else if(prevsuby < 127 && proj->suby > 128)
    {
        proj->ypos--;
    }
}

void draw_projectile(Projectile* proj)
{
    spr = oam_meta_spr(proj->xpos, proj->ypos, spr, pelletSprite);
}


void spawn_projectile(unsigned char sx, unsigned char sy, enum ProjectileType type, char xvel, char yvel)
{
    if(spawnedProjectiles < MAX_PROJECTILES)
    {
        projList[spawnedProjectiles].xpos = sx;
        projList[spawnedProjectiles].ypos = sy;
        projList[spawnedProjectiles].projtype = type;
        projList[spawnedProjectiles].subx = 0;
        projList[spawnedProjectiles].suby = 0;
        projList[spawnedProjectiles].xvel = xvel;
        projList[spawnedProjectiles].yvel = yvel;
        spawnedProjectiles++;
    }
}

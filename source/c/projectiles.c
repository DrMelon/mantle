#include "projectiles.h"
#include "globals.h"
#include "neslib.h"

const unsigned char pelletSprite[]={
    0, 0, 0xB4, 6,
    128
};


void update_projectile(Projectile* proj)
{
    // Type-specific logic goes here

    // Subpixel movement: up to 16 glorious subpixels of precision!
    proj->subx += proj->xvel;
    if(proj->subx > 16)
    {
        proj->subx -= 16;
        proj->xpos++;
    }
    if(proj->subx < 0)
    {
        proj->subx += 16;
        proj->xpos--;
    }
    proj->suby += proj->yvel;
    if(proj->suby > 16)
    {
        proj->suby -= 16;
        proj->ypos++;
    }
    if(proj->suby < 0)
    {
        proj->suby += 16;
        proj->ypos--;
    }
}

void draw_projectile(Projectile* proj)
{
    spr = oam_meta_spr(proj->xpos, proj->ypos, spr, pelletSprite);
}


void spawn_projectile(unsigned char sx, unsigned char sy, enum ProjectileType type, int xvel, int yvel)
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

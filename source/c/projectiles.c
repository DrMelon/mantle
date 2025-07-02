#include "projectiles.h"
#include "globals.h"
#include "neslib.h"
#include "utils.h"

const unsigned char pelletSprite[]={
    0, 0, 0xB4, 6,
    128
};

const unsigned char arrowLeftSprite[]={
    0, 0, 0xA1, 7,
    8, 0, 0xA2, 7,
    128
};
const unsigned char arrowRightSprite[]={
    0, 0, 0xA2, 7 | OAM_FLIP_H,
    8, 0, 0XA1, 7 | OAM_FLIP_H,
    128
};
const unsigned char arrowUpSprite[]={
    0, 0, 0xA3, 7 | OAM_FLIP_V,
    0, 8, 0x93, 7 | OAM_FLIP_V,
    128
};
const unsigned char arrowDownSprite[]={
    0, 0, 0x93, 7,
    0, 8, 0xA3, 7,
    128
};

CODE_BANK(1);
void update_projectile(Projectile* proj)
{
    // Type-specific logic goes here

    // Subpixel movement: up to 16 glorious subpixels of precision!
    proj->subx += proj->xvel;
    while(proj->subx > 16)
    {
        proj->subx -= 16;
        proj->xpos++;
    }
    while(proj->subx < 0)
    {
        proj->subx += 16;
        proj->xpos--;
    }
    proj->suby += proj->yvel;
    while(proj->suby > 16)
    {
        proj->suby -= 16;
        proj->ypos++;
    }
    while(proj->suby < 0)
    {
        proj->suby += 16;
        proj->ypos--;
    }

    // Projectiles despawn when they leave the "screen"
    if(proj->xpos < 32 || proj->ypos < 48 || proj->xpos > 208 || proj->ypos > 160)
    {
        projList[i] = projList[spawnedProjectiles-1];
        spawnedProjectiles--;

        // Why does this affect the monsterList?
        // Some pointer shenanigans?
    }

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

void draw_projectile(Projectile* proj)
{
    if(proj->projtype == P_FRIENDLINESS_PELLET)
    {
        spr = oam_meta_spr(proj->xpos, proj->ypos, spr, pelletSprite);
    }
    else if(proj->projtype == P_ARROW)
    {
        if(abs(proj->xvel) > abs(proj->yvel))
        {
            if(proj->xvel < 0)
            {
                spr = oam_meta_spr(proj->xpos, proj->ypos, spr, arrowLeftSprite);
            }
            else
            {
                spr = oam_meta_spr(proj->xpos, proj->ypos, spr, arrowRightSprite);
            }
        }
        else
        {
            if(proj->yvel < 0)
            {
                spr = oam_meta_spr(proj->xpos, proj->ypos, spr, arrowUpSprite);
            }
            else
            {
                spr = oam_meta_spr(proj->xpos, proj->ypos, spr, arrowDownSprite);
            }
        }
    }
}

CODE_BANK_POP();

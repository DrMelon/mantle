#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"
#include "utils.h"
#include "projectiles.h"

#include "kris_anims.h"
#include "monster_anims.h"

const unsigned char* const * const characterWalkAnims[]={
    krisWalkAnims
};

const unsigned char* const * const characterStrikeAnims[]={
    krisStrikeAnims
};

CODE_BANK(0);
void update_character(WalkingCharacter* chara)
{
    switch (chara->substate)
    {
        case S_NORMAL:
        {
            int did_walk = 0;

            if(chara->chartype == CH_KRIS)
            {
                if(pad_trig&PAD_A && playerLevel > 0)
                    {
                        chara->substate = S_ATTACK;
                        chara->animframe = 0;
                        break;
                    }
                if(pad&PAD_DOWN)
                    {
                        chara->direction = 0;
                        if(solidity_check(chara->xpos, chara->ypos + 1)) chara->ypos++;
                        did_walk = 1;
                        if(chara->ypos > 160)
                        {
                            bank_push(0);
                            roomSwitchDir = 0;
                            switch_to_room(roomPtr[0]);
                            bank_pop();
                        }
                    }
                if(pad&PAD_RIGHT)
                    {
                        chara->direction = 1;
                        if(solidity_check(chara->xpos + 1, chara->ypos)) chara->xpos++;
                        did_walk = 1;
                        if(chara->xpos > 208)
                        {
                            bank_push(0);
                            roomSwitchDir = 1;
                            switch_to_room(roomPtr[1]);
                            bank_pop();
                        }
                    }
                if(pad&PAD_UP)
                    {
                        chara->direction = 2;
                        if(solidity_check(chara->xpos, chara->ypos - 1)) chara->ypos--;
                        did_walk = 1;
                        if(chara->ypos < 48)
                        {
                            bank_push(0);
                            roomSwitchDir = 2;
                            switch_to_room(roomPtr[2]);
                            bank_pop();
                        }
                    }
                if(pad&PAD_LEFT)
                    {
                        chara->direction = 3;
                        if(solidity_check(chara->xpos - 1, chara->ypos)) chara->xpos--;
                        did_walk = 1;
                        if(chara->xpos < 32)
                        {
                            bank_push(0);
                            roomSwitchDir = 3;
                            switch_to_room(roomPtr[3]);
                            bank_pop();
                        }
                    }
            }
            if(did_walk && framecount%16 == 0)
            {
                chara->animframe++;
            }
            if(did_walk && spawnedTeles != 0)
            {
                for(i2 = 0; i2 < spawnedTeles; i2++)
                {
                    if(chara->xpos+7 >> 4 == teleList[i2].tx+2 && chara->ypos+7 >> 4 == teleList[i2].ty+3)
                    {
                        // Do teleport
                        bank_push(0);
                        tele_to_room(teleList[i2].targetroom, teleList[i2].targetx+2, teleList[i2].targety+3);
                        bank_pop();
                        i2 = spawnedTeles;
                    }
                }
            }
            break;
        }
        case S_ATTACK:
        {
            if(framecount%6 == 0)
            {
                chara->animframe++;
            }
            if(chara->animframe == 1 && framecount%6 == 0)
            {
                // Attack frame - do checks against monsters, smashable tiles, etc

                // Check for smashable tiles (palm trees, ferns, cacti) and monsters at sword's location
                // (just check up, down, left, right tile of kris current center location?)
                x = (chara->xpos + 7) >> 4;
                y = (chara->ypos + 7) >> 4;
                if(chara->direction == 0) y++;
                else if(chara->direction == 1) x++;
                else if(chara->direction == 2) y--;
                else if(chara->direction == 3) x--;

                // Tile check (adjust pos)
                x -= 2;
                y -= 3;
                i = (y*12)+x+4;
                i2 = roomPtr[i];

                if(i2 == TILE_D_FERN && playerLevel >= 2)
                {
                    set_map_tile_in_room(x, y, 0);
                }
                else if(i2 == TILE_D_CACTUS && playerLevel >= 3)
                {
                    set_map_tile_in_room(x, y, 0);
                }
                else if(i2 == TILE_D_TREE && playerLevel >= 4)
                {
                    set_map_tile_in_room(x, y, 0);
                }
                else
                {
                    // Play *dink* sound!
                    // TODO: only play it if the struck tile *is* killable though. reorganize this code!
                }

                // Better sword check!
                sword_check(chara);

                // Sword swing SFX
                sfx_play(1, 0);
            }
            if(chara->animframe > 2)
            {
                chara->animframe = 0;
                chara->substate = S_NORMAL;
            }
            break;
        }
        case S_HURT:
        {
            // TODO:
            // 1. Knockback in opposite direction to facing
            // 2. Knockback movement needs to check tile solidity
            break;
        }
    }
}

void sword_check(WalkingCharacter* chara)
{
    char offsetx = 0;
    char offsety = 0;
    // Check for monsters along the sword's length, based on its direction.
    if(chara->direction == 0)
    {
        offsetx = 4;
        offsety = 24;
    }
    else if(chara->direction == 1)
    {
        offsetx = 24;
        offsety = 12;
    }
    else if(chara->direction == 2)
    {
        offsetx = 12;
        offsety = -12;
    }
    else if(chara->direction == 3)
    {
        offsetx = -12;
        offsety = 12;
    }
    for(i = 0; i < spawnedMonsters; i++)
    {
         x = monsterList[i].xpos;
         y = monsterList[i].ypos;
         if(point_in_rect(chara->xpos + offsetx, chara->ypos + offsety, x, y, x+16, y+16))
         {
             // TODO: Split this level check and substate check so that we can play a *dink* sound on strong monsters
             if(monsterList[i].level <= playerLevel && (monsterList[i].substate == S_NORMAL || monsterList[i].substate == S_WINDUP || monsterList[i].substate == S_JUMPING))
             {
                 if(monsterList[i].health > 0)
                     monsterList[i].health--;
                 monsterList[i].substate = S_HURT;
                 if(monsterList[i].montype != MON_LIZARD)
                 {
                     monsterList[i].animframe = 0;
                 }
                 else if(monsterList[i].arcid == 255)
                 {
                     // Lizard is not jumping, do regular hurt logic
                     monsterList[i].animframe = 0;
                 }
             }
         }
    }
}
CODE_BANK_POP();

void draw_character(WalkingCharacter* chara)
{
    // Character is walking, play walk anim for facing dir
    if(chara->substate == S_NORMAL)
    {
        spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterWalkAnims[chara->chartype][chara->animframe%2 + (chara->direction*2)]);
    }
    // Character is attacking, play attack anim for facing dir (Kris, Noelle only)
    if(chara->substate == S_ATTACK)
    {
        spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterStrikeAnims[chara->chartype][chara->animframe + (chara->direction*3)]);
    }
}


#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"
#include "utils.h"
#include "projectiles.h"

#include "kris_anims.h"
#include "monster_anims.h"

const unsigned char** characterWalkAnims[]={
    krisWalkAnims
};

const unsigned char** characterStrikeAnims[]={
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
             if(monsterList[i].level <= playerLevel && monsterList[i].substate == S_NORMAL)
             {
                 if(monsterList[i].health > 0)
                     monsterList[i].health--;
                 monsterList[i].substate = S_HURT;
                 monsterList[i].animframe = 0;
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

CODE_BANK(1);
void update_monster(Monster* monster)
{
    if(monster->montype == MON_WALKER)
    {
        update_mon_walker(monster);
    }
    else if(monster->montype == MON_SHOOTER)
    {
        update_mon_shooter(monster);
    }
}

void update_mon_walker(Monster* walker)
{
    // Walker: walks around in a random pattern.
    if(walker->substate == S_NORMAL)
    {
        if(framecount%10 == 0)
        {
            walker->animframe++;
        }
        if(framecount%60 == 0 && rand8() > 127) // every second a 1/4 chance to try and change direction
        {
            walker->direction = (walker->direction + 1) % 4;
        }
        if(framecount%4 == 0)
        {
            // try to walk in given direction
            if(walker->direction == 0)
            {
                if(solidity_check(walker->xpos, walker->ypos + 1)) walker->ypos++;
            }
            else if(walker->direction == 1)
            {
                if(solidity_check(walker->xpos + 1, walker->ypos)) walker->xpos++;
            }
            else if(walker->direction == 2)
            {
                if(solidity_check(walker->xpos, walker->ypos - 1)) walker->ypos--;
            }
            else if(walker->direction == 3)
            {
                if(solidity_check(walker->xpos - 1, walker->ypos)) walker->xpos--;
            }
        }
    }
    else if(walker->substate == S_HURT)
    {
       if(framecount % 3 == 0)
       {
           walker->animframe++;
           walker->ypos--;
       }
       if(walker->animframe > 12)
       {
           if(walker->health == 0)
           {
               earn_exp();
               deadList[walker->uniqueid] = 1; // update deadlist

               // delete monster
               // USING i2 HERE BECAUSE i IS STOMPED BY COLLISION CHECKS
               delete_monster(i2);
           }
           else
           {
               walker->substate = S_NORMAL;
           }
       }
    }
}

void update_mon_shooter(Monster* shooter)
{
    // 16-bit math needed here, since signed 8-bit numbers kind of suck to use.
    int player_offsetx;
    int player_offsety;
    if(shooter->substate == S_NORMAL || shooter->substate == S_HURT)
    {
        update_mon_walker(shooter); // Behaves like a Walker until it wants to fire spears.
        if(framecount % 64 == 0)
        {
            // Every 64 frames, roll the dice and decide whether to shoot or not.
            if(rand8() < 127)
            {
                shooter->substate = S_WINDUP;
                shooter->animframe = 0;
            }
        }
    }
    //else if(shooter->substate == S_HURT)
    //{
    //    // TODO: Do hurt anim & knockback for monster.
    //}
    else if(shooter->substate == S_WINDUP)
    {
        // Winding up to shoot the player.
        // Toggle the flash anim frame.
        if(framecount % 2 == 0)
        {
            shooter->animframe++;
        }
        if(shooter->animframe >= 12)
        {
            // Wind-up over, shoot at the player
            shooter->substate = S_ATTACK;
            shooter->animframe = 0;
        }
    }
    else if(shooter->substate == S_ATTACK)
    {
        if(framecount % 2 == 0)
        {
            shooter->animframe++;
        }

        if(shooter->animframe == 8)
        {
            shooter->animframe++;
            // Create "arrow" projectile, fire it roughly towards the player.
            // Need to calculate x, y velocity to shoot towards.
            // So, we need the player's position.
            player_offsetx = kris.xpos - shooter->xpos;
            player_offsety = kris.ypos - shooter->ypos;

            // Select x or y major
            if(abs(player_offsetx) > abs(player_offsety))
            {
                player_offsetx = sign(player_offsetx);
                player_offsety = 0;
            }
            else
            {
                player_offsety = sign(player_offsety);
                player_offsetx = 0;
            }

            spawn_projectile(shooter->xpos+4, shooter->ypos+4, P_ARROW, player_offsetx<<5, player_offsety<<5);
        }
        if(shooter->animframe >= 16)
        {
            shooter->substate = S_NORMAL;
        }
    }
}

void earn_exp()
{
    if(currentEnvironment == E_DESERT)
    {
        if(playerLevel < 2)
            playerExp += 6; // become stronger. 3 enemies to hit lv 2
        else if(playerLevel < 3)
            playerExp += 1; // become stronger. 16 enemies to hit lv 3.
        else if(playerLevel < 4)
            playerExp += 2; // become stronger. 8 enemies to hit lvmax.
    }
    hudDirty = 1;
}
CODE_BANK_POP();

void draw_monster(Monster* monster)
{
    if(monster->montype == MON_WALKER)
    {
        draw_walker(monster);
    }
    if(monster->montype == MON_SHOOTER)
    {
        draw_shooter(monster);
    }
}

void draw_walker(Monster* walker)
{
    if(walker->substate == S_NORMAL)
    {
        spr = oam_meta_spr(walker->xpos, walker->ypos, spr, monWalkerAnims[walker->animframe%2]);
    }
    else if(walker->substate == S_HURT)
    {
        spr = oam_meta_spr(walker->xpos, walker->ypos, spr, monWalkerAnims[(walker->animframe%2) + 2]);
    }
}

void draw_shooter(Monster* shooter)
{
    if(shooter->substate == S_NORMAL)
    {
        spr = oam_meta_spr(shooter->xpos, shooter->ypos, spr, monShooterAnims[shooter->animframe%2]);
    }
    else if(shooter->substate == S_WINDUP)
    {
        spr = oam_meta_spr(shooter->xpos, shooter->ypos, spr, monShooterAnims[(shooter->animframe%2) + 2]);
    }
    else if(shooter->substate == S_ATTACK)
    {
        spr = oam_meta_spr(shooter->xpos, shooter->ypos, spr, monShooterAnims[(shooter->animframe%2) + 4]);
    }
    else if(shooter->substate == S_HURT)
    {
        spr = oam_meta_spr(shooter->xpos, shooter->ypos, spr, monShooterAnims[(shooter->animframe%2) + 6]);
    }
}

CODE_BANK(1);
void delete_monster(unsigned char idx)
{
    // use a classic remove and swap back to remove a monster from the update list
    monsterList[idx] = monsterList[spawnedMonsters-1];
    spawnedMonsters--;
}
CODE_BANK_POP();

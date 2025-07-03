#include "monsters.h"
#include "monster_anims.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "actors.h"
#include "globals.h"
#include "utils.h"
#include "maps.h"

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
    else if(monster->montype == MON_FISH)
    {
        update_mon_fish(monster);
    }
    else if(monster->montype == MON_FLOWER)
    {
        update_mon_flower(monster);
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

void update_mon_fish(Monster* fish)
{
    // Fish are different from walking monsters;
    // they swim instead of walk. Let's just fake this by making their collision function inverted
    // from normal monsters, so they just "walk" inside solid tiles and only ever place them in water.

    if(fish->substate == S_NORMAL)
    {
        // Does this behaviour differ when not in the desert? Yeah, probably. In the dungeon i think.
        if(currentEnvironment == E_DESERT)
        {
           // 1. make the fish move in its current direction
           // 2. if it hits something solid, make it turn left
            if(framecount % 4 == 0)
            {
                // try to walk in given direction, turn if we can't.
                if(fish->direction == 0)
                {
                    if(swim_check(fish->xpos, fish->ypos + 1))
                         fish->ypos++;
                    else
                        fish->direction++;
                }
                else if(fish->direction == 1)
                {
                    if(swim_check(fish->xpos + 1, fish->ypos))
                       fish->xpos++;
                    else
                        fish->direction++;
                }
                else if(fish->direction == 2)
                {
                    if(swim_check(fish->xpos, fish->ypos - 1))
                       fish->ypos--;
                    else
                        fish->direction++;
                }
                else if(fish->direction == 3)
                {
                    if(swim_check(fish->xpos - 1, fish->ypos))
                        fish->xpos--;
                    else
                        fish->direction = 0;
                }
            }
        }
    }
    else if(fish->substate == S_HURT)
    {
        if(framecount % 3 == 0)
        {
            fish->animframe++;
            if(fish->animframe > 12)
            {
                fish->animframe = 0;
                if(fish->health == 0)
                {
                    // fish dies..!
                    earn_exp();
                    deadList[fish->uniqueid] = 1; // update deadlist
                    delete_monster(i2);
                }
                else
                {
                    fish->substate = S_NORMAL;
                }
            }
        }
    }
}

void earn_exp()
{
    if(currentEnvironment == E_DESERT)
    {
        if(playerLevel < 2)
            playerExp += 8; // become stronger. 3 enemies to hit lv 2
        else if(playerLevel < 3)
            playerExp += 1; // become stronger. 24 enemies to hit lv 3.
        else if(playerLevel < 4)
            playerExp += 2; // become stronger. 8 enemies to hit lvmax.
    }
    hudDirty = 1;
}

void update_mon_flower(Monster* flower)
{
    int player_offsetx;
    int player_offsety;
    if(flower->substate == S_NORMAL)
    {
       // flowers seethe with anger and desire only to blast bullets directly at the player's head
       flower->animframe++;
       if(flower->animframe > 120 && rand8()<40)
       {
           flower->substate = S_WINDUP;
           flower->animframe = 0;
       }
    }
    else if(flower->substate == S_WINDUP)
    {
       if(framecount%2 == 0)
       {
           flower->animframe++;
           if(flower->animframe == 12)
           {
              flower->substate = S_ATTACK;
              flower->animframe = 0;
           }
       }
    }
    else if(flower->substate == S_ATTACK)
    {
        flower->animframe++;
        if(flower->animframe == 1)
        {
             // create friendliness pellet!
            // Need to calculate x, y velocity to shoot towards.
            // So, we need the player's position.
            player_offsetx = kris.xpos - flower->xpos;
            player_offsety = kris.ypos - flower->ypos;

            // then we "normalize" this without actual division for now, just shift or something.
            while(abs(player_offsetx) > 32 || abs(player_offsety) > 32)
            {
              player_offsetx = player_offsetx >> 1;
              player_offsety = player_offsety >> 1;
            }

            spawn_projectile(flower->xpos+4, flower->ypos+4, P_FRIENDLINESS_PELLET, player_offsetx, player_offsety);
        }
        else if(flower->animframe == 24)
        {
            flower->substate = S_NORMAL;
        }
    }
    else if(flower->substate == S_HURT)
    {
      if(framecount%3 == 0)
      {
        flower->animframe++;
        if(flower->animframe >= 12)
        {
            if(flower->health < 1)
            {
                earn_exp();
                deadList[flower->uniqueid] = 1;
                delete_monster(i2);
            }
        }
      }
    }
}


// DRAWING ROUTINES
void draw_monster(Monster* monster)
{
    if(monster->montype == MON_WALKER)
    {
        draw_walker(monster);
    }
    else if(monster->montype == MON_SHOOTER)
    {
        draw_shooter(monster);
    }
    else if(monster->montype == MON_FISH)
    {
        draw_fish(monster);
    }
    else if(monster->montype == MON_FLOWER)
    {
        draw_flower(monster);
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

void draw_fish(Monster* fish)
{
    if(fish->substate == S_NORMAL)
    {
       spr = oam_meta_spr(fish->xpos, fish->ypos, spr, fishSwimAnims[fish->direction]);
    }
    else if(fish->substate == S_HURT)
    {
       spr = oam_meta_spr(fish->xpos, fish->ypos, spr, fishHurtAnims[(fish->direction*2)+fish->animframe%2]);
    }
}

void draw_flower(Monster* flower)
{
   if(flower->substate == S_NORMAL || flower->substate == S_ATTACK)
   {
       spr = oam_meta_spr(flower->xpos, flower->ypos, spr, flowerAnims[0]);
   }
   else if(flower->substate == S_WINDUP)
   {
       spr = oam_meta_spr(flower->xpos, flower->ypos, spr, flowerAnims[1+(flower->animframe%2)]);
   }
   else if(flower->substate == S_HURT)
   {
       spr = oam_meta_spr(flower->xpos, flower->ypos, spr, flowerAnims[3+(flower->animframe%2)]);
   }
}

void delete_monster(unsigned char idx)
{
    // use a classic remove and swap back to remove a monster from the update list
    monsterList[idx] = monsterList[spawnedMonsters-1];
    spawnedMonsters--;
}
CODE_BANK_POP();

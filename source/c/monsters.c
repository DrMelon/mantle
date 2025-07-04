#include "monsters.h"
#include "monster_anims.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "actors.h"
#include "globals.h"
#include "utils.h"
#include "maps.h"
#include "jump_luts.h"

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
    else if(monster->montype == MON_LIZARD)
    {
        update_mon_lizard(monster);
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
            playerExp += 2; // become stronger. 12 enemies to hit lvmax.
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

void update_mon_lizard(Monster* lizard)
{
    unsigned char jump_tile_walkable = 0;
    JumpArcState* jump_arc;
    int test_tile_x = 0;
    int test_tile_y = 0;
    // Lizards:
    // They walk randomly like the Walkers. Then, they jump!
    if(lizard->substate == S_NORMAL)
    {
        update_mon_walker(lizard);

        // Randomly choose when to jump.
        if(framecount % 64 == 0)
        {
            // Every 64 frames, roll the dice and decide whether to shoot or not.
            if(rand8() < 127)
            {
                lizard->substate = S_WINDUP;
                lizard->animframe = 0;
            }
        }
    }
    else if(lizard->substate == S_WINDUP)
    {
        if(framecount % 2 == 0)
        {
            lizard->animframe++;

            if(lizard->animframe > 16)
            {
                // 1. They will pick a tile to jump to.
                // |-> Picking a tile appears to be a random walkable tile within 5 tiles of the current tile.
                while(jump_tile_walkable != 1)
                {
                    test_tile_x = rand8() >> 5; // get a number from 0-7
                    test_tile_y = rand8() >> 5;
                    test_tile_x -= 4; // now it's -4 - 3
                    test_tile_y -= 4;

                    test_tile_x = (lizard->xpos >> 4) + test_tile_x;
                    test_tile_y = (lizard->ypos >> 4) + test_tile_y;

                    if(test_tile_x < 0) test_tile_x = 0;
                    if(test_tile_y < 0) test_tile_y = 0;
                    if(test_tile_x > 11) test_tile_x = 11;
                    if(test_tile_y > 7) test_tile_y = 7;

                    jump_tile_walkable = !tilemap_solid(test_tile_x, test_tile_y);
                }


                // 2. They will initiate a jump to that tile.
                // |-> The jump arc draws no shadow, but draws a target to help the player see where the lizard is going. Might need to skip this..?
                // |-> The arc's X position proceeds linearly. The Y position always travels at least one tile above the target tile's y pos,
                // |-> at the midpoint of the arc. The smooth-ish movement might necessitate a LUT or subpixel movement. LUT's probably the lightest way of doing it
                // |-> The lizard can be hit in the air, unlike the bird.

                if(jumpArcs < MAX_JUMP_ARCS)
                {
                    lizard->substate = S_JUMPING;
                    lizard->animframe = 0;
                    lizard->arcid = jumpArcs;

                    // set up a jump arc and select type.

                    // first calculate the jump arc type.
                    test_tile_x = test_tile_x - (lizard->xpos >> 4) + 4; // get back to 0-7 on each axis
                    test_tile_y = test_tile_y - (lizard->ypos >> 4) + 4;

                    jumpArcList[jumpArcs].jump_arc_type = test_tile_x + (test_tile_y * 8); // 8 possible x-axis values, 8 possible y-axis values.
                    jumpArcList[jumpArcs].start_x = lizard->xpos;
                    jumpArcList[jumpArcs].start_y = lizard->ypos;
                    jumpArcs++;
                }

            }
        }
    }
    else if(lizard->substate == S_JUMPING)
    {
        jump_arc = &jumpArcList[lizard->arcid];
        // Evaluate the jump arc for the current anim frame.
        // Jump arcs universally take 2 seconds.
        // We update at half-rate, so it's about 30 frames.
        if(framecount % 2 == 0)
        {
            // get jump x coords and jump y coords for current frame
            if(lizard->animframe < 30)
            {
                // need to access bank 2
                x = lizard->animframe;
                y = jump_arc->jump_arc_type;
                banked_call(2, jumpLutXLookup);
                lizard->xpos = x + jump_arc->start_x - 127;
                x = lizard->animframe;
                banked_call(2, jumpLutYLookup);
                lizard->ypos = x + jump_arc->start_y - 127;
                lizard->animframe++;
            }
            else
            {
                jumpArcList[lizard->arcid] = jumpArcList[jumpArcs];
                lizard->arcid = 255;
                jumpArcs--;
                lizard->substate = S_NORMAL;
            }
        }
    }
    else if(lizard->substate == S_HURT)
    {
        if(lizard->arcid != 255) // lizard is jumping, do special hurt logic
        {
            if(framecount % 64 == 0)
            {
                if(lizard->health < 1)
                {
                    earn_exp();
                    delete_monster(i2);
                    deadList[lizard->uniqueid] = 1; // update deadlist
                }
                else
                {
                    lizard->substate = S_JUMPING;
                }
            }
        }
        else
        {
            if(framecount % 2 == 0)
            {
                lizard->animframe++;
                if(lizard->animframe > 12)
                {
                    if(lizard->health < 1)
                    {
                        earn_exp();
                        delete_monster(i2);
                        deadList[lizard->uniqueid] = 1;
                    }
                    else
                    {
                        lizard->substate = S_NORMAL;
                    }
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
    else if(monster->montype == MON_LIZARD)
    {
        draw_lizard(monster);
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

void draw_lizard(Monster* lizard)
{
    unsigned char liz_face_right = 0;
    liz_face_right = lizard->direction < 2;
    if(lizard->substate == S_NORMAL)
    {
        spr = oam_meta_spr(lizard->xpos, lizard->ypos, spr, lizardIdleAnims[liz_face_right]);
    }
    else if(lizard->substate == S_WINDUP)
    {
        spr = oam_meta_spr(lizard->xpos, lizard->ypos, spr, lizardPrepareAnims[(liz_face_right*2)+(lizard->animframe%2)]);
    }
    else if(lizard->substate == S_JUMPING)
    {
        spr = oam_meta_spr(lizard->xpos, lizard->ypos, spr, lizardJumpAnims[liz_face_right]);
    }
    else if(lizard->substate == S_HURT)
    {
        spr = oam_meta_spr(lizard->xpos, lizard->ypos, spr, lizardHurtAnims[(liz_face_right*2)+(framecount%2)]);
    }
}

void delete_monster(unsigned char idx)
{
    // use a classic remove and swap back to remove a monster from the update list
    monsterList[idx] = monsterList[spawnedMonsters-1];
    spawnedMonsters--;
}
CODE_BANK_POP();

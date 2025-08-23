#include "monsters.h"
#include "monster_anims.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "actors.h"
#include "globals.h"
#include "utils.h"
#include "maps.h"
#include "jump_luts.h"
#include "items.h"

CODE_BANK(MONSTER_PROJECTILES_BANK);
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
    else if(monster->montype == MON_BIRD)
    {
        update_mon_bird(monster);
    }
    else if(monster->montype == MON_SINGCAT)
    {
        update_mon_cat(monster);
    }
    else if(monster->montype == MON_ICEBLOCK)
    {
        update_mon_iceblock(monster);
    }
}

void update_mon_walker(Monster* walker)
{
    // Walker: walks around in a random pattern.
    if(walker->substate == S_NORMAL)
    {
        if(stripefc%10 == 0)
        {
            walker->animframe++;
        }
        if(stripefc%60 == 0 && rand8() > 127) // every second a 1/4 chance to try and change direction
        {
            walker->direction = (walker->direction + 1) % 4;
        }
        if(stripefc%4 == 0)
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
       if(stripefc % 3 == 0)
       {
           walker->animframe++;
       }
       if(walker->animframe > 12)
       {
           if(walker->health == 0)
           {
               earn_exp();
               mark_dead(walker->uniqueid);

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
        if(stripefc % 64 == 0 && monsterAggression > 0)
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
        if(stripefc % 2 == 0)
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
        if(stripefc % 2 == 0)
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

            spawn_projectile(shooter->xpos+4, shooter->ypos+4, P_ARROW, (player_offsetx<<9), (player_offsety<<9));
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
        // Fish behave differently whether they are in the overworld or dungeons.
        if(currentEnvironment == E_DESERT || currentEnvironment == E_ISLAND)
        {
           // 1. make the fish move in its current direction
           // 2. if it hits something solid, make it turn left
            if(stripefc % 4 == 0)
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
        else if(stripefc % 4 == 0)
        {
            // In the other environments, they swim on regular tiles.if(fish->direction == 0)
            if(fish->direction == 0)
            {
                if(solidity_check(fish->xpos, fish->ypos + 1))
                        fish->ypos++;
                else
                    fish->direction++;
            }
            else if(fish->direction == 1)
            {
                if(solidity_check(fish->xpos + 1, fish->ypos))
                    fish->xpos++;
                else
                    fish->direction++;
            }
            else if(fish->direction == 2)
            {
                if(solidity_check(fish->xpos, fish->ypos - 1))
                    fish->ypos--;
                else
                    fish->direction++;
            }
            else if(fish->direction == 3)
            {
                if(solidity_check(fish->xpos - 1, fish->ypos))
                    fish->xpos--;
                else
                    fish->direction = 0;
            }
        }
    }
    else if(fish->substate == S_HURT)
    {
        if(stripefc % 3 == 0)
        {
            fish->animframe++;
            if(fish->animframe > 12)
            {
                fish->animframe = 0;
                if(fish->health == 0)
                {
                    // fish dies..!
                    earn_exp();
                    mark_dead(fish->uniqueid);
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
    else if(currentEnvironment == E_ISLAND)
    {
        monsterAggression = 1; // monsters become aggressive on the Island as soon as one dies.
        if(playerLevel < 2)
            playerExp += 2;// 10 enemies to hit lv2.
        if(playerExp == 20)
            playerExp += 4; // last one counts extra
    }
    else if(currentEnvironment == E_ICEPALACE)
    {
        if(playerLevel < 3)
            playerExp += 8; // 3 enemies to reach lv3
        else if(playerLevel < 4)
            playerExp += 2; // 12 enemies to hit lvmax
    }
    else if(currentEnvironment == E_CITY)
    {
        if(playerLevel < 2)
            playerExp += 24; // instant level up from Susie...
        else if(playerLevel < 3)
            playerExp += 24; // instant level up from Ralsei...
    }
    else if(currentEnvironment == E_DUNGEON)
    {
        if(playerLevel < 4)
            playerExp += 1; // 24 monsters...
    }
    if(roomLocked && spawnedMonsters <= 1) // last monster in room? unlock doors
    {
        unlock_room_doors();
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
       if(flower->animframe > 120 && rand8()<40 && monsterAggression > 0)
       {
           flower->substate = S_WINDUP;
           flower->animframe = 0;
       }
    }
    else if(flower->substate == S_WINDUP)
    {
       if(stripefc%2 == 0)
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

            spawn_projectile(flower->xpos+4, flower->ypos+4, P_FRIENDLINESS_PELLET, player_offsetx << 4, player_offsety << 4);
        }
        else if(flower->animframe == 24)
        {
            flower->substate = S_NORMAL;
        }
    }
    else if(flower->substate == S_HURT)
    {
      if(stripefc%3 == 0)
      {
        flower->animframe++;
        if(flower->animframe >= 12)
        {
            if(flower->health < 1)
            {
                earn_exp();
                mark_dead(flower->uniqueid);
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
        if(stripefc % 64 == 0)
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
        if(stripefc % 2 == 0)
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
        if(stripefc % 2 == 0)
        {
            // get jump x coords and jump y coords for current frame
            if(lizard->animframe < 30)
            {
                // need to access bank 2
                x = lizard->animframe;
                y = jump_arc->jump_arc_type;
                banked_call(JUMP_LUT_BANK, jumpLutXLookup);
                lizard->xpos = x + jump_arc->start_x - 127;
                x = lizard->animframe;
                banked_call(JUMP_LUT_BANK, jumpLutYLookup);
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
            if(stripefc % 64 == 0)
            {
                if(lizard->health < 1)
                {
                    earn_exp();
                    mark_dead(lizard->uniqueid);
                    delete_monster(i2);
                }
                else
                {
                    lizard->substate = S_JUMPING;
                }
            }
        }
        else
        {
            if(stripefc % 2 == 0)
            {
                lizard->animframe++;
                if(lizard->animframe > 12)
                {
                    if(lizard->health < 1)
                    {
                        earn_exp();
                        mark_dead(lizard->uniqueid);
                        delete_monster(i2);
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

void update_mon_bird(Monster* bird)
{
    // bird has very simple behaviour in the ice palace and just obstructs kris
    if(currentEnvironment == E_ICEPALACE)
    {
        bird->level = 5; // bird cannot be killed unless turned to ice here
        bird->health = 2;
        if(stripefc % 2 == 0)
        {
            bird->substate = S_NORMAL;
            bird->xpos = 160;
            bird->ypos = kris.ypos;
            if(bird->ypos < 62)
            {
                bird->ypos = 62;
            }
            else if(bird->ypos > 145)
            {
                bird->ypos = 145;
            }
            if(kris.xpos+14 > bird->xpos-1)
                kris.xpos = bird->xpos-15; //bird forcefield lol
        }
    }
    else // if its in the desert, it alternates between idling and flying
    {
        if(bird->substate == S_NORMAL)
        {
            if(stripefc % 2 == 0)
                bird->animframe++;

            if(bird->animframe > 40 && rand8()%2 == 0)
            {
                bird->animframe = 0; // using animframe for current flight stage; 0-15 = takeoff, then 18-33 for landing. mid stage for flight is 16-17, which loops until target reached
                bird->substate = S_FLY; // bird chooses a direction to fly in and flies off

                // get random coords
                x2 = rand8() >> 4;
                y2 = rand8() >> 4;
                // bound within room
                if(x2 > 10)
                {
                    x2 = x2 % 10;
                }
                if(y2 > 6)
                {
                    y2 = y2 % 6;
                }
                if(x2 < 1)
                {
                    x2 = 1;
                }
                if(y2 < 1)
                {
                        y2 = 1;
                }
                // offset from tilespace
                x2 += 2;
                y2 += 3;

                bird->direction = x2 << 4; // using direction for target x coord
                bird->arcid = y2 << 4; //using arcid for target y coord
            }
        }
        else if(bird->substate == S_FLY)
        {
            // first, do "takeoff" animation (mostly done in draw)
            if(bird->animframe < 16)
            {
                if(stripefc % 2 == 0)
                    bird->animframe++;
            }
            else if(bird->animframe < 18)
            {
                if(stripefc % 4 == 0)
                {
                    unsigned char birdmoved = 0;
                    bird->animframe++;
                    if(bird->xpos < bird->direction)
                    {
                        bird->xpos+=2;
                        birdmoved = 1;
                    }
                    else if(bird->xpos > bird->direction)
                    {
                        bird->xpos-=2;
                        birdmoved = 1;
                    }
                    if(bird->ypos < bird->arcid)
                    {
                        bird->ypos+=2;
                        birdmoved = 1;
                    }
                    else if(bird->ypos > bird->arcid)
                    {
                        bird->ypos-=2;
                        birdmoved = 1;
                    }
                    if(birdmoved == 1)
                    {
                        if(bird->animframe > 17)
                            bird->animframe = 16; // loop anim until reached target
                    }
                }
            }
            else if(bird->animframe < 33)
            {
                // perform landing
                if(stripefc % 2 == 0)
                    bird->animframe++;
            }
            else
            {
                // finish flight
                oam_clear(); // clear OAM because bird sprite count changes
                bird->substate = S_NORMAL;
                bird->animframe = 0;
            }
        }
        else if(bird->substate == S_HURT)
        {
            if(stripefc % 2 == 0)
            {
                bird->animframe++;
                if(bird->animframe > 12)
                {
                    if(bird->health < 1)
                    {
                        earn_exp();
                        mark_dead(bird->uniqueid);
                        delete_monster(i2);
                    }
                    else
                    {
                        bird->substate = S_NORMAL;
                    }
                }
            }
        }
    }
}

void update_mon_cat(Monster* cat)
{
    int dx;
    int dy;
    if(cat->substate == S_NORMAL)
    {
        if(cat->arcid == 0) // nonsinging cat
        {
            // do nothing if the singing cat is alive
            if(narrative_flag_get(NARFLAG_KILLED_SONGCAT) == 0)
                return;

            // we should stop the other cats from updating while this one is moving,
            // since they attack the player one at a time. this is pretty easy to do just by blasting the iterator lol
            // (but it also means my swapback deletion gets wonky if i don't take special care of it)
            i2 = spawnedMonsters;

            // also wait for a 1-sec alignment (so that they wait for a bit before attacking)
            if(cat->animframe == 0 && stripefc%60 != 0)
                return;

            // otherwise, move towards the player!
            if(kris.substate != S_HURT && stripefc % 4 == 0)
            {
                cat->animframe = 1; // catAnger sprite

                dx = sign(kris.xpos - cat->xpos);
                dy = sign(kris.ypos - cat->ypos);

                cat->xpos += (dx << 1);
                cat->ypos += (dy << 1);
            }
            return;
        }
        else
        {
            // singing cat behaviour has been simplified quite a bit for this.
            // move around a little bit randomly.
            dx = rand8();
            dy = rand8();
            dx -= 127;
            dy -= 127;

            if(solidity_check(cat->xpos + dx, cat->ypos + dy))
            {
                cat->xpos += sign(dx) << 1;
                cat->ypos += sign(dy) << 1;
            }

            // shoot song note projectiles in random directions every so often
            if((cat->animframe == 0) && (stripefc % 30 == 0))
            {
                cat->animframe = 1;
                spawn_projectile(cat->xpos+7, cat->ypos+7, P_NOTE, (dx<<3), (dy<<3));
                // TODO: play a random "sing" sound..?
            }

            // simple frame toggle
            if(cat->animframe == 1 && stripefc % 35 == 0)
            {
                cat->animframe = 0;
            }
        }
    }
    else
    {
        if(stripefc % 2 == 0)
        {
            cat->animframe++;
            if(cat->animframe > 12)
            {
                if(cat->health < 1)
                {
                    earn_exp();
                    if(currentEnvironment == E_DUNGEON && currentRoom == 5)
                    {
                        // killing the song cat in room 5 sets the narrative flag
                        narrative_flag_set(NARFLAG_KILLED_SONGCAT);
                    }
                    if(cat->arcid == 0) //nonsinging cat, which means the iterator might go wonky so we should stop this func short after deletion
                    {
                        mark_dead(cat->uniqueid);
                        delete_monster(i2);
                        return;
                    }
                    else
                    {
                        mark_dead(cat->uniqueid);
                        delete_monster(i2);
                    }
                }
                else
                {
                    cat->substate = S_NORMAL;
                }
            }
        }
        if(cat->arcid == 0) // do our update one-at-a-time thing
            i2 = spawnedMonsters;
    }
}

unsigned char iceblock_check(Monster* iceblock, unsigned char xpos, unsigned char ypos)
{
    unsigned char iceblockidx = 0;
    Monster* otherblock;
    while(iceblockidx < spawnedMonsters)
    {
        otherblock = &monsterList[iceblockidx];
        iceblockidx++;
        if(otherblock->montype != MON_ICEBLOCK)
            continue;
        if(otherblock->uniqueid == iceblock->uniqueid)
            continue;

        if(point_in_rect(xpos+7, ypos+7, otherblock->xpos, otherblock->ypos, otherblock->xpos+16, otherblock->ypos+16))
        {
            return 0;
        }
    }
    return 1;
}

void update_mon_iceblock(Monster* iceblock)
{
    if(iceblock->substate == S_FLY)
    {
        iceblock->animframe++;
        // move in direction until ya can't!
        // also check other iceblocks when moving, so we don't collide into them.
        if(iceblock->direction == 0)
        {
            if(solidity_check(iceblock->xpos, iceblock->ypos + 2) && iceblock_check(iceblock, iceblock->xpos+7, iceblock->ypos+18))
            {
                iceblock->ypos+=2;
            }
            else
            {
                iceblock->substate = S_NORMAL;
            }
        }
        else if(iceblock->direction == 1)
        {
            if(solidity_check(iceblock->xpos + 2, iceblock->ypos) && iceblock_check(iceblock, iceblock->xpos+18, iceblock->ypos+7))
            {
                iceblock->xpos+=2;
            }
            else
            {
                iceblock->substate = S_NORMAL;
            }
        }
        else if(iceblock->direction == 2)
        {
            if(solidity_check(iceblock->xpos, iceblock->ypos - 2) && iceblock_check(iceblock, iceblock->xpos+7, iceblock->ypos-2))
            {
                iceblock->ypos-=2;
            }
            else
            {
                iceblock->substate = S_NORMAL;
            }
        }
        else if(iceblock->direction == 3)
        {
            if(solidity_check(iceblock->xpos - 2, iceblock->ypos) && iceblock_check(iceblock, iceblock->xpos-2, iceblock->ypos+7))
            {
                iceblock->xpos-=2;
            }
            else
            {
                iceblock->substate = S_NORMAL;
            }
        }
        if(iceblock->animframe == 8)
        {
            iceblock->substate = S_NORMAL;
            iceblock->animframe = 0;
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
    else if(monster->montype == MON_BIRD)
    {
        draw_bird(monster);
    }
    else if(monster->montype == MON_SINGCAT)
    {
        draw_cat(monster);
    }
    else if(monster->montype == MON_ICEBLOCK)
    {
        draw_iceblock(monster);
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
        spr = oam_meta_spr(lizard->xpos, lizard->ypos, spr, lizardHurtAnims[(liz_face_right*2)+(stripefc%2)]);
    }
}

void draw_bird(Monster* bird)
{
    if(bird->substate == S_NORMAL)
    {
        spr = oam_meta_spr(bird->xpos, bird->ypos, spr, birdAnims[0]);
    }
    else if(bird->substate == S_FLY)
    {
        // display flappy, with an offset based on current takeoff/landing frame
        if(bird->animframe < 16)
            spr = oam_meta_spr(bird->xpos, bird->ypos - bird->animframe, spr, birdAnims[bird->animframe%2]);
        else if(bird->animframe < 18)
            spr = oam_meta_spr(bird->xpos, bird->ypos - 16, spr, birdAnims[bird->animframe%2]);
        else if(bird->animframe < 33)
            spr = oam_meta_spr(bird->xpos, bird->ypos - 16 + (bird->animframe-16), spr, birdAnims[bird->animframe%2]);
        // show shadow
        spr = oam_meta_spr(bird->xpos, bird->ypos + 10, spr, birdShadow);
    }
    else if(bird->substate == S_HURT)
    {
        spr = oam_meta_spr(bird->xpos, bird->ypos, spr, birdAnims[2 + (bird->animframe%2)]);
    }
}

void draw_cat(Monster* cat)
{
    if(cat->substate != S_HURT)
    {
        if(cat->arcid == 0) // nonsinging cat
        {
            spr = oam_meta_spr(cat->xpos, cat->ypos, spr, catAnims[cat->animframe]);
        }
        else if(cat->arcid == 1) // singing cat
        {
            spr = oam_meta_spr(cat->xpos, cat->ypos, spr, catAnims[2+(cat->animframe%2)]);
        }
    }
    else
    {
        spr = oam_meta_spr(cat->xpos, cat->ypos, spr, catAnims[4+(cat->animframe%2)]);
    }
}

void draw_iceblock(Monster* iceblock)
{
    spr = oam_meta_spr(iceblock->xpos, iceblock->ypos, spr, iceBlockSprite);
}

void delete_monster(unsigned char idx)
{
    // create a burst effect
    spawn_projectile(monsterList[idx].xpos, monsterList[idx].ypos, P_BURST, 0, 0);

    // use a classic remove and swap back to remove a monster from the update list
    spawn_candy(monsterList[idx].xpos, monsterList[idx].ypos);
    monsterList[idx] = monsterList[spawnedMonsters-1];
    spawnedMonsters--;



    oam_clear();
}
CODE_BANK_POP();

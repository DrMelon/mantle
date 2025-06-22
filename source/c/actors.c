#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"

#include "kris_anims.h"
#include "monster_anims.h"

const unsigned char** characterWalkAnims[]={
    krisWalkAnims
};

const unsigned char** characterStrikeAnims[]={
    krisStrikeAnims
};

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
                            switch_to_room(environment_rooms[currentEnvironment][currentRoom][0]);
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
                            switch_to_room(environment_rooms[currentEnvironment][currentRoom][1]);
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
                            switch_to_room(environment_rooms[currentEnvironment][currentRoom][2]);
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
                            switch_to_room(environment_rooms[currentEnvironment][currentRoom][3]);
                            bank_pop();
                        }
                    }
            }
            if(did_walk && framecount%16 == 0)
            {
                chara->animframe++;
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

                // Check for smashable tiles (palm tree, cactus) and monsters at sword's location
                // (just check up, down, left, right tile of kris current center location?)
                x = (chara->xpos + 7) >> 4;
                y = (chara->ypos + 7) >> 4;
                if(chara->direction == 0) y++;
                else if(chara->direction == 1) x++;
                else if(chara->direction == 2) y--;
                else if(chara->direction == 3) x--;

                // Monster check
                // TODO: increase fidelity of sword hitbox or do quick 8px line cast?
                // up, down, left, right is OK for tiles but kinda sucks for monsters
                for(i = 0; i < spawnedMonsters; i++)
                {
                    if(x == (monsterList[i].xpos + 7) >> 4 && y == (monsterList[i].ypos + 7) >> 4)
                    {
                        if(monsterList[i].level <= playerLevel && monsterList[i].substate == S_NORMAL)
                        {
                            monsterList[i].health--;
                            monsterList[i].substate = S_HURT;
                            monsterList[i].animframe = 0;
                        }
                    }
                }


                // Tile check (adjust pos)
                x -= 2;
                y -= 3;
                i = (y*12)+x+4;
                if(roomPtr[i] == 1)
                {
                    set_map_tile_in_room(x, y, 0);
                }

                // SFX
                sfx_play(1, 0);
            }
            if(chara->animframe > 2)
            {
                chara->animframe = 0;
                chara->substate = S_NORMAL;
            }
        }
    }
}

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

void update_monster(Monster* monster)
{
    if(monster->living != 1) return;
    if(monster->montype == MON_WALKER)
    {
        update_mon_walker(monster);
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
           if(walker->health < 1)
           {
               walker->living = 0;
               playerExp += 8; // become stronger.
               hudDirty = 1;
               deadList[walker->uniqueid] = 1; // update deadlist
           }
           else
           {
               walker->substate = S_NORMAL;
           }
       }
    }
}

void draw_monster(Monster* monster)
{
    if(monster->living != 1) return;
    if(monster->montype == MON_WALKER)
    {
        draw_walker(monster);
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

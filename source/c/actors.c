#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"
#include "utils.h"
#include "projectiles.h"
#include "ui.h"
#include "roomstuff.h"

#include "kris_anims.h"
#include "monster_anims.h"

const unsigned char* const * const characterWalkAnims[]={
    krisWalkAnims
};

const unsigned char* const * const characterStrikeAnims[]={
    krisStrikeAnims
};

const unsigned char* const * const characterHurtAnims[]={
    krisHurtAnims
};

CODE_BANK(ACTOR_LOGIC_BANK);
void update_character(WalkingCharacter* chara)
{
    switch (chara->substate)
    {
        case S_NORMAL:
        {
            int did_walk = 0;

            if(chara->chartype == CH_KRIS)
            {
                // Do projectile and monster damage checks
                if(monsterAggression > 0 && framecount % 2 == 0)
                {
                    for(i = 0; i < spawnedMonsters; i++)
                    {
                        if(monsterList[i].substate == S_NORMAL || monsterList[i].substate == S_JUMPING)
                        {
                            if(point_in_rect(monsterList[i].xpos + 8, monsterList[i].ypos + 8, chara->xpos+2, chara->ypos+2, chara->xpos+14, chara->ypos+14))
                            {
                                get_hurt(chara);
                                break;
                            }
                        }
                    }
                    for(i = 0; i < spawnedProjectiles; i++)
                    {
                        if(projList[i].projtype != P_ICEMAGIC)
                        {
                            if(point_in_rect(projList[i].xpos + 4, projList[i].ypos + 4, chara->xpos+4, chara->ypos+4, chara->xpos+12, chara->ypos+12))
                            {
                                get_hurt(chara);
                                break;
                            }
                        }
                    }
                }

                if(pad_trig&PAD_A)
                {
                    if(playerLevel > 0)
                    {
                        chara->substate = S_ATTACK;
                        chara->animframe = 0;
                    }

                    break;
                }
                if(pad&PAD_DOWN)
                {
                    chara->direction = 0;
                    if(solidity_check_nocactus(chara->xpos, chara->ypos + 1)) chara->ypos++;
                    if(!cactus_check(chara->xpos, chara->ypos))
                    {
                        get_hurt(chara);
                        chara->ypos -= 4;
                    }
                    did_walk = 1;
                    if(chara->ypos > 160)
                    {
                        roomSwitchDir = 0;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
                if(pad&PAD_RIGHT)
                {
                    chara->direction = 1;
                    if(solidity_check_nocactus(chara->xpos + 1, chara->ypos)) chara->xpos++;
                    if(!cactus_check(chara->xpos, chara->ypos))
                    {
                        get_hurt(chara);
                        chara->xpos -= 4;
                    }
                    did_walk = 1;
                    if(chara->xpos > 208)
                    {
                        roomSwitchDir = 1;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
                if(pad&PAD_UP)
                {
                    chara->direction = 2;
                    if(solidity_check_nocactus(chara->xpos, chara->ypos - 1)) chara->ypos--;
                    if(!cactus_check(chara->xpos, chara->ypos))
                    {
                        get_hurt(chara);
                        chara->ypos += 4;
                    }
                    did_walk = 1;
                    if(chara->ypos < 48)
                    {
                        roomSwitchDir = 2;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
                if(pad&PAD_LEFT)
                {
                    chara->direction = 3;
                    if(solidity_check_nocactus(chara->xpos - 1, chara->ypos)) chara->xpos--;
                    if(!cactus_check(chara->xpos, chara->ypos))
                    {
                        get_hurt(chara);
                        chara->xpos += 4;
                    }
                    did_walk = 1;
                    if(chara->xpos < 32)
                    {
                        roomSwitchDir = 3;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
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
                        x = teleList[i2].targetroom;
                        x2 = teleList[i2].targetx+2;
                        y2 = teleList[i2].targety+3;

                        banked_call(ROOM_LOGIC_BANK, tele_to_room);
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
                oam_clear();
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
                i = (y*12)+x;
                i2 = currentRoomColl[i];

                if(currentEnvironment == E_DESERT)
                {
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
                    else if(i2 == TILE_D_CHEST_CLOSED)
                    {
                        // Open the chest and perform the Ice Key theatric.

                        // Stop any music currently playing, then play the ice key jingle
                        //music_stop();

                        // Spawn the ice key sprite above the chest

                        // Summon the ice key text crawl and set text delay high
                        //queue_text(icekey_found_0, 1);
                        x2 = 0;
                        banked_call(UI_BANK, queue_text_banked);
                        textDelay = 6;
                        // Set the tile to the Open Chest tile
                        set_map_tile_in_room(x, y, TILE_D_CHEST_OPEN);

                        // TODO: Set the appropriate Theatrics flag & timer so that we can transition to the 2nd stage at the right time.
                        //skip_to_island();
                    }
                    else
                    {
                        // Play *dink* sound!
                        // TODO: only play it if the struck tile *is* killable though. reorganize this code!
                    }
                }
                else if(currentEnvironment == E_ISLAND)
                {
                    if(i2 == TILE_I_FERN && playerLevel >= 2)
                    {
                        set_map_tile_in_room(x, y, 0);
                    }
                    else if(i2 == TILE_I_WFERN && playerLevel >= 2)
                    {
                        set_map_tile_in_room(x, y, TILE_I_WATER);
                    }
                }



                // Better sword check!
                sword_check(chara);

                // Sword swing SFX
                //sfx_play(1, 0);
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
            // 1. Knockback in opposite direction to facing
            // 2. Knockback movement needs to check tile solidity
            if(framecount % 3 == 0)
            {
                chara->animframe++;

                if(chara->chartype == CH_KRIS)
                {
                    if(chara->direction == 2)
                    {
                        if(solidity_check(chara->xpos, chara->ypos + 2)) chara->ypos+=2;
                    }
                    if(chara->direction == 3)
                    {
                        if(solidity_check(chara->xpos + 2, chara->ypos)) chara->xpos+=2;
                    }
                    if(chara->direction == 0)
                    {
                        if(solidity_check(chara->xpos, chara->ypos - 2)) chara->ypos-=2;
                    }
                    if(chara->direction == 1)
                    {
                        if(solidity_check(chara->xpos - 2, chara->ypos)) chara->xpos-=2;
                    }
                }
            }

            // After anim over, check death status.
            if(chara->animframe >= 6)
            {
                // It's die time!
                if(chara->chartype == CH_KRIS)
                {
                    // TODO: Game death sequence, then restart from last environment/flag point.
                    if(playerHp < 1)
                    {
                        // do someething
                    }
                    else
                    {
                        chara->substate = S_NORMAL;
                    }
                }
                else
                {
                    // Susie/Ralsei must shrimply disappear... and award a lot of exp.
                    if(playerLevel < 3)
                        playerLevel++;
                    // Play level up jingle
                    // TODO: Disappear susie/ralsei
                }
            }
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

void get_hurt(WalkingCharacter *chara)
{
    if(chara->substate == S_NORMAL)
    {
        chara->substate = S_HURT;
        if(chara->chartype == CH_KRIS)
            playerHp--;
        chara->animframe = 0;
        hudDirty = 1;
    }
}

CODE_BANK_POP();

CODE_BANK(KRIS_ANIMS_BANK);

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
    // Character is hurt, play hurt anim for facing dir
    if(chara->substate == S_HURT)
    {
        spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterHurtAnims[chara->chartype][(chara->animframe%2) + (chara->direction*2)]);
    }
}
CODE_BANK_POP();

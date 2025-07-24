#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"
#include "utils.h"
#include "projectiles.h"
#include "ui.h"
#include "roomstuff.h"
#include "jump_arc.h"
#include "jump_luts.h"
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
    JumpArcState* jump_arc;
    unsigned char control_override = 0;
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

                if(theatricActive == 1) // if certain theatrics are on, don't take input.
                {
                    if(theatricIndex == TH_GETSWORD || theatricIndex == TH_GETICEKEY)
                    {
                        control_override = 1;
                    }
                }
                if(control_override == 0)
                {
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
                        if(chara->raft == NULL)
                        {
                            if(solidity_check_nocactus(chara->xpos, chara->ypos + 1)) chara->ypos++;

                            if(!cactus_check(chara->xpos, chara->ypos))
                            {
                                get_hurt(chara);
                                chara->ypos -= 4;
                            }
                        }
                        else // rafting!
                        {
                            if(swim_check(chara->xpos, chara->ypos + 1)) chara->ypos++;
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
                        if(chara->raft == NULL)
                        {
                            if(solidity_check_nocactus(chara->xpos + 1, chara->ypos)) chara->xpos++;
                            if(!cactus_check(chara->xpos, chara->ypos))
                            {
                                get_hurt(chara);
                                chara->xpos -= 4;
                            }
                        }
                        else
                        {
                            if(swim_check(chara->xpos + 1, chara->ypos)) chara->xpos++;
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
                        if(chara->raft == NULL)
                        {
                            if(solidity_check_nocactus(chara->xpos, chara->ypos - 1)) chara->ypos--;
                            if(!cactus_check(chara->xpos, chara->ypos))
                            {
                                get_hurt(chara);
                                chara->ypos += 4;
                            }
                        }
                        else
                        {
                            if(swim_check(chara->xpos, chara->ypos - 1)) chara->ypos--;
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
                        if(chara->raft == NULL)
                        {
                            if(solidity_check_nocactus(chara->xpos - 1, chara->ypos)) chara->xpos--;
                            if(!cactus_check(chara->xpos, chara->ypos))
                            {
                                get_hurt(chara);
                                chara->xpos += 4;
                            }
                        }
                        else
                        {
                        if(swim_check(chara->xpos - 1, chara->ypos)) chara->xpos--;
                        }
                        did_walk = 1;
                        if(chara->xpos < 32)
                        {
                            roomSwitchDir = 3;
                            banked_call(ROOM_LOGIC_BANK, switch_to_room);
                        }
                    }
                }

            }
            if(did_walk && framecount%16 == 0)
            {
                if(chara->raft == NULL)
                {
                    chara->animframe++;
                }
            }
            if(did_walk && spawnedTeles != 0)
            {
                for(i2 = 0; i2 < spawnedTeles; i2++)
                {
                    if(chara->xpos+7 >> 4 == teleList[i2].tx+2 && chara->ypos+7 >> 4 == teleList[i2].ty+3)
                    {
                        // Remove raft, if any
                        if(chara->raft != NULL)
                        {
                            chara->raft->assignedchar = NULL;
                            chara->raft = NULL;
                        }

                        // Do teleport
                        x = teleList[i2].targetroom;
                        x2 = teleList[i2].targetx+2;
                        y2 = teleList[i2].targety+3;

                        banked_call(ROOM_LOGIC_BANK, tele_to_room);
                        i2 = spawnedTeles;
                    }
                }
            }
            if(did_walk && spawnedRafts != 0)
            {
                // TODO: attempt to board a raft if we're not on one and one is nearby
                if(chara->raft == NULL)
                {
                    for(i2 = 0; i2 < spawnedRafts; i2++)
                    {
                        if(raftList[i2].currentRoom != currentRoom) continue; // don't hop on rafts that aren't in the same room, dummy
                        if(chara->direction == 0)
                        {
                            if(point_in_rect(chara->xpos+7, chara->ypos+18, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(chara, &raftList[i2]);
                            }
                        }
                        else if(chara->direction == 1)
                        {
                            if(point_in_rect(chara->xpos+18, chara->ypos+7, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(chara, &raftList[i2]);
                            }
                        }
                        else if(chara->direction == 2)
                        {
                            if(point_in_rect(chara->xpos+7, chara->ypos-2, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(chara, &raftList[i2]);
                            }
                        }
                        else if(chara->direction == 3)
                        {
                            if(point_in_rect(chara->xpos-2, chara->ypos+7, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(chara, &raftList[i2]);
                            }
                        }
                    }
                }

                // TODO: attempt to leave a raft if we *are* on one and the tile one over from us is a dock
                else
                {
                    if(chara->direction == 0)
                    {
                        if(bridge_check(chara->xpos+7, chara->ypos+18))
                        {
                            chara->xpos = ((chara->xpos+7) >> 4) << 4;
                            chara->ypos = ((chara->ypos+7) >> 4) << 4;
                            leave_raft(chara, chara->raft, (chara->xpos) >> 4, ((chara->ypos) >> 4) + 1);
                        }
                    }
                    else if(chara->direction == 1)
                    {
                        if(bridge_check(chara->xpos+18, chara->ypos+7))
                        {
                            chara->xpos = ((chara->xpos+7) >> 4) << 4;
                            chara->ypos = ((chara->ypos+7) >> 4) << 4;
                            leave_raft(chara, chara->raft, ((chara->xpos) >> 4)+1, (chara->ypos) >> 4);
                        }
                    }
                    else if(chara->direction == 2)
                    {
                        if(bridge_check(chara->xpos+7, chara->ypos-2))
                        {
                            chara->xpos = ((chara->xpos+7) >> 4) << 4;
                            chara->ypos = ((chara->ypos+7) >> 4) << 4;
                            leave_raft(chara, chara->raft, (chara->xpos) >> 4, ((chara->ypos) >> 4) - 1);
                        }
                    }
                    else if(chara->direction == 3)
                    {
                        if(bridge_check(chara->xpos-2, chara->ypos+7))
                        {
                            chara->xpos = ((chara->xpos+7) >> 4) << 4;
                            chara->ypos = ((chara->ypos+7) >> 4) << 4;
                            leave_raft(chara, chara->raft, ((chara->xpos) >> 4)-1, (chara->ypos) >> 4);
                        }
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
                        music_stop();

                        // Summon the ice key text crawl and set text delay high
                        //queue_text(icekey_found_0, 1);
                        x2 = 0;
                        banked_call(UI_BANK, queue_text_banked);
                        textDelay = 6;
                        // Set the tile to the Open Chest tile
                        set_map_tile_in_room(x, y, TILE_D_CHEST_OPEN);

                        // TODO: Set the appropriate Theatrics flag & timer so that we can transition to the 2nd stage at the right time.
                        theatricActive = 1;
                        theatricIndex = TH_GETICEKEY;
                        theatricStage = 0;
                        theatricTimer = 0;
                    }
                    else
                    {
                        // Play *dink* sound!
                        // TODO: only play it if the struck tile *is* killable though.
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
                    else if(currentRoom == 23 && playerLevel >= 2 && x == 7 && y == 3)
                    {
                        set_map_tile_in_room(x, y, TILE_I_STAIRS);
                    }
                    else if(i2 == TILE_I_DELTDOOR_BL || i2 == TILE_I_DELTDOOR_BR)
                    {
                        // Made it to the ice key door.
                        // TODO: Do the "UNLOCKED WITH ICE KEY" text, wait for a sec, then transport to ice palace interior
                        skip_to_ice_palace();
                    }
                }

                // Better sword check for monsters!
                sword_check(chara);

                // TODO: Sword swing SFX, should play on button press
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
                        // play a death sound
                        oam_clear();
                        chara->animframe = 0;
                        chara->substate = S_DIE;
                        currentState = GS_DEATH;
                        pal_col(0, 0x0F); // black BG
                        ppu_off();
                        ppu_on_spr(); // sprites only
                        music_stop();
                        framecount = 0;
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
        case S_JUMPING:
        {
            // follow jump arc
            jump_arc = &jumpArcList[chara->arcid];
            // Evaluate the jump arc for the current anim frame.
            // get jump x coords and jump y coords for current frame
            if(chara->animframe < 30)
            {
                // need to access bank 2
                x = chara->animframe;
                y = jump_arc->jump_arc_type;
                banked_call(JUMP_LUT_BANK, jumpLutXLookup);
                chara->xpos = x + jump_arc->start_x - 127;
                x = chara->animframe;
                banked_call(JUMP_LUT_BANK, jumpLutYLookup);
                chara->ypos = x + jump_arc->start_y - 127;
                chara->animframe++;
            }
            else
            {
                jumpArcList[chara->arcid] = jumpArcList[jumpArcs];
                chara->arcid = 255;
                jumpArcs--;
                chara->substate = S_NORMAL;
                chara->xpos = ((chara->xpos+7) >> 4) << 4;
                chara->ypos = ((chara->ypos+7) >> 4) << 4;

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
    unsigned char sprite_override = 0;
    if(theatricActive == 1)
    {
        // theatric is on, we might want to override the sprite
        if(theatricIndex == TH_GETSWORD)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, krisHold);
            sprite_override = 1;
        }
        else if(theatricIndex == TH_GETICEKEY)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, krisHoldIceKey);
            sprite_override = 1;
        }
    }

    if(sprite_override == 0)
    {
        // Character is walking, play walk anim for facing dir
        if(chara->substate == S_NORMAL)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterWalkAnims[chara->chartype][chara->animframe%2 + (chara->direction*2)]);
        }
        // Character is attacking, play attack anim for facing dir (Kris, Noelle only)
        else if(chara->substate == S_ATTACK)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterStrikeAnims[chara->chartype][chara->animframe + (chara->direction*3)]);
        }
        // Character is hurt, play hurt anim for facing dir
        else if(chara->substate == S_HURT)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterHurtAnims[chara->chartype][(chara->animframe%2) + (chara->direction*2)]);
        }
        else if(chara->substate == S_DIE)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, krisDieAnims[(chara->animframe%2)]);
        }
        else if(chara->substate == S_JUMPING)
        {
            spr = oam_meta_spr(chara->xpos, chara->ypos, spr, characterWalkAnims[chara->chartype][0 + (chara->direction*2)]);
        }
    }


}
CODE_BANK_POP();

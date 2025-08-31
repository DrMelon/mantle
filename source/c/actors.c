#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"
#include "utils.h"
#include "projectiles.h"
#include "monsters.h"
#include "ui.h"
#include "roomstuff.h"
#include "jump_arc.h"
#include "jump_luts.h"
#include "kris_anims.h"
#include "noelle_anims.h"
#include "susie_anims.h"
#include "ralsei_anims.h"
#include "monster_anims.h"

const unsigned char* const * const characterWalkAnims[]={
   krisWalkAnims,
   noelleWalkAnims,
   susieWalkAnims,
   ralseiWalkAnims
};

const unsigned char* const * const characterStrikeAnims[]={
    krisStrikeAnims,
};

const unsigned char* const * const characterHurtAnims[]={
    krisHurtAnims,
    noelleHurtAnims
};

CODE_BANK(ACTOR_LOGIC_BANK);
void update_kris()
{
    JumpArcState* jump_arc;
    unsigned char control_override = 0;
    unsigned char did_move = 0;
    switch (kris.substate)
    {
        case S_NORMAL:
        {
            unsigned char did_walk = 0;
            Monster* mon;
            Projectile* proj;

            // Do projectile and monster damage checks
            if(monsterAggression > 0 && framecount % 2 == 0)
            {
                for(i = 0; i < spawnedMonsters; i++)
                {
                    // Check for ice blocks to push
                    mon = &monsterList[i];
                    if(mon->montype == MON_ICEBLOCK)
                    {
                        // TODO using only one point in rect sucks for this, do a better edge check for ice blocks
                        x2 = 0; // store ice collision in x2
                        if(kris.direction == 0)
                        {
                            x2 = point_in_rect(kris.xpos+7, kris.ypos+14, mon->xpos, mon->ypos, mon->xpos+16, mon->ypos+16);
                        }
                        else if(kris.direction == 1)
                        {
                            x2 = point_in_rect(kris.xpos+14, kris.ypos+7, mon->xpos, mon->ypos, mon->xpos+16, mon->ypos+16);
                        }
                        else if(kris.direction == 2)
                        {
                            x2 = point_in_rect(kris.xpos+7, kris.ypos+2, mon->xpos, mon->ypos, mon->xpos+16, mon->ypos+16);
                        }
                        else if(kris.direction == 3)
                        {
                            x2 = point_in_rect(kris.xpos+2, kris.ypos+7, mon->xpos, mon->ypos, mon->xpos+16, mon->ypos+16);
                        }

                        if(x2 == 1)
                        {
                            if(mon->substate == S_NORMAL) //pushable, try a push
                            {
                                int dx;
                                int dy;
                                mon->substate = S_FLY; //tell this block to "fly"
                                // figure out direction

                                dx = (int)(mon->xpos+8) - (int)(kris.xpos+8);
                                dy = (int)(mon->ypos+8) - (int)(kris.ypos+8);

                                if(abs(dx) > abs(dy))
                                {
                                    if(dx < 0)
                                    {
                                        mon->direction = 3;
                                        kris.xpos += 2;
                                    }
                                    else
                                    {
                                        mon->direction = 1;
                                        kris.xpos -= 2;
                                    }
                                }
                                else
                                {
                                    if(dy < 0)
                                    {
                                        mon->direction = 2;
                                        kris.ypos += 2;
                                    }
                                    else
                                    {
                                        mon->direction = 0;
                                        kris.ypos -= 2;
                                    }
                                }
                            }

                            return; // can't walk past an iceblock
                        }

                    }
                    else if(mon->substate == S_NORMAL || mon->substate == S_JUMPING)
                    {
                        if(point_in_rect(mon->xpos + 8, mon->ypos + 8, kris.xpos+2, kris.ypos+2, kris.xpos+14, kris.ypos+14))
                        {
                            get_hurt();
                            break;
                        }
                    }
                }
                for(i = 0; i < spawnedProjectiles; i++)
                {
                    proj = &projList[i];
                    if(proj->projtype != P_ICEMAGIC && proj->projtype != P_BURST)
                    {
                        if(point_in_rect(FP_WHOLE(proj->xpos) + 4, FP_WHOLE(proj->ypos) + 4, kris.xpos+4, kris.ypos+4, kris.xpos+12, kris.ypos+12))
                        {
                            get_hurt();
                            break;
                        }
                    }
                }
            }

            if(theatricActive == 1) // if certain theatrics are on, don't take input.
            {
                control_override = 1;
                // list of theatrics that allow control go here:
            }
            if(control_override == 0)
            {
                if(pad_trig&PAD_A)
                {
                    if(playerLevel > 0)
                    {
                        kris.substate = S_ATTACK;
                        kris.animframe = 0;
                    }

                    // twisted stuff
                    else
                    {
                        if(twisted.init == 2 && twisted.state == TA_FINAL)
                        {
                            // do exp bar push stuff
                        }
                    }

                    break;
                }
                if(pad&PAD_DOWN)
                {
                    kris.direction = 0;
                    if(kris.raft == NULL)
                    {
                       unsigned char coll = solidity_check(kris.xpos, kris.ypos + 1);
                       if(coll == 2)
                       {
                           get_hurt();
                           kris.ypos -= 4;
                       }
                       else if(coll == 0)
                       {
                           kris.ypos++;
                           did_move = 1;
                       }
                   }
                    else // rafting!
                    {
                        if(swim_check(kris.xpos, kris.ypos + 1)) kris.ypos++;
                    }
                    did_walk = 1;
                    if(kris.ypos > 160)
                    {
                        roomSwitchDir = 0;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
                if(pad&PAD_RIGHT)
                {
                    kris.direction = 1;
                    if(kris.raft == NULL)
                    {
                       unsigned char coll = solidity_check(kris.xpos + 1, kris.ypos);
                       if(coll == 2)
                       {
                           get_hurt();
                           kris.xpos -= 4;
                       }
                       else if(coll == 0)
                       {
                           kris.xpos++;
                           did_move = 1;
                       }
                    }
                    else
                    {
                        if(swim_check(kris.xpos + 1, kris.ypos)) kris.xpos++;
                    }
                    did_walk = 1;
                    if(kris.xpos > 208)
                    {
                        roomSwitchDir = 1;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
                if(pad&PAD_UP)
                {
                    kris.direction = 2;
                    if(kris.raft == NULL)
                    {
                       unsigned char coll = solidity_check(kris.xpos, kris.ypos - 1);
                       if(coll == 2)
                       {
                           get_hurt();
                           kris.ypos += 4;
                       }
                       else if(coll == 0)
                       {
                           kris.ypos--;
                           did_move = 1;
                       }
                    }
                    else
                    {
                        if(swim_check(kris.xpos, kris.ypos - 1))
                        {
                            kris.ypos--;
                            did_move = 1;
                        }
                    }
                    did_walk = 1;
                    if(kris.ypos < 48)
                    {
                        roomSwitchDir = 2;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
                if(pad&PAD_LEFT)
                {
                    kris.direction = 3;
                    if(kris.raft == NULL)
                    {
                       unsigned char coll = solidity_check(kris.xpos - 1, kris.ypos);
                       if(coll == 2)
                       {
                           get_hurt();
                           kris.xpos += 4;
                       }
                       else if(coll == 0)
                       {
                           kris.xpos--;
                           did_move = 1;
                       }
                    }
                    else
                    {
                        if(swim_check(kris.xpos - 1, kris.ypos))
                        {
                            kris.xpos--;
                            did_move = 1;
                        }
                    }
                    did_walk = 1;
                    if(kris.xpos < 32)
                    {
                        roomSwitchDir = 3;
                        banked_call(ROOM_LOGIC_BANK, switch_to_room);
                    }
                }
            }
            if(did_walk && framecount%16 == 0)
            {
                if(kris.raft == NULL)
                {
                    kris.animframe++;
                }
            }
            if(did_walk && spawnedTeles != 0)
            {
                Teleporter* tele;
                for(i2 = 0; i2 < spawnedTeles; i2++)
                {
                    tele = &teleList[i2].tele;

                    if(kris.xpos+7 >> 4 == tele->tx+2 && kris.ypos+7 >> 4 == tele->ty+3)
                    {
                        // Remove raft, if any
                        if(kris.raft != NULL)
                        {
                            // the only teleports with a raft are the two waterfall entrance tiles, so we can play the jingle here
                            sfx_play(SFX_EQUIP, FAMISTUDIO_SFX_CH0);
                            kris.raft->assignedchar = NULL;
                            kris.raft = NULL;
                        }
                        if(currentEnvironment == E_CITY && tele->targetroom == 66)
                        {
                            // Dungeon time
                            banked_call(ROOM_LOGIC_BANK, skip_to_dungeon);
                        }
                        else
                        {
                            // Do teleport
                            x = tele->targetroom;
                            x2 = tele->targetx+2;
                            y2 = tele->targety+3;

                            banked_call(ROOM_LOGIC_BANK, tele_to_room);
                        }
                        i2 = spawnedTeles;
                    }
                }
            }
            if(did_walk && spawnedRafts != 0)
            {
                // TODO: attempt to board a raft if we're not on one and one is nearby
                if(kris.raft == NULL)
                {
                    for(i2 = 0; i2 < spawnedRafts; i2++)
                    {
                        if(raftList[i2].currentRoom != currentRoom) continue; // don't hop on rafts that aren't in the same room, dummy
                        if(kris.direction == 0)
                        {
                            if(point_in_rect(kris.xpos+7, kris.ypos+18, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(&kris, &raftList[i2]);
                            }
                        }
                        else if(kris.direction == 1)
                        {
                            if(point_in_rect(kris.xpos+18, kris.ypos+7, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(&kris, &raftList[i2]);
                            }
                        }
                        else if(kris.direction == 2)
                        {
                            if(point_in_rect(kris.xpos+7, kris.ypos-2, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(&kris, &raftList[i2]);
                            }
                        }
                        else if(kris.direction == 3)
                        {
                            if(point_in_rect(kris.xpos-2, kris.ypos+7, raftList[i2].xpos, raftList[i2].ypos, raftList[i2].xpos+16, raftList[i2].ypos+16))
                            {
                                // attempt to board this raft
                                board_raft(&kris, &raftList[i2]);
                            }
                        }
                    }
                }

                // TODO: attempt to leave a raft if we *are* on one and the tile one over from us is a dock
                else
                {
                    if(kris.direction == 0)
                    {
                        if(bridge_check(kris.xpos+7, kris.ypos+18))
                        {
                            kris.xpos = ((kris.xpos+7) >> 4) << 4;
                            kris.ypos = ((kris.ypos+7) >> 4) << 4;
                            leave_raft(&kris, kris.raft, (kris.xpos) >> 4, ((kris.ypos) >> 4) + 1);
                        }
                    }
                    else if(kris.direction == 1)
                    {
                        if(bridge_check(kris.xpos+18, kris.ypos+7))
                        {
                            kris.xpos = ((kris.xpos+7) >> 4) << 4;
                            kris.ypos = ((kris.ypos+7) >> 4) << 4;
                            leave_raft(&kris, kris.raft, ((kris.xpos) >> 4)+1, (kris.ypos) >> 4);
                        }
                    }
                    else if(kris.direction == 2)
                    {
                        if(bridge_check(kris.xpos+7, kris.ypos-2))
                        {
                            kris.xpos = ((kris.xpos+7) >> 4) << 4;
                            kris.ypos = ((kris.ypos+7) >> 4) << 4;
                            leave_raft(&kris, kris.raft, (kris.xpos) >> 4, ((kris.ypos) >> 4) - 1);
                        }
                    }
                    else if(kris.direction == 3)
                    {
                        if(bridge_check(kris.xpos-2, kris.ypos+7))
                        {
                            kris.xpos = ((kris.xpos+7) >> 4) << 4;
                            kris.ypos = ((kris.ypos+7) >> 4) << 4;
                            leave_raft(&kris, kris.raft, ((kris.xpos) >> 4)-1, (kris.ypos) >> 4);
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
                kris.animframe++;
                oam_dirty = 1;
            }
            if(kris.animframe == 1 && framecount%6 == 0)
            {
                unsigned char dink = 0;
                // Attack frame - do checks against monsters, smashable tiles, etc

                // Check for smashable tiles (palm trees, ferns, cacti) and monsters at sword's location
                // (just check up, down, left, right tile of kris current center location?)
                x = (kris.xpos + 7) >> 4;
                y = (kris.ypos + 7) >> 4;
                if(kris.direction == 0) y++;
                else if(kris.direction == 1) x++;
                else if(kris.direction == 2) y--;
                else if(kris.direction == 3) x--;

                // Tile check (adjust pos)
                x -= 2;
                y -= 3;
                i = (y*12)+x+4;
                i2 = unpackedRoom[i];

                if(currentEnvironment == E_DESERT)
                {
                    if(i2 == TILE_D_FERN && playerLevel >= 2)
                    {
                        set_map_tile_in_room(x, y, 0);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_D_CACTUS && playerLevel >= 3)
                    {
                        set_map_tile_in_room(x, y, 0);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_D_TREE && playerLevel >= 4)
                    {
                        set_map_tile_in_room(x, y, 0);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_D_CHEST_CLOSED)
                    {
                        // Open the chest and perform the Ice Key theatric.

                        // Stop any music currently playing, then play the ice key jingle
                        music_stop();
                        sfx_play(SFX_ICEKEY, FAMISTUDIO_SFX_CH1);

                        // Summon the ice key text crawl and set text delay high
                        x2 = 0;
                        banked_call(UI_BANK, queue_text_banked);
                        textDelay = 6;
                        // Set the tile to the Open Chest tile
                        set_map_tile_in_room(x, y, TILE_D_CHEST_OPEN);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                        start_theatric(TH_GETICEKEY);
                    }
                    else if(i2 == TILE_D_FERN || i2 == TILE_D_CACTUS || i2 == TILE_D_TREE)
                    {
                        dink = 1;
                    }
                }
                else if(currentEnvironment == E_ISLAND)
                {
                    if(i2 == TILE_I_FERN && playerLevel >= 2)
                    {
                        set_map_tile_in_room(x, y, 0);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_I_WFERN && playerLevel >= 2)
                    {
                        set_map_tile_in_room(x, y, TILE_I_WATER);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(currentRoom == 23 && playerLevel >= 2 && x == 7 && y == 3)
                    {
                        set_map_tile_in_room(x, y, TILE_I_STAIRS);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_I_DELTDOOR_BL || i2 == TILE_I_DELTDOOR_BR)
                    {
                        // Made it to the ice key door.
                        x2 = 2;
                        banked_call(UI_BANK, queue_text_banked);
                        start_theatric(TH_USEICEKEY);
                        music_stop();
                    }
                    else if(i2 == TILE_I_FERN || i2 == TILE_I_WFERN)
                    {
                        dink = 1;
                    }
                }
                else if(currentEnvironment == E_ICEPALACE)
                {
                    if(i2 == TILE_IP_TREE && playerLevel >= 4)
                    {
                        set_map_tile_in_room(x, y, TILE_IP_FLOOR);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_IP_DELTDOOR_BL || i2 == TILE_IP_DELTDOOR_BR)
                    {
                        // Made it to the Big Door.
                        music_stop();
                        queue_text(icepalace_text_2, 1);
                        textDelay = 6;
                        start_theatric(TH_USED_UP);
                    }
                    else if(i2 == TILE_IP_TREE)
                    {
                        dink = 1;
                    }
                }
                else if(currentEnvironment == E_CITY)
                {
                    if(i2 == TILE_CITY_BOLLARD || i2 == TILE_CITY_BOLLARD_V || i2 == TILE_CITY_BOLLARD_CORNER)
                    {
                        if(playerLevel >= 3)
                        {
                            set_map_tile_in_room(x, y, TILE_CITY_FLOOR);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                        }
                        else
                        {
                            dink = 1;
                        }
                    }
                }
                else if(currentEnvironment == E_DUNGEON)
                {
                    if(i2 == TILE_DUNGEON_TREE && playerLevel >= 4)
                    {
                        set_map_tile_in_room(x, y, TILE_DUNGEON_FLOOR);
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                    else if(i2 == TILE_DUNGEON_TREE)
                    {
                        dink = 1;
                    }
                }
                else if(currentEnvironment == E_SHELTERFOREST)
                {
                    if(i2 == TILE_FOREST_SHELTER9 || i2 == TILE_FOREST_SHELTER10 || i2 == TILE_FOREST_SHELTER5 || i2 == TILE_FOREST_SHELTER6)
                    {
                        // Do shelter entry theatric
                        queue_text(shelter_text_0, 1);
                        start_theatric(TH_ENTER_SHELTER);
                    }
                }
                else if(currentEnvironment == E_SHELTER)
                {
                    if(i2 == TILE_SHELTER_SPIKES && playerLevel >= 1)
                    {
                        set_map_tile_in_room(x, y, TILE_SHELTER_FLOOR); // player can destroy spikes as long as they have a sword at all
                        sfx_play(SFX_KILL, FAMISTUDIO_SFX_CH2);
                    }
                }

                // Better sword check for monsters!
                dink |= sword_check();

                // Sword swing SFX, should play on button press
                sfx_play(SFX_SWORD, FAMISTUDIO_SFX_CH0);

                if(dink)
                {
                    sfx_play(SFX_DINK, FAMISTUDIO_SFX_CH1);
                }
            }
            if(kris.animframe > 2)
            {
                kris.animframe = 0;
                kris.substate = S_NORMAL;
            }
            break;
        }
        case S_HURT:
        {
            // 1. Knockback in opposite direction to facing
            // 2. Knockback movement needs to check tile solidity
            if(framecount % 3 == 0)
            {
                kris.animframe++;

                if(kris.direction == 2)
                {
                    if(!solidity_check(kris.xpos, kris.ypos + 2))
                    {
                        kris.ypos+=2;
                        did_move = 1;
                    }
                }
                if(kris.direction == 3)
                {
                    if(!solidity_check(kris.xpos + 2, kris.ypos))
                    {
                        kris.xpos+=2;
                        did_move = 1;
                    }
                }
                if(kris.direction == 0)
                {
                    if(!solidity_check(kris.xpos, kris.ypos - 2))
                    {
                        kris.ypos-=2;
                        did_move = 1;
                    }
                }
                if(kris.direction == 1)
                {
                    if(!solidity_check(kris.xpos - 2, kris.ypos))
                    {
                        kris.xpos-=2;
                        did_move = 1;
                    }
                }
            }

            // After anim over, check death status.
            if(kris.animframe >= 6)
            {
                // It's die time!
                if(playerHp < 1)
                {
                    // play a death sound
                    oam_clear();
                    kris.animframe = 0;
                    kris.substate = S_DIE;
                    currentState = GS_DEATH;
                    pal_col(0, 0x0F); // black BG
                    ppu_off();
                    ppu_on_spr(); // sprites only
                    music_stop();
                    framecount = 0;
                }
                else
                {
                    kris.substate = S_NORMAL;
                }
            }
            break;
        }
        case S_JUMPING:
        {
            // follow jump arc
            jump_arc = &jumpArcList[kris.arcid];
            // Evaluate the jump arc for the current anim frame.
            // get jump x coords and jump y coords for current frame
            if(kris.animframe < 30)
            {
                // need to access bank 2
                x = kris.animframe;
                y = jump_arc->jump_arc_type;
                banked_call(JUMP_LUT_BANK, jumpLutXLookup);
                kris.xpos = x + jump_arc->start_x - 127;
                x = kris.animframe;
                banked_call(JUMP_LUT_BANK, jumpLutYLookup);
                kris.ypos = x + jump_arc->start_y - 127;
                kris.animframe++;
            }
            else
            {
                jumpArcList[kris.arcid] = jumpArcList[jumpArcs];
                kris.arcid = 255;
                jumpArcs--;
                kris.substate = S_NORMAL;
                sfx_play(SFX_RAFT, FAMISTUDIO_SFX_CH0);
                kris.xpos = ((kris.xpos+7) >> 4) << 4;
                kris.ypos = ((kris.ypos+7) >> 4) << 4;

            }

            break;
        }
    }
    // If any move was made this frame, put it in the position buffer. Followers use this buffer to move themselves around.
    if(did_move)
    {
        // store last move pos in ring buffer
        followPositions[lastFollowPosIdx] = kris.xpos;
        followPositions[lastFollowPosIdx+1] = kris.ypos;
        lastFollowPosIdx = (lastFollowPosIdx + 2) % 64;
    }
}

unsigned char sword_check()
{
    char offsetx = 0;
    char offsety = 0;
    Monster* mon;
    // Check for monsters along the sword's length, based on its direction.
    if(kris.direction == 0)
    {
        offsetx = 4;
        offsety = 24;
    }
    else if(kris.direction == 1)
    {
        offsetx = 24;
        offsety = 12;
    }
    else if(kris.direction == 2)
    {
        offsetx = 12;
        offsety = -12;
    }
    else if(kris.direction == 3)
    {
        offsetx = -12;
        offsety = 12;
    }
    for(i = 0; i < spawnedMonsters; i++)
    {
         mon = &monsterList[i];
         x = mon->xpos;
         y = mon->ypos;
         if(point_in_rect(kris.xpos + offsetx, kris.ypos + offsety, x, y, x+16, y+16))
         {
             // Swap position with iceblock if you hit it. Stops softlocks...
             if(mon->montype == MON_ICEBLOCK && mon->substate == S_NORMAL)
             {
                 mon->xpos = ((kris.xpos+7) >> 4) << 4;
                 mon->ypos = ((kris.ypos+7) >> 4) << 4;
                 kris.xpos = ((x+7) >> 4) << 4;
                 kris.ypos = ((y+7) >> 4) << 4;
                 break;
             }
             if((mon->substate == S_NORMAL || mon->substate == S_WINDUP || mon->substate == S_JUMPING))
             {
                 if(mon->level > playerLevel) return 1; // dink sound
                 if(mon->health > 0)
                     mon->health--;
                 mon->substate = S_HURT;

                 // play monster hurt sound
                 sfx_play(SFX_HIT, FAMISTUDIO_SFX_CH1);

                 if(mon->montype != MON_LIZARD)
                 {
                     mon->animframe = 0;
                 }
                 else if(mon->arcid == 255)
                 {
                     // Lizard is not jumping, do regular hurt logic
                     mon->animframe = 0;
                 }
             }
         }
    }
    // Susie & Ralsei check...
    if(currentEnvironment == E_CITY)
    {
        if(!narrative_flag_get(NARFLAG_KILLED_SUSIE))
        {
            if(point_in_rect(kris.xpos + offsetx, kris.ypos + offsety, followerA.xpos+2, followerA.ypos+2, followerA.xpos+14, followerA.ypos+14))
            {
                narrative_flag_set(NARFLAG_KILLED_SUSIE);
                banked_call(MONSTER_PROJECTILES_BANK, earn_exp);
                x2 = followerA.xpos;
                y2 = followerA.ypos;
                banked_call(MONSTER_PROJECTILES_BANK, quickspawn_burst);
                return 0;
            }
        }
        if(!narrative_flag_get(NARFLAG_KILLED_RALSEI))
        {
            if(point_in_rect(kris.xpos + offsetx, kris.ypos + offsety, followerB.xpos+2, followerB.ypos+2, followerB.xpos+14, followerB.ypos+14))
            {
                narrative_flag_set(NARFLAG_KILLED_RALSEI);
                banked_call(MONSTER_PROJECTILES_BANK, earn_exp);
                x2 = followerB.xpos;
                y2 = followerB.ypos;
                banked_call(MONSTER_PROJECTILES_BANK, quickspawn_burst);
                return 0;
            }
        }
    }
    // Twisted check
    else if(currentEnvironment == E_SHELTER)
    {
        if(twisted.invuln == 0 && twisted.emot != TE_HURT)
        {
            // twisted phase 1
            if(twisted.init == 1 && (twisted.state == TA_IDLE || twisted.state == TA_MOVE_TO_POINT_WORLD))
            {
                x2 = FP_WHOLE(twisted.xpos);
                y2 = FP_WHOLE(twisted.ypos);
                if(point_in_rect(kris.xpos + offsetx, kris.ypos + offsety, x2-4, y2, x2 + 12, y2 + 16))
                {
                    sfx_play(SFX_HIT, FAMISTUDIO_SFX_CH1);
                    twisted.emot = TE_HURT;
                    twisted.state = TA_IDLE;
                    twisted.stateTimer = 45;
                    twisted.fightStage++; // count number of hits in phase 1
                }
            }
            // twisted phase 2
            else if(twisted.init == 2 && twisted.state == TA_CHARGE_ACROSS_SCREEN)
            {
                x2 = FP_WHOLE(twisted.xpos);
                y2 = FP_WHOLE(twisted.ypos);
                if(point_in_rect(kris.xpos + offsetx, kris.ypos + offsety, x2-4, y2, x2 + 12, y2 + 16))
                {
                    twisted.emot = TE_HURT;
                    twisted.state = TA_IDLE;
                    twisted.stateTimer = 45;
                    twisted.fightStage++; // count number of hits in phase 2
                }
            }
        }

    }

    return 0;
}

void get_hurt()
{
    if(kris.substate == S_NORMAL)
    {
        sfx_play(SFX_DAMAGE, FAMISTUDIO_SFX_CH0);
        kris.substate = S_HURT;
        playerHp--;
        kris.animframe = 0;
        hudDirty = 1;
    }
}

void update_followers()
{
    if(currentEnvironment == E_ICEPALACE && narrative_flag_get(NARFLAG_FOUND_NOELLE))
    {
        if(followerA.substate == S_NORMAL || followerA.substate == S_ATTACK) // noelle walks even when "attacking" - the attack is really just a cooldown
        {
            if(currentRoom != 7) // Noelle is in a "dormant" state in her starting room. She can be attacked, but will not move on her own.
            {
                int dx;
                int dy;
                // Noelle follows slightly closer than a one-tile-away distance
                dx = (int)followPositions[(lastFollowPosIdx+12)%64] - (int)followerA.xpos;
                dy = (int)followPositions[(lastFollowPosIdx+13)%64] - (int)followerA.ypos;

                if(abs(dx) > 0 || abs(dy) > 0)
                {
                    if(framecount % 8 == 0) followerA.animframe++;

                    followerA.xpos += dx;
                    followerA.ypos += dy;
                    if(dy > 0)
                    {
                        followerA.direction = 0;
                    }
                    else if(dy < 0)
                    {
                        followerA.direction = 2;
                    }
                    if(dx > 0)
                    {
                        followerA.direction = 1;
                    }
                    else if(dx < 0)
                    {
                        followerA.direction = 3;
                    }
                }

                if(pad_trig & PAD_A && followerA.substate != S_ATTACK) // noelle attack!
                {
                    follower_shoot_check();
                    // set her state to attack so she can't refire right away
                    followerA.substate = S_ATTACK;

                }
                else if(followerA.substate == S_ATTACK)
                {
                    // if noelle fired recently, wait until 1.5 sec frame count alignment to reset cooldown. jank but fast
                    if(framecount % 90 == 0) followerA.substate = S_NORMAL;
                }
            }
        }
    }
    else if(currentEnvironment == E_CITY)
    {
       // Susie
       if(!narrative_flag_get(NARFLAG_KILLED_SUSIE))
       {
            int dx;
            int dy;
            dx = (int)followPositions[(lastFollowPosIdx+34)%64] - (int)followerA.xpos;
            dy = (int)followPositions[(lastFollowPosIdx+35)%64] - (int)followerA.ypos;

            if(abs(dx) > 0 || abs(dy) > 0)
            {
                if(framecount % 8 == 0) followerA.animframe++;

                followerA.xpos += dx;
                followerA.ypos += dy;
                if(dy > 0)
                {
                    followerA.direction = 0;
                }
                else if(dy < 0)
                {
                    followerA.direction = 2;
                }
                if(dx > 0)
                {
                    followerA.direction = 1;
                }
                else if(dx < 0)
                {
                    followerA.direction = 3;
                }
            }
       }
       // Ralsei
       if(!narrative_flag_get(NARFLAG_KILLED_RALSEI))
       {
            int dx;
            int dy;
            dx = (int)followPositions[(lastFollowPosIdx+2)%64] - (int)followerB.xpos;
            dy = (int)followPositions[(lastFollowPosIdx+3)%64] - (int)followerB.ypos;

            if(abs(dx) > 0 || abs(dy) > 0)
            {
                if(framecount % 8 == 0) followerB.animframe++;

                followerB.xpos += dx;
                followerB.ypos += dy;
                if(dy > 0)
                {
                    followerB.direction = 0;
                }
                else if(dy < 0)
                {
                    followerB.direction = 2;
                }
                if(dx > 0)
                {
                    followerB.direction = 1;
                }
                else if(dx < 0)
                {
                    followerB.direction = 3;
                }
            }
       }
    }

}

CODE_BANK_POP();

CODE_BANK(KRIS_ANIMS_BANK);
void draw_kris()
{
    unsigned char sprite_override = 0;
    if(theatricActive == 1)
    {
        // theatric is on, we might want to override the sprite
        if(theatricIndex == TH_GETSWORD)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, krisHold);
            sprite_override = 1;
        }
        else if(theatricIndex == TH_GETICEKEY)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, krisHoldIceKey);
            sprite_override = 1;
        }
    }

    if(sprite_override == 0)
    {
        // Character is walking, play walk anim for facing dir
        if(kris.substate == S_NORMAL)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, characterWalkAnims[CH_KRIS][kris.animframe%2 + (kris.direction*2)]);
        }
        // Character is attacking, play attack anim for facing dir (Kris, Noelle only)
        else if(kris.substate == S_ATTACK)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, characterStrikeAnims[CH_KRIS][kris.animframe + (kris.direction*3)]);
        }
        // Character is hurt, play hurt anim for facing dir
        else if(kris.substate == S_HURT)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, characterHurtAnims[CH_KRIS][(kris.animframe%2) + (kris.direction*2)]);
        }
        else if(kris.substate == S_DIE)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, krisDieAnims[(kris.animframe%2)]);
        }
        else if(kris.substate == S_JUMPING)
        {
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, characterWalkAnims[CH_KRIS][0 + (kris.direction*2)]);
        }
    }
}

void draw_followers()
{
    if(currentEnvironment == E_ICEPALACE && narrative_flag_get(NARFLAG_FOUND_NOELLE))
    {
        if(followerA.substate == S_NORMAL || followerA.substate == S_ATTACK)
        {
            spr = oam_meta_spr(followerA.xpos, followerA.ypos, spr, characterWalkAnims[CH_NOELLE][followerA.animframe%2 + (followerA.direction*2)]);
        }
    }
    else if(currentEnvironment == E_CITY)
    {
        if(!narrative_flag_get(NARFLAG_KILLED_SUSIE))
        {
            spr = oam_meta_spr(followerA.xpos, followerA.ypos, spr, characterWalkAnims[CH_SUSIE][followerA.animframe%2 + (followerA.direction*2)]);
        }
        if(!narrative_flag_get(NARFLAG_KILLED_RALSEI))
        {
            spr = oam_meta_spr(followerB.xpos, followerB.ypos, spr, characterWalkAnims[CH_RALSEI][followerB.animframe%2 + (followerB.direction*2)]);
        }
    }
}

CODE_BANK_POP();

#include "actors.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "maps.h"
#include "kris_anims.h"

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

void update_item(Item* item, WalkingCharacter* chara)
{
    unsigned char kx, ky;
    if(item->living != 1)
    {
        return;
    }
    // Check to see if Kris has walked onto the same tile as this item
    x = item->xpos >> 4;
    y = item->ypos >> 4;
    kx = chara->xpos >> 4;
    ky = chara->ypos >> 4;

    if(x == kx && y == ky)
    {
        // Touching item. Let's do some logic!
        if(item->itemtype == ITEM_SWORD)
        {
            // Increase player level to 1!
            if(playerLevel < 1)
            {
                playerLevel++;
            }
            // Play jingle
            // Destroy self
            deadList[item->uniqueid] = 1;
            item->living = 0;
        }
    }
}

const unsigned char swordSprite[]={
    0, 0, 0x27, 4,
    0, 8, 0x37, 4,
    128
};

void draw_item(Item* item)
{
    if(item->living != 1) return;
    if(item->itemtype == ITEM_SWORD)
    {
        spr = oam_meta_spr(item->xpos, item->ypos, spr, swordSprite);
    }
}

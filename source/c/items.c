#include "items.h"
#include "globals.h"
#include "neslib.h"
#include "utils.h"

const unsigned char swordSprite[]={
    0, 0, 0x27, 4,
    0, 8, 0x37, 4,
    128
};

const unsigned char candySprite[]={
    0, 0, 0xA8, 4,
    4, 4, 0xA8, 4 | OAM_FLIP_H | OAM_FLIP_V,
    128
};

void update_item(Item* item, WalkingCharacter* chara)
{
    // Check to see if Kris has walked onto the same tile as this item

    if(point_in_rect(item->xpos+4, item->ypos+4, chara->xpos, chara->ypos, chara->xpos+16, chara->ypos+16))
    {
        // Touching item. Let's do some logic!
        if(item->itemtype == ITEM_SWORD)
        {
            // Increase player level to 1!
            if(playerLevel < 1)
            {
                playerLevel++;
                hudDirty = 1;
                // play jingle!
                music_stop();
                sfx_play(SFX_EQUIP, FAMISTUDIO_SFX_CH0);
                theatricActive = 1;
                theatricIndex = TH_GETSWORD;
                theatricStage = 0;
                theatricTimer = 0;
            }
            // Destroy self
            mark_dead(item->uniqueid);

            delete_item(i);
        }
        else if(item->itemtype == ITEM_CANDY)
        {
            if(playerHp < 16)
            {
                playerHp += 6;
                if(playerHp > 16) playerHp = 16;
                hudDirty = 1;
            }

            delete_item(i);
        }
    }
}


void draw_item(Item* item)
{
    if(item->itemtype == ITEM_SWORD)
    {
        spr = oam_meta_spr(item->xpos, item->ypos, spr, swordSprite);
    }
    else if(item->itemtype == ITEM_CANDY)
    {
        spr = oam_meta_spr(item->xpos, item->ypos, spr, candySprite);
    }
}

void delete_item(unsigned char idx)
{
    // remove-at-swapback
    itemList[idx] = itemList[spawnedItems-1];
    spawnedItems--;
    oam_dirty = 1;
}

void spawn_candy(unsigned char px, unsigned char py)
{
    // Increasing chance to spawn candy as hp gets low
    if(spawnedItems < MAX_ITEMS && rand8() < (48 + (2*(16-playerHp))))
    {
        itemList[spawnedItems].itemtype = ITEM_CANDY;
        itemList[spawnedItems].xpos = px;
        itemList[spawnedItems].ypos = py;
        spawnedItems++;
    }
}


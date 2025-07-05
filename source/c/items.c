#include "items.h"
#include "globals.h"
#include "neslib.h"

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
    unsigned char kx, ky;
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
                hudDirty = 1;
                // Start playing the song!!
                // TODO: Delay this, but play the jingle instead.
                // Then after the jingle, teleport out and *then* start the music.
                // TODO: play the weirder version in the islands environment
                music_play(0);

                // teleport out of the sword zone
                if(currentEnvironment == E_DESERT)
                {
                    x = 5;
                    y = 3;
                    queueTele = 5;
                }

            }
            // Destroy self
            deadList[item->uniqueid] = 1;

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
    oam_clear();
}

CODE_BANK(1);
void spawn_candy(unsigned char px, unsigned char py)
{
    if(spawnedItems < MAX_ITEMS && rand8() < 64)
    {
        itemList[spawnedItems].itemtype = ITEM_CANDY;
        itemList[spawnedItems].xpos = (px >> 4) << 4;
        itemList[spawnedItems].ypos = (py >> 4) << 4;
        spawnedItems++;
    }
}
CODE_BANK_POP();

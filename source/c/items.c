#include "items.h"
#include "globals.h"
#include "neslib.h"

const unsigned char swordSprite[]={
    0, 0, 0x27, 4,
    0, 8, 0x37, 4,
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
    }
}


void draw_item(Item* item)
{
    if(item->itemtype == ITEM_SWORD)
    {
        spr = oam_meta_spr(item->xpos, item->ypos, spr, swordSprite);
    }
}

void delete_item(unsigned char idx)
{
    // remove-at-swapback
    itemList[idx] = itemList[spawnedItems-1];
    spawnedItems--;
}

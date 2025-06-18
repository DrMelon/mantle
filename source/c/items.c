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
                // Start playing the song!!
                music_play(0);
            }
            // Destroy self
            deadList[item->uniqueid] = 1;
            item->living = 0;
        }
    }
}


void draw_item(Item* item)
{
    if(item->living != 1) return;
    if(item->itemtype == ITEM_SWORD)
    {
        spr = oam_meta_spr(item->xpos, item->ypos, spr, swordSprite);
    }
}

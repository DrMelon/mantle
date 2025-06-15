// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"
#include "actors.h"
#include "kris_anims.h";

//
// Global Variables (zeropage) 
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//
#pragma bss-name(push, "ZEROPAGE")
    unsigned char i;
    unsigned char framecount;
    unsigned char spr;
    unsigned char pad,pad_trig;
    unsigned char animFrame;
    unsigned char playerLevel;
    unsigned char currentMap;
    WalkingCharacter kris;
#pragma bss-name(pop)

//
// Normal Variables
// You can define larger, less frequently accessed variables here. You have about 1.5k to work with.
//
// EXAMPLE: 
// unsigned char myBigBufferArray[32];
unsigned char testVariable;

//
// Constant variables
// Anything with const in front of it will go into write-only prg instead of the very limited ram we have.
//
const unsigned char welcomeMessage[] = "Having fun, Kris..?";

// Color palette for the screen to use
const unsigned char paletteArea1[] = {
    0x0f, 0x00, 0x10, 0x30,
    0x27, 0x37, 0x38, 0x28,
    0x0f, 0x06, 0x16, 0x26,
    0x0f, 0x09, 0x19, 0x29
};

// Color palette for Kris & Monsters
const unsigned char palSprites[16] = {
    0x0f, 0x01, 0x33, 0x3c,
    0x0f, 0x15, 0x18, 0x19,
    0x0f, 0x28, 0x29, 0x2a,
    0x0f, 0x30, 0x10, 0x0f
};

const unsigned char** characterWalkAnims[]={
    krisWalkAnims
};

const unsigned char** characterStrikeAnims[]={
    krisStrikeAnims
};

// forward decls
void update_character(WalkingCharacter* chara);
void draw_character(WalkingCharacter* chara);

//
// Main entrypoint
// This is where your game will start running. It should essentially be an endless loop in most
// cases. The name "main" tells the runtime to run this. You can add more methods in this file
// or others and call them as your game expands. 
// 
void main(void) {

    // Init Kris
    kris.xpos = 16;
    kris.ypos = 16;
    kris.chartype = CH_KRIS;
    kris.substate = S_NORMAL;
    kris.direction = 0;
    kris.animframe = 0;

    // Turn off the screen
    ppu_off();

    // Load the background palette for Area 1
    pal_bg(paletteArea1);

    // Load the sprite palette
    pal_spr(palSprites);

    // Set sprite bank to bank 1
    bank_spr(1);

    // Write the address $2064 to the ppu, where we can start drawing text on the screen
    vram_adr(0x2064);

    i = 0;
    while (welcomeMessage[i]) {
        // Add 0x60 to the ascii value of each character, to get it to line up with where the ascii table is in our chr file
        vram_put(welcomeMessage[i] + 0x80);
        ++i;
    }

    // Set up game state
    playerLevel = 1;
    currentMap = 0;


    // Set the scroll to 0,0
    scroll(0, 0);


    // Turn the screen back on
    ppu_on_all();

    // Update variable used in unit tests
    testVariable = 1;

    // Play the first song built into the rom. By default it is the title song from Shiru's game, Lan Master
    //music_play(0);

    // Infinite loop to end things
    while (1) {
        framecount++;

        // Do input
        pad_trig = pad_trigger(0);
        pad = pad_state(0);

        // Wipe oams (perf?)
        oam_clear();

        // Update characters
        update_character(&kris);

        // Update monsters & projectiles

        // Set sprite count to 0
        spr = 0;

        // Draw characters
        draw_character(&kris);

        // Draw monsters & projectiles


        // Don't run until a frame has run.
        ppu_wait_nmi();
    }
}

void update_character(WalkingCharacter* chara)
{
    switch (chara->substate)
    {
        case S_NORMAL:
        {
            int did_walk = 0;
            if(pad_trig&PAD_A && chara->chartype == CH_KRIS && playerLevel > 0)
            {
                chara->substate = S_ATTACK;
                chara->animframe = 0;
                break;
            }

            if(pad&PAD_DOWN)
            {
                chara->direction = 0;
                chara->ypos++;
                did_walk = 1;
            }
            if(pad&PAD_RIGHT)
            {
                chara->direction = 1;
                chara->xpos++;
                did_walk = 1;
            }
            if(pad&PAD_UP)
            {
                chara->direction = 2;
                chara->ypos--;
                did_walk = 1;
            }
            if(pad&PAD_LEFT)
            {
                chara->direction = 3;
                chara->xpos--;
                did_walk = 1;
            }
            if(did_walk && framecount%16 == 0)
            {
                chara->animframe++;
            }
            break;
        }
        case S_ATTACK:
        {
            if(framecount%8 == 0)
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

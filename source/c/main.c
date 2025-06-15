// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"
#include "actors.h"
#include "kris_anims.h"
#include "maps.h"

//
// Global Variables (zeropage) 
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//
#pragma bss-name(push, "ZEROPAGE")
    unsigned char i,x,y;
    unsigned int attrib_addr;
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
const unsigned char hudMessage[] = "HP -----  LV _____  ";

// Color palette for the screen to use
const unsigned char paletteArea1[] = {
    0x17, 0x00, 0x10, 0x30,
    0x17, 0x27, 0x37, 0x28,
    0x17, 0x06, 0x16, 0x26,
    0x17, 0x09, 0x19, 0x29
};

// Color palette for Kris & Monsters
const unsigned char palSprites[16] = {
    0x0f, 0x03, 0x22, 0x3c,
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
void load_room(unsigned char roomNumber);
void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettenum);
int solidity_check(unsigned char px, unsigned char py);
int tilemap_solid(unsigned char tx, unsigned char ty);
int tile_solid(unsigned char tile);
//
// Main entrypoint
// This is where your game will start running. It should essentially be an endless loop in most
// cases. The name "main" tells the runtime to run this. You can add more methods in this file
// or others and call them as your game expands. 
// 
void main(void) {

    // Init Kris
    kris.xpos = 48;
    kris.ypos = 96;
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

    load_room(0);

    // Write the address $2064 to the ppu, where we can start drawing text on the screen
    vram_adr(NTADR_A(4,5));

    i = 0;
    while (welcomeMessage[i]) {
        // Add 0x60 to the ascii value of each character, to get it to line up with where the ascii table is in our chr file
        vram_put(hudMessage[i] + 0x80);
        ++i;
    }

    // Load first room of first map.

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

        spr = 0;

        // Update characters
        update_character(&kris);


        // Update monsters & projectiles


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
                if(solidity_check(chara->xpos, chara->ypos + 1)) chara->ypos++;
                did_walk = 1;
            }
            if(pad&PAD_RIGHT)
            {
                chara->direction = 1;
                if(solidity_check(chara->xpos + 1, chara->ypos)) chara->xpos++;
                did_walk = 1;
            }
            if(pad&PAD_UP)
            {
                chara->direction = 2;
                if(solidity_check(chara->xpos, chara->ypos - 1)) chara->ypos--;
                did_walk = 1;
            }
            if(pad&PAD_LEFT)
            {
                chara->direction = 3;
                if(solidity_check(chara->xpos - 1, chara->ypos)) chara->xpos--;
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

// TODO: Load room into specific H/V mirror space depending on exit travelled to
// TODO: Room exit traversal
void load_room(unsigned char roomNumber)
{
   unsigned char currentTileID = 0;
   // Set palette for desert


   for(x = 0; x < 12; x++)
   {
       for(y = 0; y < 8; y++)
       {
           i = (x + (y*12)) + 4; // add 4 to skip entrances/exits of room
           currentTileID = desert_room_0[i];
           vram_adr(NTADR_A((x+2)*2,(y+3)*2));
           vram_put(desert_metatiles[(currentTileID*5)]);
           vram_put(desert_metatiles[(currentTileID*5)+1]);
           vram_adr(NTADR_A((x+2)*2,((y+3)*2+1)));
           vram_put(desert_metatiles[(currentTileID*5)+2]);
           vram_put(desert_metatiles[(currentTileID*5)+3]);

           set_palette_for_bg_tile(x+2, y+3, desert_metatiles[(currentTileID*5)+4]);
       }
   }

}

void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettemask)
{
   unsigned char tilemask=0;
   // attrib table is 64 bytes long
   // each byte controls a square of 4x4 tiles
   // so for our metatiles, that means each byte controls 2x2 of them
   // so to set 1 metatile's palette we have to set only 2 bits of that attrib entry
   // first get current
   attrib_addr = 0x23C0 + (ty/2) * 8 + (tx/2);
   vram_adr(attrib_addr);
   vram_read(&i, 1); // now i contains current value
   // next we need to calculate the mask for this specific tile
   if(tx % 2 == 1 && ty % 2 == 1) tilemask = 0b11000000;
   if(tx % 2 == 0 && ty % 2 == 1) tilemask = 0b00110000;
   if(tx % 2 == 1 && ty % 2 == 0) tilemask = 0b00001100;
   if(tx % 2 == 0 && ty % 2 == 0) tilemask = 0b00000011;

   palettemask = i | (palettemask & tilemask);
   // then mask the existing data out with the mask so that we only set the bits for the current tile
   // then write that entry back into vram
   vram_adr(attrib_addr);
   vram_put(palettemask);
}

int solidity_check(unsigned char px, unsigned char py)
{
    // TopLeft
    x = (px+2) >> 4;
    y = (py+2) >> 4;
    if(tilemap_solid(x, y)) return 0;

    // TopRight
    x = (px+14) >> 4;
    y = (py+2) >> 4;
    if(tilemap_solid(x, y)) return 0;

    // BottomLeft
    x = (px+2) >> 4;
    y = (py+14) >> 4;
    if(tilemap_solid(x, y)) return 0;

    // BottomRight
    x = (px+14) >> 4;
    y = (py+14) >> 4;
    if(tilemap_solid(x, y)) return 0;

    return 1;
}

int tilemap_solid(unsigned char tx, unsigned char ty)
{
    x = tx - 2; // account for centering
    y = ty - 3;
    if(x < 0 || x >= 12) return 0;
    if(y < 0 || y >= 8) return 0;
    i = (x + (y*12)) + 4;
    return tile_solid(desert_room_0[i]);
}

int tile_solid(unsigned char tile)
{
    return tile == 1 || tile == 3 || tile == 4;
}

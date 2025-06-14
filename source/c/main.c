// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"

//
// Global Variables (zeropage) 
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//
#pragma bss-name(push, "ZEROPAGE")
    unsigned char i;
    unsigned char framecount;
    unsigned char spr;
    unsigned char pad;
    unsigned char animFrame;
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
const unsigned char welcomeMessage[] = "CONTROLLER NOT FOUND!";

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

// Kris metasprites
const unsigned char krisWalkDown0[]={
    0, 0, 0x01, 4,
    8, 0, 0x02, 4,
    0, 8, 0x03, 4,
    8, 8, 0x04, 4,
    128
};
const unsigned char krisWalkDown1[]={
    0, 0, 0x01, 4,
    8, 0, 0x02, 4,
    0, 8, 0x05, 4,
    8, 8, 0x06, 4,
    128
};
// forward decls
void draw_kris(unsigned char drawx, unsigned char drawy);

//
// Main entrypoint
// This is where your game will start running. It should essentially be an endless loop in most
// cases. The name "main" tells the runtime to run this. You can add more methods in this file
// or others and call them as your game expands. 
// 
void main(void) {
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
        // If the user is pressing A, make a sound!
        if (pad_poll(0) & PAD_A) {
            // Play sound effect 0 on channel 0 (second argument can be 0-3, lower is higher priority)
            sfx_play(0, 0);
        }

        spr = 0;
        draw_kris(16, 16);


        // Do input
        pad = pad_poll(0);


        // Don't run until a frame has run.
        ppu_wait_nmi();
    }
}

void draw_kris(unsigned char drawx, unsigned char drawy)
{
    if(framecount%16 == 0)
    {
        animFrame++;
    }
    spr = oam_meta_spr(drawx, drawy, spr, animFrame%2==0?krisWalkDown0:krisWalkDown1);
}

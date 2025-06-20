// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"
#include "globals.h"
#include "actors.h"
#include "kris_anims.h"
#include "maps.h"
#include "bank_helpers.h"
#include "items.h"


//
// Global Variables (zeropage) 
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//
#pragma bss-name(push, "ZEROPAGE")
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
    0x0f, 0x05, 0x15, 0x24,
    0x0f, 0x1d, 0x1b, 0x20,
    0x0f, 0x10, 0x12, 0x16
};

// forward decls
void load_room();
void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettenum);
void switch_to_room(unsigned char room);
void switch_environment(enum Environment env);

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
    vram_adr(NTADR_A(4,5));

    i = 0;
    while (hudMessage[i]) {
        // Add 0x60 to the ascii value of each character, to get it to line up with where the ascii table is in our chr file
        vram_put(hudMessage[i] + 0x80);
        ++i;
    }

    // Set up game state
    currentState = GS_GAMEPLAY;
    playerLevel = 0;
    currentEnvironment = E_DESERT;
    currentRoom = 0;
    soundTestNum = 0;

    // Set dead list empty
    for(i = 0; i < TOTAL_SPAWNABLES; i++)
    {
      deadList[i] = 0;
    }

    // Init Kris
    kris.xpos = 48;
    kris.ypos = 96;
    kris.chartype = CH_KRIS;
    kris.substate = S_NORMAL;
    kris.direction = 0;
    kris.animframe = 0;

    // Load first room of first map.
    bank_push(0);
    load_room();
    bank_pop();

    // Set the scroll to 0,0
    scroll(0, 0);

    // Turn the screen back on
    ppu_on_all();

    // Update variable used in unit tests
    testVariable = 1;

    // Infinite loop to end things
    while (1) {
        framecount++;

        // Do input
        pad_trig = pad_trigger(0);
        pad = pad_state(0);

        if(currentState == GS_GAMEPLAY)
        {
          // Wipe oams (perf?)
          oam_clear();
          spr = 0;

          // Update characters
          update_character(&kris);

          // SOUND TEST
          if(pad_trig & PAD_SELECT)
          {
              music_stop();
              music_play(soundTestNum);
              soundTestNum++;
              if(soundTestNum > 5)
              {
                soundTestNum = 0;
              }
          }

          // Update items
          for(i = 0; i < spawnedItems; i++)
          {
              update_item(&itemList[i], &kris);
          }


          // Update monsters & projectiles
          for(i = 0; i < spawnedMonsters; i++)
          {
             update_monster(&monsterList[i]);
          }


          // Draw characters
          draw_character(&kris);

          // Draw items
          for(i = 0; i < spawnedItems; i++)
          {
              draw_item(&itemList[i]);
          }

          // Draw monsters & projectiles
          for(i = 0; i < spawnedMonsters; i++)
          {
              draw_monster(&monsterList[i]);
          }
        }
        if(currentState == GS_SCREENTRANS)
        {
            oam_clear();
            spr = 0;
            // Check screen transition direction and move Kris in that direction until threshold is reached
            if(roomSwitchDir == 0)
            {
                kris.ypos -= 4;
                if(kris.ypos <= 48+4)
                {
                    // Once Kris is there, switch state back and turn on bgs
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                }
            }
            if(roomSwitchDir == 1)
            {
                kris.xpos -= 4;
                if(kris.xpos <= 32+4)
                {
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                }
            }
            if(roomSwitchDir == 2)
            {
                kris.ypos += 4;
                if(kris.ypos >= 160-4)
                {
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                }
            }
            if(roomSwitchDir == 3)
            {
                kris.xpos += 4;
                if(kris.xpos >= 208-4)
                {
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                }
            }
            // Render kris
            draw_character(&kris);
        }

        // Don't run until a frame has run.
        ppu_wait_nmi();

    }
}


// ROOM HANDLING FUNCTIONS
CODE_BANK(0);
void load_room()
{
   unsigned char currentTileID = 0;

   spawnedItems = 0;

   roomPtr = (unsigned char*)environment_rooms[currentEnvironment][currentRoom];
   metatilesPtr = (unsigned char*)environment_metatiles[currentEnvironment];
   // Load tiles into BG
   for(x = 0; x < 12; x++)
   {
       for(y = 0; y < 8; y++)
       {
           i = (x + (y*12)) + 4; // add 4 to skip entrances/exits of room
           currentTileID = roomPtr[i];
           vram_adr(NTADR_A((x+2)*2,(y+3)*2));
           vram_put(environment_metatiles[currentEnvironment][(currentTileID*6)]);
           vram_put(environment_metatiles[currentEnvironment][(currentTileID*6)+1]);
           vram_adr(NTADR_A((x+2)*2,((y+3)*2+1)));
           vram_put(environment_metatiles[currentEnvironment][(currentTileID*6)+2]);
           vram_put(environment_metatiles[currentEnvironment][(currentTileID*6)+3]);

           set_palette_for_bg_tile(x+2, y+3, environment_metatiles[currentEnvironment][(currentTileID*6)+4]);
       }
   }

   // Clear spawned item list
   spawnedItems = 0;
   for(i = 0; i < MAX_ITEMS; i++)
   {
       itemList[i].living = 0;
   }

   spawnedMonsters = 0;
   for(i = 0; i < MAX_MONSTERS; i++)
   {
       monsterList[i].living = 0;
   }

   // Load entity spawns
   for(i = (12*8) + 4; roomPtr[i] != 128; i+=5)
   {
       if(deadList[roomPtr[i+4]]) continue;
       if(roomPtr[i] == 2) // Spawn a sword
       {
           itemList[spawnedItems].itemtype = ITEM_SWORD;
           itemList[spawnedItems].xpos = ((roomPtr[i+1]+2) << 4) + 4;
           itemList[spawnedItems].ypos = ((roomPtr[i+2]+3) << 4) + 4;
           itemList[spawnedItems].living = 1;
           itemList[spawnedItems].uniqueid = roomPtr[i+4];
           spawnedItems++;
       }
       if(roomPtr[i] == 0) // Spawn a monster
       {
           monsterList[spawnedMonsters].montype = roomPtr[i+3];
           monsterList[spawnedItems].xpos = ((roomPtr[i+1]+2) << 4) + 4;
           monsterList[spawnedItems].ypos = ((roomPtr[i+2]+3) << 4) + 4;
           monsterList[spawnedItems].living = 1;
           monsterList[spawnedItems].uniqueid = roomPtr[i+4];
           spawnedMonsters++;
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

void switch_to_room(unsigned char room)
{
    currentState = GS_SCREENTRANS;
    // Turn off PPU
    ppu_off();
    // Load next room
    currentRoom = room;
    load_room();
    // Turn on sprites only for transfer
    ppu_on_spr();
}
CODE_BANK_POP();

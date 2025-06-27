// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"
#include "globals.h"
#include "actors.h"
#include "kris_anims.h"
#include "maps.h"
#include "bank_helpers.h"
#include "items.h"
#include "ui.h"

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


// Color palette for the screen to use
const unsigned char paletteDesert[] = {
    0x17, 0x0f, 0x16, 0x30, // Text
    0x17, 0x27, 0x37, 0x28, // Desert Sand & Vegetation
    0x17, 0x06, 0x16, 0x26, // Red Rocks, Bridges
    0x17, 0x27, 0x22, 0x31 // Water & Watery Rock
};

// BG Palettes per environment
const unsigned char* envPalettes[] =
{
    paletteDesert
//    paletteIsland,
//    paletteIcePalace,
//    paletteCity,
//    paletteShelter
};

// Color palettes for sprites (Kris & Monsters)
const unsigned char palSpritesDesert[16] = {
    0x0f, 0x03, 0x22, 0x3c,
    0x0f, 0x0f, 0x21, 0x30,
    0x0f, 0x28, 0x30, 0x15,
    0x0f, 0x10, 0x12, 0x16
};

const unsigned char* envSprPalettes[] =
{
    palSpritesDesert
};

// forward decls
void load_environment(enum Environment env);
void load_room();
void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettenum);
void switch_to_room(unsigned char room);

//
// Main entrypoint
// This is where your game will start running. It should essentially be an endless loop in most
// cases. The name "main" tells the runtime to run this. You can add more methods in this file
// or others and call them as your game expands. 
// 
void main(void) {
    // Turn off the screen
    ppu_off();

    // Set sprite bank to bank 1
    bank_spr(1);

    bank_push(0);
    draw_ui_borders();
    bank_pop();

    // Set up game state
    currentState = GS_GAMEPLAY;
    playerLevel = 0;
    playerHp = 16;
    playerExp = 0;
    currentRoom = 0;
    soundTestNum = 0;
    hudDirty = 1;

    // Set dead list empty
    for(i = 0; i < TOTAL_SPAWNABLES; i++)
    {
      deadList[i] = 0;
    }

    // Init Kris
    kris.xpos = 128;
    kris.ypos = 128;
    kris.chartype = CH_KRIS;
    kris.substate = S_NORMAL;
    kris.direction = 0;
    kris.animframe = 0;

    // Load first room of first map, in Desert.
    bank_push(0);
    load_environment(E_DESERT);
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

        // Clear vram write flags
        set_vram_update(NULL);
        writingVram = 0;

        // Do input
        pad_trig = pad_trigger(0);
        pad = pad_state(0);

        if(currentState == GS_GAMEPLAY)
        {
          // Wipe oams (perf?)
          oam_clear();
          spr = 0;

          // Update level logic
          if(playerExp == 16 && playerLevel < 4)
          {
             playerLevel++;
             playerExp = 0;
             hudDirty = 1;
          }

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
          for(i2 = 0; i2 < spawnedMonsters; i2++)
          {
             update_monster(&monsterList[i2]);
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

          // Update HUD if needed and possible
          if(hudDirty == 1)
          {
            if(writingVram == 0)
            {
              bank_push(0);
              refresh_hud_bars(playerHp, playerLevel, playerExp);
              bank_pop();
            }
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
                    pal_col(0, envPalettes[currentEnvironment][0]);
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                    set_map_tile_on_character(&kris, 0);
                }
            }
            if(roomSwitchDir == 1)
            {
                kris.xpos -= 4;
                if(kris.xpos <= 32+4)
                {
                    pal_col(0, envPalettes[currentEnvironment][0]);
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                    set_map_tile_on_character(&kris, 0);
                }
            }
            if(roomSwitchDir == 2)
            {
                kris.ypos += 4;
                if(kris.ypos >= 160-4)
                {
                    pal_col(0, envPalettes[currentEnvironment][0]);
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                    set_map_tile_on_character(&kris, 0);
                }
            }
            if(roomSwitchDir == 3)
            {
                kris.xpos += 4;
                if(kris.xpos >= 208-4)
                {
                    pal_col(0, envPalettes[currentEnvironment][0]);
                    ppu_on_all();
                    currentState = GS_GAMEPLAY;
                    set_map_tile_on_character(&kris, 0);
                }
            }
            // Render kris
            draw_character(&kris);
        }

        // Don't run until a frame has run.
        ppu_wait_nmi();

    }
}


// ROOM & ENVIRONMENT HANDLING FUNCTIONS in ROM_00
CODE_BANK(0);

void load_environment(enum Environment env)
{
    currentEnvironment = env;

    pal_bg(envPalettes[currentEnvironment]);
    pal_spr(envSprPalettes[currentEnvironment]);
}

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
           i2 = (x + (y*12));
           currentTileID = roomPtr[i];
           currentRoomColl[i2] = currentTileID;
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
   spawnedMonsters = 0;

   // Load entity spawns
   for(i = (12*8) + 4; roomPtr[i] != 128; i+=5)
   {
       if(deadList[roomPtr[i+4]]) continue;
       if(roomPtr[i] == 2) // Spawn a sword
       {
           itemList[spawnedItems].itemtype = ITEM_SWORD;
           itemList[spawnedItems].xpos = ((roomPtr[i+1]+2) << 4) + 4;
           itemList[spawnedItems].ypos = ((roomPtr[i+2]+3) << 4) + 4;
           itemList[spawnedItems].uniqueid = roomPtr[i+4];
           spawnedItems++;
       }
       if(roomPtr[i] == 0) // Spawn a monster
       {
           monsterList[spawnedMonsters].montype = roomPtr[i+3];
           monsterList[spawnedMonsters].xpos = ((roomPtr[i+1]+2) << 4);
           monsterList[spawnedMonsters].ypos = ((roomPtr[i+2]+3) << 4);
           monsterList[spawnedMonsters].health = 1;
           monsterList[spawnedMonsters].direction = rand8() >> 6;
           monsterList[spawnedMonsters].uniqueid = roomPtr[i+4];
           spawnedMonsters++;
       }
   }

}

void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettemask)
{
   unsigned char tilemask=0;
   unsigned char pal=0;
   // attrib table is 64 bytes long
   // each byte controls a square of 4x4 tiles
   // so for our metatiles, that means each byte controls 2x2 of them
   // so to set 1 metatile's palette we have to set only 2 bits of that attrib entry
   // first get current
   attrib_addr = 0x23C0 + (ty/2) * 8 + (tx/2);
   vram_adr(attrib_addr);
   vram_read(&pal, 1); // now pal contains current value
   // next we need to calculate the mask for this specific tile
   if(tx % 2 == 1 && ty % 2 == 1) tilemask = 0b11000000;
   if(tx % 2 == 0 && ty % 2 == 1) tilemask = 0b00110000;
   if(tx % 2 == 1 && ty % 2 == 0) tilemask = 0b00001100;
   if(tx % 2 == 0 && ty % 2 == 0) tilemask = 0b00000011;

   palettemask = (pal & ~tilemask) | (palettemask & tilemask);   // then mask the existing data out with the mask so that we only set the bits for the current tile
   // then write that entry back into vram
   vram_adr(attrib_addr);
   vram_put(palettemask);
}

void switch_to_room(unsigned char room)
{
    currentState = GS_SCREENTRANS;

    pal_col(0, 0x0F);
    ppu_wait_nmi();
    // Turn off PPU
    ppu_off();
    // Load next room
    currentRoom = room;
    load_room();
    // Turn on sprites only for transfer
    ppu_on_spr();
}
CODE_BANK_POP();

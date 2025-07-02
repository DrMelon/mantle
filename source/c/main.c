// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"
#include "globals.h"
#include "actors.h"
#include "kris_anims.h"
#include "maps.h"
#include "bank_helpers.h"
#include "mapper.h"
#include "items.h"
#include "ui.h"
#include "palettes.h"

//
// Global Variables (zeropage) 
// Small, frequently-used variables should go in this space. There are only around 250 bytes to go around, so choose wisely!
//


//
// Normal Variables
// You can define larger, less frequently accessed variables here. You have about 1.5k to work with.
//
// EXAMPLE: 
// unsigned char myBigBufferArray[32];

//
// Constant variables
// Anything with const in front of it will go into write-only prg instead of the very limited ram we have.
//

// forward decls
void load_environment(enum Environment env);
void load_room();
void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettenum);
void switch_to_room(unsigned char room);
void funny_test();
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

          if(queueTele != 0)
          {
            tele_to_room(queueTele, x+2, y+3);
            queueTele = 0;
            continue;
          }

          // Update level logic
          if(playerExp >= 16 && playerLevel < 4)
          {
             playerLevel++;
             playerExp = 0;
             hudDirty = 1;
          }

          // Update characters
          bank_push(0);
          update_character(&kris);
          bank_pop();

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

          if(pad_trig & PAD_START)
          {
              bank_push(0);
              if(textLength > 0)
              {
                clear_text();
              }
              else
              {
                queue_text(dialog_3, 1);
              }
              bank_pop();
          }

          // Update items
          for(i = 0; i < spawnedItems; i++)
          {
              update_item(&itemList[i], &kris);
          }


          // Update monsters & projectiles
          bank_push(1);
          for(i2 = 0; i2 < spawnedMonsters; i2++)
          {
             update_monster(&monsterList[i2]);
          }
          for(i = 0; i < spawnedProjectiles; i++)
          {
             update_projectile(&projList[i]);
             // TODO: Investigate why projectile deletion sometimes causes subsequent monsters to just Not Appear on other screens (even though _spawnedMonsters is still the correct value and _monsterList is populated.)
          }
          bank_pop();

          // Draw characters
          draw_character(&kris);

          // Draw items
          for(i = 0; i < spawnedItems; i++)
          {
              draw_item(&itemList[i]);
          }

          // Draw monsters & projectiles
          bank_push(1);
          for(i2 = 0; i2 < spawnedMonsters; i2++)
          {
              draw_monster(&monsterList[i2]);
          }

          for(i = 0; i < spawnedProjectiles; i++)
          {
             draw_projectile(&projList[i]);
          }
          bank_pop();

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
            // If the room we're switching to is a special room in the desert...
            if(currentEnvironment == E_DESERT)
            {
                //... we switch the palette to the desert-ice palette or the normal desert.
                if(currentRoom == 27)
                {
                    envPalettes[E_DESERT] = paletteDesertIce;
                    pal_bg(envPalettes[E_DESERT]);
                    pal_col(0, 0x0F);
                }
                else if(currentRoom == 25)
                {
                    envPalettes[E_DESERT] = paletteDesert;
                    pal_bg(envPalettes[E_DESERT]);
                    pal_col(0, 0x0F);
                }
            }

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
        if(currentState == GS_SCREENTRANS_TELE)
        {
           // Teleporting to room
           kris.xpos = x << 4;
           kris.ypos = y << 4;
           pal_col(0, envPalettes[currentEnvironment][0]);
           ppu_on_all();
           currentState = GS_GAMEPLAY;

           //entering/leaving the shop room in the desert?
           if(currentEnvironment == E_DESERT)
           {
                if(currentRoom == 19)
                {
                    bank_push(0);
                    if(playerLevel < 3)
                    {
                      queue_text(instruct_0, 1);
                    }
                    else if(playerLevel < 4)
                    {
                      queue_text(instruct_1, 1);
                    }
                    bank_pop();
                }
                else if(prevRoom == 19)
                {
                    bank_push(0);
                    clear_text();
                    bank_pop();
                }
            }
        }

        // Text routines
        bank_push(0);
        update_text();
        bank_pop();

        // Don't run until a frame has run.
        ppu_wait_nmi();

    }
}

// ROOM & ENVIRONMENT HANDLING FUNCTIONS in ROM_00
CODE_BANK(0);

void load_environment(enum Environment env)
{
    currentEnvironment = env;

    mmc1_set_chr_bank_0(env * 2);
    mmc1_set_chr_bank_1((env * 2) + 1);

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
   spawnedTeles = 0;
   spawnedProjectiles = 0;

   // Load entity spawns
   for(i = (12*8) + 4; roomPtr[i] != 128; i+=6)
   {
       if(roomPtr[i] == 1) // Entrance/exit/teleporter
       {
           teleList[spawnedTeles].tx = roomPtr[i+1];
           teleList[spawnedTeles].ty = roomPtr[i+2];
           teleList[spawnedTeles].targetroom = roomPtr[i+3];
           teleList[spawnedTeles].targetx = roomPtr[i+4];
           teleList[spawnedTeles].targety = roomPtr[i+5];
           spawnedTeles++;
           continue;
       }
       if(deadList[roomPtr[i+5]]) continue;
       if(roomPtr[i] == 2) // Spawn a sword
       {
           itemList[spawnedItems].itemtype = ITEM_SWORD;
           itemList[spawnedItems].xpos = ((roomPtr[i+1]+2) << 4) + 12;
           itemList[spawnedItems].ypos = ((roomPtr[i+2]+3) << 4) + 12;
           itemList[spawnedItems].uniqueid = roomPtr[i+5];
           spawnedItems++;
       }
       else if(roomPtr[i] == 0) // Spawn a monster
       {
           monsterList[spawnedMonsters].montype = roomPtr[i+3];
           monsterList[spawnedMonsters].xpos = ((roomPtr[i+1]+2) << 4);
           monsterList[spawnedMonsters].ypos = ((roomPtr[i+2]+3) << 4);
           monsterList[spawnedMonsters].health = 1;
           if(monsterList[spawnedMonsters].montype == MON_SHOOTER)
           {
               monsterList[spawnedMonsters].health = 2;
               monsterList[spawnedMonsters].level = 1;
           }
           monsterList[spawnedMonsters].direction = 0;
           monsterList[spawnedMonsters].uniqueid = roomPtr[i+5];
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
    prevRoom = currentRoom;
    currentRoom = room;
    load_room();
    // Turn on sprites only for transfer
    ppu_on_spr();
}

void tele_to_room(unsigned char room, unsigned char telex, unsigned char teley)
{
    currentState = GS_SCREENTRANS_TELE;

    pal_col(0, 0x0F); // Black BG
    ppu_wait_nmi(); // wait till end of frame

    // Turn off PPU
    ppu_off();

    // Load room
    prevRoom = currentRoom;
    currentRoom = room;
    load_room();

    // Set sprite pos
    x = telex;
    y = teley;

}
CODE_BANK_POP();

#include "neslib.h"
#include "bank_helpers.h"
#include "roomstuff.h"
#include "globals.h"
#include "palettes.h"
#include "mapper.h"
#include "ui.h"

// ROOM & ENVIRONMENT HANDLING FUNCTIONS in ROM_00
CODE_BANK(ROOM_LOGIC_BANK);

void load_env_target_banked()
{
    load_environment(currentEnvironment);
}

void load_environment(enum Environment env)
{
    currentEnvironment = env;

    // Set dead list empty (as it is a per-environment tracker)
    for(i = 0; i < TOTAL_SPAWNABLES; i++)
    {
      deadList[i] = 0;
    }


    if(env == E_DESERT)
    {
        currentRoom = 0;
        monsterAggression = 0; // Monsters start docile in the desert.
        treeRoomVisits = 0;
    }
    if(env == E_ISLAND)
    {
        currentRoom = 3;
        monsterAggression = 0; // Monsters start docile in the island too, but get angry faster.
        playerLevel = 0;
        playerHp = 16;
        playerExp = 0;
        kris.xpos = 130;
        kris.ypos = 120;
    }

    kris.direction = 0;
    kris.animframe = 0;

    mmc1_set_chr_bank_0(env * 2);
    mmc1_set_chr_bank_1((env * 2) + 1);

    pal_bg(envPalettes[currentEnvironment]);
    pal_spr(envSprPalettes[currentEnvironment]);
}

void load_room_intro()
{
   // 1. Set up the "loading heart" sprite and set OAM mem, drawing it when ready.
   // also, load the room collision data while we're here and set up the relevant room pointers etc.
   framecount = 0;
   scroll(0, 0);
   oam_clear();
   spr = 0;
   spawnedItems = 0;
   spawnedMonsters = 0;
   spawnedTeles = 0;
   spawnedProjectiles = 0;
   roomPtr = (unsigned char*)environment_rooms[currentEnvironment][currentRoom];
   metatilesPtr = (unsigned char*)environment_metatiles[currentEnvironment];

   // Wait for 1 second
   while(framecount < 60)
   {
       framecount++;
       ppu_wait_nmi();
   }

   // 2. Draw UI and wait for a frame
   banked_call(0, init_hud_refresh_banked);
   ppu_wait_nmi(); // wait till end of frame

   // 3. For each tile line in the room, load and draw room line and attribs, and wait for a handful of frame between, like the teleport blackout.

   // 4. As the line approaches kris's position and midpoint, load in kris's top and bottom half sprites and set OAM mem.

   // 5. once all lines are drawn, hand over to game state completely. we did it, folks!
   // don't forget to load the teleporters though.
   currentState = GS_GAMEPLAY;

 
}

void load_room()
{
   unsigned char currentTileID = 0;

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
           if(x == 4 && y == 4)
           {
                if(currentEnvironment == E_DESERT && currentRoom == 26)
                {
                        // Are we in the Tree Room?
                        // Increment the tree-room-visits counter
                        treeRoomVisits++;

                        if(treeRoomVisits >= 4)
                        {
                             treeRoomVisits = 4;
                             // Spawn the chest instead of the usual tile
                             currentTileID = TILE_D_CHEST_CLOSED;
                        }
                }
           }

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
           monsterList[spawnedMonsters].level = 1;
           monsterList[spawnedMonsters].substate = S_NORMAL;
           if(monsterList[spawnedMonsters].montype == MON_SHOOTER)
           {
               monsterList[spawnedMonsters].health = 2;
           }
           if(monsterList[spawnedMonsters].montype == MON_FISH)
           {
               monsterList[spawnedMonsters].level = 2;
           }
           if(monsterList[spawnedMonsters].montype == MON_LIZARD)
           {
               monsterList[spawnedMonsters].health = 3;
               monsterList[spawnedMonsters].arcid = 255;
           }
           monsterList[spawnedMonsters].direction = rand8();
           monsterList[spawnedMonsters].direction = monsterList[spawnedMonsters].direction >> 6;
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
    // Set sprite pos
    x = telex;
    y = teley;

    prevRoom = currentRoom;
    currentRoom = room;
}

// REQUIRES BANKED CALL
void skip_to_island()
{
    ppu_off();
    load_environment(E_ISLAND);
    load_room();
    ppu_on_all();
}

CODE_BANK_POP();

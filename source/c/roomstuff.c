#include "neslib.h"
#include "bank_helpers.h"
#include "roomstuff.h"
#include "globals.h"
#include "palettes.h"
#include "mapper.h"
#include "ui.h"
#include "rafts.h"
#include "utils.h"

// ROOM & ENVIRONMENT HANDLING FUNCTIONS in ROM_00
CODE_BANK(ROOM_LOGIC_BANK);

const unsigned char heartSpr0[]={
   4, 4, 0x90, 3,
   128
};

const unsigned char heartSpr1[]={
   0, 0, 0x01, 4,
   8, 0, 0x02, 4,
   4, 4, 0x90, 3,
   128
};

const unsigned char heartSpr2[]={
   0, 0, 0x01, 0,
   8, 0, 0x02, 0,
   0, 8, 0x03, 0,
   8, 8, 0x04, 0,
   5, 9, 0x0D, 2, // scarf
   128
};

const unsigned char* const loadingHeartAnim[]={
    heartSpr0, // heart alone
    heartSpr1, // kris top half of sprite on top of heart
    heartSpr2 // full kris body
};

void reload_area()
{
    // Load env
    load_environment(currentEnvironment);
    oam_clear();
    spr = 0;

    pal_col(0, 0x0F); // Black BG
    ppu_wait_nmi(); // wait till end of frame
    // Turn off PPU
    ppu_off();
    // Load room
    load_room();

    ppu_on_all();

    pal_col(0, envPalettes[currentEnvironment][0]);
    hudDirty = 1;
}

void load_env_target_banked()
{
    load_environment(currentEnvironment);
}

void load_environment(enum Environment env)
{
    currentEnvironment = env;

    // Set dead list empty (as it is a per-environment tracker)
    for(i = 0; i < DEAD_LIST_LEN; i++)
    {
      deadList[i] = 0;
    }

    if(env == E_DESERT)
    {
        mmc1_set_chr_bank_0(0);
        mmc1_set_chr_bank_1(0 + 1);
        playerHp = 16;
        playerExp = 0;
        playerLevel = 0;
        currentRoom = 0;
        monsterAggression = 0; // Monsters start docile in the desert.
        treeRoomVisits = 0;
        kris.xpos = 128;
        kris.ypos = 128;
    }
    else if(env == E_ISLAND)
    {
        mmc1_set_chr_bank_0(2);
        mmc1_set_chr_bank_1(2 + 1);
        currentRoom = 3;
        monsterAggression = 0; // Monsters start docile in the island too, but get angry faster.
        playerLevel = 0;
        playerHp = 16;
        playerExp = 0;
        kris.xpos = 130;
        kris.ypos = 120;
    }
    else if(env == E_ICEPALACE)
    {
       mmc1_set_chr_bank_0(2);
       mmc1_set_chr_bank_1(2 + 1);
       currentRoom = 0;
       monsterAggression = 1;
       playerLevel = 2;
       playerHp = 16;
       playerExp = 0;
       kris.xpos = 128;
       kris.ypos = 128;
       narrative_flag_clr(NARFLAG_FOUND_NOELLE);
       // Glaceir.
       music_play(MUSIC_GLACEIR);
    }
    else if(env == E_CITY)
    {
       mmc1_set_chr_bank_0(4);
       mmc1_set_chr_bank_1(4 + 1);
       currentRoom = 0;
       monsterAggression = 0;
       playerLevel = 0;
       playerHp = 16;
       playerExp = 0;
       kris.xpos = 128;
       kris.ypos = 128;

       followerA.xpos = 130;
       followerA.ypos = 130;
       followerA.chartype = CH_SUSIE;
       followerA.direction = 0;
       followerB.xpos = 140;
       followerB.ypos = 140;
       followerB.chartype = CH_RALSEI;
       followerB.direction = 3;

       narrative_flag_clr(NARFLAG_KILLED_SUSIE);
       narrative_flag_clr(NARFLAG_KILLED_RALSEI);

    }
    else if(env == E_DUNGEON)
    {
       mmc1_set_chr_bank_0(2);
       mmc1_set_chr_bank_1(2 + 1);
       currentRoom = 0;
       monsterAggression = 1;
       playerLevel = 3;
       playerHp = 16;
       playerExp = 0;
       kris.xpos = 128;
       kris.ypos = 128;

       narrative_flag_clr(NARFLAG_KILLED_SONGCAT);
    }
    else if(env == E_SHELTERFOREST)
    {
       mmc1_set_chr_bank_0(4);
       mmc1_set_chr_bank_1(4 + 1);
       currentRoom = 0;
       monsterAggression = 1;
       playerLevel = 4;
       playerHp = 16;
       playerExp = 0;
       kris.xpos = 128;
       kris.ypos = 128;
    }
    else if(env == E_SHELTER)
    {
       mmc1_set_chr_bank_0(4);
       mmc1_set_chr_bank_1(4 + 1);
       currentRoom = 0;
       monsterAggression = 1;
       playerLevel = 4;
       playerHp = 16;
       playerExp = 0;
       kris.xpos = 128;
       kris.ypos = 128;
    }

    kris.direction = 0;
    kris.animframe = 0;
    kris.substate = S_NORMAL;

    spawnedRafts = 0; // only reset raft spawns when starting an environment over

    pal_bg(envPalettes[currentEnvironment]);
    pal_spr(envSprPalettes[currentEnvironment]);

    hudDirty = 1;

}

void unpack_room()
{
   // Unpacks currentRoom into memory via RLE
   roomPtr = environment_rooms[currentEnvironment][currentRoom];

   // first four bytes are directional data
   unpackedRoom[0] = roomPtr[0];
   unpackedRoom[1] = roomPtr[1];
   unpackedRoom[2] = roomPtr[2];
   unpackedRoom[3] = roomPtr[3];

   // Begin RLE unpack.
   i = 4; // current seek index
   i2 = 0; // number of tiles unpacked so far
   while(i2 < 12*8) // unpack all level tiles
   {
     x = roomPtr[i]; // get tile id
     x2 = roomPtr[i+1]; // get tile rle count
     y = 0;
     while(y < x2) // go through this rle count
     {
       unpackedRoom[4+i2] = x; // unpack tile
       i2++;
       y++;
     }
     i+=2; // skip to next tile rle entry
   }

   // Finish by unpacking game object data
   while(1)
   {
     unpackedRoom[4+i2] = roomPtr[i];
     if(unpackedRoom[4+i2] == 128)
       break;
     i++;
     i2++;
   }

   // Now set roomPtr to unpackedRoom, so that game functions can read that room data.
   roomPtr = unpackedRoom;
}

void load_room_intro()
{
   unsigned char currentTileID = 0;
   unsigned int ntrAdr = 0;
   unsigned char palmask = 0;
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
   unpack_room();
   metatilesPtr = environment_metatiles[currentEnvironment];

   // load heart and draw it
   spr = oam_meta_spr(kris.xpos, kris.ypos, spr, loadingHeartAnim[0]);

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
   for(y = 0; y < 8; y++)
   {
        if(y+3 == kris.ypos >> 4)
        {
            spr = 0;
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, loadingHeartAnim[1]);
        }
        else if(y+3 > kris.ypos >> 4)
        {
            spr = 0;
            spr = oam_meta_spr(kris.xpos, kris.ypos, spr, loadingHeartAnim[2]);
        }
        framecount = 0;

        // TODO: Set attribute table.
        // Since each attribute *memory entry* controls a 4x4 grid of tiles, we'll need to calculate
        // each of the 4 tiles for the current memory entry. so we only want to do it when y % 2 == 0.
        if(y % 2 == 0 || y == 7)
        {
           palmask = 0;
           ntrAdr = 0x23C0 + (((y+3)/2) * 8) + 1;
           palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
           palmTreeBuffer[1] = LSB(ntrAdr);
           palmTreeBuffer[2] = 6;
           for(x = 0; x < 6; x++)
           {
              // calculate the 4x4 as required

              // bottomright
              if(y != 7)
              {
                x2 = x<<1;
                y2 = y+1;
                i = (x2 + (y2*12)) + 4;
                currentTileID = roomPtr[i]; // get tile id for this tile & calc this part of the mask
                palmask |= metatilesPtr[(currentTileID*6)+4] & 0b11000000;

                // bottomleft
                x2++;
                i = (x2 + (y2*12)) + 4;
                currentTileID = roomPtr[i];
                palmask |= metatilesPtr[(currentTileID*6)+4] & 0b00110000;
              }
              // topright
              if(y > 0)
              {
                x2 = x<<1;
                y2 = y;
                i = (x2 + (y2*12)) + 4;
                currentTileID = roomPtr[i];
                palmask |= metatilesPtr[(currentTileID*6)+4] & 0b00001100;

                // topleft
                x2++;
                i = (x2 + (y2*12)) + 4;
                currentTileID = roomPtr[i];
                palmask |= metatilesPtr[(currentTileID*6)+4] & 0b00000011;
              }

              palmTreeBuffer[3+x] = palmask;
           }
           palmTreeBuffer[9] = NT_UPD_EOF;
           set_vram_update(palmTreeBuffer);
           framecount++;
           ppu_wait_nmi();
        }


        ntrAdr = NTADR_A(4,(y+3)*2);
        palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
        palmTreeBuffer[1] = LSB(ntrAdr);
        palmTreeBuffer[2] = 24;
        for(x = 0; x < 12; x++)
        {
            i = (x + (y*12)) + 4; // add 4 to skip entrances/exits of room
            i2 = (x + (y*12));
            currentTileID = roomPtr[i];

            // queue up a line update
            palmTreeBuffer[(x*2)+3] = metatilesPtr[currentTileID*6];
            palmTreeBuffer[(x*2)+4] = metatilesPtr[(currentTileID*6)+1];

            writingVram = 1;
        }

        palmTreeBuffer[27] = NT_UPD_EOF;

        set_vram_update(palmTreeBuffer);
        framecount++;
        ppu_wait_nmi();

        ntrAdr = NTADR_A(4,((y+3)*2)+1);
        palmTreeBuffer[0] = MSB(ntrAdr) | NT_UPD_HORZ;
        palmTreeBuffer[1] = LSB(ntrAdr);
        palmTreeBuffer[2] = 24;
        for(x = 0; x < 12; x++)
        {
            i = (x + (y*12)) + 4; // add 4 to skip entrances/exits of room
            currentTileID = roomPtr[i];
            //unpackedRoom[i] = currentTileID; // set collision

            // queue up a line update
            palmTreeBuffer[(x*2)+3] = metatilesPtr[(currentTileID*6)+2];
            palmTreeBuffer[(x*2)+4] = metatilesPtr[(currentTileID*6)+3];
            writingVram = 1;
        }
        palmTreeBuffer[27] = NT_UPD_EOF;

        set_vram_update(palmTreeBuffer);
        while(framecount < 30)
        {
            framecount++;
            ppu_wait_nmi();
        }


   }


   // 5. once all lines are drawn, hand over to game state completely. we did it, folks!
   // don't forget to load the teleporters though.
   for(i = (12*8) + 4; roomPtr[i] != 128; i+=6)
   {
       if(roomPtr[i] == 1) // Entrance/exit/teleporter
       {
           teleList[spawnedTeles].tele.tx = roomPtr[i+1];
           teleList[spawnedTeles].tele.ty = roomPtr[i+2];
           teleList[spawnedTeles].tele.targetroom = roomPtr[i+3];
           teleList[spawnedTeles].tele.targetx = roomPtr[i+4];
           teleList[spawnedTeles].tele.targety = roomPtr[i+5];
           spawnedTeles++;
           continue;
       }
    }


   currentState = GS_GAMEPLAY;
}

void load_room()
{
   unsigned char currentTileID = 0;

   unpack_room();
   metatilesPtr = (unsigned char*)environment_metatiles[currentEnvironment];
   // Load tiles into BG
   for(x = 0; x < 12; x++)
   {
       for(y = 0; y < 8; y++)
       {
           i = (x + (y*12)) + 4; // add 4 to skip entrances/exits of room
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
                             unpackedRoom[i] = TILE_D_CHEST_CLOSED;
                        }
                }
           }
           if(y == 4)
           {
             if(x == 5 || x == 6)
             {
                if(currentEnvironment == E_ISLAND && currentRoom == 15 && prevRoom == 6) // in island bridge room, take away bridge if on raft
                {
                   currentTileID = TILE_I_WATER;
                   unpackedRoom[i] = currentTileID;
                }
             }
           }

           vram_adr(NTADR_A((x+2)*2,(y+3)*2));
           vram_put(metatilesPtr[(currentTileID*6)]);
           vram_put(metatilesPtr[(currentTileID*6)+1]);
           vram_adr(NTADR_A((x+2)*2,((y+3)*2+1)));
           vram_put(metatilesPtr[(currentTileID*6)+2]);
           vram_put(metatilesPtr[(currentTileID*6)+3]);

           set_palette_for_bg_tile(x+2, y+3, metatilesPtr[(currentTileID*6)+4]);
       }
   }

   // Clear spawned item list
   spawnedItems = 0;
   spawnedMonsters = 0;
   spawnedTeles = 0;
   spawnedProjectiles = 0;
   spawnedButtons = 0;
   // Load entity spawns
   for(i = (12*8) + 4; roomPtr[i] != 128; i+=6)
   {
       if(roomPtr[i] == 1) // Entrance/exit/teleporter
       {
           teleList[spawnedTeles].tele.tx = roomPtr[i+1];
           teleList[spawnedTeles].tele.ty = roomPtr[i+2];
           teleList[spawnedTeles].tele.targetroom = roomPtr[i+3];
           teleList[spawnedTeles].tele.targetx = roomPtr[i+4];
           teleList[spawnedTeles].tele.targety = roomPtr[i+5];
           spawnedTeles++;
           continue;
       }
       if(is_dead(roomPtr[i+5])) continue; // don't respawn killed things
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
           monsterList[spawnedMonsters].arcid = 0;
           monsterList[spawnedMonsters].animframe = 0;
           monsterList[spawnedMonsters].level = 1;
           monsterList[spawnedMonsters].substate = S_NORMAL;
           if(monsterList[spawnedMonsters].montype == MON_WALKER)
           {
               if(roomPtr[i+4] == 1)
                 monsterList[spawnedMonsters].level = 5; // BEEFY!!!
           }
           if(monsterList[spawnedMonsters].montype == MON_SHOOTER)
           {
               monsterList[spawnedMonsters].health = 2;
           }
           if(monsterList[spawnedMonsters].montype == MON_FISH)
           {
               if(currentEnvironment == E_DESERT)
                 monsterList[spawnedMonsters].level = 2;
           }
           if(monsterList[spawnedMonsters].montype == MON_LIZARD)
           {
               monsterList[spawnedMonsters].health = 3;
               monsterList[spawnedMonsters].arcid = 255;
           }
           if(monsterList[spawnedMonsters].montype == MON_BIRD)
           {
               monsterList[spawnedMonsters].health = 3;
               monsterList[spawnedMonsters].level = 4; // birds are stronk
           }
           if(monsterList[spawnedMonsters].montype == MON_SINGCAT)
           {
               monsterList[spawnedMonsters].arcid = roomPtr[i+4]; // set singing/waiting type
           }
           monsterList[spawnedMonsters].direction = rand8();
           monsterList[spawnedMonsters].direction = monsterList[spawnedMonsters].direction >> 6;
           monsterList[spawnedMonsters].uniqueid = roomPtr[i+5];
           spawnedMonsters++;
       }
       else if(roomPtr[i] == 3) // spawn a raft
       {
            raftList[spawnedRafts].xpos = ((roomPtr[i+1]+2)) << 4;
            raftList[spawnedRafts].ypos = ((roomPtr[i+2]+3)) << 4;
            raftList[spawnedRafts].assignedchar = NULL;
            raftList[spawnedRafts].currentRoom = currentRoom;
            spawnedRafts++;
            mark_dead(roomPtr[i+5]); // do *not* respawn rafts conventionally.
       }
       else if(roomPtr[i] == 4) // spawn a button (ice palace, dungeon)
       {
            teleList[spawnedButtons].btn.xpos = ((roomPtr[i+1]+2)) << 4;
            teleList[spawnedButtons].btn.ypos = ((roomPtr[i+2]+3)) << 4;
            teleList[spawnedButtons].btn.pressed = 0;
            spawnedButtons++;
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

void switch_to_room()
{
    unsigned char room = roomPtr[roomSwitchDir];
    currentState = GS_SCREENTRANS;

    if(room == 255) // invalid room transition? must be a special room.
    {
        if(currentEnvironment == E_DESERT)
        {
            room = 26; // use the "tree room" in the desert.
        }
    }

    // Load next room
    prevRoom = currentRoom;
    currentRoom = room;

    pal_col(0, 0x0F);
    ppu_wait_nmi();
    ppu_off();
    load_room();
    ppu_on_spr(); // sprite only for transfer

}

// BANKED: x=  room, x2 = telex, y2 = teley
void tele_to_room()
{
    unsigned char room = x;
    unsigned char telex = x2;
    unsigned char teley = y2;
    currentState = GS_SCREENTRANS_TELE;
    // Set sprite pos when main game update occurs
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

void skip_to_ice_palace()
{
    ppu_off();
    load_environment(E_ICEPALACE);
    load_room();
    ppu_on_all();
}

void skip_to_city()
{
    ppu_off();
    load_environment(E_CITY);
    load_room();
    ppu_on_all();
}

void skip_to_forest()
{
    ppu_off();
    load_environment(E_SHELTERFOREST);
    load_room();
    ppu_on_all();
}

void skip_to_dungeon()
{
    ppu_off();
    load_environment(E_DUNGEON);
    load_room();
    ppu_on_all();
}

void skip_to_shelter()
{
    ppu_off();
    load_environment(E_SHELTER);
    load_room();
    ppu_on_all();
}

void on_enter_special_room()
{
   // For all the special logic upon entering a room; text displays, narrative bits, etc.
    if(currentEnvironment == E_DESERT)
    {
        if(currentRoom == 27) // Desert/Cold Desert pallete switcher
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

        if(currentRoom == 19) // Shop text
        {
            if(playerLevel < 3)
            {
                queue_text(instruct_0, 1);
            }
            else if(playerLevel < 4)
            {
                queue_text(instruct_1, 1);
            }
        }
        else if(prevRoom == 19) // Clear text
        {
            clear_text();
        }
    }
    else if(currentEnvironment == E_ISLAND)
    {
        // Northern Lights isle palette swap
        if(currentRoom == 44 || currentRoom == 43 || currentRoom == 42)
        {
            envPalettes[E_ISLAND] = paletteIslandIce;
            pal_bg(envPalettes[E_ISLAND]);
            pal_col(0, 0x0F);
        }
        else if(currentRoom == 34 || currentRoom == 35 || currentRoom == 36)
        {
            envPalettes[E_ISLAND] = paletteIsland;
            pal_bg(envPalettes[E_ISLAND]);
            pal_col(0, 0x0F);
        }

        // Entering a "Go To Northern Light" room
        if(currentRoom == 28 || currentRoom == 29)
        {
            queue_text(northernlight_0, 1);
            music_play(MUSIC_NORTHERNLIGHT);
        }
        else if(prevRoom == 28 || prevRoom == 29) // Clear text
        {
            clear_text();
            music_play(MUSIC_SWORD_SLOW);
        }
    }
    else if(currentEnvironment == E_ICEPALACE)
    {
        // If noelle not yet spawned...
        if(currentRoom == 7 && narrative_flag_get(NARFLAG_FOUND_NOELLE) == 0)
        {
            // ... spawn her and adjust room exit to travel to the "back side" of the palace
            narrative_flag_set(NARFLAG_FOUND_NOELLE);
            followerA.chartype = CH_NOELLE;
            followerA.direction = 0; //facing down
            followerA.substate = S_NORMAL;
            followerA.animframe = 0;
            // spawns at right side
            followerA.xpos = 120+32;
            followerA.ypos = 104;

            unpackedRoom[3] = 13; // switch travel destination
        }
    }
}


CODE_BANK_POP();

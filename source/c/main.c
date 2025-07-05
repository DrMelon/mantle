// Include defines for various pieces of the NES hardware
#include "system-defines.h"
#include "neslib.h"
#include "globals.h"
#include "actors.h"
#include "kris_anims.h"
#include "roomstuff.h"
#include "maps.h"
#include "bank_helpers.h"
#include "mapper.h"
#include "items.h"
#include "ui.h"
#include "palettes.h"

// forward decls
void load_environment(enum Environment env);
void load_room();
void set_palette_for_bg_tile(unsigned char tx, unsigned char ty, unsigned char palettenum);
void switch_to_room(unsigned char room);

//
// Main entrypoint
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
          spr = 0;

          if(queueTele != 0)
          {
            tele_to_room(queueTele, x+2, y+3);
            queueTele = 0;
            continue;
          }

          // Update level logic
          if(playerExp >= 24 && playerLevel < 4)
          {
             playerLevel++;
             playerExp = 0;
             if(playerLevel == 2 && currentEnvironment == E_DESERT)
             {
                 monsterAggression++; // monsters only become violent and dangerous after lvl 2 in the desert
             }
             if(playerLevel == 4)
             {
                 playerExp = 24; // full exp
             }
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


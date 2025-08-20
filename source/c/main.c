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
#include "cheats.h"
#include "intro.h"
#include "utils.h"
#include "twisted.h"
#include "teleport.h"

//
// Main entrypoint
//

void main(void) {
    // Turn off the screen
    ppu_off();

    // Init bg and spr banks
    bank_bg(0);
    bank_spr(1);

    // init sound driver
    bank_push(MUSIC_BANK);
    famistudio_init(FAMISTUDIO_PLATFORM_NTSC, music_data_mantle);
    bank_pop();

    // Set up game state
    narrativeFlags = 0; // ensure narrative flags are blank
    queueTele = 255;
    currentState = GS_NOTHING; // transition to GS_GAMEPLAY after...
    playerLevel = 0;
    playerHp = 16;
    playerExp = 0;
    currentRoom = 0;
    soundTestNum = 0;
    hudDirty = 1;

    // Init Kris
    kris.xpos = 128;
    kris.ypos = 128;
    kris.substate = S_NORMAL;
    kris.direction = 0;
    kris.animframe = 0;

    // Set the scroll to 0,0
    scroll(0, 0);

    // Turn the screen back on
    //ppu_on_all();

    #ifdef CHEATS_ENABLED
    cheatInputIdx = 0;
    #endif

    // Initialise intro

    bank_push(INTRO_BANK);
    load_and_show_intro();
    bank_pop();

    banked_call(TWISTED_BANK, init_twisted);

    music_play(MUSIC_INTRO);

    // Infinite loop to end things
    while (1) {
        framecount++;

        // Clear vram write flags
        set_vram_update(NULL);
        writingVram = 0;

        // Do input
        pad_trig = pad_trigger(0);
        pad = pad_state(0);

        // do cheats check if cheats compiled
        #ifdef CHEATS_ENABLED
        bank_push(CHEAT_CODES_BANK);
        update_cheats();
        bank_pop();
        #endif

        // Always update theatrics, no matter which game state
        // this lets us do theatrics *during* gameplay etc
        if(theatricActive == 1)
        {
            if(theatricIndex == TH_INTRO)
            {
                bank_push(INTRO_BANK);
                update_intro();
                bank_pop();
            }
            else if(theatricIndex == TH_GETSWORD)
            {
              //simply run down a timer and expire this theatric when done
              theatricTimer++;
              if(theatricTimer > 240)
              {
                theatricTimer = 0;
                theatricIndex = 0;
                theatricActive = 0;
                // teleport out of the sword zone and play the sword tune
                if(currentEnvironment == E_DESERT)
                {
                    music_play(MUSIC_SWORD);
                    x = 5;
                    y = 3;
                    queueTele = 5;
                }
                else if(currentEnvironment == E_ISLAND)
                {
                    music_play(MUSIC_SWORD_SLOW);
                    queueTele = 0;
                    x = 2;
                    y = 3;
                }
              }
            }
            else if(theatricIndex == TH_GETICEKEY)
            {
              // wait until ominous music/jingle stops
              theatricTimer++;
              if(theatricTimer > 240)
              {
                end_theatric();
                // clear text
                textQueued = 3;
                // go to island
                bank_push(ROOM_LOGIC_BANK);
                skip_to_island();
                bank_pop();
              }
            }
            else if(theatricIndex == TH_USEICEKEY)
            {
              theatricTimer++;
              if(theatricTimer > 240)
              {
                end_theatric();
                textQueued = 3;
                banked_call(ROOM_LOGIC_BANK, skip_to_ice_palace);
              }
            }
            else if(theatricIndex == TH_TEXT_GENERIC)
            {
              // Reusable theatric that queues text to some parameter.
              if(textEntriesLeft > 0 && textQueued == 0) // waiting for player input
              {
                 if(pad_trig & PAD_A)
                 {
                     theatricStage++;
                     bank_push(UI_BANK);
                     clear_text();
                     update_text();
                     ppu_wait_nmi();
                     queue_text(currentDialogPtr[theatricStage], 1);
                     bank_pop();
                 }
              }
              else if(textEntriesLeft == 0 && textQueued == 0)
              {
                if(pad_trig & PAD_A)
                {
                    end_theatric();
                    clear_text();
                }
              }
            }
        }

        // Main state machine selection
        if(currentState == GS_GAMEPLAY)
        {
          spr = 0;

          // blergle glergle
          //banked_call(TWISTED_BANK, update_twisted);
          //banked_call(TWISTED_BANK, draw_twisted);

          if(queueTele != 255)
          {
            bank_push(ROOM_LOGIC_BANK);
            x2 = x+2;
            y2 = y+3;
            x = queueTele;
            tele_to_room();
            bank_pop();
            queueTele = 255;
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
          bank_push(ACTOR_LOGIC_BANK);
          update_kris();
          update_followers();
          for(i = 0; i < spawnedRafts; i++)
          {
            update_raft(&raftList[i]);
          }
          bank_pop();

          // SOUND TEST
          if(pad_trig & PAD_SELECT)
          {

              bank_push(MUSIC_BANK);
              famistudio_music_stop();
              famistudio_music_play(soundTestNum);
              bank_pop();
              soundTestNum++;
              if(soundTestNum > 9)
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
          bank_push(MONSTER_PROJECTILES_BANK);
          for(i2 = 0; i2 < spawnedMonsters; i2++)
          {
            stripefc = framecount+i2; // use "striped" framecount for monsters, so they don't always update all on the same frame.
            update_monster(&monsterList[i2]);
          }
          for(i = 0; i < spawnedProjectiles; i++)
          {
            update_projectile(&projList[i]);
          }
          bank_pop();

          // Update buttons
          update_buttons();

          // Draw characters
          bank_push(KRIS_ANIMS_BANK);
          draw_kris();
          draw_followers();
          bank_pop();

          bank_push(ACTOR_LOGIC_BANK);
          for(i = 0; i < spawnedRafts; i++)
          {
            draw_raft(&raftList[i]);
          }
          bank_pop();

          // Draw items
          for(i = 0; i < spawnedItems; i++)
          {
              draw_item(&itemList[i]);
          }

          // Draw monsters & projectiles
          bank_push(MONSTER_PROJECTILES_BANK);
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
        else if(currentState == GS_SCREENTRANS)
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
                    lock_room_doors();
                    reset_follow_pos();
                    banked_call(ROOM_LOGIC_BANK, on_enter_special_room);
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
                    lock_room_doors();
                    reset_follow_pos();
                    banked_call(ROOM_LOGIC_BANK, on_enter_special_room);
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
                    lock_room_doors();
                    reset_follow_pos();
                    banked_call(ROOM_LOGIC_BANK, on_enter_special_room);
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
                    lock_room_doors();
                    reset_follow_pos();
                    banked_call(ROOM_LOGIC_BANK, on_enter_special_room);
                }
            }
            // Render kris
            bank_push(KRIS_ANIMS_BANK);
            draw_kris();
            bank_pop();
        }
        else if(currentState == GS_SCREENTRANS_TELE)
        {
          // Teleporting to room
          kris.xpos = x << 4;
          kris.ypos = y << 4;

          // Black out screen in a spiral pattern
          x = 0;
          y = 0;
          x2 = 11;
          y2 = 7;
          oam_clear();
          while(x <= x2 && y <= y2)
          {
              // Spiral running right
              for(i = x; i <= x2; i++)
              {
                  bank_push(ROOM_LOGIC_BANK);
                  draw_black_tile_in_room(i, y);
                  bank_pop();
                  ppu_wait_frame();
              }
              y++;

              // Spiral running down
              for(i = y; i <= y2; i++)
              {
                  bank_push(ROOM_LOGIC_BANK);
                  draw_black_tile_in_room(x2, i);
                  bank_pop();
                  ppu_wait_frame();
              }
              x2--;

              // Spiral running left
              for(i = x2; i >= x && i != 255; i--)
              {
                  bank_push(ROOM_LOGIC_BANK);
                  draw_black_tile_in_room(i, y2);
                  bank_pop();
                  ppu_wait_frame();
              }
              y2--;

              // Spiral running up
              for(i = y2; i >= y; i--)
              {
                  bank_push(ROOM_LOGIC_BANK);
                  draw_black_tile_in_room(x, i);
                  bank_pop();
                  ppu_wait_frame();
              }
              x++;

          }

          set_vram_update(NULL);
          pal_col(0, 0x0F); // Black BG
          ppu_wait_nmi(); // wait till end of frame

          // Turn off PPU
          ppu_off();

          // Switch to ice palette when teleporting into the NorthernLight rooms.
          if(currentEnvironment == E_ISLAND)
          {
            if(currentRoom == 29 || currentRoom == 28)
            {
              envPalettes[E_ISLAND] = paletteIslandIce;
              pal_bg(envPalettes[E_ISLAND]);
              pal_col(0, 0x0F);
            }
            else if(prevRoom == 29 || prevRoom == 28)
            {
              envPalettes[E_ISLAND] = paletteIsland;
              pal_bg(envPalettes[E_ISLAND]);
              pal_col(0, 0x0F);
            }
          }

          // Load room
          bank_push(ROOM_LOGIC_BANK);
          load_room();
          bank_pop();

          ppu_on_all();

          pal_col(0, envPalettes[currentEnvironment][0]);
          currentState = GS_GAMEPLAY;
          reset_follow_pos();

          banked_call(ROOM_LOGIC_BANK, on_enter_special_room);

        }
        else if(currentState == GS_DEATH)
        {
            // in death state, wait for a couple of seconds, then switch Kris' sprite from being their body to being just the heart, and turn the background to
            // "darker than dark"
            if(framecount == 120 && kris.animframe == 0)
            {
              framecount = 0;
              oam_clear();
              kris.animframe = 1;
              pal_col(0, 0x0D); // "darker than dark"
            }
            else if(framecount == 120 && kris.animframe == 1)
            {
              // then go back to the last "checkpoint" (based on environment and flags)
              // load environment and load room
              bank_push(ROOM_LOGIC_BANK);
              reload_area();
              bank_pop();
              currentState = GS_GAMEPLAY;
              reset_follow_pos();
            }

            bank_push(KRIS_ANIMS_BANK);
            draw_kris();
            bank_pop();

        }

        // Text routines
        bank_push(UI_BANK);
        update_text();
        bank_pop();

        // Don't run until a frame has run.
        ppu_wait_nmi();
    }
}

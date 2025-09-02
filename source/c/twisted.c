
#include "twisted.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"
#include "utils.h"
#include "ui.h"
#include "roomstuff.h"
#include "palettes.h"
#include "actors.h"

CODE_BANK(TWISTED_BANK);

// Sprites!
const unsigned char twistedEyeCenter[]={
  0, 0, 0x69, 1,
  128
};
const unsigned char twistedEyeDown[]={
  0, 0, 0x6D, 1,
  128
};
const unsigned char twistedEyeRight[]={
  0, 0, 0x6C, 1,
  128
};
const unsigned char twistedEyeUp[]={
  0, 0, 0x6A, 1,
  128
};
const unsigned char twistedEyeLeft[]={
  0, 0, 0x6B, 1,
  128
};

const unsigned char twistedEyeAngryL[]={
  0, 0, 0x7C, 1 | OAM_FLIP_H,
  128
};
const unsigned char twistedEyeAngryR[]={
  0, 0, 0x7C, 1,
  128
};

const unsigned char twistedEyeShootArrowL[]={
  0, 0, 0x1D, 1 | OAM_FLIP_H,
  128
};
const unsigned char twistedEyeShootArrowR[]={
  0, 0, 0x1D, 1,
  128
};

const unsigned char twistedMouthShootArrow[]={
  0, 0, 0x4E, 1,
  128
};

const unsigned char twistedEyeShootPelletL[]={
  0, 0, 0x44, 1 | OAM_FLIP_H,
  128
};
const unsigned char twistedEyeShootPelletR[]={
  0, 0, 0x44, 1,
  128
};

const unsigned char twistedMouthShootPellet[]={
  0, 0, 0x31, 1,
  128
};

const unsigned char twistedEyeHurt[]={
  0, 0, 0x7B, 1,
  128
};

const unsigned char twistedEyeHurtF[]={
  0, 0, 0x7B, 3,
  128
};

const unsigned char twistedMouthClosedL[]={
 0, 0, 0x7A, 1,
 128
};

const unsigned char twistedMouthOpenL[]={
 0, 0, 0x79, 1,
 128
};

const unsigned char twistedMouthOpenR[]={
 0, 0, 0x79, 1 | OAM_FLIP_H,
 128
};

// big mode sprites
const unsigned char twistedBigModeEye[]={
  0, 0, 0x89, 1,
  8, 0, 0x89, 1 | OAM_FLIP_H,
  0, 8, 0x89, 1 | OAM_FLIP_V,
  8, 8, 0x89, 1 | OAM_FLIP_H | OAM_FLIP_V,
  128
};

const unsigned char twistedBigModeEyeHurt[]={
  0, 0, 0x99, 1,
  8, 0, 0x99, 1 | OAM_FLIP_H,
  0, 8, 0x99, 1 | OAM_FLIP_V,
  8, 8, 0x99, 1 | OAM_FLIP_H | OAM_FLIP_V,
  128
};

const unsigned char twistedBigModeMouth0[]=
{
  0, 0, 0x8A, 1,
  6, 0, 0x8A, 1 | OAM_FLIP_H,
  0, 8, 0x9A, 1,
  6, 8, 0x8B, 1 | OAM_FLIP_H,
  128
};
const unsigned char twistedBigModeMouth1[]=
{
  0, 0, 0x8A, 1,
  6, 0, 0x8A, 1 | OAM_FLIP_H,
  0, 7, 0x8B, 1,
  7, 8, 0x8B, 1 | OAM_FLIP_H,
  128
};
const unsigned char twistedBigModeMouth2[]=
{
  0, 0, 0x8A, 1,
  7, 0, 0x8A, 1 | OAM_FLIP_H,
  0, 8, 0x8B, 1,
  6, 7, 0x9A, 1 | OAM_FLIP_H,
  128
};

const unsigned char* const bigMouthSprites[]={
  twistedBigModeMouth0,
  twistedBigModeMouth1,
  twistedBigModeMouth2
};


#define PINGPONG_LEN 16
const int linearPingPongOffset[]={
  -4, -3, -2, -1, 0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3
};
#define SMOOTH_PINGPONG_LEN 24
const int smoothPingPongOffset[]={
  -4, -4, -4, -3, -3, -2, -1, 0, 1, 2, 3, 3, 4, 4, 4, 3, 3, 2, 1, 0, -1, -2, -3, -3,
};

void init_twisted()
{
  if(twisted.init == 0) twisted.init = 1;
  twisted.state = TA_IDLE;
  twisted.emot = TE_NEUTRAL;
  twisted.xpos = 120 << FP;
  twisted.ypos = 0 << FP;
  twisted.mouth.xoffset = 0;
  twisted.mouth.yoffset = 8;
  twisted.leftEye.xoffset = -10;
  twisted.leftEye.yoffset = -5;
  twisted.rightEye.xoffset = 10;
  twisted.rightEye.yoffset = -8;
  if(!narrative_flag_get(NARFLAG_FOUGHT_TWISTED_ONCE))
  {
    theatricActive = 1;
    theatricIndex = TH_TWISTED_INTRO;
  }
  else
  {
    twisted.ypos = 120 << FP;
    start_dialog(twisted_refight_dialogs, 1);
  }
}

void twisted_theatrics()
{
  if(theatricIndex == TH_TWISTED_INTRO)
  {
    // Move in!
    twisted.ypos += fastlerp(twisted.ypos>>FP, 120, 8);

    // Once moved in, we can switch to TA_IDLE
    if(FP_WHOLE(twisted.ypos) == 120)
    {
      twisted.state = TA_IDLE;
      // Kick off initial text theatric.
      start_dialog(twisted_intro_dialogs, 8);
    }
  }
  else if(theatricIndex == TH_TEXT_GENERIC)
  {
    if(currentDialogPtr == twisted_intro_dialogs)
    {
      // When twisted is chatting during dialogs, move him up and to the right a little before his last line.
      if(theatricStage >= 6)
      {
        twisted.xpos += fastlerp(twisted.xpos>>FP, 196, 32);
        twisted.ypos += fastlerp(twisted.ypos>>FP, 96, 32);
      }
    }
    // phase2 dialog emotion changes & scene setting
    else if(currentDialogPtr == twisted_phase2_dialogs)
    {
      if(theatricStage >= 0 && theatricStage <= 2)
      {
        twisted.xpos += fastlerp(twisted.xpos>>FP, 120, 64);
        twisted.ypos += fastlerp(twisted.ypos>>FP, 64, 64);
      }
      else if(theatricStage == 3)
      {
        twisted.emot = TE_SAD;
      }
      else if(theatricStage == 8)
      {
        twisted.emot = TE_ANGRY;
      }
    }
  }
}

void update_twisted()
{
  if(currentRoom < 3) return;
  if(twisted.invuln)
  {
    twisted.invuln--;
  }

  // Idling/Talking
  if(twisted.state == TA_IDLE)
  {
    if(textQueued == 0 && twisted.emot != TE_HURT)
    {
      twisted.mouthAnimFrame = 0;
      // If twisted just finished talking...
      if(theatricActive == 0 && theatricPrev == TH_TEXT_GENERIC)
      {
        if(currentDialogPtr == twisted_intro_dialogs || (currentDialogPtr == twisted_refight_dialogs && twisted.init == 1))
        {
          // Tele player to room and begin fight Phase 1.
          currentDialogPtr = NULL;
          twisted.fightStage = 0;
          narrative_flag_set(NARFLAG_FOUGHT_TWISTED_ONCE);
          currentRoom = 4;
          kris.xpos = (5+2)<<4;
          kris.ypos = (6+3)<<4;
          music_play(MUSIC_BURNING_EYES);
          ppu_off();
          banked_call(ROOM_LOGIC_BANK, load_room);
          ppu_on_all();
          twisted.stateTimer = rand8();
          return;
        }
        else if(currentDialogPtr == twisted_phase2_dialogs || (currentDialogPtr == twisted_refight_dialogs && twisted.init == 2))
        {
         // begin phase 2!
         twisted.fightStage = 0;
         currentDialogPtr = NULL;
         music_play(MUSIC_TWISTED);
         currentRoom = 5;
         kris.xpos = (5+2)<<4;
         kris.ypos = (4+3)<<4;
         ppu_off();
         banked_call(ROOM_LOGIC_BANK, load_room);
         ppu_on_all();
         twisted.init = 2;
         twisted.stateTimer = 30;
        }
      }
    }
    else
    {
      // Twisted's mouth anim updates when text is happening.
      if(framecount % 6 == 0)
      {
        twisted.mouthAnimFrame++;
      }
    }

    if(twisted.emot == TE_HURT) // when hurt in idle state...
    {
      if(framecount % 4 == 0)
      {
        twisted.mouthAnimFrame++;
        twisted.floatFrame++; // speed up float frames when hurt
      }
      twisted.stateTimer--;
      if(twisted.stateTimer == 0)
      {
        twisted.emot = (twisted.init == 2 ? TE_ANGRY : TE_NEUTRAL); // be angry when in phase 2
        twisted.stateTimer = 5; // act quick after harm
        twisted.invuln = 60; // no hitloops thx <3
        if(twisted.init == 2) // in 2nd phase, spawn candy after hurt
        {
          x2 = twisted.xpos>>FP;
          y2 = twisted.ypos>>FP;
          spawn_candy(x2, y2);
        }
      }
    }
    else
    {
      if(twisted.init == 1 && theatricActive == 0)
      {
          // check if this fight phase is over
          if(twisted.fightStage == 12) // 12 hits seems to feel pretty good at this stage.
          {
              // start the 2nd phase transition dialog
              music_stop();
              twisted.emot = TE_ANGRY;
              start_dialog(twisted_phase2_dialogs, 17);
              twisted.init = 2;
          }

          // phase 1 fight? pick a direction to move and use TA_MOVE_TO_POINT_WORLD at random intervals.
          twisted.stateTimer--;
          if(twisted.stateTimer == 0)
          {
            twisted.state = TA_MOVE_TO_POINT_WORLD;
            if(FP_WHOLE(twisted.ypos) > 64 && FP_WHOLE(twisted.xpos) == 196) // move up
            {
               twisted.stateDataX = 196;
               twisted.stateDataY = 64;
            }
            else if(FP_WHOLE(twisted.xpos) > 52 && FP_WHOLE(twisted.ypos) == 64) // move left
            {
               twisted.stateDataX = 52;
               twisted.stateDataY = 64;
            }
            else if(FP_WHOLE(twisted.ypos) < 144 && FP_WHOLE(twisted.xpos) == 52) // move down
            {
              twisted.stateDataX = 52;
              twisted.stateDataY = 144;
            }
            else if(FP_WHOLE(twisted.xpos) < 196 && FP_WHOLE(twisted.ypos) == 144) // move right
            {
              twisted.stateDataX = 196;
              twisted.stateDataY = 144;
            }
          }
      }
      else if(twisted.init == 2 && theatricActive == 0)
      {
        // phase 2 fight is different:
        // 1. move out of screen bounds and shoot arrows/pellets while flying along the chosen side
        // 2. occasionally swap screen sides, charging at player. dmg them if contact is made during this movement, but this is when twisted is vulnerable to attacks which returns them to idle state
        // 3. repeat 1-2, but every 4 hits, switch into "eat exp bar" phase/theatric.
        // 4. switch arenas to respawn spikes and move player after exp bar change
        // 5. when exp bar hits lv2, do the 3rd palette swap
        // 6. when exp bar is at lv1, do the lv0 scene, where final arena is chosen and twisted moves behind wall of spikes for the final confrontation.

        twisted.stateTimer--;
        if(twisted.stateTimer == 0)
        {
          // handle special states
          if(twisted.fightStage == 4 || twisted.fightStage == 9 || twisted.fightStage == 14 || twisted.fightStage == 23) // 4 hits made! (with 1 extra inbetween... we don't want to retrigger eat after all. and final stage is longer)
          {
            // begin eat exp bar & queue the text too
            twisted.state = TA_EAT_EXP;
            twisted.stateTimer = 45;
            playerLevel--;
            playerExp = 24; // start with bar full so that twisted can munch

            twisted.stateDataX = 0; // munch substate starts with flying up to the exp bar
            queue_text(rand8() < 127 ? twisted_eat_0 : twisted_eat_1,1); // queue random dialog for munching
          }
          else
          {
            // re-use lookdir to randomly pick which side of the screen to leave towards
            twisted.lookDir = rand8();
            twisted.lookDir = twisted.lookDir >> 6;
            twisted.state = TA_LEAVE_SCREEN;
          }

        }
      }
    }
  }
  else if(twisted.state == TA_MOVE_TO_POINT_WORLD)
  {
      int dx = 0;
      int dy = 0;

      dx = ((int)(twisted.stateDataX));
      dx -= ((int)(twisted.xpos>>FP));
      dy = ((int)(twisted.stateDataY));
      dy -= ((int)(twisted.ypos>>FP));

      if(dy > 0)
      {
          twisted.lookDir = 0;
          twisted.ypos += 1<<FP;
      }
      else if(dx > 0)
      {
          twisted.lookDir = 1;
          twisted.xpos += 1<<FP;
      }
      else if(dy < 0)
      {
          twisted.lookDir = 2;
          twisted.ypos -= 1<<FP;
      }
      else if(dx < 0)
      {
          twisted.lookDir = 3;
          twisted.xpos -= 1<<FP;
      }
      else if(dx == 0 && dy == 0) // arrived. shoot at player
      {
         twisted.state = TA_WINDUP;
         twisted.stateTimer = 30;
      }
  }
  else if(twisted.state == TA_WINDUP)
  {
     twisted.stateTimer--;
     if(twisted.stateTimer == 0)
     {
         // randomly shoot an arrow or a pellet
         if(rand8() < 127 && twisted.init != 2)
         {
           twisted.state = TA_SHOOT_ARROW;
           twisted.stateTimer = 45;
         }
         else
         {
           twisted.state = TA_SHOOT_PELLET;
           twisted.stateTimer = 45;
         }
     }
  }
  else if(twisted.state == TA_SHOOT_ARROW)
  {
    if(twisted.stateTimer == 45)
    {
      // shoot at player
      twisted_shoot_arrow();
    }
    twisted.stateTimer--;
    if(twisted.stateTimer == 0)
    {
      twisted.state = twisted.init == 2 ? TA_MOVE_TO_POINT_SCREEN : TA_IDLE;
      twisted.stateTimer = rand8()>>2;
    }
  }
  else if(twisted.state == TA_SHOOT_PELLET)
  {
    if(twisted.stateTimer == 45)
    {
      twisted_shoot_pellet();
    }
    twisted.stateTimer--;
    if(twisted.stateTimer == 0)
    {
      twisted.state = twisted.init == 2 ? TA_MOVE_TO_POINT_SCREEN : TA_IDLE;
      twisted.stateTimer = rand8()>>2;
    }
  }
  else if(twisted.state == TA_LEAVE_SCREEN) // re-use movement for charge state
  {
    // using lookDir, fly offscreen
    if(twisted.lookDir == 0)
    {
        twisted.ypos += fastlerp(twisted.ypos>>FP, 200, 32);
        if(twisted.ypos>>FP == 200)
        {
          // offscreen, so now switch to TA_MOVE_TO_POINT_SCREEN to slide along that axis.
          twisted.state = TA_MOVE_TO_POINT_SCREEN;
          twisted.lookDir = rand8()<127 ? 1 : 3; // randomly pick whether to slide left or right.
          twisted.stateTimer = rand8()>>1;
        }
    }
    else if(twisted.lookDir == 1)
    {
        twisted.xpos += fastlerp(twisted.xpos>>FP, 232, 32);
        if(twisted.xpos>>FP == 232)
        {
          // offscreen, so now switch to TA_MOVE_TO_POINT_SCREEN to slide along that axis.
          twisted.state = TA_MOVE_TO_POINT_SCREEN;
          twisted.lookDir = rand8()<127 ? 0 : 2; // randomly pick whether to slide up or down.
          twisted.stateTimer = rand8()>>1;
        }
    }
    else if(twisted.lookDir == 2)
    {
        twisted.ypos += fastlerp(twisted.ypos>>FP, 32, 32);
        if(twisted.ypos>>FP == 32)
        {
          // offscreen, so now switch to TA_MOVE_TO_POINT_SCREEN to slide along that axis.
          twisted.state = TA_MOVE_TO_POINT_SCREEN;
          twisted.lookDir = rand8()<127 ? 1 : 3; // randomly pick whether to slide left or right.
          twisted.stateTimer = rand8()>>1;
        }
    }
    else if(twisted.lookDir == 3)
    {
        twisted.xpos += fastlerp(twisted.xpos>>FP, 24, 32);
        if(twisted.xpos>>FP == 24)
        {
          // offscreen, so now switch to TA_MOVE_TO_POINT_SCREEN to slide along that axis.
          twisted.state = TA_MOVE_TO_POINT_SCREEN;
          twisted.lookDir = rand8()<127 ? 0 : 2; // randomly pick whether to slide up or down.
          twisted.stateTimer = rand8()>>1;
        }
    }
  }
  else if(twisted.state == TA_CHARGE_ACROSS_SCREEN)
  {
      if(twisted.stateTimer > 0)
      {
        twisted.stateTimer--;
        if(twisted.stateTimer == 0)
        {
          sfx_play(SFX_SPEAR, FAMISTUDIO_SFX_CH2);
        }
        twisted.floatFrame++;
        if(playerLevel == 1) // in final arena, twisted can go BIG MODE
        {
            twisted.xpos += fastlerp(twisted.xpos>>FP, kris.xpos, 64);
            twisted.ypos += fastlerp(twisted.ypos>>FP, 2, 64); // twisted goes to hide to transform
                                                               //
            if(twisted.ypos>>FP <= 4)
            {
                // go bigmode
                twisted.emot = TE_BIG;
                twisted.lookDir = 0;
            }
        }
      }
      else
      {
        // while charging, check for collisions with player
        x2 = twisted.xpos>>FP;
        y2 = twisted.ypos>>FP;
        if(point_in_rect(kris.xpos+7, kris.ypos+7, x2-4, y2, x2+12, y2+16))
        {
          banked_call(ACTOR_LOGIC_BANK, get_hurt);
        }

        if(twisted.lookDir == 0)
        {
          twisted.ypos += (3 << FP);
          if((twisted.ypos>>FP) > 198)
          {
            twisted.state = TA_MOVE_TO_POINT_SCREEN;
            twisted.lookDir = rand8()<127 ? 1 : 3; // randomly pick whether to slide left or right
            twisted.stateTimer = rand8()>>1;
            twisted.emot = TE_ANGRY;
          }
        }
        else if(twisted.lookDir == 1)
        {
          twisted.xpos += (3 << FP);
          if((twisted.xpos>>FP) > 230)
          {
            twisted.state = TA_MOVE_TO_POINT_SCREEN;
            twisted.lookDir = rand8()<127 ? 0 : 2;
            twisted.stateTimer = rand8()>>1;
            twisted.emot = TE_ANGRY;
          }
        }
        else if(twisted.lookDir == 2)
        {
          twisted.ypos -= (3 << FP);
          if((twisted.ypos>>FP) < 24)
          {
            twisted.state = TA_MOVE_TO_POINT_SCREEN;
            twisted.lookDir = rand8()<127 ? 1 : 3; // randomly pick whether to slide left or right
            twisted.stateTimer = rand8()>>1;
            twisted.emot = TE_ANGRY;
          }
        }
        else if(twisted.lookDir == 3)
        {
          twisted.xpos -= (3 << FP);
          if((twisted.xpos>>FP) < 34)
          {
            twisted.state = TA_MOVE_TO_POINT_SCREEN;
            twisted.lookDir = rand8()<127 ? 0 : 2;
            twisted.stateTimer = rand8()>>1;
            twisted.emot = TE_ANGRY;
          }
        }
      }
  }
  else if(twisted.state == TA_MOVE_TO_POINT_SCREEN)
  {
    // twisted slides along an axis until they hit that axis' edge, where they turn around
    if(twisted.lookDir == 0)
    {
      twisted.ypos += (2 << FP);
      if((twisted.ypos>>FP) > 198)
      {
        twisted.lookDir = 2;
        twisted.stateDataX = 0;
      }
    }
    else if(twisted.lookDir == 2)
    {
      twisted.ypos -= (2 << FP);
      if((twisted.ypos>>FP) < 34)
      {
        twisted.lookDir = 0;
        twisted.stateDataX = 0;
      }
    }
    else if(twisted.lookDir == 1)
    {
      twisted.xpos += (2 << FP);
      if((twisted.xpos>>FP) > 230)
      {
        twisted.lookDir = 3;
        twisted.stateDataX = 0;
      }
    }
    else if(twisted.lookDir == 3)
    {
      twisted.xpos -= (2 << FP);
      if((twisted.xpos>>FP) < 24)
      {
        twisted.lookDir = 1;
        twisted.stateDataX = 0;
      }
    }
    // when twisted's state timer runs dry, they shoot at the player.
    // and the timer is reset after they come out of that respective state.
    twisted.stateTimer--;
    if(twisted.stateTimer == 0)
    {
        if(rand8() > 85)
        {
          twisted.state = TA_WINDUP;
          twisted.stateTimer = 45;
        }
        else // 33% chance not to shoot and instead reset timer
        {
          twisted.stateTimer = rand8()>>1;
        }
    }
    else     // if they aren't shooting, then with a high probability (~80%) they will charge the player when their x/y pos is in range
    {
      if(twisted.stateDataX == 0)
      {
        if(twisted.lookDir == 0 || twisted.lookDir == 2)
        {
            // check ypos
            int ydiff = (twisted.ypos>>FP);
            ydiff -= (kris.ypos);
            if(abs(ydiff) < 16)
            {
              if(rand8() < 204)
              {
                twisted.state = TA_CHARGE_ACROSS_SCREEN;
                twisted.lookDir = (twisted.xpos>>FP) < 100 ? 1 : 3; // pick right-left charge dir
                twisted.stateTimer = 45;
              }
              else
              {
                twisted.stateDataX = 1; // don't dive until turned around at the screen edge
              }
            }
        }
        else
        {
            // check xpos
            int xdiff = (twisted.xpos>>FP);
            xdiff -= (kris.xpos-4);
            if(abs(xdiff) < 16)
            {
              if(rand8() < 204)
              {
                twisted.state = TA_CHARGE_ACROSS_SCREEN;
                twisted.lookDir = (twisted.ypos>>FP) < 100 ? 0 : 2; // pick down-up charge dir
                twisted.stateTimer = 45;
              }
              else
              {
                twisted.stateDataX = 1;
              }
            }
        }
      }
    }
  }
  else if(twisted.state == TA_EAT_EXP)
  {
    twisted.emot = TE_NEUTRAL;
    if(twisted.stateDataX == 0) // flying up to eat the exp
    {
      twisted.xpos += fastlerp(twisted.xpos>>FP, 196, 16);
      twisted.ypos += fastlerp(twisted.ypos>>FP, 32, 16);
      if(twisted.xpos>>FP == 196 && twisted.ypos>>FP == 32)
      {
        // flew up, so now start chomping left.
        twisted.stateDataX = 1;
        twisted.stateDataY = 196 - 8;
        twisted.stateTimer = 45;
      }
    }
    else if(twisted.stateDataX == 1)
    {
      if(twisted.stateTimer > 0)
      {
        twisted.stateTimer--;
        twisted.mouthAnimFrame = 0;
      }
      if(twisted.stateTimer == 0)
      {
        if(playerExp == 0)
        {
          // munching is done, we can leave this state now!
          twisted.emot = TE_ANGRY;
          twisted.state = TA_IDLE;
          twisted.fightStage++;
          twisted.stateTimer = 45;
          clear_text();

          // depending on playerlevel, switch arena
          if(playerLevel == 2)
          {
              currentRoom = 7;
              kris.xpos = (5+2)<<4;
              kris.ypos = (6+3)<<4;
              // set palette line
              pal_col(4, envPalettes[currentEnvironment][8]);
              pal_col(5, envPalettes[currentEnvironment][9]);
              pal_col(6, envPalettes[currentEnvironment][10]);
              pal_col(7, envPalettes[currentEnvironment][11]);
              ppu_off();
              banked_call(ROOM_LOGIC_BANK, load_room);
              ppu_on_all();
          }
          else if(playerLevel == 1)
          {
              currentRoom = 6;
              kris.xpos = (5+2)<<4;
              kris.ypos = (4+3)<<4;
              ppu_off();
              banked_call(ROOM_LOGIC_BANK, load_room);
              ppu_on_all();
          }
          else if(playerLevel == 0) // load final room and begin final confrontation!
          {
              music_stop();
              currentRoom = 9; // final arena
              kris.xpos = (2+2)<<4;
              kris.ypos = (3+3)<<4;
              ppu_off();
              banked_call(ROOM_LOGIC_BANK, load_room);
              ppu_on_all();
              twisted.state = TA_FINAL;
          }
        }
        else
        {
          // move to next chunk, update hud
          twisted.mouthAnimFrame = 1;
          twisted.xpos += fastlerp(twisted.xpos>>FP, twisted.stateDataY, 64);
          if(twisted.xpos>>FP == twisted.stateDataY)
          {
            playerExp -= 4;
            hudDirty = 1;
            sfx_play(SFX_DAMAGE, FAMISTUDIO_SFX_CH1);
            twisted.stateTimer = 45;
            twisted.stateDataY -= 8;
          }
        }
      }
    }
  }
  else if(twisted.state == TA_FINAL)
  {
    // Final Phase:
    // first, move to middle-right and start gloating,
    // and start dialogue.
    twisted.emot = TE_GLOAT;

    // then, after that dialogue concludes, wait for the player's level to go back up
    if(playerLevel > 0 && floorpbimpbomp)
    {
        // do the wait! wait! dialogue
        twisted.emot = TE_TERROR;

    }

    // when final hit happens
    if(bompbimpdonk)
    {
       // force palette to white/grey/black, shake horizontally, stop orbiting briefly
       twisted.emot = TE_SHOCK;

       // during dialogue, start corrupting random tiles on the Bg by building random vram adjustments.
       // allow them to overwrite eachother
       // increase the intensity as the dialogue scene continues
       // finally, begin replacing colours with 0x0D (darker than dark) one by one, starting from the end of the palettes, alternating between sprite and background
       // until kris palette is the only thing left. then make it go monochrome, then fade down, then gone.
    }

  }


  if(framecount % 5 == 0)
    twisted.floatFrame++;

  // Twisted floaty movement on components. Does this in most states.
  if(twisted.floatFrame == 48)
    twisted.floatFrame = 0;
  twisted.rightEye.yoffset = -8 + linearPingPongOffset[(twisted.floatFrame + 3) % PINGPONG_LEN];
  twisted.leftEye.yoffset = -5 + ((linearPingPongOffset[(twisted.floatFrame + 12) % PINGPONG_LEN]) >> 1);

  twisted.mouth.xoffset = smoothPingPongOffset[(twisted.floatFrame)%SMOOTH_PINGPONG_LEN];
  twisted.mouth.yoffset = 8 + smoothPingPongOffset[((twisted.floatFrame) + 6)%SMOOTH_PINGPONG_LEN];

}

void draw_twisted()
{
  // Depending on emotional & action state, draw eyes and mouth at their locations.
  const unsigned char* eyeSpr = twistedEyeCenter;
  const unsigned char* mouthSpr = twistedMouthClosedL;

  if(currentRoom < 3) return;
  // Invuln flicker
  if(twisted.invuln && (framecount % 4 == 0))
  {
    oam_dirty = 1;
    return;
  }

  // Attack states
  if(twisted.state == TA_WINDUP)
  {
     // scramble the sprites
     if(framecount % 4 == 0)
     {
      scramble_spr_0[2] = rand8();
      scramble_spr_1[2] = rand8();
     }
     if(framecount % 7 == 0)
     {
      scramble_spr_0[3] = rand8();
      scramble_spr_1[3] = rand8();
     }

     // Eye 1
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, scramble_spr_0);
     // Eye 2
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, scramble_spr_0);
     // Mouth
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, scramble_spr_1);

     return;
  }
  else if(twisted.state == TA_SHOOT_ARROW)
  {
     // Eye 1
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twistedEyeShootArrowR);
     // Eye 2
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twistedEyeShootArrowL);
     // Mouth
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twistedMouthShootArrow);

    return;
  }
  else if(twisted.state == TA_SHOOT_PELLET)
  {
     // Eye 1
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twistedEyeShootPelletR);
     // Eye 2
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twistedEyeShootPelletL);
     // Mouth
     spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twistedMouthShootPellet);

     return;
  }

  if(twisted.state == TA_MOVE_TO_POINT_WORLD)
  {
    if(twisted.lookDir == 0) eyeSpr = twistedEyeDown;
    else if(twisted.lookDir == 1) eyeSpr = twistedEyeRight;
    else if(twisted.lookDir == 2) eyeSpr = twistedEyeUp;
    else if(twisted.lookDir == 3) eyeSpr = twistedEyeLeft;
  }

  if(twisted.emot == TE_NEUTRAL)
  {
      // Eye 1
      spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, eyeSpr);
      // Eye 2
      spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, eyeSpr);
      // Mouth
      spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedMouthClosedL : twistedMouthOpenL);
  }
  else if(twisted.emot == TE_HURT) // hurt: flash between hurt eye frames, wiggle faster, open mouth
  {
    // big mode hack lol
    if(playerLevel == 1)
    {
      goto twbigmode; // i will defend goto, my beloved, with my dying breath
    }
     // Eye 1
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedEyeHurt : twistedEyeHurtF);
    // Eye 2
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twisted.mouthAnimFrame%2 == 1 ? twistedEyeHurt : twistedEyeHurtF);
    // Mouth
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedMouthOpenL : twistedMouthOpenR);
  }
  else if(twisted.emot == TE_ANGRY)
  {
    // Eye 1
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twistedEyeAngryL);
    // Eye 2
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twistedEyeAngryR);
    // Mouth
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedMouthOpenL : twistedMouthOpenR);
  }
  else if(twisted.emot == TE_SAD)
  {
       // Eye 1
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twistedEyeAngryR);
    // Eye 2
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twistedEyeAngryL);
    // Mouth
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedMouthOpenL : twistedMouthClosedL);
  }
  else if(twisted.emot == TE_BIG)
  {
    twbigmode:
    // Eye 1
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset-4, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twisted.emot == TE_HURT ? twistedBigModeEyeHurt : twistedBigModeEye);
    // Eye 2
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset+4, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twisted.emot == TE_HURT ? twistedBigModeEyeHurt : twistedBigModeEye);
    // Mouth
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset+4, spr, bigMouthSprites[twisted.mouthAnimFrame%3]);
  }
}

CODE_BANK_POP();

void twisted_shoot_arrow()
{
  int dx;
  int dy;
  dx = (int)kris.xpos;
  dx -= FP_WHOLE(twisted.xpos);
  dy = (int)kris.ypos;
  dy -= FP_WHOLE(twisted.ypos);

  // Select x or y major
  if(abs(dx) > abs(dy))
  {
      dx = sign(dx);
      dy = 0;
  }
  else
  {
      dy = sign(dy);
      dx = 0;
  }

  sfx_play(SFX_SPEAR, FAMISTUDIO_SFX_CH2);
  bank_push(MONSTER_PROJECTILES_BANK);
  spawn_projectile(FP_WHOLE(twisted.xpos)+twisted.mouth.xoffset, FP_WHOLE(twisted.ypos)+twisted.mouth.yoffset, P_ARROW, (dx<<9), (dy<<9));
  bank_pop();
}

void twisted_shoot_pellet()
{
  int player_offsetx = (int)kris.xpos+7;
  int player_offsety = (int)kris.ypos+7;
  player_offsetx -= FP_WHOLE(twisted.xpos)+twisted.mouth.xoffset;
  player_offsety -= FP_WHOLE(twisted.ypos)+twisted.mouth.yoffset;

  // then we "normalize" this without actual division for now, just shift or something.
  while(abs(player_offsetx) > 32 || abs(player_offsety) > 32)
  {
    player_offsetx = player_offsetx >> 1;
    player_offsety = player_offsety >> 1;
  }

  sfx_play(SFX_SPEAR, FAMISTUDIO_SFX_CH2);
  bank_push(MONSTER_PROJECTILES_BANK);
  spawn_projectile(FP_WHOLE(twisted.xpos)+twisted.mouth.xoffset, FP_WHOLE(twisted.ypos)+twisted.mouth.yoffset, P_FRIENDLINESS_PELLET, (player_offsetx<<4), (player_offsety<<4));
  bank_pop();
}

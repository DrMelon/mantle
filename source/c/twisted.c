
#include "twisted.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"
#include "utils.h"
#include "ui.h"
#include "roomstuff.h"

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
  twisted.init = 1;
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
  if(narrative_flag_get(NARFLAG_FOUGHT_TWISTED_ONCE))
  {

  }
  else
  {
    theatricActive = 1;
    theatricIndex = TH_TWISTED_INTRO;
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
  }
}

void update_twisted()
{
  // Idling/Talking
  if(twisted.state == TA_IDLE)
  {
    if(textQueued == 0 && twisted.emot != TE_HURT)
    {
      twisted.mouthAnimFrame = 0;
      // If twisted just finished talking in the intro...
      if(theatricActive == 0 && theatricPrev == TH_TEXT_GENERIC && currentDialogPtr == twisted_intro_dialogs)
      {
         // Tele player to room and begin fight Phase 1.
         currentDialogPtr = NULL;
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
        twisted.emot = TE_NEUTRAL; // be angry when in phase 2
        twisted.stateTimer = 5; // act quick after harm
      }
    }
    else
    {
      if(twisted.init == 1 && theatricActive == 0)
      {
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

      // phase 2 fight is different:
      // 1. move out of screen bounds and shoot arrows
      // 1.5 move back into screen bounds
      // 2. swap screen sides quickly, charging at player with angry eyes. dmg them if contact is made during this movement
      // 3. spawn spikes when none exist anymore (random arena selection)
      // 4. every 3 hits, eat exp bar.
      // (until lv 0 scene, where final arena is chosen and twisted moves behind spikes)
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
         if(rand8() < 127)
         {
           twisted.state = TA_SHOOT_ARROW;
           twisted.stateTimer = 45;
         }
         else
         {
           twisted.state = TA_SHOOT_ARROW;
           twisted.stateTimer = 45;
         }
     }
  }
  else if(twisted.state = TA_SHOOT_ARROW)
  {
    if(twisted.stateTimer == 45)
    {
      // shoot at player
      twisted_shoot_arrow();
    }
    twisted.stateTimer--;
    if(twisted.stateTimer == 0)
    {
      twisted.state = TA_IDLE;
      twisted.stateTimer = rand8()>>2;
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
     // Eye 1
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedEyeHurt : twistedEyeHurtF);
    // Eye 2
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twisted.mouthAnimFrame%2 == 1 ? twistedEyeHurt : twistedEyeHurtF);
    // Mouth
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedMouthOpenL : twistedMouthOpenR);
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

  bank_push(MONSTER_PROJECTILES_BANK);
  spawn_projectile(FP_WHOLE(twisted.xpos)+twisted.mouth.xoffset, FP_WHOLE(twisted.ypos)+twisted.mouth.yoffset, P_ARROW, (dx<<9), (dy<<9));
  bank_pop();
}

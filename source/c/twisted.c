
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
  theatricActive = 1;
  theatricIndex = TH_TWISTED_INTRO;
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
        twisted.emot = TE_NEUTRAL; // or angry in phase 2
        twisted.stateTimer = rand8();
      }
    }
    else
    {
      // phase 1 fight? pick a direction to move and use TA_MOVE_TO_POINT_WORLD at random intervals.
      // upon arriving at a target point, shoot an arrow.
      //
      // phase 2 fight is different:
      // 1. move out of screen bounds and shoot arrows
      // 1.5 move back into screen bounds
      // 2. swap screen sides quickly, charging at player with angry eyes. dmg them if contact is made during this movement
      // 3. spawn spikes when none exist anymore (random arena selection)
      // 4. every 3 hits, eat exp bar.
      // (until lv 0 scene, where final arena is chosen and twisted moves behind spikes)
    }
  }

  if(framecount % 5 == 0)
    twisted.floatFrame++;

  // Twisted floaty movement on components. Does this in most states.
  twisted.rightEye.yoffset = -8 + linearPingPongOffset[(twisted.floatFrame + 3) % PINGPONG_LEN];
  twisted.leftEye.yoffset = -5 + ((linearPingPongOffset[(twisted.floatFrame + 12) % PINGPONG_LEN]) >> 1);

  twisted.mouth.xoffset = smoothPingPongOffset[(twisted.floatFrame)%SMOOTH_PINGPONG_LEN];
  twisted.mouth.yoffset = 8 + smoothPingPongOffset[((twisted.floatFrame) + 6)%SMOOTH_PINGPONG_LEN];

}

void draw_twisted()
{
  // Depending on emotional state, draw eyes and mouth at their locations.

  if(twisted.emot == TE_NEUTRAL)
  {
    // Eye 1
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twistedEyeCenter);
    // Eye 2
    spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twistedEyeCenter);
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

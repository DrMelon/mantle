
#include "twisted.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"
#include "utils.h"

CODE_BANK(TWISTED_BANK);

// Sprites!
const unsigned char twistedEyeCenter[]={
  0, 0, 0x69, 1,
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
  twisted.xpos = 120 << FP;
  twisted.ypos = 120 << FP;
  twisted.mouth.xoffset = 0;
  twisted.mouth.yoffset = 8;
  twisted.leftEye.xoffset = -10;
  twisted.leftEye.yoffset = -5;
  twisted.rightEye.xoffset = 10;
  twisted.rightEye.yoffset = -8;
}

void update_twisted()
{
  if(framecount % 6 == 0)
  {
    twisted.mouthAnimFrame++;
  }

  twisted.rightEye.yoffset = -8 + linearPingPongOffset[(twisted.mouthAnimFrame + 3) % PINGPONG_LEN];
  twisted.leftEye.yoffset = -5 + ((linearPingPongOffset[(twisted.mouthAnimFrame + 12) % PINGPONG_LEN]) >> 1);

  twisted.mouth.xoffset = smoothPingPongOffset[(twisted.mouthAnimFrame)%SMOOTH_PINGPONG_LEN];
  twisted.mouth.yoffset = 8 + smoothPingPongOffset[((twisted.mouthAnimFrame) + 6)%SMOOTH_PINGPONG_LEN];

  twisted.xpos += 64; //1/4 px a frame
}

void draw_twisted()
{
  // Depending on emotional state, draw eyes and mouth at their locations.

  // TODO: emotion

  // Eye 1
  spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.leftEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.leftEye.yoffset, spr, twistedEyeCenter);
  // Eye 2
  spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.rightEye.xoffset, FP_WHOLE(twisted.ypos) + twisted.rightEye.yoffset, spr, twistedEyeCenter);
  // Mouth
  spr = oam_meta_spr(FP_WHOLE(twisted.xpos) + twisted.mouth.xoffset, FP_WHOLE(twisted.ypos) + twisted.mouth.yoffset, spr, twisted.mouthAnimFrame%2 == 0 ? twistedMouthClosedL : twistedMouthOpenL);



}

CODE_BANK_POP();

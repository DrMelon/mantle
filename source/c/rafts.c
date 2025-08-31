
#include "rafts.h"
#include "globals.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "jump_arc.h"
#include "utils.h"

CODE_BANK(ACTOR_LOGIC_BANK);

const unsigned char raftSprite[]={
  0, 0, 0x81, 1,
  8, 0, 0x82, 1,
  0, 8, 0x91, 1,
  8, 8, 0x92, 1,
  128
};

void board_raft(WalkingCharacter* chara, Raft* raft)
{
  int test_tile_x = 0;
  int test_tile_y = 0;
  chara->xpos = ((chara->xpos+7) >> 4) << 4;
  chara->ypos = ((chara->ypos+7) >> 4) << 4;

  raft->assignedchar = chara;
  chara->raft = raft;

  // calculate a jump arc for the player
  if(jumpArcs < MAX_JUMP_ARCS)
  {
    sfx_play(SFX_JUMP, FAMISTUDIO_SFX_CH1);
    chara->substate = S_JUMPING;
    chara->animframe = 0;
    chara->arcid = jumpArcs;

    test_tile_x = raft->xpos >> 4;
    test_tile_y = raft->ypos >> 4;

    // first calculate the jump arc type.
    test_tile_x = test_tile_x - (chara->xpos >> 4) + 4; // get back to 0-7 on each axis
    test_tile_y = test_tile_y - (chara->ypos >> 4) + 4;

    jumpArcList[jumpArcs].jump_arc_type = test_tile_x + (test_tile_y * 8); // 8 possible x-axis values, 8 possible y-axis values.
    jumpArcList[jumpArcs].start_x = chara->xpos;
    jumpArcList[jumpArcs].start_y = chara->ypos;
    jumpArcs++;
  }
}

void update_raft(Raft* raft)
{
  if(raft->assignedchar == NULL) return;

  // If the character is here and in NORMAL state, follow their movements.
  if(raft->assignedchar->substate == S_NORMAL)
  {
    raft->xpos = raft->assignedchar->xpos;
    raft->ypos = raft->assignedchar->ypos;
    raft->currentRoom = currentRoom;
  }
}

void draw_raft(Raft* raft)
{
  unsigned char yoffset = 0;
  if(currentRoom != raft->currentRoom) return; // don't draw unless in same room as raft.
  if(framecount % 32 > 16) yoffset = 1;

  if(raft->assignedchar != NULL && raft->assignedchar->substate == S_NORMAL) yoffset += 4;

  spr = oam_meta_spr(raft->xpos, raft->ypos + yoffset, spr, raftSprite);
}

void leave_raft(WalkingCharacter* chara, Raft* raft, unsigned char tx, unsigned char ty)
{
  int test_tile_x = 0;
  int test_tile_y = 0;
  raft->xpos = ((chara->xpos + 7) >> 4) << 4;
  raft->ypos = ((chara->ypos + 7) >> 4) << 4;

  raft->assignedchar = NULL;
  chara->raft = NULL;
  sfx_play(SFX_JUMP, FAMISTUDIO_SFX_CH1);

  // calculate a jump arc for the player
  if(jumpArcs < MAX_JUMP_ARCS)
  {
    chara->substate = S_JUMPING;
    chara->animframe = 0;
    chara->arcid = jumpArcs;

    test_tile_x = tx;
    test_tile_y = ty;

    // first calculate the jump arc type.
    test_tile_x = test_tile_x - (chara->xpos >> 4) + 4; // get back to 0-7 on each axis
    test_tile_y = test_tile_y - (chara->ypos >> 4) + 4;

    jumpArcList[jumpArcs].jump_arc_type = test_tile_x + (test_tile_y * 8); // 8 possible x-axis values, 8 possible y-axis values.
    jumpArcList[jumpArcs].start_x = chara->xpos;
    jumpArcList[jumpArcs].start_y = chara->ypos;
    jumpArcs++;
  }

}

CODE_BANK_POP();

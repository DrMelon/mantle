
#ifndef __RAFTS_H_
#define __RAFTS_H_

typedef struct Raft Raft;

#include "actors.h"

struct Raft
{
  unsigned char xpos;
  unsigned char ypos;
  WalkingCharacter* assignedchar;
  unsigned char currentRoom;
};

extern void board_raft(WalkingCharacter* chara, Raft* raft);
extern void update_raft(Raft* raft);
extern void draw_raft(Raft* raft);
extern void leave_raft(WalkingCharacter* chara, Raft* raft, unsigned char tx, unsigned char ty);

#endif

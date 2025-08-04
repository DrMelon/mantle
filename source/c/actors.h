#ifndef __ACTORS_H
#define __ACTORS_H

typedef struct WalkingCharacter WalkingCharacter;

#include "rafts.h"

enum Substate
{
  S_NORMAL,
  S_HURT,
  S_WINDUP,
  S_ATTACK,
  S_JUMPING,
  S_FLY,
  S_DIE
};

enum CharacterType // in order of appearance...
{
  CH_KRIS,
  CH_NOELLE,
  CH_SUSIE,
  CH_RALSEI,
};


struct WalkingCharacter
{
  unsigned char xpos;
  unsigned char ypos;
  unsigned char direction; // 0 = down, 1 = right, 2 = up, 3 = left
  unsigned char animframe;
  unsigned char arcid; // for jump arcs getting onto/off rafts
  Raft* raft; // for raft handling
  enum Substate substate;
  enum CharacterType chartype;
};

extern const unsigned char* const * const characterWalkAnims[];
extern const unsigned char* const * const characterStrikeAnims[];
extern const unsigned char* const * const characterHurtAnims[];

extern void switch_to_room();
extern void tele_to_room();

extern void earn_exp();
extern void update_character(WalkingCharacter* chara);
extern void draw_character(WalkingCharacter* chara);
extern void sword_check(WalkingCharacter* chara);
extern void get_hurt(WalkingCharacter* chara);


#endif

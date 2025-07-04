#ifndef __ACTORS_H
#define __ACTORS_H
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

enum CharacterType
{
  CH_KRIS,
  CH_SUSIE,
  CH_RALSEI,
  CH_NOELLE
};


typedef struct
{
  unsigned char xpos;
  unsigned char ypos;
  unsigned char direction; // 0 = down, 1 = right, 2 = up, 3 = left
  unsigned char animframe;
  enum Substate substate;
  enum CharacterType chartype;
} WalkingCharacter;


const unsigned char* const * const characterWalkAnims[];
const unsigned char* const * const characterStrikeAnims[];

extern void switch_to_room(unsigned char room);
extern void tele_to_room(unsigned char room, unsigned char telex, unsigned char teley);

extern void earn_exp();
extern void update_character(WalkingCharacter* chara);
extern void draw_character(WalkingCharacter* chara);
extern void sword_check(WalkingCharacter* chara);


#endif

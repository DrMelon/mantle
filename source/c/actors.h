#ifndef __CHARACTER_H
#define __CHARACTER_H
enum Substate
{
  S_NORMAL,
  S_HURT,
  S_WINDUP,
  S_ATTACK,
  S_DOCILE
};

enum CharacterType
{
  CH_KRIS,
  CH_SUSIE,
  CH_RALSEI,
  CH_NOELLE
};

enum MonsterType
{
  MON_NORM,
  MON_SHOOT,
  MON_FISH,
  MON_DRAGON,
  MON_FLOWER,
  MON_BIRD,
  MON_SINGCAT
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

typedef struct
{
  unsigned char xpos;
  unsigned char ypos;
  unsigned char level;
  enum Substate substate;
  enum MonsterType montype;
} Monster;
#endif

#ifndef __ACTORS_H
#define __ACTORS_H
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

enum ItemType
{
  ITEM_SWORD,
  ITEM_CANDY,
  ITEM_ICEKEY,
  ITEM_SHELTERKEY
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

typedef struct
{
  unsigned char xpos;
  unsigned char ypos;
  enum ItemType itemtype;
  unsigned char living;
  unsigned char uniqueid;
} Item;

const unsigned char** characterWalkAnims[];
const unsigned char** characterStrikeAnims[];

void switch_to_room(unsigned char room);

void update_character(WalkingCharacter* chara);
void draw_character(WalkingCharacter* chara);

void update_item(Item* item, WalkingCharacter* chara);
void draw_item(Item* item);

#endif

#ifndef __ACTORS_H
#define __ACTORS_H
enum Substate
{
  S_NORMAL,
  S_HURT,
  S_WINDUP,
  S_ATTACK,
  S_DOCILE,
  S_DIE
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
  MON_WALKER,
  MON_SHOOTER,
  MON_FISH,
  MON_DRAGON,
  MON_FLOWER,
  MON_BIRD,
  MON_SINGCAT,
  MON_MIRROR,
  MON_STONE,
  MON_MANTLE,
  MON_SUMMON
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
  unsigned char uniqueid;
  unsigned char animframe;
  unsigned char direction;
  unsigned char health;
} Monster;


const unsigned char** characterWalkAnims[];
const unsigned char** characterStrikeAnims[];

void switch_to_room(unsigned char room);

void update_character(WalkingCharacter* chara);
void draw_character(WalkingCharacter* chara);

void update_monster(Monster* monster);
void update_mon_walker(Monster* walker);

void draw_monster(Monster* monster);
void draw_walker(Monster* walker);
void delete_monster(unsigned char idx);
#endif

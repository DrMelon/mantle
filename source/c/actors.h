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

extern void switch_to_room(unsigned char room);
extern void tele_to_room(unsigned char room, unsigned char telex, unsigned char teley);

extern void earn_exp();
extern void update_character(WalkingCharacter* chara);
extern void draw_character(WalkingCharacter* chara);
extern void sword_check(WalkingCharacter* chara);

extern void update_monster(Monster* monster);
extern void update_mon_walker(Monster* walker);
extern void update_mon_shooter(Monster* shooter);

extern void draw_monster(Monster* monster);
extern void draw_walker(Monster* walker);
extern void delete_monster(unsigned char idx);
#endif

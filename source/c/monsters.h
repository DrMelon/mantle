#ifndef __MONSTERS_H_
#define __MONSTERS_H_

#include "actors.h"
#include "jump_arc.h"

enum MonsterType
{
  MON_WALKER,
  MON_SHOOTER,
  MON_FISH,
  MON_LIZARD,
  MON_FLOWER,
  MON_BIRD,
  MON_SINGCAT,
  MON_ICEBLOCK
  // there is no MON_MANTLE. It isn't here, after all.
};

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
  unsigned char arcid;
} Monster;

extern void update_monster(Monster* monster);
extern void update_mon_walker(Monster* walker);
extern void update_mon_shooter(Monster* shooter);
extern void update_mon_fish(Monster* fish);
extern void update_mon_flower(Monster* flower);
extern void update_mon_lizard(Monster* lizard);
extern void update_mon_bird(Monster* bird);
extern void update_mon_cat(Monster* cat);

extern void draw_monster(Monster* monster);
extern void draw_walker(Monster* walker);
extern void draw_shooter(Monster* shooter);
extern void draw_fish(Monster* fish);
extern void draw_flower(Monster* flower);
extern void draw_lizard(Monster* lizard);
extern void draw_bird(Monster* bird);
extern void draw_cat(Monster* cat);

extern void delete_monster(unsigned char idx);


#endif

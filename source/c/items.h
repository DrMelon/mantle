#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "actors.h"

enum ItemType
{
  ITEM_SWORD,
  ITEM_CANDY,
  ITEM_ICEKEY,
  ITEM_SHELTERKEY,
};

typedef struct
{
  unsigned char xpos;
  unsigned char ypos;
  enum ItemType itemtype;
  unsigned char uniqueid;
} Item;

extern void update_item(Item* item, WalkingCharacter* chara);
extern void draw_item(Item* item);
extern void delete_item(unsigned char idx);
extern void spawn_candy(unsigned char px, unsigned char py);
extern void spawn_candy_bnk();
#endif

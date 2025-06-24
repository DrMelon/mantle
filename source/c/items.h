#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "actors.h"

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
  enum ItemType itemtype;
  unsigned char living;
  unsigned char uniqueid;
} Item;

void update_item(Item* item, WalkingCharacter* chara);
void draw_item(Item* item);
void delete_item(unsigned char idx);
#endif

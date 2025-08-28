#include "teleport.h"
#include "globals.h"
#include "roomstuff.h"
#include "maps.h"
#include "utils.h"
#include "monsters.h"
#include "neslib.h"

void update_buttons()
{
  Button* btn;
  Monster* mon;
  unsigned char should_be_pressed = 0;
  // If the room is unlocked, buttons no longer do anything.
  if(roomLocked == 0) return;
  x2 = 0; // current button pressed count
  for(i = 0; i < spawnedButtons; i++)
  {
    btn = &teleList[i].btn;

    // Kris check.
    should_be_pressed = point_in_rect(kris.xpos+7, kris.ypos+7, btn->xpos+2, btn->ypos+2, btn->xpos+14, btn->ypos+14);
    if(!should_be_pressed)
    {
      // ice block check
      for(i2 = 0; i2 < spawnedMonsters; i2++)
      {
        mon = &monsterList[i2];
        if(mon->montype == MON_ICEBLOCK)
        {
          if(point_in_rect(mon->xpos+7, mon->ypos+7, btn->xpos+2, btn->ypos+2, btn->xpos+14, btn->ypos+14))
          {
            should_be_pressed = 1;
            i2 = spawnedMonsters;
          }
        }
      }
    }

    if(btn->pressed != should_be_pressed)
    {
      // button state change!
      if(currentEnvironment == E_ICEPALACE)
      {
        set_map_tile_in_room((btn->xpos >> 4)-2, (btn->ypos >> 4)-3, should_be_pressed ? TILE_IP_BUTTON_DOWN : TILE_IP_BUTTON);
        ppu_wait_nmi(); //hehe
      }

      btn->pressed = should_be_pressed;
    }

    if(btn->pressed)
      x2++;
  }

  if(x2 == spawnedButtons) // all buttons pressed, unlock room!
  {
    unlock_room_doors();
  }

}

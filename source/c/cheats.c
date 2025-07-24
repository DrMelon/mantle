
#include "cheats.h"
#include "neslib.h"
#include "globals.h"
#include "bank_helpers.h"
#include "roomstuff.h"
#include "ui.h"

#ifdef CHEATS_ENABLED

unsigned char cheatInputIdx;
unsigned char lastEightInputs[8];
const unsigned char maxLevelCheat[] = { PAD_B, PAD_RIGHT, PAD_LEFT, PAD_RIGHT, PAD_UP, PAD_UP, PAD_DOWN, PAD_DOWN };
const unsigned char refillHpCheat[] = { PAD_B, PAD_UP, PAD_B, PAD_DOWN, PAD_B, PAD_UP, PAD_B, PAD_DOWN };
const unsigned char skipToIslandCheat[] = { PAD_B, PAD_UP, PAD_UP, PAD_UP, PAD_B, PAD_DOWN, PAD_DOWN, PAD_DOWN };
const unsigned char skipToPalaceCheat[] = { PAD_B, PAD_UP, PAD_UP, PAD_UP, PAD_B, PAD_LEFT, PAD_LEFT, PAD_LEFT };
const unsigned char skipToCityCheat[] = { PAD_B, PAD_UP, PAD_UP, PAD_UP, PAD_B, PAD_RIGHT, PAD_RIGHT, PAD_RIGHT };
const unsigned char skipToForestCheat[] = { PAD_B, PAD_UP, PAD_UP, PAD_UP, PAD_B, PAD_UP, PAD_UP, PAD_UP };
const unsigned char secretMessage[] = { PAD_UP, PAD_UP, PAD_DOWN, PAD_DOWN, PAD_LEFT, PAD_RIGHT, PAD_LEFT, PAD_RIGHT };
const unsigned char secretMessage2[] = { PAD_LEFT, PAD_RIGHT, PAD_LEFT, PAD_RIGHT, PAD_B, PAD_A, PAD_SELECT, PAD_START};

CODE_BANK(CHEAT_CODES_BANK);
void update_cheats()
{
  unsigned char did_input = 0;
  // Read current pad inputs
  if(pad_trig & PAD_LEFT)
  {
      lastEightInputs[cheatInputIdx] = PAD_LEFT;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_RIGHT)
  {
      lastEightInputs[cheatInputIdx] = PAD_RIGHT;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_UP)
  {
      lastEightInputs[cheatInputIdx] = PAD_UP;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_DOWN)
  {
      lastEightInputs[cheatInputIdx] = PAD_DOWN;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_A)
  {
      lastEightInputs[cheatInputIdx] = PAD_A;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_B)
  {
      lastEightInputs[cheatInputIdx] = PAD_B;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_SELECT)
  {
      lastEightInputs[cheatInputIdx] = PAD_SELECT;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }
  if(pad_trig & PAD_START)
  {
      lastEightInputs[cheatInputIdx] = PAD_START;
      cheatInputIdx = (cheatInputIdx + 1) % 8;
      did_input = 1;
  }

  // Read over the cheat inputs and see if any cheats match
  if(did_input)
  {
    // Scan through all the last inputs and match
    if(cheat_check(maxLevelCheat))
    {
       playerLevel = 4;
       hudDirty = 1;
       return;
    }
    else if(cheat_check(refillHpCheat))
    {
        playerHp = 16;
        hudDirty = 1;
        return;
    }
    else if(cheat_check(skipToIslandCheat))
    {
       banked_call(ROOM_LOGIC_BANK, skip_to_island);
       return;
    }
    else if(cheat_check(skipToPalaceCheat))
    {
       banked_call(ROOM_LOGIC_BANK, skip_to_ice_palace);
       return;
    }
    else if(cheat_check(secretMessage))
    {
       x2 = 1;
       return;
    }
    else if(cheat_check(secretMessage2) && x2 == 1)
    {
       banked_call(UI_BANK, queue_text_banked);
       return;
    }
    else if(cheat_check(skipToCityCheat))
    {
       banked_call(ROOM_LOGIC_BANK, skip_to_city);
       return;
    }
    else if(cheat_check(skipToForestCheat))
    {
      banked_call(ROOM_LOGIC_BANK, skip_to_forest);
      return;
    }
   }

}

unsigned char cheat_check(const unsigned char* cheat)
{
    for(i = 0; i < 8; i++)
    {
        i2 = ((8 - cheatInputIdx) + i) % 8; // offset through the ring buffer
        if(lastEightInputs[i] != cheat[i2])
        {
            // As soon as one check is failed, return
            return 0;
        }
    }

    return 1;
}
CODE_BANK_POP();
#endif

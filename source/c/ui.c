#include "ui.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"

const unsigned char hpText[] = "HP ";
const unsigned char lv1Text[] = "Lv1 ";
const unsigned char lv2Text[] = "Lv2 ";
const unsigned char lv3Text[] = "Lv3 ";
const unsigned char lvMaxText[] = "MAX ";

const unsigned char dialog_0[] = "Looking for someone..?";
const unsigned char dialog_1[] = "Or perhaps, for someTHING?";
const unsigned char dialog_2[] = "They cannot be found here.";
const unsigned char dialog_3[] = "But YOU knew that already, didn't you..?";

const unsigned char barBlocks[] = {0x20, 0x30, 0x40, 0x50, 0x60};
const unsigned char* lvStrings[] = {lv1Text, lv2Text, lv3Text, lvMaxText};

// RAM
unsigned char hudUpdateBuffer[24];

CODE_BANK(0);
void draw_ui_borders()
{
    // Draw the HUD borders
    vram_adr(NTADR_A(0, 0));
    for(y = 0; y < 30; y++)
    {
      for(x = 0; x < 32; x++)
      {
         vram_put(0x10);
      }
    }
}

void refresh_hud_bars(char hp, char lvl, char exp)
{
    unsigned int ntAdr = NTADR_A(4, 5); // starting pos of HUD ui
    char expCalc; // TODO: expcalc
    // begin hud update buffer
    // MSB|NT_UPD_HORZ, LSB, LEN, [bytes] for a horizontal sequence
    hudUpdateBuffer[0] = MSB(ntAdr)|NT_UPD_HORZ;
    hudUpdateBuffer[1] = LSB(ntAdr);

    // composing the horizontal sequence of bytes.
    // first, HP message
    i = 0;
    i2 = 3;
    while(hpText[i])
    {
       hudUpdateBuffer[i+i2] = hpText[i] + 0x80; // text offset
       i++;
    }
    i2 = i+i2;
    i = 0;

    // now draw HP bar based on hp status
    // 4 blocks, each with 5 possible fillstates - so we can represent up to 16 hp i guess?
    // 0x20, 0x30, 0x40, 0x50, 0x60
    // 0%,   25%,  50%,  75%,  100%
    for(i = 0; i < 4; i++)
    {
       // if hp = 0, empty bar.
       if(hp == 0)
       {
         hudUpdateBuffer[i+i2] = barBlocks[0];
         continue;
       }
       // block i's value should be:
       // 1. a full block if (i+1)*4 <= hp
       if((i+1)*4 <= hp)
       {
          hudUpdateBuffer[i+i2] = barBlocks[4];
          continue;
       }
       // 2. a partially-filled block if (i+1)*4 > hp
       if((i+1)*4 > hp)
       {
          hudUpdateBuffer[i+i2] = barBlocks[hp % 4];
       }
       // 3. an empty block if (i+1)*4 - hp > 4;
       if((i+1)*4 - hp > 4)
       {
          hudUpdateBuffer[i+i2] = barBlocks[0];
       }
    }

    // Spacer
    i2 = i+i2;
    for(i = 0; i < 3; i++)
    {
      hudUpdateBuffer[i+i2] = 0x10;
    }

    i2 = i+i2;
    i = 0;

    if(lvl > 0)
    {
      while(lvStrings[lvl-1][i])
      {
        hudUpdateBuffer[i+i2] = lvStrings[lvl-1][i] + 0x80; // text offset
        i++;
      }
      i2 = i+i2;
      i = 0;

      // Draw exp bar (same exact same as hp bar)
      for(i = 0; i < 4; i++)
      {
       // if hp = 0, empty bar.
       if(exp == 0)
       {
         hudUpdateBuffer[i+i2] = barBlocks[0];
         continue;
       }
       if((i+1)*4 <= exp)
       {
          hudUpdateBuffer[i+i2] = barBlocks[4];
          continue;
       }
       if((i+1)*4 > exp)
       {
          hudUpdateBuffer[i+i2] = barBlocks[exp % 4];
       }
       if((i+1)*4 - exp > 4)
       {
          hudUpdateBuffer[i+i2] = barBlocks[0];
       }
      }
    }

    i2 = i+i2;
    i = 0;
    for(i = 0; i+i2 <= 22; i++)
    {
      hudUpdateBuffer[i+i2] = 0x10; // final spacers
    }

    hudUpdateBuffer[2] = 20; // length of hud
    hudUpdateBuffer[23] = NT_UPD_EOF;
    set_vram_update(hudUpdateBuffer);
    writingVram = 1;
    hudDirty = 0;

}
CODE_BANK_POP();

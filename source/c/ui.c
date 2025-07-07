#include "ui.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"

const unsigned char hpText[] = "HP ";
const unsigned char lv1Text[] = "LV1 ";
const unsigned char lv2Text[] = "LV2 ";
const unsigned char lv3Text[] = "LV3 ";
const unsigned char lvMaxText[] = "MAX ";
const unsigned char lvZeroText[] = "LV0 ";

#define TEXT_LINE_MAX_LEN 27
#define TEXT_MAX_LINES 2
const unsigned char dialog_0[] = "Looking for someone..?";
const unsigned char dialog_1[] = "Or perhaps, for someTHING?";
const unsigned char dialog_2[] = "Neither will be found here.";
const unsigned char dialog_3[] = "But YOU knew that already, didn't you..?";

const unsigned char icekey_found_0[] = "YOU GOT THE ICE KEY";

const unsigned char instruct_0[] = "     BECOME STRONGER";
const unsigned char instruct_1[] = "     BECAME STRONGER";

const unsigned char barBlocks[] = {0x20, 0x30, 0x40, 0x50, 0x60};
const unsigned char* lvStrings[] = {lv1Text, lv2Text, lv3Text, lvMaxText};

// RAM
unsigned char hudUpdateBuffer[24];
unsigned char textBuffer[TEXT_LINE_MAX_LEN*TEXT_MAX_LINES];
unsigned char textVRAMBuffer[(TEXT_LINE_MAX_LEN*TEXT_MAX_LINES)+28];

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

      // Draw exp bar (up to 24 exp)
      for(i = 0; i < 6; i++)
      {
       // if exp = 0, empty bar.
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

void queue_text(const unsigned char* textLine, unsigned char mode)
{
   // Process text line and load into buffer
   i = 0;
   while(textLine[i])
   {
       textBuffer[i] = textLine[i] + 0x80;
       i++;
   }
   textQueued = mode;
   textLength = i;
   textColOffset = 0;
   textLineOffset = 0;
   textSeekChar = 0;
   textDelay = 2; // Can override this with different line display speeds.
}

void update_text()
{
  unsigned int ntAdr;
  if(textQueued == 0) return;
  // VRAM already being written to, cancel.
  if(writingVram == 1) return;

  // Text update modes:
  // 0 = Nothing
  // 1 = Character-by-character
  // 2 = All at once
  // 3 = Clear text
  if(textQueued == 1)
  {
    if(framecount % textDelay != 0) return; // delay appropriately
    // Update the next char on the screen.
    if(textSeekChar < textLength)
    {
       if(textColOffset < TEXT_LINE_MAX_LEN)
       {
          textColOffset++;
       }
       if(textColOffset >= TEXT_LINE_MAX_LEN)
       {
          textLineOffset++;
          textColOffset = 0;
       }

       // Draw at this x, y
       ntAdr = NTADR_A(3 + textColOffset, 24 + textLineOffset);
       textVRAMBuffer[0] = MSB(ntAdr);
       textVRAMBuffer[1] = LSB(ntAdr);
       textVRAMBuffer[2] = textBuffer[textSeekChar];
       textVRAMBuffer[3] = NT_UPD_EOF;

       textSeekChar++;
       set_vram_update(textVRAMBuffer);
       writingVram = 1;
    }
    else
    {
      textQueued = 0;
    }
  }
  else if(textQueued == 3)
  {
    for(y = 0; y < TEXT_MAX_LINES; y++)
    {
       ntAdr = NTADR_A(3, y+24);
       textVRAMBuffer[0+(y*(TEXT_LINE_MAX_LEN+3))] = MSB(ntAdr) | NT_UPD_HORZ;
       textVRAMBuffer[1+(y*(TEXT_LINE_MAX_LEN+3))] = LSB(ntAdr);
       textVRAMBuffer[2+(y*(TEXT_LINE_MAX_LEN+3))] = TEXT_LINE_MAX_LEN;
       for(x = 0; x < TEXT_LINE_MAX_LEN; x++)
       {
         textVRAMBuffer[3+x+(y*(TEXT_LINE_MAX_LEN+3))] = 0xA0;
       }
    }
    textVRAMBuffer[((TEXT_LINE_MAX_LEN+3)*TEXT_MAX_LINES)] = NT_UPD_EOF;
    set_vram_update(textVRAMBuffer);
    writingVram = 1;
    textQueued = 0;
    textLength = 0;
    textSeekChar = 0;
  }
}

void clear_text()
{
  textQueued = 3;
  textColOffset = 0;
  textLineOffset = 0;
}

CODE_BANK_POP();

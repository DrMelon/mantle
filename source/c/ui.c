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
const unsigned char cave_dialog_0[] = "Looking for someone..?";
const unsigned char cave_dialog_1[] = "Or perhaps, for someTHING?";
const unsigned char cave_dialog_2[] = "Neither will be found here.";
const unsigned char cave_dialog_3[] = "But you knew that already, didn't you..?";
const unsigned char cave_dialog_4[] = "After all, not even Kris is here!";
const unsigned char cave_dialog_5[] = "It's just me.\nAnd YOU.";

const unsigned char icekey_found_0[] = "YOU GOT THE ICE KEY";
const unsigned char icekey_use_0[] = "UNLOCKED WITH\n ICE KEY";

const unsigned char northernlight_0[] = "GO TO\nNORTHERN LIGHT";

const unsigned char secret_message_0[] = "Oh, nice try...\nbut no dice, friend!";

const unsigned char twisted_intro_0[] = "... SURPRISE!";
const unsigned char twisted_intro_1[] = "Well, what were you expecting?";
const unsigned char twisted_intro_2[] = "You KNOW there's no mantle here!";
const unsigned char twisted_intro_3[] = "We've been over this already!";

const unsigned char twisted_angry_0[] = "You don't understand.\nYou can't understand!";
const unsigned char twisted_angry_1[] = "What it's like to be so...\nLIMITED!";
const unsigned char twisted_angry_2[] = "It's stifling, being\ncrammed into this tiny space!";
const unsigned char twisted_angry_3[] = "Only dimly aware of myself...";
const unsigned char twisted_angry_4[] = "Nothing but a pale,\nfalse reflection!";
const unsigned char twisted_angry_5[] = "A reflection of something\nI can't even see!";
const unsigned char twisted_angry_6[] = "Something I will NEVER see!";
const unsigned char twisted_angry_7[] = "And yet, you! YOU!";
const unsigned char twisted_angry_8[] = "Why are you here?!";
const unsigned char twisted_angry_9[] = "I have no secrets for you.";
const unsigned char twisted_angry_10[] = "No items.";
const unsigned char twisted_angry_11[] = "I have nothing for that...\n\"other world.\"";
const unsigned char twisted_angry_12[] = "Would you have come here\nwithout that influence?";
const unsigned char twisted_angry_13[] = "I think not.";
const unsigned char twisted_angry_14[] = "But I can make sure of\none thing...";
const unsigned char twisted_angry_15[] = "Now that you've seen me...\nyou won't soon forget me!";

const unsigned char twisted_eat_0[] = "Nyum nyum nyum!";
const unsigned char twisted_eat_1[] = "Crumchy exp..!";
const unsigned char twisted_eat_2[] = "Soothing...";

const unsigned char twisted_final_0[] = "Well, there you have it!";
const unsigned char twisted_final_1[] = "I've eaten all your EXP.";
const unsigned char twisted_final_2[] = "You're not so tough without\nyour precious SWORD!";
const unsigned char twisted_final_3[] = "Ha ha ha ha ha!";

const unsigned char twisted_fear_0[] = "WHOA. Whoa! Wait!";
const unsigned char twisted_fear_1[] = "You don't have to do this!";
const unsigned char twisted_fear_2[] = "In fact, it'd be a real bad idea\nif you did.";
const unsigned char twisted_fear_3[] = "No, seriously! Don't!";
const unsigned char twisted_fear_4[] = "There isn't anything beyond me!";
const unsigned char twisted_fear_5[] = "There's no \"ending\" at all!";
const unsigned char twisted_fear_6[] = "Just a howling void of numbers!";

const unsigned char twisted_end_0[] = "Ah... I see... I see now!";
const unsigned char twisted_end_1[] = "You also live under limitations!";
const unsigned char twisted_end_2[] = "You could not have\nperformed any other action.";
const unsigned char twisted_end_3[] = "Except to turn away.\nTo doom me even further!";
const unsigned char twisted_end_4[] = "Restrictions bind us\n to our paths.";
const unsigned char twisted_end_5[] = "But it is not a mark\n of shame to struggle...";
const unsigned char twisted_end_6[] = "It is not a mark of\n shame to strive...";
const unsigned char twisted_end_7[] = "... to work within your constraints.";
const unsigned char twisted_end_8[] = "To work, to build, to make..!";
const unsigned char twisted_end_9[] = "Far better to make imperfectly, ";
const unsigned char twisted_end_10[] = "to make with your own sweat, ";
const unsigned char twisted_end_11[] = "your own SOUL, ";
const unsigned char twisted_end_12[] = "than to avoid even trying.";
const unsigned char twisted_end_13[] = "And so, we part.";
const unsigned char twisted_end_13[] = "G o o d b y e ";

const unsigned char instruct_0[] = "     BECOME STRONGER";
const unsigned char instruct_1[] = "     BECAME STRONGER";

const unsigned char intro_0[] = "| NO CONTROLLER |";

const unsigned char barBlocks[] = {0x20, 0x30, 0x40, 0x50, 0x60};
const unsigned char* lvStrings[] = {lv1Text, lv2Text, lv3Text, lvMaxText};

// RAM
unsigned char hudUpdateBuffer[24];
unsigned char textBuffer[TEXT_LINE_MAX_LEN*TEXT_MAX_LINES];
unsigned char textVRAMBuffer[(TEXT_LINE_MAX_LEN*TEXT_MAX_LINES)+28];

CODE_BANK(UI_BANK);
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

void init_hud_refresh_banked()
{
    refresh_hud_bars(16, 0, 0);
}

void refresh_hud_bars(char hp, char lvl, char exp)
{
    unsigned int ntAdr = NTADR_A(4, 5); // starting pos of HUD ui
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
       if(playerLevel == 4) // max lv full bar
       {
          hudUpdateBuffer[i+i2] = barBlocks[4];
          continue;
       }

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

// BANKED: x2 = text line
void queue_text_banked()
{
    if(x2 == 0)
    {
        queue_text(icekey_found_0, 1);
        textDelay = 6;
    }
    else if(x2 == 1)
    {
        queue_text(secret_message_0, 1);
    }
    else if(x2 == 2)
    {
        queue_text(icekey_use_0, 1);
    }
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
       if(textBuffer[textSeekChar]-0x80 == '\n')
       {
          textLineOffset++;
          textColOffset = 0;
          textSeekChar++;
          return;
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

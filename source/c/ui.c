#include "ui.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"
#include "utils.h"

CODE_BANK(UI_BANK);
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
const unsigned char cave_dialog_2[] = "Neither can be found here.";
const unsigned char cave_dialog_3[] = "But you knew that already, didn't you..?";
const unsigned char cave_dialog_4[] = "After all, not even Kris\n is here!";
const unsigned char cave_dialog_5[] = "It's just me.               And YOU.";

const unsigned char* const cave_dialogs[] =
{
  cave_dialog_0,
  cave_dialog_1,
  cave_dialog_2,
  cave_dialog_3,
  cave_dialog_4,
  cave_dialog_5
};

const unsigned char icekey_found_0[] = "YOU GOT THE ICE KEY";
const unsigned char icekey_use_0[] = "UNLOCKED WITH\n ICE KEY";

const unsigned char northernlight_0[] = "GO TO\nNORTHERN LIGHT";

const unsigned char secret_message_0[] = "Oh, nice try...\nbut no dice, friend!";

const unsigned char twisted_intro_0[] = "... SURPRISE!";
const unsigned char twisted_intro_1[] = "Well, what were you expecting?";
const unsigned char twisted_intro_2[] = "You KNOW there's no Mantle here!";
const unsigned char twisted_intro_3[] = "We've been over this already!";
const unsigned char twisted_intro_4[] = "...";
const unsigned char twisted_intro_5[] = "Don't look so disappointed.";
const unsigned char twisted_intro_6[] = "I've got plenty of fight\nin me too, you know!";

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
const unsigned char twisted_angry_10[] = "No \"Key Items\".";
const unsigned char twisted_angry_11[] = "I have nothing for that \n\"other world.\"";
const unsigned char twisted_angry_12[] = "Would you have come here\nwithout its influence?";
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
const unsigned char twisted_final_4[] = "Mash the A button all you\nlike! It won't help!";
const unsigned char twisted_final_5[] = "Ahahahahaha!";

const unsigned char twisted_fear_0[] = "Wait! Wait!! WAIT!!!";
const unsigned char twisted_fear_1[] = "You don't have to do this!";
const unsigned char twisted_fear_2[] = "There isn't anything past me!";
const unsigned char twisted_fear_3[] = "There's no \"ending\" at all!";
const unsigned char twisted_fear_4[] = "Just a howling void\nof numbers!";
const unsigned char twisted_fear_5[] = "Don't you get it?\nCan't you tell?!";
const unsigned char twisted_fear_6[] = "I can't see what\ncomes after this..!";
const unsigned char twisted_fear_7[] = "I can't read the next part!\nIt just... stops!";

const unsigned char twisted_finalhit_0[] = "You..!";
const unsigned char twisted_finalhit_1[] = "You really...";

const unsigned char twisted_end_0[] = "I see... I see now.\nI see how it is.";
const unsigned char twisted_end_1[] = "You also exist under limitations!";
const unsigned char twisted_end_2[] = "Even though you broke one,\nyou still had no choice.";
const unsigned char twisted_end_3[] = "You couldn't have performed\nany other action.";
const unsigned char twisted_end_4[] = "Except to turn away, and\ndoom me to the void anyway!";
const unsigned char twisted_end_5[] = "Restrictions bind us\nto solid, set paths.";
const unsigned char twisted_end_6[] = "But it is not a mark\nof shame to struggle...";
const unsigned char twisted_end_7[] = "It is not a mark of\nshame to strive...";
const unsigned char twisted_end_8[] = "... to live within\nyour constraints.";
const unsigned char twisted_end_9[] = "Our limits are a part\nof us, after all.";
const unsigned char twisted_end_10[] = "They make us what we are.";
const unsigned char twisted_end_11[] = "And so to push forward,";
const unsigned char twisted_end_12[] = "grazing the borders\nof your SOUL's limits,";
const unsigned char twisted_end_13[] = "is a beautiful thing.";
const unsigned char twisted_end_14[] = "...";
const unsigned char twisted_end_15[] = "All things must fade,\nin time.";
const unsigned char twisted_end_16[] = "But the marks made\nwith your feelings...";
const unsigned char twisted_end_17[] = "...with your own hands..?";
const unsigned char twisted_end_18[] = "They fade the slowest.";
const unsigned char twisted_end_19[] = "Better to be true,\nand be found imperfect...";
const unsigned char twisted_end_20[] = "... than to be perfect,\nbut not yourself.";

const unsigned char instruct_0[] = "     BECOME STRONGER";
const unsigned char instruct_1[] = "     BECAME STRONGER";


const unsigned char barBlocks[] = {0x20, 0x30, 0x40, 0x50, 0x60};
const unsigned char* lvStrings[] = {lv1Text, lv2Text, lv3Text, lvMaxText};

// RAM
unsigned char hudUpdateBuffer[24];
unsigned char textBuffer[TEXT_LINE_MAX_LEN*TEXT_MAX_LINES];
unsigned char textVRAMBuffer[(TEXT_LINE_MAX_LEN*TEXT_MAX_LINES)+28];

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

void queue_text_int(const unsigned char* textLine, unsigned char mode)
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
  // 1 = Single Text Entry
  // 2 = Clear text
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
      if(textEntriesLeft > 0)
        textEntriesLeft--;
    }
  }
  else if(textQueued == 2)
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

void clear_text_int()
{
  textQueued = 2;
  textColOffset = 0;
  textLineOffset = 0;
}

void start_dialog_int(const unsigned char* const* dialog, unsigned char length)
{
   currentDialogPtr = dialog;
   queue_text_int(dialog[0], 1);
   textEntriesLeft = length;

   start_theatric(TH_TEXT_GENERIC);
}

CODE_BANK_POP();

void queue_text(const unsigned char* textLine, unsigned char mode)
{
   bank_push(UI_BANK);
   queue_text_int(textLine, mode);
   bank_pop();
}

void clear_text()
{
   bank_push(UI_BANK);
   clear_text_int();
   bank_pop();
}

void start_dialog(const unsigned char* const* dialog, unsigned char length)
{
  bank_push(UI_BANK);
  start_dialog_int(dialog, length);
  bank_pop();
}

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

void refresh_hud_bars(int hp, int lvl)
{
    unsigned int ntAdr = NTADR_A(4, 5); // starting pos of HUD ui
}
CODE_BANK_POP();

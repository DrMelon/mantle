
#include "intro.h"
#include "neslib.h"
#include "bank_helpers.h"
#include "globals.h"
#include "mapper.h"
#include "palettes.h"
#include "ui.h"

CODE_BANK(INTRO_BANK);

const unsigned char intro_tile_layout[]={
  0,  1,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  5,  6,  7,  8,  9,
 10, 11, 12, 13, 14,  3, 15, 16, 17, 11, 18, 18, 19, 11, 20, 21, 18, 18, 22,
 23, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 24,
 25, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 26,
 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 18, 18, 18, 18, 24
};

void load_and_show_intro()
{
    theatricActive = 1;
    theatricIndex = TH_INTRO;
    theatricStage = 0;
    theatricTimer = 0;
    framecount = 0;

    // PPU off, set CHR bank & load palettes
    ppu_off();

    mmc1_set_chr_bank_0(6);
    mmc1_set_chr_bank_1(1);

    pal_bg(paletteIntro);
    pal_spr(palSpritesDesert);

    // clear screen
    vram_adr(NTADR_A(0, 0));
    vram_fill(0x00, 0x3BF);
    vram_adr(0x23C0);
    vram_fill(0x00, 0xFF);

    // load mantle logo in

    vram_adr(NTADR_A(2, 3));
    for(y = 0; y < 5; y++)
    {
        for(x = 0; x < 19; x++)
        {
            vram_adr(NTADR_A(x+6, y+10));
            i = x + (y*19);
            vram_put(intro_tile_layout[i]);
        }
    }

    // Set palette fully to green
    pal_col(0, 0x29);
    pal_col(1, 0x29);
    pal_col(2, 0x29);
    pal_col(3, 0x29);

    // start the music - the whooshing waves, that then give way to the intro tune.

    ppu_on_all();
}

void update_intro()
{
    if(theatricStage == 0)
    {
        // begin the initial sequence, with the green & blue flashes
        // this is done initially by just toggling bg color
        if(framecount % 45 == 0)
        {
            if(framecount == 90)
            {
               framecount = 0;
               // prevents uneven wrapping at 255
            }
            theatricTimer++;
            if(theatricTimer % 2 == 0)
            {
                pal_col(0, 0x01);
                pal_col(1, 0x01);
                pal_col(2, 0x01);
                pal_col(3, 0x01);
            }
            else
            {
                pal_col(0, 0x29);
                pal_col(1, 0x29);
                pal_col(2, 0x29);
                pal_col(3, 0x29);
            }
            // flashes 4 times, then proceeds to next step
            if(theatricTimer >= 5)
            {
                theatricStage = 1;
                theatricTimer = 0;
                pal_col(0, 0x0F); // set to fade-up colors, starting at black
                pal_col(1, 0x0F); // set to fade-up colors, starting at black
                pal_col(2, 0x0F); // set to fade-up colors, starting at black
                pal_col(3, 0x0F); // set to fade-up colors, starting at black
            }
        }
    }
    else if(theatricStage == 1)
    {
        // twinkly stars and fade-up start here

        // TODO: twinkly stars. use stars from ice magic sprites?
        // encode them as Projectiles i guess...

        if(framecount % 60 == 0)
        {
            if(framecount == 60)
            {
               framecount = 0;
            }
            theatricTimer++;
            if(theatricTimer== 1)
            {
                pal_col(0, 0x0F);
                pal_col(1, paletteIntro[13]);
                pal_col(2, paletteIntro[14]);
                pal_col(3, paletteIntro[15]);
            }
            if(theatricTimer== 2)
            {
                // first-stage fadeup
                pal_col(0, 0x01);
                pal_col(1, paletteIntro[9]);
                pal_col(2, paletteIntro[10]);
                pal_col(3, paletteIntro[11]);
            }
            else if(theatricTimer == 3)
            {
                // fade-up complete
                pal_col(0, 0x01);
                pal_col(1, paletteIntro[1]);
                pal_col(2, paletteIntro[2]);
                pal_col(3, paletteIntro[3]);
            }
            else if(theatricTimer == 4)
            {
                // fade-up complete, show controller disconnected screen now

                // ppu off and write to screen
                ppu_off();

                // clear screen then write text box
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);

                vram_adr(NTADR_A(7, 14));
                for(i = 0; i < 17; i++)
                {
                    vram_put(intro_0[i] + 0x80); // main message
                }

                // Borders on top and bottom
                vram_adr(NTADR_A(8, 13));
                for(i = 0; i < 15; i++)
                {
                    vram_put('-' + 0x80); // main message
                }

                vram_adr(NTADR_A(8, 15));
                for(i = 0; i < 15; i++)
                {
                    vram_put('-' + 0x80); // main message
                }

                ppu_on_all(); // turn it back on
            }
            else if(theatricTimer == 6) // wait for controller to "be plugged in"
            {
                // now we can move to the final stage, showing the green & black patterns before finally performing a first-load of the selected environment (currentEnvironment),
                // showing the SOUL within minikris,
                // and transitioning to gameplay
                theatricStage = 2;
                theatricTimer = 0;
                framecount = 0;

                // ppu off, clear, set to green & black palette, draw a green square in the lower left
                ppu_off();

                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);

                pal_col(0, 0x0F);
                pal_col(1, paletteIntro[5]);
                pal_col(2, paletteIntro[6]);
                pal_col(3, paletteIntro[7]);

                for(y = 15; y < 30; y++)
                {
                    for(x = 0; x < 16; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();
            }
        }
    }
    else if(theatricStage == 2)
    {
        if(framecount % 15 == 0 && theatricTimer < 7)
        {
            framecount = 0;
            theatricTimer++;

            if(theatricTimer == 1)
            {
                // top-right square
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);
                for(y = 0; y < 15; y++)
                {
                    for(x = 16; x < 32; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();
            }
            else if(theatricTimer == 2)
            {
                // top-left square
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);
                for(y = 0; y < 15; y++)
                {
                    for(x = 0; x < 16; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();
            }
            else if(theatricTimer == 3)
            {
                // bottom-right square
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);
                for(y = 15; y < 30; y++)
                {
                    for(x = 16; x < 32; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();
            }
            else if(theatricTimer == 4)
            {
                // bottom third
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);
                for(y = 20; y < 30; y++)
                {
                    for(x = 0; x < 32; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();

            }
            else if(theatricTimer == 5)
            {
                // top third
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);
                for(y = 0; y < 10; y++)
                {
                    for(x = 0; x < 32; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();
            }
            else if(theatricTimer == 6)
            {
                // middle third
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x00, 0x3BF);
                for(y = 10; y < 20; y++)
                {
                    for(x = 0; x < 32; x++)
                    {
                        vram_adr(NTADR_A(x, y));
                        vram_put(0x31);
                    }
                }

                ppu_on_all();

                // "fast" framecount
                framecount = 7;
            }
            else if(theatricTimer == 7)
            {
                // all green with black stripes, scrolls upwards rapidly
                ppu_off();
                vram_adr(NTADR_A(0, 0));
                vram_fill(0x31, 0x3BF);
                for(y = 0; y < 30; y++)
                {
                    if(y == 4 || y == 30-4)
                    {
                        for(x = 0; x < 32; x++)
                        {
                            vram_adr(NTADR_A(x, y));
                            vram_put(0x00);
                        }
                    }
                }

                ppu_on_all();
                framecount = 0;
                y2 = 1; // scroll velocity
                y = 0; // current scroll
            }
        }
        else if(theatricTimer == 7)
        {
            y += y2 >> 3;
            if(y2 < 255)
            {
                y2++;
            }

            scroll(0, y);

            // Once scroll has accelerated enough and framecount is at a 1-second boundary, end intro and begin the initial room "load-in" sequence.
            if(y2 == 255 && framecount % 60 == 0)
            {
                theatricActive = 0;
            }

        }

    }
}

CODE_BANK_POP();

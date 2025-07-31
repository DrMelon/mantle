#include "utils.h"
#include "globals.h"
#include "famistudio_cc65.h"

unsigned char point_in_rect(unsigned char px, unsigned char py, unsigned char sx, unsigned char sy, unsigned char ex, unsigned char ey)
{
    if(px < sx) return 0;
    if(px > ex) return 0;
    if(py < sy) return 0;
    if(py > ey) return 0;
    return 1;
}

int sign(int value)
{
    if(value < 0) return -1;
    if(value > 0) return 1;
    return 0;
}

int abs(int value)
{
    if(value < 0)
        return value * -1;
    return value;
}

void music_play(unsigned char music)
{
    bank_push(MUSIC_BANK);
    famistudio_music_play(music);
    bank_pop();
}

void music_stop()
{
    bank_push(MUSIC_BANK);
    famistudio_music_stop();
    bank_pop();
}

unsigned char narrative_flag_get(unsigned char narrativeFlag)
{
    return (narrativeFlags & narrativeFlag);
}

void narrative_flag_set(unsigned char narrativeFlag)
{
    narrativeFlags |= narrativeFlag;
}

void narrative_flag_clr(unsigned char narrativeFlag)
{
    narrativeFlags &= ~narrativeFlag;
}

unsigned char is_dead(unsigned char monsterid)
{
    unsigned char bitidx = monsterid % 8;
    unsigned char mainidx = monsterid >> 3;
    return deadList[mainidx] & (1 << bitidx);
}

void mark_dead(unsigned char monsterid)
{
    unsigned char bitidx = monsterid % 8;
    unsigned char mainidx = monsterid >> 3;
    deadList[mainidx] |= (1 << bitidx);
}

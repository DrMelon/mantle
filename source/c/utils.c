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

void start_theatric(unsigned char theatricId)
{
    theatricIndex = theatricId;
    theatricTimer = 0;
    theatricStage = 0;
    theatricActive = 1;
}

void end_theatric()
{
    theatricTimer = 0;
    theatricIndex = 0;
    theatricStage = 0;
    theatricActive = 0;
}

void reset_follow_pos()
{
    for(i2 = 0; i2 < 64; i2+=2)
    {
        followPositions[i2] = kris.xpos;
        followPositions[i2+1] = kris.ypos;
    }
}

void follower_shoot_check()
{
    // spawn an ice blast particle in noelle's facing direction and fire it off.
    bank_push(MONSTER_PROJECTILES_BANK);
    if(followerA.direction == 0)
    {
        spawn_projectile(followerA.xpos+4, followerA.ypos+4, P_ICEMAGIC, 0, 512);
    }
    else if(followerA.direction == 1)
    {
        spawn_projectile(followerA.xpos+4, followerA.ypos+4, P_ICEMAGIC, 512, 0);
    }
    else if(followerA.direction == 2)
    {
        spawn_projectile(followerA.xpos+4, followerA.ypos+4, P_ICEMAGIC, 0, -512);
    }
    else if(followerA.direction == 3)
    {
        spawn_projectile(followerA.xpos+4, followerA.ypos+4, P_ICEMAGIC, -512, 0);
    }
    bank_pop();
}

#ifndef __UTILS_H_
#define __UTILS_H_

extern unsigned char point_in_rect(unsigned char px, unsigned char py, unsigned char sx, unsigned char sy, unsigned char ex, unsigned char ey);
extern int sign(int value);
extern int abs(int value);
extern void music_play(unsigned char music);
extern void music_stop();
extern unsigned char narrative_flag_get(unsigned char narrativeFlag);
extern void narrative_flag_set(unsigned char narrativeFlag);
extern void narrative_flag_clr(unsigned char narrativeFlag);

//8.8 fixed point stuff
#define FP 8
#define FP_WHOLE(x) (x >> FP)
#define FP_FRACT(x) (x & 0x00FF)

// packed boolean array utils for deadlist
extern unsigned char is_dead(unsigned char monsterid);
extern void mark_dead(unsigned char monsterid);

// theatrics
extern void start_theatric(unsigned char theatricId);
extern void end_theatric();

#endif

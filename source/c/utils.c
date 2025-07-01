#include "utils.h"

unsigned char point_in_rect(unsigned char px, unsigned char py, unsigned char sx, unsigned char sy, unsigned char ex, unsigned char ey)
{
    if(px < sx) return 0;
    if(px > ex) return 0;
    if(py < sy) return 0;
    if(py > ey) return 0;
    return 1;
}

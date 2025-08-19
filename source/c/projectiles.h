#ifndef __PROJECTILES_H_
#define __PROJECTILES_H_

enum ProjectileType
{
    P_ARROW,
    P_FRIENDLINESS_PELLET,
    P_ICEMAGIC,
    P_NOTE,
    P_BURST
};

typedef struct
{
  unsigned int xpos;
  unsigned int ypos;
  int xvel;
  int yvel;
  enum ProjectileType projtype;
} Projectile;


extern void update_projectile(Projectile* proj);
extern void draw_projectile(Projectile* proj);
extern void spawn_projectile(unsigned char sx, unsigned char sy, enum ProjectileType type, int xvel, int yvel);
extern void quickspawn_burst();

#endif

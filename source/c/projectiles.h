#ifndef __PROJECTILES_H_
#define __PROJECTILES_H_

enum ProjectileType
{
    P_ARROW,
    P_FRIENDLINESS_PELLET,
    P_BOMBWAVE
};

typedef struct
{
  unsigned char xpos;
  unsigned char ypos;
  unsigned char xvel;
  unsigned char yvel;
  enum ProjectileType projtype;
} Projectile;


extern void update_projectile(Projectile* proj);
extern void draw_projectile(Projectile* proj);
extern void spawn_projectile(unsigned char sx, unsigned char sy, enum ProjectileType type, unsigned char xvel, unsigned char yvel);

#endif

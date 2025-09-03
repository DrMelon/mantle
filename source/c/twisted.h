#ifndef __TWISTED_H_
#define __TWISTED_H_

// Showtime!
enum TwistedEmotionState
{
  TE_NEUTRAL,
  TE_SMUG,
  TE_SAD,
  TE_ANGRY,
  TE_HURT,
  TE_BIG,
  TE_GLOAT,
  TE_TERROR,
  TE_SHOCK,
};

enum TwistedActionState
{
  TA_APPEAR,
  TA_IDLE,
  TA_MOVE_TO_POINT_WORLD,
  TA_LEAVE_SCREEN,
  TA_MOVE_TO_POINT_SCREEN,
  TA_CHARGE_ACROSS_SCREEN,
  TA_WINDUP,
  TA_SHOOT_ARROW,
  TA_SHOOT_PELLET,
  TA_EAT_EXP,
  TA_FINAL
};

typedef struct
{
  int xoffset;
  int yoffset;
} TwistedComponent;

typedef struct
{
  unsigned char init;
  unsigned int xpos;
  unsigned int ypos;
  TwistedComponent leftEye;
  TwistedComponent rightEye;
  TwistedComponent mouth;
  enum TwistedEmotionState emot;
  enum TwistedActionState state;
  unsigned char mouthAnimFrame;
  unsigned char floatFrame;
  unsigned char stateTimer;
  unsigned char stateDataX;
  unsigned char stateDataY;
  unsigned char lookDir;
  unsigned char invuln;
  unsigned char fightStage;
} Twisted;

extern void init_twisted();
extern void twisted_theatrics();
extern void update_twisted();
extern void draw_twisted();
extern void twisted_shoot_arrow();
extern void twisted_shoot_pellet();

#endif

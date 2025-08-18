#ifndef __TELEPORT_H_
#define __TELEPORT_H_

typedef struct
{
  unsigned char tx;
  unsigned char ty;
  unsigned char targetroom;
  unsigned char targetx;
  unsigned char targety;
} Teleporter;

typedef struct
{
  unsigned char xpos;
  unsigned char ypos;
  unsigned char pressed;
} Button;

typedef union
{
  Teleporter tele;
  Button btn;
} Telebutton;

extern void update_buttons();

#endif

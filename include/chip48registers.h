#ifndef CHIP48REGISTERS_H
#define CHIP48REGISTERS_H

#include "config.h"

struct chip48_registers
{
  unsigned char V[CHIP48_TOTAL_DATA_REGISTERS];
  unsigned short I;
  unsigned char delay_timer;
  unsigned char sound_timer;
  unsigned short PC;
  unsigned char SP;
};

#endif
#ifndef CHIP48_H
#define CHIP48_H

#include "config.h"
#include "chip48memory.h"
#include "chip48registers.h"
#include "chip48stack.h"
#include "chip48keyboard.h"

struct chip48
{
  struct chip48_memory memory;
  struct chip48_stack stack;
  struct chip48_registers registers;
  struct chip48_keyboard keyboard;
};

void chip48_init(struct chip48* chip48);

#endif  
#ifndef CHIP48_H
#define CHIP48_H

#include "config.h"
#include "chip48memory.h"
#include "chip48registers.h"
#include "chip48stack.h"
#include "chip48keyboard.h"
#include "chip48screen.h"
#include <stddef.h>

struct chip48
{
  struct chip48_memory memory;
  struct chip48_stack stack;
  struct chip48_registers registers;
  struct chip48_keyboard keyboard;
  struct chip48_screen screen;
};

void chip48_init(struct chip48* chip48);
void chip48_load(struct chip48* chip48, const char* buf, size_t size);
void chip48_exec(struct chip48* chip48, unsigned short opcode);

#endif
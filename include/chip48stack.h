#ifndef CHIP48STACK_H
#define CHIP48STACK_H

#include "config.h"

struct chip48;
struct chip48_stack
{
  unsigned short stack[CHIP48_TOTAL_STACK_DEPTH];
};

void chip48_stack_push(struct chip48* chip48, unsigned short val);
unsigned short chip48_stack_pop(struct chip48* chip48);

#endif
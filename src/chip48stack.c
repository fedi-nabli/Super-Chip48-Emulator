#include "chip48stack.h"
#include "chip48.h"
#include <assert.h>

static void chip48_stack_in_bounds(struct chip48* chip48)
{
  assert(chip48->registers.SP < CHIP48_TOTAL_STACK_DEPTH);
}

void chip48_stack_push(struct chip48* chip48, unsigned short val)
{
  chip48_stack_in_bounds(chip48);

  chip48->stack.stack[chip48->registers.SP] = val;
  chip48->registers.SP += 1;
}

unsigned short chip48_stack_pop(struct chip48* chip48)
{
  chip48->registers.SP -= 1;
  chip48_stack_in_bounds(chip48);

  unsigned short val = chip48->stack.stack[chip48->registers.SP];
  return val;
}
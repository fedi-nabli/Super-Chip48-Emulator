#include "chip48memory.h"
#include <assert.h>

static void chip48_is_memory_in_bounds(int index)
{
  assert(index >= 0 && index < CHIP48_MEMORY_SIZE);
}

void chip48_memory_set(struct chip48_memory* memory, int index, unsigned char val)
{
  chip48_is_memory_in_bounds(index);

  memory->memory[index] = val;
}

unsigned char chip48_memory_get(struct chip48_memory* memory, int index)
{
  return memory->memory[index];
}
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
  chip48_is_memory_in_bounds(index);
  return memory->memory[index];
}

unsigned short chip48_memory_get_short(struct chip48_memory* memory, int index)
{
  unsigned char byte1 = chip48_memory_get(memory, index);
  unsigned char byte2 = chip48_memory_get(memory, index+1);
  return byte1 << 8 | byte2;
}
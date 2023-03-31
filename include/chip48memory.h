#ifndef CHIP48MEMORY_H
#define CHIP48MEMORY_H

#include "config.h"

struct chip48_memory
{
  unsigned char memory[CHIP48_MEMORY_SIZE];
};

void chip48_memory_set(struct chip48_memory* memory, int index, unsigned char val);
unsigned char chip48_memory_get(struct chip48_memory* memory, int index);

#endif
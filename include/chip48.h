#ifndef CHIP48_H
#define CHIP48_H

#include "config.h"
#include "chip48memory.h"
#include "chip48registers.h"

struct chip48
{
  struct chip48_memory memory;
  struct chip48_registers registers;
};

#endif  
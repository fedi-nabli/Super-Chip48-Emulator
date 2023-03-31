#include "chip48.h"
#include <memory.h>

void chip48_init(struct chip48* chip48)
{
  memset(chip48, 0, sizeof(struct chip48));
}
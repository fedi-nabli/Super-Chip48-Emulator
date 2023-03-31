#include "chip48keyboard.h"
#include <assert.h>

static void chip48_keyboard_ensure_in_bounds(int key)
{
  assert(key >= 0 && key < CHIP48_TOTAL_KEYS);
}

int chip48_keyboard_map(const char* map, char key)
{
  for (int i = 0; i < CHIP48_TOTAL_KEYS; i++)
  {
    if (map[i] == key)
    {
      return i;
    }
  }

  return -1;
}

void chip48_keyboard_down(struct chip48_keyboard* keyboard, int key)
{
  keyboard->keyboard[key] = true;
}

void chip48_keyboard_up(struct chip48_keyboard* keyboard, int key)
{
  keyboard->keyboard[key] = false;
}

bool chip48_keyboard_is_down(struct chip48_keyboard* keyboard, int key)
{
  return keyboard->keyboard[key];
}
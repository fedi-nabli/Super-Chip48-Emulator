#include "chip48keyboard.h"
#include <assert.h>

static void chip48_keyboard_ensure_in_bounds(int key)
{
  assert(key >= 0 && key < CHIP48_TOTAL_KEYS);
}

void chip48_keyboard_set_map(struct chip48_keyboard* keyboard, const char* map)
{
  keyboard->keyboard_map = map;
}

int chip48_keyboard_map(struct chip48_keyboard* keyboard, char key)
{
  for (int i = 0; i < CHIP48_TOTAL_KEYS; i++)
  {
    if (keyboard->keyboard_map[i] == key)
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
#include "chip48screen.h"
#include <assert.h>

static void chip48_screen_check_bounds(int x, int y)
{
  assert(x >= 0 && x < CHIP48_WIDTH && y >= 0 && y < CHIP48_HEIGHT);
}

void chip48_screen_set(struct chip48_screen* screen, int x, int y)
{
  chip48_screen_check_bounds(x, y);
  screen->pixels[y][x] = true;
}

bool chip48_screen_is_set(struct chip48_screen* screen, int x, int y)
{
  chip48_screen_check_bounds(x, y);
  return screen->pixels[y][x];
}
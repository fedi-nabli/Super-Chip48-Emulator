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

bool chip48_screen_draw_sprite(struct chip48_screen* screen, int x, int y, const char* sprite, int num)
{
  bool pixel_collision = false;

  for (int ly = 0; ly < num; ly++)
  {
    char c = sprite[ly];
    for (int lx = 0; lx < 8; lx++)
    {
      if((c & (0b10000000 >> lx)) == 0)
        continue;

      if (screen->pixels[(ly+y) % CHIP48_HEIGHT][(lx+x) % CHIP48_WIDTH])
      {
        pixel_collision = true;
      }

      screen->pixels[(ly+y) % CHIP48_HEIGHT][(lx+x) % CHIP48_WIDTH] ^= true;
    }
  }

  return pixel_collision;
}
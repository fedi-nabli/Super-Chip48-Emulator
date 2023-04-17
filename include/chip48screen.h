#ifndef CHIP48SCREEN_H
#define CHIP48SCREEN_H

#include <stdbool.h>
#include "config.h"

struct chip48_screen
{
  bool pixels[CHIP48_HEIGHT][CHIP48_WIDTH];
};

void chip48_screen_clear(struct chip48_screen* screen);
void chip48_screen_set(struct chip48_screen* screen, int x, int y);
bool chip48_screen_is_set(struct chip48_screen* screen, int x, int y);
bool chip48_screen_draw_sprite(struct chip48_screen* screen, int x, int y, const char* sprite, int num);

#endif
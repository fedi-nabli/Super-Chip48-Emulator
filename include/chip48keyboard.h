#ifndef CHIP48KEYBOARD_H
#define CHIP48KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct chip48_keyboard
{
  bool keyboard[CHIP48_TOTAL_KEYS];
};

int chip48_keyboard_map(const char* map, char key);
void chip48_keyboard_down(struct chip48_keyboard* keyboard, int key);
void chip48_keyboard_up(struct chip48_keyboard* keyboard, int key);
bool chip48_keyboard_is_down(struct chip48_keyboard* keyboard, int key);

#endif
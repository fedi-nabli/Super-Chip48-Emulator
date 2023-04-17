#ifndef CHIP48KEYBOARD_H
#define CHIP48KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct chip48_keyboard
{
  bool keyboard[CHIP48_TOTAL_KEYS];
  const char* keyboard_map;
};

void chip48_keyboard_set_map(struct chip48_keyboard* keyboard, const char* map);
int chip48_keyboard_map(struct chip48_keyboard* keyboard, char key);
void chip48_keyboard_down(struct chip48_keyboard* keyboard, int key);
void chip48_keyboard_up(struct chip48_keyboard* keyboard, int key);
bool chip48_keyboard_is_down(struct chip48_keyboard* keyboard, int key);

#endif
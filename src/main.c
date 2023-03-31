#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "chip48.h"
#include "chip48keyboard.h"

const char keyboard_map[CHIP48_TOTAL_KEYS] = {
  SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
  SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
  SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char** argv)
{
  struct chip48 chip48;
  chip48_init(&chip48);

  chip48_screen_draw_sprite(&chip48.screen, 32, 100, &chip48.memory.memory[0x00], 5);

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow(
    EMULATOR_WINDOW_TITLE,
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    CHIP48_WIDTH * CHIP48_WINDOW_MULTIPLIER,
    CHIP48_HEIGHT * CHIP48_WINDOW_MULTIPLIER,
    SDL_WINDOW_SHOWN
  );

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
  while (1)
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          goto out;
        break;

        case SDL_KEYDOWN:
        {
          char key = event.key.keysym.sym;
          int vkey = chip48_keyboard_map(keyboard_map, key);
          if (vkey != -1)
          {
            chip48_keyboard_down(&chip48.keyboard, vkey);
          }
        }
        break;

        case SDL_KEYUP:
        {
          char key = event.key.keysym.sym;
          int vkey = chip48_keyboard_map(keyboard_map, key);
          if (vkey != -1)
          {
            chip48_keyboard_up(&chip48.keyboard, vkey);
          }
        }
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    
    for (int x = 0; x < CHIP48_WIDTH; x++)
    {
      for (int y = 0; y < CHIP48_HEIGHT; y++)
      {
        if (chip48_screen_is_set(&chip48.screen, x, y))
        {
          SDL_Rect r;
          r.x = x * CHIP48_WINDOW_MULTIPLIER;
          r.y = y * CHIP48_WINDOW_MULTIPLIER;
          r.w = CHIP48_WINDOW_MULTIPLIER;
          r.h = CHIP48_WINDOW_MULTIPLIER;
          SDL_RenderFillRect(renderer, &r);
        }
      }
    }

    SDL_RenderPresent(renderer);
  }

out:
  SDL_DestroyWindow(window);
  return 0;
}
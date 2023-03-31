#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip48.h"

int main(int argc, char** argv)
{
  struct chip48 chip48;
  chip48.registers.V[0x0f] = 50;

  chip48_memory_set(&chip48.memory, 0x400, 'Z'); // 0x400 in hexa translates to 1024 in decimat
  printf("%c\n", chip48_memory_get(&chip48.memory, 50)); // To see the Z char printed put the address 1024 instead of 50

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
      if (event.type == SDL_QUIT)
      {
        goto out;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 40;
    r.h = 40;
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
  }

out:
  SDL_DestroyWindow(window);
  return 0;
}
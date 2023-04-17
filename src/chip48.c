#include "chip48.h"
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

const char chip48_default_character_set[] = {
  0xf0, 0x90, 0x90, 0x90, 0xf0,
  0x20, 0x60, 0x20, 0x20, 0x70,
  0xf0, 0x10, 0xf0, 0x80, 0xf0,
  0xf0, 0x10, 0xf0, 0x10, 0xf0,
  0x90, 0x90, 0xf0, 0x10, 0x10,
  0xf0, 0x80, 0xf0, 0x10, 0xf0,
  0xf0, 0x80, 0xf0, 0x90, 0xf0,
  0xf0, 0x10, 0x20, 0x40, 0x40,
  0xf0, 0x90, 0xf0, 0x90, 0xf0,
  0xf0, 0x90, 0xf0, 0x10, 0xf0,
  0xf0, 0x90, 0xf0, 0x90, 0x90,
  0xe0, 0x90, 0xe0, 0x90, 0xe0,
  0xf0, 0x80, 0x80, 0x80, 0xf0,
  0xe0, 0x90, 0x90, 0x90, 0xe0,
  0xf0, 0x80, 0xf0, 0x80, 0xf0,
  0xf0, 0x80, 0xf0, 0x80, 0x80
};

void chip48_init(struct chip48* chip48)
{
  memset(chip48, 0, sizeof(struct chip48));
  memcpy(&chip48->memory.memory, chip48_default_character_set, sizeof(chip48_default_character_set));
}

void chip48_load(struct chip48* chip48, const char* buf, size_t size)
{
  assert(size+CHIP48_PROGRAM_LOAD_ADDRESS < CHIP48_MEMORY_SIZE);
  memcpy(&chip48->memory.memory[CHIP48_PROGRAM_LOAD_ADDRESS], buf, size);
  chip48->registers.PC = CHIP48_PROGRAM_LOAD_ADDRESS;
}

static void chip48_execute_extended_eight(struct chip48* chip48, unsigned short opcode)
{
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char final_four_bits = opcode & 0x000f;
  unsigned short tmp = 0;

  switch (final_four_bits)
  {
    // 8xy0 - LD Vx, Vy, Vx = Vy
    case 0x00:
      chip48->registers.V[x] = chip48->registers.V[y];
    break;

    // 8xy1 - OR Vx, Vy. Performs a bitwise OR on Vx and Vy and stores the result in Vx
    case 0x01:
      chip48->registers.V[x] = chip48->registers.V[x] | chip48->registers.V[y];
    break;

    // 8xy2 - AND Vx, Vy. Performs a bitwise AND on Vx and Vy and stores the result in Vx
    case 0x02:
      chip48->registers.V[x] = chip48->registers.V[x] & chip48->registers.V[y];
    break;

    // 8xy3 - XOR Vx, Vy. Performs a bitwise XOR on Vx and Vy and stores the result in Vx
    case 0x03:
      chip48->registers.V[x] = chip48->registers.V[x] ^ chip48->registers.V[y];
    break;

    // 8xy4 - ADD Vx, Vy. Sets Vx = Vx + Vy, set VF = carry
    case 0x04:
      tmp = chip48->registers.V[x] + chip48->registers.V[y];

      chip48->registers.V[0x0f] = false;
      if (tmp > 0xff)
      {
        chip48->registers.V[0x0f] = true;
      }

      chip48->registers.V[x] = tmp;
    break;

    // 8xy5 - SUB Vx, Vy. Set Vx = Vx - Vy, set VF = Not borrow
    case 0x05:
      chip48->registers.V[0x0f] = false;
      if (chip48->registers.V[x] > chip48->registers.V[y])
      {
        chip48->registers.V[0x0f] = true;
      }
      chip48->registers.V[x] = chip48->registers.V[x] - chip48->registers.V[y];
    break;

    // 8xy6 - SHR Vx {, Vy}
    case 0x06:
      chip48->registers.V[0x0f] = chip48->registers.V[x] & 0x01;
      chip48->registers.V[x] = chip48->registers.V[x] / 2;
    break;

    // 8xy7 - SUBN Vx, Vy
    case 0x07:
      chip48->registers.V[0x0f] = chip48->registers.V[y] > chip48->registers.V[x];
      chip48->registers.V[x] = chip48->registers.V[y] - chip48->registers.V[x];
    break;

    // 8xyE - SHL Vx {, Vy}
    case 0x0E:
      chip48->registers.V[0x0f] = chip48->registers.V[x] & 0b10000000;
      chip48->registers.V[x] = chip48->registers.V[x] * 2;
    break;
  }
}

static char chip48_wait_for_key_press(struct chip48* chip48)
{
  SDL_Event event;

  while (SDL_WaitEvent(&event))
  {
    if (event.type != SDL_KEYDOWN)
      continue;

    char c = event.key.keysym.sym;
    char chip48_key = chip48_keyboard_map(&chip48->keyboard, c);
    if (chip48_key != -1)
    {
      return chip48_key;
    }
  }

  return -1;
}

static void chip48_execute_extended_F(struct chip48* chip48, unsigned short opcode)
{
  unsigned char x = (opcode >> 8) & 0x000f;

  switch (opcode & 0x00ff)
  {
    // fx07 - LD Vx, DT. Set Vx to the delay timer value
    case 0x07:
      chip48->registers.V[x] = chip48->registers.delay_timer;
    break;

    // fx0a - LD Vx, K
    case 0x0A:
    {
      char pressed_key = chip48_wait_for_key_press(chip48);
      chip48->registers.V[x] = pressed_key;
    }
    break;

    // fx15 - LD DT, Vx. Set the delay timer to Vx
    case 0x15:
      chip48->registers.delay_timer = chip48->registers.V[x];
    break;

    // fx18 - LD ST, Vx. Set the sound timer to Vx
    case 0x18:
      chip48->registers.sound_timer = chip48->registers.V[x];
    break;

    // fx1e - ADD I, Vx
    case 0x1e:
      chip48->registers.I += chip48->registers.V[x];
    break;

    // fx29 - LD F, Vx
    case 0x29:
      chip48->registers.I = chip48->registers.V[x] * CHIP48_DEFAULT_SPRITE_HEIGHT;
    break;

    // fx33 - LD,= B, Vx
    case 0x33:
    {
      unsigned char hundreds = chip48->registers.V[x] / 100;
      unsigned char tens = chip48->registers.V[x] / 10 % 10;
      unsigned char units = chip48->registers.V[x] % 10;
      chip48_memory_set(&chip48->memory, chip48->registers.I, hundreds);
      chip48_memory_set(&chip48->memory, chip48->registers.I+1, tens);
      chip48_memory_set(&chip48->memory, chip48->registers.I+2, units); 
    }
    break;

    // fx55 - LD [I], Vx
    case 0x55:
    {
      for (int i = 0; i <= x; i++)
      {
        chip48_memory_set(&chip48->memory, chip48->registers.I+i, chip48->registers.V[i]);
      }
    }
    break;

    // fx65 - LD Vx, [i]
    case 0x65:
    {
      for (int i = 0; i <= x; i++)
      {
        chip48->registers.V[i] = chip48_memory_get(&chip48->memory, chip48->registers.I+i);
      }
    }
    break;
  }
}

static void chip48_execute_extended(struct chip48* chip48, unsigned short opcode)
{
  unsigned short nnn = opcode & 0x0fff;
  unsigned char x = (opcode >> 8) & 0x000f;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char kk = opcode & 0x00ff;
  unsigned char n = opcode & 0x000f;

  switch (opcode & 0xf000)
  {
    // JP addr, 1nnn Jum to location nnn's
    case 0x1000:
      chip48->registers.PC = nnn;
    break;

    // CALL addr, 2nnn Call subroutine at location nnn
    case 0x2000:
      chip48_stack_push(chip48, chip48->registers.PC);
      chip48->registers.PC = nnn;
    break;

    // SE Vx, byte - 3xkk Skip next instruction if Vx=kk
    case 0x3000:
      if (chip48->registers.V[x] == kk)
      {
        chip48->registers.PC += 2;
      }
    break;

    // SNE Vx, byte - 4xkk Skip next instruction if Vx!=kk
    case 0x4000:
      if (chip48->registers.V[x] != kk)
      {
        chip48->registers.PC += 2;
      }
    break;

    // 5xy0 - SE Vx, Vy, skip the next instruction if Vx = Vy
    case 0x5000:
      if (chip48->registers.V[x] == chip48->registers.V[y])
      {
        chip48->registers.PC += 2;
      }
    break;

    // 6xkk - LD Vx, byte - Vx = kk
    case 0x6000:
      chip48->registers.V[x] = kk;
    break;

    // 7xkk - ADD Vx, byte - Set Vx = Vx + kk
    case 0x7000:
      chip48->registers.V[x] += kk;
    break;

    case 0x8000:
      chip48_execute_extended_eight(chip48, opcode);
    break;

    // 9xy0 - SNE Vx, Vy. Skip next instruction if Vx != Vy
    case 0x9000:
      if (chip48->registers.V[x] != chip48->registers.V[y])
      {
        chip48->registers.PC += 2;
      }
    break;

    // Annn - LD I, addr. Sets the I register to nnn
    case 0xA000:
      chip48->registers.I = nnn;
    break;

    // Bnnn - JP V0, addr - Jump to location nnn + V0
    case 0xB000:
      chip48->registers.PC = nnn + chip48->registers.V[0x00];
    break;

    // Cxkk - RND Vx, byte
    case 0xC000:
      srand(clock());
      chip48->registers.V[x] = (rand() % 255) & kk;
    break;

    // Dxyn - DRW Vx, Vy, nibble. Draws sprite to the screen
    case 0xD000:
    {
      const char* sprite = (const char*) &chip48->memory.memory[chip48->registers.I];
      chip48->registers.V[0x0f] = chip48_screen_draw_sprite(&chip48->screen, chip48->registers.V[x], chip48->registers.V[y], sprite, n);
    }
    break;

    // Keyboard operations
    case 0xE000:
    {
      switch (opcode & 0x00ff)
      {
        // Ex9e - SKP Vx, Skip the next instruction if the key with the value of Vx is pressed
        case 0x9e:
          if (chip48_keyboard_is_down(&chip48->keyboard, chip48->registers.V[x]))
          {
            chip48->registers.PC += 2;
          }
        break;
        
        // Exa1 - SKNP Vx, Skip the next instruction if the key with the value of Vx is not pressed
        case 0xa1:
          if (!chip48_keyboard_is_down(&chip48->keyboard, chip48->registers.V[x]))
          {
            chip48->registers.PC += 2;
          }
        break;
      }
    }
    break;

    case 0xF000:
      chip48_execute_extended_F(chip48, opcode);
    break;
  }
}

void chip48_exec(struct chip48* chip48, unsigned short opcode)
{
  switch (opcode)
  {
    // CLS: Clear The Display
    case 0x00E0:
      chip48_screen_clear(&chip48->screen);
    break;

    // RET: Return from subroutine
    case 0x00EE:
      chip48->registers.PC = chip48_stack_pop(chip48);
    break;

    default:
      chip48_execute_extended(chip48, opcode);
  }
}
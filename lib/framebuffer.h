#include <stdint.h>

#define FRAMEBUFFER_WIDTH 160
#define FRAMEBUFFER_HEIGHT 160

#define PALETTE_COUNT 3
#define PALETTE_SIZE 12

__attribute__((aligned(4))) extern uint8_t FRAMEBUFFER0[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT]; 
__attribute__((aligned(4))) extern uint8_t FRAMEBUFFER1[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

extern uint16_t palette[PALETTE_COUNT][PALETTE_SIZE][3];

uint8_t* get_framebuffer(uint8_t index);
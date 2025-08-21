#include <stdint.h>

#define FRAMEBUFFER_WIDTH 160
#define FRAMEBUFFER_HEIGHT 160

__attribute__((aligned(4))) extern uint8_t FRAMEBUFFER0[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT]; 
__attribute__((aligned(4))) extern uint8_t FRAMEBUFFER1[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

uint8_t* get_framebuffer(uint8_t index);
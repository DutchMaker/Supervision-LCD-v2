// ST7796S LCD interface (8080-8) for RP2040

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>
#include "framebuffer.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 320

// LCD SETUP NOTES:
// Ensure the panel's IM pins are strapped for 8080-8:
// IM2=0, IM1=1, IM0=1.
// Also make sure your BACKLIGHT is powered separately.

#define LCD_PIN_D0 0 // D0..D7 must be contiguous
#define LCD_PIN_WR 8
#define LCD_PIN_RD 9
#define LCD_PIN_DC 10
#define LCD_PIN_CS 11
#define LCD_PIN_RST 12

#define LCD_DATA_MASK 0xFFu // GPIO 0..7
#define LCD_WR_MASK (1u << LCD_PIN_WR)
#define LCD_RD_MASK (1u << LCD_PIN_RD)
#define LCD_DC_MASK (1u << LCD_PIN_DC)
#define LCD_CS_MASK (1u << LCD_PIN_CS)
#define LCD_RST_MASK (1u << LCD_PIN_RST)

extern uint8_t sync;

// Init gpoi and run the LCD controller init sequence.
void lcd_init(void);

// Convert RGB to RGB565.
uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);

// Return an RGB565 grayscale color.
uint16_t GRAY(uint8_t v);

// Set a palette entry (0-255) to an RGB565 colour.
void lcd_set_palette(uint8_t index, uint16_t rgb565);

// Fill the entire screen with a single palette color (0-255).
void lcd_fill_screen(uint8_t buffer_index, uint8_t palette_color);

// Render the whole framebuffer to the display.
void lcd_render_framebuffer(uint8_t buffer_index);

#endif // LCD_DRIVER_H

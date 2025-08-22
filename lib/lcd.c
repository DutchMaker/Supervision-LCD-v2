#include "pico/stdlib.h"
#include <stdio.h>
#include "lcd.h"

static uint16_t lcd_palette[256];
uint8_t sync = 0;


/* ===== Color conversion functions ===== */
static inline uint16_t rgb565_pack(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(b & 0xF8) << 8) |
                     ((uint16_t)(g & 0xFC) << 3) |
                     ((uint16_t)(r >> 3)));
}

uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)~rgb565_pack(r, g, b);
}

uint16_t GRAY(uint8_t v)
{
    return (uint16_t)~rgb565_pack(v, v, v);
}


/* ===== LCD control functions ===== */
static inline void wr_pulse(void)
{
    gpio_put(LCD_PIN_WR, 0);
    __asm volatile("nop; nop; nop; nop;");
    gpio_put(LCD_PIN_WR, 1);
}

static inline void bus_write(uint8_t v)
{
    gpio_put_masked(LCD_DATA_MASK, v);
}

static inline void lcd_cmd(uint8_t c)
{
    gpio_put(LCD_PIN_DC, 0);
    bus_write(c);
    wr_pulse();
}

static inline void lcd_data8(uint8_t d)
{
    gpio_put(LCD_PIN_DC, 1);
    bus_write(d);
    wr_pulse();
}

static inline void lcd_data16(uint16_t d)
{
    lcd_data8(d >> 8);
    lcd_data8(d & 0xFF);
}

static inline void lcd_write_pixel(uint16_t c)
{
    bus_write((uint8_t)(c >> 8));
    wr_pulse();
    bus_write((uint8_t)(c & 0xFF));
    wr_pulse();
}

static void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Column address set
    lcd_cmd(0x2A);
    lcd_data16(x0);
    lcd_data16(x1);

    // Row address set
    lcd_cmd(0x2B);
    lcd_data16(y0);
    lcd_data16(y1);

    // Memory write
    lcd_cmd(0x2C);
}


/* ===== Public LCD functions ===== */
void lcd_init(void)
{
    for (int g = 0; g <= 7; ++g)
    {
        gpio_init(g);
        gpio_set_dir(g, GPIO_OUT);
        gpio_put(g, 0);
    }
    
    gpio_init(LCD_PIN_WR);
    gpio_set_dir(LCD_PIN_WR, GPIO_OUT);
    gpio_put(LCD_PIN_WR, 1);
    gpio_init(LCD_PIN_RD);
    gpio_set_dir(LCD_PIN_RD, GPIO_OUT);
    gpio_put(LCD_PIN_RD, 1);
    gpio_init(LCD_PIN_DC);
    gpio_set_dir(LCD_PIN_DC, GPIO_OUT);
    gpio_put(LCD_PIN_DC, 0);
    gpio_init(LCD_PIN_CS);
    gpio_set_dir(LCD_PIN_CS, GPIO_OUT);
    gpio_put(LCD_PIN_CS, 0);
    gpio_init(LCD_PIN_RST);
    gpio_set_dir(LCD_PIN_RST, GPIO_OUT);
    gpio_put(LCD_PIN_RST, 1);

    // Hardware reset
    gpio_put(LCD_PIN_RST, 0);
    sleep_ms(20);
    gpio_put(LCD_PIN_RST, 1);
    sleep_ms(120);

    // Sleep out
    lcd_cmd(0x11);
    sleep_ms(120);

    // Memory Access Control
    lcd_cmd(0x36);
    lcd_data8(0x40);

    // Pixel Format 16-bit
    lcd_cmd(0x3A);
    lcd_data8(0x55);

    // Tearing Effect Line ON
    lcd_cmd(0x35);
    sleep_ms(20);

    // Display ON
    lcd_cmd(0x29);
    sleep_ms(20);

    lcd_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}

void lcd_set_palette(uint8_t index, uint16_t color)
{
    lcd_palette[index] = color;
}

void lcd_set_predefined_palette(uint8_t palette_index)
{
    for (uint8_t i = 0; i < PALETTE_SIZE; ++i)
    {
        uint8_t r = palette[palette_index][i][0];
        uint8_t g = palette[palette_index][i][1];
        uint8_t b = palette[palette_index][i][2];
        lcd_set_palette(i, RGB(r, g, b));
    }
}

void lcd_fill_screen(uint8_t buffer_index, uint8_t palette_color)
{
    uint8_t* buffer = get_framebuffer(buffer_index);
    uint32_t count = (uint32_t)FRAMEBUFFER_WIDTH * (uint32_t)FRAMEBUFFER_HEIGHT;

    while (count--)
    {
        buffer[count] = palette_color;
    }
}

// Render the 160x160 framebuffer to the 320x320 screen.
// Each pixel is doubled in size.
void __inline __not_in_flash("lcd_render_framebuffer") lcd_render_framebuffer(uint8_t buffer_index)
{
    gpio_put(LCD_PIN_DC, 1);

    const uint8_t* buffer = get_framebuffer(buffer_index);
    uint32_t count = (uint32_t)FRAMEBUFFER_WIDTH * (uint32_t)FRAMEBUFFER_HEIGHT;
    uint8_t line = 0;

    while (count--)
    {
        uint16_t color = lcd_palette[buffer[count]];
        lcd_write_pixel(color);
        lcd_write_pixel(color);

        if (count % FRAMEBUFFER_WIDTH == 0) {
            if (line++ == 0) {
                count += FRAMEBUFFER_WIDTH; // Repeat current line
            }
            else {
                line = 0; // Reset line counter
            }
        }
    }

    sync = 0; // Indicates that drawing is done and framebuffer may be flipped.
}
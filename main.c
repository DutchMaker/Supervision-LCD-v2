#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <hardware/vreg.h>
#include <hardware/clocks.h>
#include "lib/lcd.h"
#include "lib/supervision.h"

#define PALETTE 1

uint8_t render_buffer_index = 0;

// LCD rendering loop.
// Runs on CPU core 1, separated from the data capture loop.
void __time_critical_func(render_core)()
{
    lcd_set_predefined_palette(0); // Load palette with intro image colors.

    lcd_init();
    lcd_render_framebuffer(render_buffer_index);

    while (true) {
        if (sync == 1)  // Only render the framebuffer once, when sync=1. 
        {               // lcd_render_framebuffer() sets sync=0 when it's done rendering.
            lcd_render_framebuffer(render_buffer_index);
        }
    }
}

// Capture LCD data from the Supervision.
// See doc file "supervision_tech-kevtris.org-kevin_horton.txt" for details on the Supervision LCD signals.
void __not_in_flash_func(capture_data)()
{
    uint8_t last_clock_state = 0;
    uint8_t last_polarity_state = 1;
    uint8_t field = 0;
    uint8_t *buffer = get_framebuffer(1);

    // Initialize GPIOs to which the Supervision is connected.
    supervision_gpio_init();

    // Load palette with Supervision colors.
    lcd_set_predefined_palette(PALETTE);

    // Wait for a new frame to start.
    while (gpio_get(SV_PIN_FRAME_POLARITY)) {
    }
    while (!gpio_get(SV_PIN_FRAME_POLARITY)) {
    }

    while (true) {
        const uint32_t bus = gpio_get_all();
        const uint8_t clock = (bus >> SV_PIN_PIXEL_CLOCK) & 1;
        const uint8_t polarity = (bus >> SV_PIN_FRAME_POLARITY) & 1;

        if (clock && !last_clock_state) // Pixel clock went from low to high, meaning we must read pixel data from the bus.
        {
            // Each frame is composed of 3 fields of 160x160 pixels.
            // Each time "polarity" changes state, we switch to a new field.
            // The data of 3 fields is combined to form one complete frame.
            // The first field sets the initial framebuffer, the second/third fields add to that data.
            // When the frame is done, each pixel will have a value of 0-3 with 0 being "off" and 3 being "fully on", and lighter shades in between.
            if (field == 0)
            {
                // Set the initial buffer value for this frame.
                *buffer++ = (bus >> SV_PIN_DATA0 & 1);
                *buffer++ = (bus >> SV_PIN_DATA1 & 1);
                *buffer++ = (bus >> SV_PIN_DATA2 & 1);
                *buffer++ = (bus >> SV_PIN_DATA3 & 1);
            }
            else
            {
                // Add to the existing buffer value for this frame.
                *buffer++ += (bus >> SV_PIN_DATA0 & 1);
                *buffer++ += (bus >> SV_PIN_DATA1 & 1);
                *buffer++ += (bus >> SV_PIN_DATA2 & 1);
                *buffer++ += (bus >> SV_PIN_DATA3 & 1);
            }
        }

        if (polarity != last_polarity_state) // Polarity state changed, meaning a new field has started.
        {
            if (field == 2) // We rendered 3 fields, it's time to flip the framebuffer
            {
                if (!sync) // Make sure we are not currently writing data to the LCD.
                {
                    render_buffer_index = !render_buffer_index;
                    sync = 1; // Indicates that the framebuffer is ready and can be rendered.
                }

                field = 0;
            }
            else
            {
                field++;
            }

            buffer = get_framebuffer(!render_buffer_index); // Reset buffer pointer to start of current framebuffer.
        }

        last_clock_state = clock;
        last_polarity_state = polarity;
    }
}

void main()
{
    // Overclock to 240 MHz.
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    sleep_ms(10);
    set_sys_clock_khz(240000, true);
    sleep_ms(10);

    // Start rendering process on other CPU core.
    // This displays the intro screen because it is in the initial framebuffer.
    sleep_ms(50);
    multicore_launch_core1(render_core);
    sleep_ms(1000);

    // Start capturing data from the Supervision.
    capture_data();
}
#include "pico/stdlib.h"
#include "supervision.h"

void supervision_gpio_init(void)
{
    gpio_init(SV_PIN_DATA0);
    gpio_init(SV_PIN_DATA1);
    gpio_init(SV_PIN_DATA2);
    gpio_init(SV_PIN_DATA3);
    gpio_init(SV_PIN_PIXEL_CLOCK);
    gpio_init(SV_PIN_FRAME_POLARITY);

    gpio_set_dir(SV_PIN_DATA0, GPIO_IN);
    gpio_set_dir(SV_PIN_DATA1, GPIO_IN);
    gpio_set_dir(SV_PIN_DATA2, GPIO_IN);
    gpio_set_dir(SV_PIN_DATA3, GPIO_IN);
    gpio_set_dir(SV_PIN_PIXEL_CLOCK, GPIO_IN);
    gpio_set_dir(SV_PIN_FRAME_POLARITY, GPIO_IN);

    gpio_pull_down(SV_PIN_DATA0);
    gpio_pull_down(SV_PIN_DATA1);
    gpio_pull_down(SV_PIN_DATA2);
    gpio_pull_down(SV_PIN_DATA3);
    gpio_pull_down(SV_PIN_PIXEL_CLOCK);
    gpio_pull_down(SV_PIN_FRAME_POLARITY);
}
#include <stdint.h>

#define SV_WIDTH 160
#define SV_HEIGHT 160

#define SV_PIN_DATA0 16
#define SV_PIN_DATA1 17
#define SV_PIN_DATA2 18
#define SV_PIN_DATA3 19
#define SV_PIN_PIXEL_CLOCK 20
#define SV_PIN_FRAME_POLARITY 21

void supervision_gpio_init(void);
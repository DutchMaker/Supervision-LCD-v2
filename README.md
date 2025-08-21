# Watara Supervision 🕹️ IPS LCD mod

Replace the stock, dotmatrix LCD of the Watara Supervision with a modern IPS LCD.  
This is a follow up on my [previous mod project](https://github.com/DutchMaker/Supervision-LCD-v1) which had a lot of issues.

_...images and video pending..._

## Hardware setup

The projects uses a Raspberry Pi Pico (RP2040) to capture the Supervision LCD signals, store them in a framebuffer and then drive an IPS LCD display to render the image.  
I'm using a 320x320 3.92" IPS TFT LCD that has an ST7796S controller interface.

### Pin Connections: Supervision > Pico

| Supervision      | Pico GPIO | Description           |
|------------------|-----------|-----------------------|
| `DATA0`          | `16`      | LCD data line 0       |
| `DATA1`          | `17`      | LCD data line 1       |
| `DATA2`          | `18`      | LCD data line 2       |
| `DATA3`          | `19`      | LCD data line 3       |
| `PIXEL_CLOCK`    | `20`      | Pixel clock signal    |
| `FRAME_POLARITY` | `21`      | Frame polarity signal |

### Pin Connections: TFT LCD > Pico

Ensure the panel's IM pins are strapped for 8080 parallel interface:

- IM0 (pin 13) = 1
- IM1 (pin 14) = 1
- IM2 (pin 15) = 0

Also make sure your BACKLIGHT is powered separately.

| LCD pin   | Pico GPIO | Description                     |
|-----------|-----------|---------------------------------|
| `D0 (19)` | `0`         | Data bit 0                    |
| `D1 (20)` | `1`         | Data bit 1                    |
| `D2 (21)` | `2`         | Data bit 2                    |
| `D3 (22)` | `3`         | Data bit 3                    |
| `D4 (23)` | `4`         | Data bit 4                    |
| `D5 (24)` | `5`         | Data bit 5                    |
| `D6 (25)` | `6`         | Data bit 6                    |
| `D7 (26)` | `7`         | Data bit 7                    |
| `WR (10)` | `8`         | Write enable                  |
| `RD (11)` | `9`         | Read enable                   |
| `DC (9)`  | `10`        | Data/command selection (RS)   |
| `CS (8)`  | `11`        | Chip select                   |
| `RST (7)` | `12`        | Reset                         |

### Supervision LCD connector pinout

```
    1  2  3  4  5  6  7  8  9  10 11 12
    |  |  |  |  |  |  |  |  |  |  |  |
.---|--|--|--|--|--|--|--|--|--|--|--|---.
|   |  |  |  |  |  |  |  |  |  |  |  |   |
|   o  o  o  o  o  o  o  o  o  x  o  x   |
|            <LCD connector>             |
|                                        |
|              | | | | | |               |
|            -             -             |
|            -    C P U    -             |
|            -  (top side) -             |
|            -             -             |
|              | | | | | |               |
|                                        |
|      <controller board connection>     |
`----------------------------------------`
```

**Supervision LCD pins:**

1. Ground
2. Data 0
3. Data 1
4. Data 2
5. Data 3
6. Pixel clock
7. Unused (Line latch)
8. Unused (Frame latch)
9. Frame polarity
10. Unused (Power control)
11. +6V
12. Unused (no idea what it does)

## IDE setup

I have used Visual Studio Code with the official Raspberry Pi Pico extension.

## Acknowledgments

🙏🏻 Many thanks to [xrip](https://github.com/xrip/watara-supervision-lcd) for his idea of using the RP2040!  

🤪 Thanks to everyone who was crazy enough to research Supervision hardware and share their insights.

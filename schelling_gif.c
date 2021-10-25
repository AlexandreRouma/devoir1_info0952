#include "schelling_gif.h"

#include <stdio.h>

#include "schelling.h"
#include "gifenc.h"


static int _pixelSize;

schellingGIF *createGif(Schelling *s, const char *filename, int pixelSize) {
    _pixelSize = pixelSize;

    ge_GIF *gif = ge_new_gif(
        filename,
        s->width * _pixelSize,
        s->height * _pixelSize,
        (uint8_t []) {          /* palette */
            0xFF, 0xFF, 0xFF,   /* 0 -> white */
            0xFF, 0x00, 0x00,   /* 1 -> red */
            0x00, 0x00, 0xFF,   /* 2 -> blue */
            0x00, 0x00, 0x00    /* 3 -> black */
        },
        2,                      /* palette depth == log2(# of colors) */
        1                       /* Play it once */
    );

    return gif;
}

void addFrame(Schelling *s, ge_GIF *gif, uint16_t delay) {
    int ind = 0;
    uint8_t color;
    for (int h = 0; h < s->height; h++) {
        for (int w = 0; w < s->width; w++) {
            switch (s->grid[h][w]) {
                case EMPTY:
                    color = 0;
                    break;

                case RED:
                    color = 1;
                    break;

                case BLUE:
                    color = 2;
                    break;

                default:
                    fprintf(stderr, "Error: Unknown type of cell in (%d, %d).\n", h, w);
                    color = 3;
            }

            for (int i = 0; i < _pixelSize; i++) {
                for (int j = 0; j < _pixelSize; j++)
                    gif->frame[ind + i * s->width * _pixelSize + j] = color;
            }

            ind += _pixelSize;
        }

        ind += (_pixelSize - 1) * _pixelSize * s->width;
    }

    ge_add_frame(gif, delay);
}

void closeGif(ge_GIF *gif) {
    ge_close_gif(gif);
}

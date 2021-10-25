#ifndef SCHELLING_GIF_H
#define SCHELLING_GIF_H

#include "schelling.h"
#include "gifenc.h"


typedef ge_GIF schellingGIF;

schellingGIF *createGif(Schelling *s, const char *filename, int pixelSize);

void addFrame(Schelling *s, ge_GIF *gif, uint16_t delay);

void closeGif(ge_GIF *gif);

#endif

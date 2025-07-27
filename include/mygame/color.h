#ifndef __COLOR_H__
#define __COLOR_H__

#include <stdint.h>

// represents rgba color
// 0-255
struct mygame_color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

extern const struct mygame_color COLOR_WHITE;
extern const struct mygame_color COLOR_BLACK;
extern const struct mygame_color COLOR_RED;
extern const struct mygame_color COLOR_GREEN;
extern const struct mygame_color COLOR_BLUE;

#endif
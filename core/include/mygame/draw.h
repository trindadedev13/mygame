#ifndef __DRAW_H__
#define __DRAW_H__

#include "mygame/color.h"
#include "mygame/image.h"

// represents a rectangle
struct mygame_rect {
  float x;  // cord x
  float y;  // cord h
  float w;  // width
  float h;  // height
};

// clear renderer
void mygame_render_clear();

// draw all elements
void mygame_render_draw();

// set currebt render color
void mygame_render_set_color(struct mygame_color);

// draw a rect (outlined 1 px)
void mygame_render_draw_rect(struct mygame_rect, struct mygame_color);

// draw a rect (outlined thickness px)
void mygame_render_draw_rect_thickness(struct mygame_rect,
                                       struct mygame_color,
                                       int);

// draw a rect (filled]
void mygame_render_fill_rect(struct mygame_rect, struct mygame_color);

// measure rainyhearts font text
bool mygame_render_mensure_rainyhears_text(char*, int*, int*);

// draw rainyhearts font text
bool mygame_render_draw_rainyhears_text(float,
                                        float,
                                        char*,
                                        struct mygame_color);

// render a image (just like mygame_render_set_color)
bool mygame_render_image(enum mygame_image_t);

// check if a rect contains x and y
bool mygame_rect_contains(struct mygame_rect, int, int);

#endif
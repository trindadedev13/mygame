#ifndef __FONT_H__
#define __FONT_H__

#include <SDL3_ttf/SDL_ttf.h>

#define RAINY_HEARTS_FONTSIZE 35

// represents the fonts used in mygame
struct mygame_fonts {
  TTF_Font* rainyhearts;
};

// load all fonts used in mygame
struct mygame_fonts* mygame_fonts_loadall();

// destroys all loaded fonts "instances"
void mygame_fonts_destroy(struct mygame_fonts*);

#endif
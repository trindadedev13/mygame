#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL3/SDL.h>

// represents a window in mygame
struct mygame_window {
  SDL_Window* raw;  // sdl window
  int w;            // window width
  int h;            // window height
};

// initializes window
struct mygame_window* mygame_window_create(char*, int, int);

// cleanup window memory
void mygame_window_destroy(struct mygame_window*);

#endif
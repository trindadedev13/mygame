#ifndef __STATE_H__
#define __STATE_H__

#include <stdbool.h>

#include <SDL3/SDL.h>

#include "mygame/assets.h"
#include "mygame/font.h"
#include "mygame/window.h"

// represents the state of game
struct mygame_state {
  struct mygame_window* window;  // game window
  struct mygame_fonts* fonts;    // contains all fonts used in the game
  struct mygame_images* images;  // contains all images used in the game
  struct mygame_assets_manager*
      assets_manager;  // manage all assets (platform-dependent)

  SDL_Renderer* renderer;  // renderer to render, draw things
};

// global state of all game
extern struct mygame_state* gbl_state;

// initializes global state
bool mygame_state_init(struct mygame_assets_manager*);

// destroys global state
void mygame_state_destroy();

#endif
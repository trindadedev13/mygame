#ifndef __HOME_H__
#define __HOME_H__

#include <SDL3/SDL.h>

#include "mygame/draw.h"

enum mygame_scene_home_option_t {
  MYGAME_SCENE_HOME_OP_START,
  MYGAME_SCENE_HOME_OP_CONTINUE
};

struct mygame_scene_home_option {
  char* title;
  struct mygame_rect rect;
  enum mygame_scene_home_option_t type;
  bool hovered;
};

// initialize needed data for home scene
void mygame_scene_home_create();

// handle sdl events for home menu scene
void mygame_scene_home_handle_events(SDL_Event);

// draw home menu scene scene
void mygame_scene_home_draw();

#endif
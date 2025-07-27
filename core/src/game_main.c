#include "mygame/game_main.h"

#include <stdbool.h>
#include <stdio.h>

#include <SDL3/SDL.h>

#include "mygame/draw.h"
#include "mygame/scene/home.h"
#include "mygame/scene/scene.h"
#include "mygame/state.h"

int mygame_main(int argc, char* argv[]) {
  mygame_scene_home_create();

  SDL_Event e;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        running = false;
      }
      // handle events for current scene
      if (current_scene == MYGAME_SCENE_HOME) {
        mygame_scene_home_handle_events(e);
      }
    }

    mygame_render_clear();

    // draw current scene
    if (current_scene == MYGAME_SCENE_HOME) {
      mygame_scene_home_draw();
    }

    mygame_render_draw();  // draw all
  }

  mygame_state_destroy();

  return 0;
}
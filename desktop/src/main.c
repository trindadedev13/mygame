#include <SDL3/SDL.h>

#include <mygame/game_main.h>
#include <mygame/state.h>

#include "desktop_assets_manager.h"

int main(int argc, char* argv[]) {
  if (!mygame_state_init(desktop_get_assets_manager())) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to initialize global game state\n");
    mygame_state_destroy();
    return 1;
  }
  return mygame_main(argc, argv);
}
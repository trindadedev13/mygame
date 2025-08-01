#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <mygame/assets.h>
#include <mygame/game_main.h>
#include <mygame/state.h>

#include "android_assets_manager_jni.h"

struct mygame_assets_manager* android_get_assets_manager() {
  struct mygame_assets_manager* mgr =
      malloc(sizeof(struct mygame_assets_manager));
  /** these android_* functions are from android_assets_manager_jni.h */
  mgr->read_text_file = android_assets_manager_readtextfile;
  mgr->read_bin_file = android_assets_manager_readbinfile;
  mgr->file_exists = android_assets_manager_file_exists;
  mgr->list_files = android_assets_manager_listfiles;
  mgr->free_file_list = android_assets_manager_freefilelist;
  return mgr;
}

int main(int argc, char* argv[]) {
  if (!mygame_state_init(android_get_assets_manager())) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to initialize global game state\n");
    mygame_state_destroy();
    return 1;
  }
  return mygame_main(argc, argv);
}
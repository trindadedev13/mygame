#include "mygame/font.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "mygame/assets.h"
#include "mygame/state.h"

struct mygame_fonts* mygame_fonts_loadall() {
  struct mygame_fonts* it = malloc(sizeof(struct mygame_fonts));
  if (it == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create mygame_window");
    return NULL;
  }

  it->rainyhearts.bytes =
      gbl_state->assets_manager->read_bin_file("fonts/rainyhearts.ttf");
  SDL_IOStream* rainyhearts_stream =
      SDL_IOFromMem(it->rainyhearts.bytes->data, it->rainyhearts.bytes->size);

  it->rainyhearts.ttf =
      TTF_OpenFontIO(rainyhearts_stream, true, RAINY_HEARTS_FONTSIZE);
  if (it->rainyhearts.ttf == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s\n",
                 SDL_GetError());
    return NULL;
  }

  return it;
}

void mygame_fonts_destroy(struct mygame_fonts* self) {
  if (self == NULL)
    return;
  if (self->rainyhearts.ttf != NULL)
    TTF_CloseFont(self->rainyhearts.ttf);
  if (self->rainyhearts.bytes != NULL)
    int8_array_destroy(self->rainyhearts.bytes);
  free(self);
}
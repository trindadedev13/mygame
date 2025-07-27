#include "mygame/font.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

struct mygame_fonts* mygame_fonts_loadall() {
  struct mygame_fonts* it = malloc(sizeof(struct mygame_fonts));
  if (it == NULL) {
    perror("Failed to create mygame_window");
    return NULL;
  }

  it->rainyhearts =
      TTF_OpenFont("assets/rainyhearts.ttf", RAINY_HEARTS_FONTSIZE);
  if (it->rainyhearts == NULL) {
    fprintf(stderr, "Failed to load font: %s\n", SDL_GetError());
    return NULL;
  }
  return it;
}

void mygame_fonts_destroy(struct mygame_fonts* self) {
  if (self == NULL)
    return;
  if (self->rainyhearts != NULL)
    TTF_CloseFont(self->rainyhearts);
  free(self);
}
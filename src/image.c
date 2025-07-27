#include "mygame/image.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "mygame/state.h"

struct mygame_images* mygame_images_loadall() {
  struct mygame_images* it = malloc(sizeof(struct mygame_images));
  if (it == NULL) {
    perror("Failed to create mygame_window");
    return NULL;
  }

  it->background =
      IMG_LoadTexture(gbl_state->renderer, "assets/background.jpg");
  if (it->background == NULL) {
    fprintf(stderr, "Failed to load background image %s\n", SDL_GetError());
    return NULL;
  }

  return it;
}

SDL_Texture* mygame_images_get_texture_from_type(struct mygame_images* self,
                                                 enum mygame_image_t type) {
  SDL_Texture* texture;
  switch (type) {
    case MYGAME_IMAGE_TYPE_BACKGROUND:
      texture = self->background;
      break;
  };
  return texture;
}

void mygame_images_destroy(struct mygame_images* self) {
  if (self == NULL)
    return;
  if (self->background != NULL)
    SDL_DestroyTexture(self->background);
  free(self);
}
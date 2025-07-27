#include "mygame/image.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "mygame/assets.h"
#include "mygame/state.h"

struct mygame_images* mygame_images_loadall() {
  struct mygame_images* it = malloc(sizeof(struct mygame_images));
  if (it == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create mygame_window");
    return NULL;
  }

  it->background.bytes =
      gbl_state->assets_manager->read_bin_file("images/background.jpg");
  SDL_IOStream* background_stream =
      SDL_IOFromMem(it->background.bytes->data, it->background.bytes->size);

  it->background.texture =
      IMG_LoadTexture_IO(gbl_state->renderer, background_stream, true);
  if (it->background.texture == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to load background image %s\n", SDL_GetError());
    return NULL;
  }

  return it;
}

SDL_Texture* mygame_images_get_texture_from_type(struct mygame_images* self,
                                                 enum mygame_image_t type) {
  SDL_Texture* texture;
  switch (type) {
    case MYGAME_IMAGE_TYPE_BACKGROUND:
      texture = self->background.texture;
      break;
  };
  return texture;
}

void mygame_images_destroy(struct mygame_images* self) {
  if (self == NULL)
    return;
  if (self->background.texture != NULL)
    SDL_DestroyTexture(self->background.texture);
  if (self->background.bytes != NULL)
    int8_array_destroy(self->background.bytes);
  free(self);
}
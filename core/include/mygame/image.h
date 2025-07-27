#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <SDL3/SDL.h>

// all types of images used in mygame
// just to make easy get image texture
enum mygame_image_t { MYGAME_IMAGE_TYPE_BACKGROUND };

// stores all images used in mygame
struct mygame_images {
  struct {
    SDL_Texture* texture;
    struct int8_array_t* bytes;
  } background;
};

// load all images used in mygame
struct mygame_images* mygame_images_loadall();

// returns the texture based on mygame_image_t
SDL_Texture* mygame_images_get_texture_from_type(struct mygame_images*,
                                                 enum mygame_image_t);

// destroy all loaded images "instances"
void mygame_images_destroy(struct mygame_images*);

#endif
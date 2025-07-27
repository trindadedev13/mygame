#include "mygame/state.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "mygame/font.h"
#include "mygame/image.h"
#include "mygame/window.h"

struct mygame_state* gbl_state = NULL;

bool mygame_state_init() {
  // initialize sdl video
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  // initialize sdl ttf
  if (!TTF_Init()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL TTF: %s\n", SDL_GetError());
    return false;
  }

  gbl_state = malloc(sizeof(struct mygame_state));
  if (gbl_state == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Something Wrong happened and State is invalid: %s\n",
            SDL_GetError());
    return false;
  }

  // calculate width and height based on display
  const SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display_id);

  // create window
  gbl_state->window = mygame_window_create("MyGame", mode->w, mode->h);
  if (gbl_state->window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s\n", SDL_GetError());
    return false;
  }

  // create sdl renderer
  gbl_state->renderer = SDL_CreateRenderer(gbl_state->window->raw, NULL);
  if (gbl_state->renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Renderer: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawBlendMode(gbl_state->renderer, SDL_BLENDMODE_BLEND);

  // load all fonts
  gbl_state->fonts = mygame_fonts_loadall();
  if (gbl_state->fonts == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load mygame_fonts: %s\n", SDL_GetError());
    return false;
  }

  // load all images
  gbl_state->images = mygame_images_loadall();
  if (gbl_state->images == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load mygame_images: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void mygame_state_destroy() {
  if (gbl_state != NULL) {
    if (gbl_state->renderer != NULL)
      SDL_DestroyRenderer(gbl_state->renderer);

    if (gbl_state->window != NULL)
      mygame_window_destroy(gbl_state->window);

    if (gbl_state->fonts != NULL)
      mygame_fonts_destroy(gbl_state->fonts);

    if (gbl_state->images != NULL)
      mygame_images_destroy(gbl_state->images);

    free(gbl_state);
  }

  TTF_Quit();
  SDL_Quit();
}
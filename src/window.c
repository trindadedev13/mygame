#include "mygame/window.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

struct mygame_window* mygame_window_create(char* title, int w, int h) {
  if (title == NULL) {
    fprintf(stderr, "Please provide a valid title for window!\n");
    return NULL;
  }

  struct mygame_window* it = malloc(sizeof(struct mygame_window));
  if (it == NULL) {
    perror("Failed to create mygame_window");
    return NULL;
  }

  // create sdl window
  it->raw = SDL_CreateWindow(title, w, h, SDL_WINDOW_RESIZABLE);
  if (it->raw == NULL) {
    fprintf(stderr, "Failed to create SDL_Window: %s\n", SDL_GetError());
    free(it);
    return NULL;
  }
  it->w = w;
  it->h = h;
  return it;
}

void mygame_window_destroy(struct mygame_window* self) {
  if (self == NULL)
    return;
  if (self->raw != NULL)
    SDL_DestroyWindow(self->raw);
  free(self);
}
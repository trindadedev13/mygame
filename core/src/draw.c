#include "mygame/draw.h"

#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "mygame/image.h"
#include "mygame/state.h"

void mygame_render_clear() {
  SDL_RenderClear(gbl_state->renderer);
}

void mygame_render_draw() {
  SDL_RenderPresent(gbl_state->renderer);
}

void mygame_render_set_color(struct mygame_color color) {
  SDL_SetRenderDrawColor(gbl_state->renderer, color.r, color.g, color.b,
                         color.a);
}

void mygame_render_draw_rect(struct mygame_rect rect,
                             struct mygame_color color) {
  mygame_render_set_color(color);
  SDL_RenderRect(gbl_state->renderer, (SDL_FRect*)&rect);
}

void mygame_render_draw_rect_thickness(struct mygame_rect rect,
                                       struct mygame_color color,
                                       int thickness) {
   for (size_t i = 0; i < thickness; i++) {
      struct mygame_rect r = {
         rect.x - i,
         rect.y - i,
         rect.w + 2 * i,
         rect.h + 2 * i
      };
      mygame_render_draw_rect(r, color);   // <- Aqui usamos r, não rect
   }
}


void mygame_render_fill_rect(struct mygame_rect rect,
                             struct mygame_color color) {
  mygame_render_set_color(color);
  SDL_RenderFillRect(gbl_state->renderer, (SDL_FRect*)&rect);
}

bool mygame_render_draw_rainyhears_text(float x,
                                        float y,
                                        char* text,
                                        struct mygame_color text_color) {
  // surface
  SDL_Color c = {.r = text_color.r,
                 .g = text_color.g,
                 .b = text_color.b,
                 .a = text_color.a};
  SDL_Surface* text_surface = TTF_RenderText_Blended(
      gbl_state->fonts->rainyhearts, text, strlen(text), c);
  if (text_surface == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Surface for draw text %s: %s\n", text,
            SDL_GetError());
    return false;
  }

  // texture
  SDL_Texture* text_texture =
      SDL_CreateTextureFromSurface(gbl_state->renderer, text_surface);
  if (text_texture == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Texture for draw text %s: %s\n", text,
            SDL_GetError());
    return false;
  }

  SDL_SetTextureBlendMode(text_texture, SDL_BLENDMODE_BLEND);

  SDL_FRect rect = {x, y, text_surface->w, text_surface->h};
  SDL_RenderTexture(gbl_state->renderer, text_texture, NULL, &rect);

  SDL_DestroyTexture(text_texture);
  SDL_DestroySurface(text_surface);
  return true;
}

bool mygame_render_mensure_rainyhears_text(char* text, int* w, int* h) {
  if (text == NULL || w == NULL || h == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "mensure_rainyhears_text: invalid params.\n");
    return false;
  }

  if (gbl_state == NULL || gbl_state->fonts->rainyhearts == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "mensure_rainyhears_text: not loaded font.\n");
    return false;
  }

  if (!TTF_GetStringSize(gbl_state->fonts->rainyhearts, text, strlen(text), w,
                         h)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to measure text '%s': %s\n", text, SDL_GetError());
    return false;
  }
  return true;
}

bool mygame_render_image(enum mygame_image_t image) {
  SDL_Texture* image_texture =
      mygame_images_get_texture_from_type(gbl_state->images, image);

  if (!SDL_RenderTexture(gbl_state->renderer, image_texture, NULL, NULL)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render texture: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

bool mygame_rect_contains(struct mygame_rect rect, int x, int y) {
  int right = rect.x + rect.w;
  int bottom = rect.y + rect.h;
  return rect.x < right && rect.y < bottom  // check for empty first
         && x >= rect.x && x < right && y >= rect.y && y < bottom;
}
#include "mygame/scene/home.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "mygame/color.h"
#include "mygame/draw.h"
#include "mygame/scene/scene.h"
#include "mygame/state.h"

// 0 = Start game
// 1 = Continue game
static struct mygame_scene_home_option options[2] = {};
static size_t options_len = 2;
static int selected_options_index = -1;

void mygame_scene_home_handle_events(SDL_Event e) {
  if (e.type == SDL_EVENT_MOUSE_MOTION) {
    for (size_t i = 0; i < options_len; ++i) {
      bool hovered =
          mygame_rect_contains(options[i].rect, e.motion.x, e.motion.y);
      options[i].hovered = hovered;
      if (hovered) {
        selected_options_index = i;
      }
    }
  } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    // check selected option
    if (options[selected_options_index].type == MYGAME_SCENE_HOME_OP_START) {
      current_scene = MYGAME_SCENE_START;  // goto start game scene
    } else if (options[selected_options_index].type ==
               MYGAME_SCENE_HOME_OP_CONTINUE) {
      current_scene = MYGAME_SCENE_CONTINUE;  // goto continue game scene
    }
  }
}

void mygame_scene_home_create() {
  options[0] =
      (struct mygame_scene_home_option){.title = "Start game",
                                        .type = MYGAME_SCENE_HOME_OP_START,
                                        .hovered = false};
  options[1] =
      (struct mygame_scene_home_option){.title = "Continue game",
                                        .type = MYGAME_SCENE_HOME_OP_CONTINUE,
                                        .hovered = false};
}

void mygame_scene_home_draw() {
  mygame_render_image(MYGAME_IMAGE_TYPE_BACKGROUND);

  int root_w = 300;
  int root_h = 400;
  struct mygame_rect root_rect = {(gbl_state->window->w - root_w) / 2,
                                  (gbl_state->window->h - root_h) / 2, root_w,
                                  root_h};

  mygame_render_draw_rect(root_rect, COLOR_WHITE);

  float item_w = root_rect.w;
  float item_h = root_rect.h / 4;

  {
    struct mygame_rect it_rect = {root_rect.x, root_rect.y + 0 * item_h, item_w,
                                  item_h};
    int title_w, title_h;
    mygame_render_mensure_rainyhears_text("MyGame", &title_w, &title_h);
    mygame_render_fill_rect(it_rect, COLOR_WHITE);
    float title_x = it_rect.x + (it_rect.w - title_w) / 2;
    float title_y = it_rect.y + (it_rect.h - title_h) / 2;
    mygame_render_draw_rainyhears_text(title_x, title_y, "MyGame", COLOR_BLACK);
  }

  for (size_t i = 1; i < options_len + 1; ++i) {
    struct mygame_rect it_rect = {root_rect.x, root_rect.y + i * item_h, item_w,
                                  item_h};

    char* title = options[i - 1].title;
    options[i - 1].rect = it_rect;

    int text_w, text_h;
    mygame_render_mensure_rainyhears_text(title, &text_w, &text_h);
    float text_x = it_rect.x + (it_rect.w - text_w) / 2;
    float text_y = it_rect.y + (it_rect.h - text_h) / 2;

    if (options[i - 1].hovered) {
      mygame_render_fill_rect(it_rect, COLOR_WHITE);
      mygame_render_draw_rainyhears_text(text_x, text_y, title, COLOR_BLACK);
    } else {
      mygame_render_draw_rainyhears_text(text_x, text_y, title, COLOR_WHITE);
      mygame_render_draw_rect(it_rect, COLOR_WHITE);
    }
  }
}
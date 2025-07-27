#ifndef __SCENE_H__
#define __SCENE_H__

// represent the scene type
enum mygame_scene_t {
  MYGAME_SCENE_HOME,
  MYGAME_SCENE_START,
  MYGAME_SCENE_CONTINUE
};

// current scene being displayed
extern enum mygame_scene_t current_scene;

#endif
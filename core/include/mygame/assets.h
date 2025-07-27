#ifndef __ASSETS_H__
#define __ASSETS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct int8_array_t {
  int8_t* data;
  size_t size;
};

static void int8_array_destroy(struct int8_array_t* self) {
  free(self->data);
  free(self);
}

typedef char* (*mygame_assets_manager_readtextfile)(const char*);
typedef struct int8_array_t* (*mygame_assets_manager_readbinfile)(const char*);
typedef bool (*mygame_assets_manager_file_exists)(const char*);
typedef char** (*mygame_assets_manager_listfiles)(const char*, size_t*);
typedef void (*mygame_assets_manager_freefilelist)(char**, size_t);

// the assets manager of mygame
// its platform-dependent
// each platform should implement it
// you can see examples in xml
struct mygame_assets_manager {
  mygame_assets_manager_readtextfile read_text_file;
  mygame_assets_manager_readbinfile read_bin_file;
  mygame_assets_manager_file_exists file_exists;
  mygame_assets_manager_listfiles list_files;
  mygame_assets_manager_freefilelist free_file_list;
};

#endif
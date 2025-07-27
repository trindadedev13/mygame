#include "desktop_assets_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>

#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
#define strdup _strdup
#else
#include <dirent.h>
#include <unistd.h>
#endif

#include <SDL3/SDL.h>

#include <mygame/assets.h>

#define ROOT_PATH "assets"

char* desktop_assets_manager_readtextfile(const char* filename) {
  size_t path_len = strlen(filename) * 2;
  char path[path_len];
  snprintf(path, sizeof(path), "%s/%s", ROOT_PATH, filename);

  FILE* f = fopen(path, "r");
  if (f == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file: %s\n",
                 path);
    return NULL;
  }

  // get size
  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  rewind(f);

  char* c;
  fread(c, sizeof(char), len, f);
  return c;
}

struct int8_array_t* desktop_assets_manager_readbinfile(const char* filename) {
  size_t path_len = strlen(filename) * 2;
  char path[path_len];
  snprintf(path, sizeof(path), "%s/%s", ROOT_PATH, filename);

  FILE* f = fopen(path, "rb");
  if (f == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file: %s\n",
                 path);
    return NULL;
  }

  // get size
  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  rewind(f);

  int8_t* c = malloc(len);
  fread(c, sizeof(int8_t), len, f);

  struct int8_array_t* a = malloc(sizeof(struct int8_array_t));
  if (a == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to allocate memory for array struct.");
    free(c);
    return NULL;
  }
  a->data = c;
  a->size = len;
  return a;
}

bool desktop_assets_manager_file_exists(const char* filename) {
  size_t path_len = strlen(filename) * 2;
  char path[path_len];
  snprintf(path, sizeof(path), "%s/%s", ROOT_PATH, filename);

  FILE* f = fopen(path, "rb");
  return !(f == NULL);
}

char** desktop_assets_manager_listfiles(const char* filename, size_t* out_len) {
  size_t path_len = strlen(filename) * 2;
  char path[path_len];
  snprintf(path, sizeof(path), "%s/%s", ROOT_PATH, filename);

  char** it_dirs =
      malloc(sizeof(char*) * DESKTOP_FILE_LIST_DIR_INITIAL_CAPACITY);
  if (!it_dirs) {
    return NULL;
  }

  size_t it_len = 0;
  size_t it_capacity = DESKTOP_FILE_LIST_DIR_INITIAL_CAPACITY;

#if defined(_WIN32) || defined(__WIN64)
  WIN32_FIND_DATA findFileData;
  char search_path[1024];
  snprintf(search_path, sizeof(search_path), "%s\\*", path);
  HANDLE hFind = FindFirstFile(search_path, &findFileData);

  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      const char* name = findFileData.cFileName;
      if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        continue;

      if (it_len >= it_capacity) {
        it_capacity *= 2;
        char** new_dirs = realloc(it_dirs, sizeof(char*) * it_capacity);
        if (!new_dirs) {
          break;
        }
        it_dirs = new_dirs;
      }

      it_dirs[it_len++] = strdup(name);
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
  }
#else
  DIR* d = opendir(path);
  struct dirent* dir;

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
        continue;

      if (it_len >= it_capacity) {
        it_capacity *= 2;
        char** new_dirs = realloc(it_dirs, sizeof(char*) * it_capacity);
        if (!new_dirs) {
          break;
        }
        it_dirs = new_dirs;
      }

      it_dirs[it_len++] = strdup(dir->d_name);
    }
    closedir(d);
  }
#endif

  *out_len = it_len;
  return it_dirs;
}

void desktop_assets_manager_freefilelist(char** dirs, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    free(dirs[i]);
  }
  free(dirs);
}

struct mygame_assets_manager* desktop_get_assets_manager() {
  struct mygame_assets_manager* mgr =
      malloc(sizeof(struct mygame_assets_manager));
  mgr->read_text_file = desktop_assets_manager_readtextfile;
  mgr->read_bin_file = desktop_assets_manager_readbinfile;
  mgr->file_exists = desktop_assets_manager_file_exists;
  mgr->list_files = desktop_assets_manager_listfiles;
  mgr->free_file_list = desktop_assets_manager_freefilelist;
  return mgr;
}
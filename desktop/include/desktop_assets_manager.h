#ifndef __DESKTOP_ASSETS_MANAGER_H__
#define __DESKTOP_ASSETS_MANAGER_H__

#include <stdbool.h>
#include <stdio.h>

#include <mygame/assets.h>

#define DESKTOP_FILE_LIST_DIR_INITIAL_CAPACITY 1

// assets manager platform implementation
char* desktop_assets_manager_readtextfile(const char*);
struct int8_array_t* desktop_assets_manager_readbinfile(const char*);
bool desktop_assets_manager_file_exists(const char*);
char** desktop_assets_manager_listfiles(const char*, size_t*);
void desktop_assets_manager_freefilelist(char**, size_t);

struct mygame_assets_manager* desktop_get_assets_manager();

#endif
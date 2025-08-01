#ifndef __ANDROID_ASSETS_MANAGER_TYPES_H__
#define __ANDROID_ASSETS_MANAGER_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <jni.h>

/** for int8_array_t* */
#include <mygame/assets.h>

/**
 * functions types for the assets_manager
 * you can use the macros below to make your own definitions, or just write it
 * manually. its maily created for the struct android_assets_manager nested
 * functions.
 */
typedef char* (*type_assets_manager_readtextfile)(const char*);
typedef struct int8_array_t* (*type_assets_manager_readbinfile)(const char*);
typedef bool (*type_assets_manager_file_exists)(const char*);
typedef char** (*type_assets_manager_listfiles)(const char*, size_t*);
typedef void (*type_assets_manager_freefilelist)(char**, size_t);

/** macros to define the above functions types safely */
#define __READ_TEXT_FILE__(name, filename) char* name(const char* filename)
#define __READ_BIN_FILE__(name, filename) \
  struct int8_array_t* name(const char* filename)
#define __FILE_EXISTS__(name, filename) bool name(const char* filename)
#define __LIST_FILES__(name, dir, out_size) \
  char** name(const char* dir, size_t* out_size)
#define __FREE_FILE_LIST__(name, list, size) void name(char** list, size_t size)

/**
 * jni method signature
 * you can change it to your package/class if you want
 */
#define __JNI_SIGNATURE__(__env, __clazz, __mgr)             \
  JNIEXPORT void JNICALL                                     \
  Java_dev_trindadedev_mygame_MyGameActivity_initAssets( \
      JNIEnv* __env, jclass __clazz, jobject __mgr)

#ifdef __cplusplus
}
#endif

#endif
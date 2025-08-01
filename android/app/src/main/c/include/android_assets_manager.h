#ifndef __ANDROID_ASSETS_MANAGER_H__
#define __ANDROID_ASSETS_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <jni.h>

#include <mygame/assets.h>

struct android_assets_manager {
  jobject o_android_assets_manager;
  jclass c_android_assets_manager;
  JavaVM* jvm;
};

extern struct android_assets_manager* asmgr;

jint JNI_OnLoad(JavaVM*, void*);

JNIEXPORT void JNICALL Java_dev_trindadedev_MyGameActivity_initAssets(JNIEnv*,
                                                                      jclass,
                                                                      jobject);

char* android_assets_manager_readtextfile(const char*);

struct int8_array_t* android_assets_manager_readbinfile(const char*);

bool android_assets_manager_file_exists(const char*);

char** android_assets_manager_listfiles(const char*, size_t*);

void android_assets_manager_freefilelist(char**, size_t);

struct mygame_assets_manager* android_get_assets_manager();

#ifdef __cplusplus
}
#endif

#endif
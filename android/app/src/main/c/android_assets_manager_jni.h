#ifndef __ANDROID_ASSETS_MANAGER_JNI_H__
#define __ANDROID_ASSETS_MANAGER_JNI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <jni.h>

typedef struct {
  jobject o_mygame_assets_manager;
  jclass c_mygame_assets_manager;
  JavaVM* jvm;
} MyGameAssetsManagerJNI;

typedef struct {
  int8_t* data;
  size_t size;
} Int8Array;

extern MyGameAssetsManagerJNI* asmgr;

jint JNI_OnLoad(JavaVM*, void*);

JNIEXPORT void JNICALL
Java_dev_trindadedev_mygame_MyGameActivity_initAssets(JNIEnv*, jclass, jobject);

char* MyGameAssetsManagerJNI_ReadTextFile(const char*);

Int8Array* MyGameAssetsManagerJNI_ReadBinaryFile(const char*);

bool MyGameAssetsManagerJNI_FileExists(const char*);

char** MyGameAssetsManagerJNI_ListFiles(const char*, int*);

void MyGameAssetsManagerJNI_FreeFileList(char**, int);

#ifdef __cplusplus
}
#endif

#endif
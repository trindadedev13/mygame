#ifndef __ANDROID_ASSETS_MANAGER_JNI_H__
#define __ANDROID_ASSETS_MANAGER_JNI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#include <jni.h>

#include "android_assets_manager_types.h"

/** stores the jni things of assets manager */
struct android_assets_manager_jni {
  jobject jobject_aam; /** the java assets manager instance (used to call
                          methods) */
  jclass jclass_aam;   /** the java assets manager instance (used to find/get
                          methods) */
  JavaVM* jvm; /** the current jvm (mainly used to get current JNIEnv*) */
};

/** the global jni assets manager state */
extern struct android_assets_manager_jni* asmgr;

/** called when jni is loaded
 * used to save jvm and instanciate things
 * like int main()
 */
jint JNI_OnLoad(JavaVM*, void*);

// see this macro at android_assets_manager_types.h
__JNI_SIGNATURE__(__env, __clazz, __mgr);

/**
 * the all methods that calls the java methods
 * you can use this methods directly if u prefer, but I recommend you to use
 * android_assets_manager.h btw, the method name is the first parameter of the
 * macro.
 *
 * example:
 *   char* content android_assets_manager_readtextfile("main.txt");
 *
 * but as i said, its better imo to use the android_assets_manager.h
 */
__READ_TEXT_FILE__(android_assets_manager_readtextfile, filename);
__READ_BIN_FILE__(android_assets_manager_readbinfile, filename);
__FILE_EXISTS__(android_assets_manager_file_exists, filename);
__LIST_FILES__(android_assets_manager_listfiles, dir, out_size);
__FREE_FILE_LIST__(android_assets_manager_freefilelist, list, size);

#ifdef __cplusplus
}
#endif

#endif
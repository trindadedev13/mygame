#include "android_assets_manager.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <jni.h>

#include <mygame/assets.h>

struct android_assets_manager* asmgr = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  asmgr = (struct android_assets_manager*)malloc(
      sizeof(struct android_assets_manager));
  if (!asmgr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to allocate memory for android_assets_manager*");
    return JNI_ERR;
  }
  asmgr->jvm = vm;
  asmgr->o_android_assets_manager = NULL;
  asmgr->c_android_assets_manager = NULL;
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_dev_trindadedev_mygame_MyGameActivity_initAssets(JNIEnv* env,
                                               jclass clazz,
                                               jobject mgr) {
  if (!asmgr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to allocate memory for android_assets_manager*");
    return;
  }

  if (asmgr->o_android_assets_manager != NULL)
    (*env)->DeleteGlobalRef(env, asmgr->o_android_assets_manager);

  if (asmgr->c_android_assets_manager != NULL)
    (*env)->DeleteGlobalRef(env, asmgr->c_android_assets_manager);

  asmgr->o_android_assets_manager = (*env)->NewGlobalRef(env, mgr);

  jclass localClass =
      (*env)->GetObjectClass(env, asmgr->o_android_assets_manager);
  asmgr->c_android_assets_manager =
      (jclass)(*env)->NewGlobalRef(env, localClass);
  (*env)->DeleteLocalRef(env, localClass);
}

static JNIEnv* jni_getenv() {
  JavaVM* jvm = asmgr->jvm;
  JNIEnv* env = NULL;
  if ((*jvm)->GetEnv(jvm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    if ((*jvm)->AttachCurrentThread(jvm, &env, NULL) != JNI_OK) {
      return NULL;
    }
  }
  return env;
}

// Reads text file
// Calls MyGameAssetsManager.java#readTextFile with JNI
// The result of it need to be free!
char* android_assets_manager_readtextfile(const char* filepath) {
  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_android_assets_manager, "readTextFile",
                          "(Ljava/lang/String;)Ljava/lang/String;");

  if (jmethod_id == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "JNI readTextFile method id not found");
    return NULL;
  }

  jstring jfilepath = (*env)->NewStringUTF(env, filepath);
  if (jfilepath == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create jstring for jfilepath");
    return NULL;
  }

  jstring jresult = (jstring)(*env)->CallObjectMethod(
      env, asmgr->o_android_assets_manager, jmethod_id, jfilepath);

  (*env)->DeleteLocalRef(env, jfilepath);

  if (jresult == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file");
    return NULL;
  }

  const char* result_cstr = (*env)->GetStringUTFChars(env, jresult, NULL);

  char* result_copy = strdup(result_cstr);

  (*env)->ReleaseStringUTFChars(env, jresult, result_cstr);
  (*env)->DeleteLocalRef(env, jresult);

  return result_copy;
}

struct int8_array_t* android_assets_manager_readbinfile(const char* filepath) {
  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_android_assets_manager,
                          "readBinaryFile", "(Ljava/lang/String;)[B");

  if (jmethod_id == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "JNI readTextFile method id not found");
    return NULL;
  }

  jstring jfilepath = (*env)->NewStringUTF(env, filepath);
  if (jfilepath == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create jstring for jfilepath");
    return NULL;
  }

  jbyteArray jresult = (jbyteArray)(*env)->CallObjectMethod(
      env, asmgr->o_android_assets_manager, jmethod_id, jfilepath);

  (*env)->DeleteLocalRef(env, jfilepath);

  if (jresult == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read file");
    return NULL;
  }

  jsize len = (*env)->GetArrayLength(env, jresult);
  jbyte* bytes = (*env)->GetByteArrayElements(env, jresult, NULL);

  int8_t* cBytes = malloc(len * sizeof(int8_t));
  if (!cBytes) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to allocate bytes for binaru file read");
    return NULL;
  }
  memcpy(cBytes, bytes, len * sizeof(int8_t));

  (*env)->ReleaseByteArrayElements(env, jresult, bytes, JNI_ABORT);
  (*env)->DeleteLocalRef(env, jresult);

  struct int8_array_t* a = malloc(sizeof(struct int8_array_t));
  a->data = cBytes;
  a->size = len;

  return a;
}

// Checks if an file exists
// Calls MyGameAssetsManager.java#fileExists with JNI
bool android_assets_manager_file_exists(const char* filepath) {
  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return false;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_android_assets_manager, "fileExists",
                          "(Ljava/lang/String;)Z");

  if (jmethod_id == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "JNI readTextFile method id not found");
    return false;
  }

  jstring jfilepath = (*env)->NewStringUTF(env, filepath);
  if (jfilepath == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create jstring for jfilepath");
    return false;
  }

  jboolean jresult = (jboolean)(*env)->CallBooleanMethod(
      env, asmgr->o_android_assets_manager, jmethod_id, jfilepath);

  (*env)->DeleteLocalRef(env, jfilepath);

  bool result = jresult == JNI_TRUE;

  return result;
}

// List files
// Returns NULL-terminated array of strings
// To get the quantity, pass a pointer to outCount (can be NULL)
// Calls MyGameAssetsManager.java#listFiles with JNI
char** android_assets_manager_listfiles(const char* filepath,
                                        size_t* outCount) {
  if (outCount)
    *outCount = 0;

  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_android_assets_manager, "listFiles",
                          "(Ljava/lang/String;)[Ljava/lang/String;");

  if (jmethod_id == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "JNI listFiles method id not found");
    return NULL;
  }

  jstring jfilepath = (*env)->NewStringUTF(env, filepath);
  if (jfilepath == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create jstring");
    return NULL;
  }

  jobjectArray jresult = (jobjectArray)(*env)->CallObjectMethod(
      env, asmgr->o_android_assets_manager, jmethod_id, jfilepath);

  (*env)->DeleteLocalRef(env, jfilepath);

  if (jresult == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Java listFiles returned NULL");
    return NULL;
  }

  jsize len = (*env)->GetArrayLength(env, jresult);
  if (len == 0) {
    (*env)->DeleteLocalRef(env, jresult);
    if (outCount)
      *outCount = 0;
    return NULL;
  }

  // allocate array of pointers to c strings
  char** result_copy = (char**)malloc(sizeof(char*) * (len + 1));
  if (!result_copy) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Memory allocation failed");
    (*env)->DeleteLocalRef(env, jresult);
    return NULL;
  }

  for (jsize i = 0; i < len; i++) {
    jstring jstr = (jstring)(*env)->GetObjectArrayElement(env, jresult, i);
    if (jstr == NULL) {
      result_copy[i] = NULL;
      continue;
    }

    const char* utfChars = (*env)->GetStringUTFChars(env, jstr, NULL);
    if (utfChars == NULL) {
      result_copy[i] = NULL;
      (*env)->DeleteLocalRef(env, jstr);
      continue;
    }

    // copy string to allocated C memory
    size_t strLen = strlen(utfChars);
    char* cstr = (char*)malloc(strLen + 1);
    if (cstr == NULL) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Memory allocation failed");
      (*env)->ReleaseStringUTFChars(env, jstr, utfChars);
      (*env)->DeleteLocalRef(env, jstr);

      // free already allocated before returning
      for (jsize j = 0; j < i; j++) {
        free((void*)result_copy[j]);
      }
      free(result_copy);
      (*env)->DeleteLocalRef(env, jresult);
      return NULL;
    }
    strcpy(cstr, utfChars);
    result_copy[i] = cstr;

    (*env)->ReleaseStringUTFChars(env, jstr, utfChars);
    (*env)->DeleteLocalRef(env, jstr);
  }
  result_copy[len] = NULL;  // Null-terminate the array

  (*env)->DeleteLocalRef(env, jresult);

  if (outCount)
    *outCount = len;

  return result_copy;
}

// free the returned array when it is no longer needed
void android_assets_manager_freefilelist(char** list, size_t count) {
  if (!list)
    return;
  for (size_t i = 0; i < count; i++) {
    free((void*)list[i]);
  }
  free(list);
}

struct mygame_assets_manager* android_get_assets_manager() {
  struct mygame_assets_manager* mgr =
      malloc(sizeof(struct mygame_assets_manager));
  mgr->read_text_file = android_assets_manager_readtextfile;
  mgr->read_bin_file = android_assets_manager_readbinfile;
  mgr->file_exists = android_assets_manager_file_exists;
  mgr->list_files = android_assets_manager_listfiles;
  mgr->free_file_list = android_assets_manager_freefilelist;
  return mgr;
}
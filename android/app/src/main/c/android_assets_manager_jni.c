#include "android_assets_manager_jni.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <jni.h>
#include <SDL3/SDL.h>

MyGameAssetsManagerJNI* asmgr = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  asmgr = (MyGameAssetsManagerJNI*)malloc(sizeof(MyGameAssetsManagerJNI));
  if (!asmgr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to allocate memory for MyGameAssetsManagerJNI*");
    return JNI_ERR;
  }
  asmgr->jvm = vm;
  asmgr->o_mygame_assets_manager = NULL;
  asmgr->c_mygame_assets_manager = NULL;
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_dev_trindadedev_mygame_MyGameActivity_initAssets(JNIEnv* env,
                                                  jclass clazz,
                                                  jobject mgr) {
  if (!asmgr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to allocate memory for MyGameAssetsManagerJNI*");
    return;
  }

  if (asmgr->o_mygame_assets_manager != NULL)
    (*env)->DeleteGlobalRef(env, asmgr->o_mygame_assets_manager);

  if (asmgr->c_mygame_assets_manager != NULL)
    (*env)->DeleteGlobalRef(env, asmgr->c_mygame_assets_manager);

  asmgr->o_mygame_assets_manager = (*env)->NewGlobalRef(env, mgr);

  jclass localClass = (*env)->GetObjectClass(env, asmgr->o_mygame_assets_manager);
  asmgr->c_mygame_assets_manager = (jclass)(*env)->NewGlobalRef(env, localClass);
  (*env)->DeleteLocalRef(env, localClass);
}

static JNIEnv* MyGameAssetsManagerJNI_GetEnv() {
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
char* MyGameAssetsManagerJNI_ReadTextFile(const char* filepath) {
  JNIEnv* env = MyGameAssetsManagerJNI_GetEnv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_mygame_assets_manager, "readTextFile",
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
      env, asmgr->o_mygame_assets_manager, jmethod_id, jfilepath);

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

Int8Array* MyGameAssetsManagerJNI_ReadBinaryFile(const char* filepath) {
  JNIEnv* env = MyGameAssetsManagerJNI_GetEnv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_mygame_assets_manager, "readBinaryFile",
                          "(Ljava/lang/String;)[B");

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
      env, asmgr->o_mygame_assets_manager, jmethod_id, jfilepath);

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

  Int8Array* a = malloc(sizeof(Int8Array));
  a->data = cBytes;
  a->size = len;

  return a;
}

// Checks if an file exists
// Calls MyGameAssetsManager.java#fileExists with JNI
bool MyGameAssetsManagerJNI_FileExists(const char* filepath) {
  JNIEnv* env = MyGameAssetsManagerJNI_GetEnv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return false;
  }

  jmethodID jmethod_id = (*env)->GetMethodID(
      env, asmgr->c_mygame_assets_manager, "fileExists", "(Ljava/lang/String;)Z");

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
      env, asmgr->o_mygame_assets_manager, jmethod_id, jfilepath);

  (*env)->DeleteLocalRef(env, jfilepath);

  bool result = jresult == JNI_TRUE;

  return result;
}

// List files
// Returns NULL-terminated array of strings
// To get the quantity, pass a pointer to outCount (can be NULL)
// Calls MyGameAssetsManager.java#listFiles with JNI
char** MyGameAssetsManagerJNI_ListFiles(const char* filepath, int* outCount) {
  if (outCount)
    *outCount = 0;

  JNIEnv* env = MyGameAssetsManagerJNI_GetEnv();
  if (env == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->c_mygame_assets_manager, "listFiles",
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
      env, asmgr->o_mygame_assets_manager, jmethod_id, jfilepath);

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
void MyGameAssetsManagerJNI_FreeFileList(char** list, int count) {
  if (!list)
    return;
  for (int i = 0; i < count; i++) {
    free((void*)list[i]);
  }
  free(list);
}
#include "android_assets_manager_jni.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <jni.h>
#include <SDL3/SDL.h>

/** for int8_array_t* */
#include <mygame/assets.h>

#ifndef LOGE
#define LOGE(fmt, ...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)
#endif

struct android_assets_manager_jni* asmgr = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  asmgr = (struct android_assets_manager_jni*)malloc(
      sizeof(struct android_assets_manager_jni));
  if (!asmgr) {
    LOGE("Failed to allocate memory for android_assets_manager_jni*");
    return JNI_ERR;
  }
  asmgr->jvm = vm;
  asmgr->jobject_aam = NULL;
  asmgr->jclass_aam = NULL;
  return JNI_VERSION_1_6;
}

__JNI_SIGNATURE__(env, clazz, mgr) {
  if (!asmgr) {
    LOGE("Failed to allocate memory for android_assets_manager_jni*");
    return;
  }

  if (asmgr->jobject_aam != NULL)
    (*env)->DeleteGlobalRef(env, asmgr->jobject_aam);

  if (asmgr->jclass_aam != NULL)
    (*env)->DeleteGlobalRef(env, asmgr->jclass_aam);

  asmgr->jobject_aam = (*env)->NewGlobalRef(env, mgr);

  jclass localClass = (*env)->GetObjectClass(env, asmgr->jobject_aam);
  asmgr->jclass_aam = (jclass)(*env)->NewGlobalRef(env, localClass);
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
__READ_TEXT_FILE__(android_assets_manager_readtextfile, filename) {
  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    LOGE("Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->jclass_aam, "readTextFile",
                          "(Ljava/lang/String;)Ljava/lang/String;");

  if (jmethod_id == NULL) {
    LOGE("JNI readTextFile method id not found");
    return NULL;
  }

  jstring jfilename = (*env)->NewStringUTF(env, filename);
  if (jfilename == NULL) {
    LOGE("Failed to create jstring for jfilename");
    return NULL;
  }

  jstring jresult = (jstring)(*env)->CallObjectMethod(env, asmgr->jobject_aam,
                                                      jmethod_id, jfilename);

  (*env)->DeleteLocalRef(env, jfilename);

  if (jresult == NULL) {
    LOGE("Failed to read file");
    return NULL;
  }

  const char* result_cstr = (*env)->GetStringUTFChars(env, jresult, NULL);

  char* result_copy = strdup(result_cstr);

  (*env)->ReleaseStringUTFChars(env, jresult, result_cstr);
  (*env)->DeleteLocalRef(env, jresult);

  return result_copy;
}

__READ_BIN_FILE__(android_assets_manager_readbinfile, filename) {
  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    LOGE("Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id = (*env)->GetMethodID(
      env, asmgr->jclass_aam, "readBinaryFile", "(Ljava/lang/String;)[B");

  if (jmethod_id == NULL) {
    LOGE("JNI readTextFile method id not found");
    return NULL;
  }

  jstring jfilename = (*env)->NewStringUTF(env, filename);
  if (jfilename == NULL) {
    LOGE("Failed to create jstring for jfilename");
    return NULL;
  }

  jbyteArray jresult = (jbyteArray)(*env)->CallObjectMethod(
      env, asmgr->jobject_aam, jmethod_id, jfilename);

  (*env)->DeleteLocalRef(env, jfilename);

  if (jresult == NULL) {
    LOGE("Failed to read file");
    return NULL;
  }

  jsize len = (*env)->GetArrayLength(env, jresult);
  jbyte* bytes = (*env)->GetByteArrayElements(env, jresult, NULL);

  int8_t* c_bytes = malloc(len * sizeof(int8_t));
  if (!c_bytes) {
    LOGE("Failed to allocate bytes for binaru file read");
    return NULL;
  }
  memcpy(c_bytes, bytes, len * sizeof(int8_t));

  (*env)->ReleaseByteArrayElements(env, jresult, bytes, JNI_ABORT);
  (*env)->DeleteLocalRef(env, jresult);

  struct int8_array_t* a = malloc(sizeof(struct int8_array_t));
  a->data = c_bytes;
  a->size = len;

  return a;
}

// Checks if an file exists
// Calls MyGameAssetsManager.java#fileExists with JNI
__FILE_EXISTS__(android_assets_manager_file_exists, filename) {
  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    LOGE("Failed to get JNIEnv*");
    return false;
  }

  jmethodID jmethod_id = (*env)->GetMethodID(
      env, asmgr->jclass_aam, "fileExists", "(Ljava/lang/String;)Z");

  if (jmethod_id == NULL) {
    LOGE("JNI readTextFile method id not found");
    return false;
  }

  jstring jfilename = (*env)->NewStringUTF(env, filename);
  if (jfilename == NULL) {
    LOGE("Failed to create jstring for jfilename");
    return false;
  }

  jboolean jresult = (jboolean)(*env)->CallBooleanMethod(
      env, asmgr->jobject_aam, jmethod_id, jfilename);

  (*env)->DeleteLocalRef(env, jfilename);

  bool result = jresult == JNI_TRUE;

  return result;
}

// List files
// Returns NULL-terminated array of strings
// To get the quantity, pass a pointer to out_size (can be NULL)
// Calls MyGameAssetsManager.java#listFiles with JNI
__LIST_FILES__(android_assets_manager_listfiles, dir, out_size) {
  if (out_size)
    *out_size = 0;

  JNIEnv* env = jni_getenv();
  if (env == NULL) {
    LOGE("Failed to get JNIEnv*");
    return NULL;
  }

  jmethodID jmethod_id =
      (*env)->GetMethodID(env, asmgr->jclass_aam, "listFiles",
                          "(Ljava/lang/String;)[Ljava/lang/String;");

  if (jmethod_id == NULL) {
    LOGE("JNI listFiles method id not found");
    return NULL;
  }

  jstring jdir = (*env)->NewStringUTF(env, dir);
  if (jdir == NULL) {
    LOGE("Failed to create jstring");
    return NULL;
  }

  jobjectArray jresult = (jobjectArray)(*env)->CallObjectMethod(
      env, asmgr->jobject_aam, jmethod_id, jdir);

  (*env)->DeleteLocalRef(env, jdir);

  if (jresult == NULL) {
    LOGE("Java listFiles returned NULL");
    return NULL;
  }

  jsize len = (*env)->GetArrayLength(env, jresult);
  if (len == 0) {
    (*env)->DeleteLocalRef(env, jresult);
    if (out_size)
      *out_size = 0;
    return NULL;
  }

  // allocate array of pointers to c strings
  char** result_copy = (char**)malloc(sizeof(char*) * (len + 1));
  if (!result_copy) {
    LOGE("Memory allocation failed");
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
      LOGE("Memory allocation failed");
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

  if (out_size)
    *out_size = len;

  return result_copy;
}

// free the returned array when it is no longer needed
__FREE_FILE_LIST__(android_assets_manager_freefilelist, list, size) {
  if (!list)
    return;
  for (size_t i = 0; i < size; i++) {
    free((void*)list[i]);
  }
  free(list);
}
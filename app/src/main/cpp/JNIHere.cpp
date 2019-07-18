//
// Created by chenzihao on 2019/7/18.
//


#include <jni.h>
#include <string>

#include "simpel_cube.cpp"
#include ".cpp"

#include "gltools.cpp"
using  namespace gltools;


extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(init)(JNIEnv *env, jclass type, jint width, jint height) {
    setupGraphics(width, height);
}

extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(step)(JNIEnv *env, jclass type) {
    renderFrame();
}



extern "C"
JNIEXPORT jstring JNICALL
SHAPEJAVAPATH(getTestString)(JNIEnv *env, jclass type) {
    std::string hello = "hello world";
    return env->NewStringUTF(hello.c_str());
}

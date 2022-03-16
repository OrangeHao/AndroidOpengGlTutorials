

#include <jni.h>
#include "SimpleTriangle.h"



SimpleTriangle* simpleTriangle;


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
SHAPEJAVAPATH(init)(JNIEnv *env, jclass type, jint width, jint height) {
    simpleTriangle=new SimpleTriangle();
    simpleTriangle->init();
}


JNIEXPORT void JNICALL
SHAPEJAVAPATH(step)(JNIEnv *env, jclass type) {
    simpleTriangle->drawFrame();
}



#ifdef __cplusplus
}
#endif
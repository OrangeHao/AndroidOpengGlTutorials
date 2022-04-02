

#include <jni.h>
#include "SimpleTriangle.h"
#include "CourseManager.h"
#include "BaseCourse.h"
#include "LogUtil.h"


#define NATIVE_CLASS_NAME "com/orange/androidopenggltutorials/render/shape/NativeLib"

BaseCourse* simpleTriangle;


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
native_setCourseById(JNIEnv *env, jobject instance,jint course_type){
    CourseManager::getInstance()->setCourseById(course_type);
}

JNIEXPORT void JNICALL
native_initCourse(JNIEnv *env, jobject instance,jint width,jint height){
    CourseManager::getInstance()->initCourse(width,height);
}

JNIEXPORT void JNICALL
native_drawCourseFrame(JNIEnv *env, jobject instance){
    CourseManager::getInstance()->drawFrame();
}


//JNIEXPORT void JNICALL
//SHAPEJAVAPATH(init)(JNIEnv *env, jclass type, jint width, jint height) {
////    simpleTriangle=new SimpleTriangle();
////    simpleTriangle->init();
//    CourseManager::getInstance()->initCourse();
//}
//
//
//JNIEXPORT void JNICALL
//SHAPEJAVAPATH(step)(JNIEnv *env, jclass type) {
////    simpleTriangle->drawFrame();
//    CourseManager::getInstance()->drawFrame();
//}



#ifdef __cplusplus
}
#endif


static int RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum)
{
    LOGE("RegisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGE("RegisterNativeMethods fail. clazz == NULL");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0)
    {
        LOGE("RegisterNativeMethods fail");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnregisterNativeMethods(JNIEnv *env, const char *className)
{
    LOGE("UnregisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGE("UnregisterNativeMethods fail. clazz == NULL");
        return;
    }
    if (env != NULL)
    {
        env->UnregisterNatives(clazz);
    }
}

static JNINativeMethod courseMethods[]={
        {"native_setCourseById",    "(I)V",     (void *)(native_setCourseById)},
        {"native_initCourse",       "(II)V",      (void *)(native_initCourse)},
        {"native_drawCourseFrame",  "()V",      (void *)(native_drawCourseFrame)}
};


//重载onLoad方法
extern "C" jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    LOGE("======JNI_OnLoad=======");
    JNIEnv *env= NULL;
    if (jvm->GetEnv((void **)(&env),JNI_VERSION_1_6)!=JNI_OK){
        return JNI_ERR;
    }

    jint regRet= RegisterNativeMethods(env,NATIVE_CLASS_NAME,courseMethods,sizeof (courseMethods)/
                                                                           sizeof(courseMethods[0]));

    if (regRet!=JNI_TRUE){
        return JNI_ERR;
    }

    LOGE("register methods succeed");
    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    LOGE("======JNI_OnUnload=======");
    JNIEnv *env= NULL;
    if (jvm->GetEnv((void **)(&env),JNI_VERSION_1_6)!=JNI_OK){
        return;
    }

    UnregisterNativeMethods(env,NATIVE_CLASS_NAME);

    LOGE("unregister methods succeed");

}

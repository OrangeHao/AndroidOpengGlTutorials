

#include <jni.h>
#include "CourseManager.h"
#include "BaseCourse.h"
#include "LogUtil.h"
#include "CameraManager.h"


#define NATIVE_CLASS_NAME "com/orange/androidopenggltutorials/render/shape/NativeLib"


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

/****************** camera *********************/

JNIEXPORT void JNICALL
native_camera_Init(JNIEnv *env, jobject instance)
{
    CameraManager::getInstance()->init();
}


JNIEXPORT void JNICALL
native_camera_OnSurfaceChanged(JNIEnv *env, jobject instance, jint width, jint height)
{
    CameraManager::getInstance()->onSurfaceChanged(width,height);
}

JNIEXPORT void JNICALL
native_camera_UpdateFrame(JNIEnv *env, jobject instance, jint format, jbyteArray bytes, jint width, jint height)
{
    int len=env->GetArrayLength(bytes);
    unsigned char* buf=new unsigned char [len];
    env->GetByteArrayRegion(bytes,0,len,reinterpret_cast<jbyte*>(buf));

    CameraManager::getInstance()->UpdateFrame(format,buf,width,height);
    delete[] buf;
}


JNIEXPORT void JNICALL
native_camera_OnDrawFrame(JNIEnv *env, jobject instance)
{
    CameraManager::getInstance()->onDrawFrame();
}

/****************** camera *********************/




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
        {"native_setCourseById",                "(I)V",          (void *)(native_setCourseById)},
        {"native_initCourse",                   "(II)V",         (void *)(native_initCourse)},
        {"native_drawCourseFrame",              "()V",           (void *)(native_drawCourseFrame)},
        {"native_camera_Init",                  "()V",           (void *)(native_camera_Init)},
        {"native_camera_OnSurfaceChanged",     "(II)V",         (void *)(native_camera_OnSurfaceChanged)},
        {"native_camera_UpdateFrame",          "(I[BII)V",      (void *)(native_camera_UpdateFrame)},
        {"native_camera_OnDrawFrame",          "()V",           (void *)(native_camera_OnDrawFrame)}
};


//重载onLoad方法
extern "C" jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    LOGD("======JNI_OnLoad=======");
    JNIEnv *env= NULL;
    if (jvm->GetEnv((void **)(&env),JNI_VERSION_1_6)!=JNI_OK){
        return JNI_ERR;
    }

    jint regRet= RegisterNativeMethods(env,NATIVE_CLASS_NAME,courseMethods,sizeof (courseMethods)/
                                                                           sizeof(courseMethods[0]));

    if (regRet!=JNI_TRUE){
        return JNI_ERR;
    }

    LOGD("register methods succeed");
    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    LOGD("======JNI_OnUnload=======");
    JNIEnv *env= NULL;
    if (jvm->GetEnv((void **)(&env),JNI_VERSION_1_6)!=JNI_OK){
        return;
    }

    UnregisterNativeMethods(env,NATIVE_CLASS_NAME);

    LOGD("unregister methods succeed");

}

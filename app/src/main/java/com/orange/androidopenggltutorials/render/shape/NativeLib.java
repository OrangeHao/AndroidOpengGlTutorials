package com.orange.androidopenggltutorials.render.shape;

/**
 * created by czh on 2019/6/24
 */
public class NativeLib {

    static {
        System.loadLibrary("opengl_native");
    }


    public static final int NATIVE_COURSE_ID_SIMPLE_TRIANGLE=1;


    public static native void native_setCourseById(int courseId);
    public static native void native_initCourse(int width,int height);
    public static native void native_drawCourseFrame();

}

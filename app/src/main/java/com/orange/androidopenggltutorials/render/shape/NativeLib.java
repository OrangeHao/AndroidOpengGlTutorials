package com.orange.androidopenggltutorials.render.shape;

/**
 * created by czh on 2019/6/24
 */
public class NativeLib {

    static {
        System.loadLibrary("shape");
    }


    public static native void init(int width,int height);
    public static native void step();


    public static native void simpleCubeInit(int width,int height);
    public static native void simpleCubeRender();


    public static native void textureCubeInit(int width,int height);
    public static native void textureCubeRender();

    public static native void lightingCubeInit(int width,int height);
    public static native void lightingCubeRender();

}

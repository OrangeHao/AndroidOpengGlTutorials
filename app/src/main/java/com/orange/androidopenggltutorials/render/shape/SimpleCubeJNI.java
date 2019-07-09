package com.orange.androidopenggltutorials.render.shape;


import com.orange.androidopenggltutorials.render.BaseRender;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * created by czh on 2019/7/3
 */
public class SimpleCubeJNI extends BaseRender {

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeLib.simpleCubeInit(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeLib.simpleCubeRender();
    }
}

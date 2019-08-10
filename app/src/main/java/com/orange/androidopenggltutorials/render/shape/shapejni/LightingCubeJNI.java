package com.orange.androidopenggltutorials.render.shape.shapejni;

import com.orange.androidopenggltutorials.render.BaseRender;
import com.orange.androidopenggltutorials.render.shape.NativeLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * created by czh on 2019/8/9
 */
public class LightingCubeJNI extends BaseRender {


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeLib.lightingCubeInit(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeLib.lightingCubeRender();
    }


}
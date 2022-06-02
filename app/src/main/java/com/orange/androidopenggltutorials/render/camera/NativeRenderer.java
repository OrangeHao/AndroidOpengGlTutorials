package com.orange.androidopenggltutorials.render.camera;

import android.opengl.GLES31;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.orange.androidopenggltutorials.render.shape.NativeLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * created by czh on 2022.5.9
 */
class NativeRenderer implements GLSurfaceView.Renderer {

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    private GLSurfaceView mGLSurfaceView;



    public NativeRenderer(){

    }

    public void init(GLSurfaceView glSurfaceView){
        mGLSurfaceView=glSurfaceView;
        mGLSurfaceView.setEGLContextClientVersion(3);
        mGLSurfaceView.setRenderer(this);
        mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    public void requestRender() {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.requestRender();
        }
    }


    /**
     * 传入数据
     * @param format 数据格式
     * @param data 数据数组
     * @param width 图像宽度
     * @param height 图像长度
     */
    public void setRenderFrame(int format, byte[] data, int width, int height) {
        Log.d("czh", "setRenderFrame() called with: data = [" + data.length + "], width = [" + width + "], height = [" + height + "]");
        NativeLib.native_camera_UpdateFrame(format,data,width,height);
    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES31.glClearColor(0.5f,0.5f,0.5f,1.0f);
        NativeLib.native_camera_Init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
//        GLES31.glViewport(0,0,width,height);
        NativeLib.native_camera_OnSurfaceChanged(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
//        GLES31.glClear(GLES31.GL_COLOR_BUFFER_BIT| GLES31.GL_DEPTH_BUFFER_BIT);
        NativeLib.native_camera_OnDrawFrame();
    }

}

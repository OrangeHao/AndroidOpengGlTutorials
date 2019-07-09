package com.orange.androidopenggltutorials.render.shape;

import android.opengl.GLES31;
import android.util.Log;
import com.orange.androidopenggltutorials.render.BaseRender;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * created by czh on 2019/6/15
 */
public class ShapeWrapperRender extends BaseRender {


    private BaseRender mShapeRender;
    private Class<? extends BaseRender> mClazz=null;


    public ShapeWrapperRender(Class<? extends BaseRender> clazz){
        setShapeRenderClass(clazz);
    }


    public void setShapeRenderClass(Class<? extends BaseRender> clazz){
        mClazz=clazz;
    }


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        GLES31.glClearColor(0.5f,0.5f,0.5f,1.0f);

        if(mClazz==null){
            return;
        }

        try {
            Constructor constructor=mClazz.getDeclaredConstructor();
            constructor.setAccessible(true);
            mShapeRender=(BaseRender) constructor.newInstance();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        if (mShapeRender==null){
            mShapeRender=new Triangle();
        }
        Log.d("czh","onSurfaceCreated");
        mShapeRender.onSurfaceCreated(gl10,eglConfig);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        GLES31.glViewport(0,0,width,height);
        if (mShapeRender==null){
            return;
        }
        Log.d("czh","onSurfaceChanged");
        mShapeRender.onSurfaceChanged(gl10,width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        GLES31.glClear(GLES31.GL_COLOR_BUFFER_BIT| GLES31.GL_DEPTH_BUFFER_BIT);
        if (mShapeRender==null){
            return;
        }
        Log.d("czh","onDrawFrame");
        mShapeRender.onDrawFrame(gl10);
    }
}

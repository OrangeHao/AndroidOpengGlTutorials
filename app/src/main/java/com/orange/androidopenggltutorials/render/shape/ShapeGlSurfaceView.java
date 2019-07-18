package com.orange.androidopenggltutorials.render.shape;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import com.orange.androidopenggltutorials.render.BaseRender;

/**
 * created by czh on 2019/6/19
 */
public class ShapeGlSurfaceView extends GLSurfaceView {

    private ShapeWrapperRender mRender=null;

    public ShapeGlSurfaceView(Context context) {
        super(context);
        init();
    }

    public ShapeGlSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init(){
        setEGLContextClientVersion(3);
        mRender=new ShapeWrapperRender(Triangle.class);
        setRenderer(mRender);
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    public void setShape(Class<? extends BaseRender> clazz){
        mRender.setShapeRenderClass(clazz);
    }
}

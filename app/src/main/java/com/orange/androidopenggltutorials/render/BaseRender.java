package com.orange.androidopenggltutorials.render;

import android.opengl.GLES31;
import android.opengl.GLSurfaceView;

/**
 * created by czh on 2019/6/15
 */
public abstract class BaseRender implements GLSurfaceView.Renderer {



    public int loadShader(int type, String shaderCode) {
        //创建shader着色器
        int shader= GLES31.glCreateShader(type);
        GLES31.glShaderSource(shader,shaderCode);
        GLES31.glCompileShader(shader);
        return shader;
    }
}

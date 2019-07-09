package com.orange.androidopenggltutorials.render.shape;

import android.opengl.GLES31;
import com.orange.androidopenggltutorials.render.BaseRender;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * created by czh on 2019/6/17
 */
public class Triangle extends BaseRender {


    //定点着色器代码
    private final String vertexShaderCode=
            "attribute vec4 vPositon;" +
            "void main() {" +
            "    gl_Position=vPositon;" +
            "}";

    //片元着色器代码
    private final String fragmentShaderCode=
            "precision mediump float;" +
            "uniform vec4 vColor;" +
            "void main() {" +
            "    gl_FragColor=vColor;" +
            "}";


    //三角形坐标zyz，右手坐标系,数值表示百分比0~1
    private float triangleCoords[]={
            0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    //颜色rgba 0~1表示0~255
    private float color[]={0.0f,0.74f,1.0f,1.0f};

    public Triangle(){

    }

    //创建gl程序
    private int getProgram(){
        int vertexShader=loadShader(GLES31.GL_VERTEX_SHADER,vertexShaderCode);
        int fragmentShader=loadShader(GLES31.GL_FRAGMENT_SHADER,fragmentShaderCode);

        int program= GLES31.glCreateProgram();
        GLES31.glAttachShader(program,vertexShader);
        GLES31.glAttachShader(program,fragmentShader);
        GLES31.glLinkProgram(program);
        return program;
    }


    //获取坐标点的buffer
    private FloatBuffer getCoordsBuffer(){
        ByteBuffer bb= ByteBuffer.allocateDirect(triangleCoords.length*4);
        bb.order(ByteOrder.nativeOrder());

        FloatBuffer fb=bb.asFloatBuffer();
        fb.put(triangleCoords);
        fb.position(0);
        return fb;
    }


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {

    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        int program=getProgram();
        GLES31.glUseProgram(program);

        //顶点着色器
        int positionHandle= GLES31.glGetAttribLocation(program,"vPositon");
        GLES31.glEnableVertexAttribArray(positionHandle);
        //坐标3个值，float每个值4个byte
        GLES31.glVertexAttribPointer(positionHandle,3,
                GLES31.GL_FLOAT,false,
                3*4,getCoordsBuffer());


        //片元着色器
        int colorHandler= GLES31.glGetUniformLocation(program,"vColor");
        GLES31.glUniform4fv(colorHandler,1,color,0);
        GLES31.glDrawArrays(GLES31.GL_TRIANGLES,0,3);

        GLES31.glDisableVertexAttribArray(positionHandle);

    }



}

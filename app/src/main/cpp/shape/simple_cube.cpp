//
// Created by chenzihao on 2019/6/26.
//

#include <jni.h>
#include <string>


#include <android/log.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "matrix.cpp"

#include "gltools.h"


const char glVertextShader[]=
    "attribute vec4 vertexPosition;\n"
    "attribute vec3 vertexColour;\n"
    "varying vec3 fragColour;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 modeView;\n"
    "\n"
    "void main() {\n"
    "    gl_Position=projection*modeView*vertexPosition;\n"
    "    fragColour=vertexColour;\n"
    "}";

const char glFragmentShader[]=
    "precision mediump float;\n"
    "varying vec3 fragColour;\n"
    "void main() {\n"
    "    gl_FragColor=vec4(fragColour,1.0);\n"
    "}";


//定义了8个点
GLfloat cubeVertices[]={
        -1.0f,1.0f,1.0f,    //正面左上0
        -1.0f,-1.0f,1.0f,   //正面左下1
        1.0f,-1.0f,1.0f,    //正面右下2
        1.0f,1.0f,1.0f,     //正面右上3
        -1.0f,1.0f,-1.0f,    //反面左上4
        -1.0f,-1.0f,-1.0f,   //反面左下5
        1.0f,-1.0f,-1.0f,    //反面右下6
        1.0f,1.0f,-1.0f,     //反面右上7
};

//一个面由2个三角形组成，一个三角形3个点
GLushort indices[]={
        6,7,4,6,4,5,    //后面
        6,3,7,6,2,3,    //右面
        6,5,1,6,1,2,    //下面
        0,3,2,0,2,1,    //正面
        0,1,5,0,5,4,    //左面
        0,7,3,0,4,7,    //上面
};

//对应上面8个点的颜色
GLfloat colour[]={
        0.0f,1.0f,0.0f,1.0f,
        0.0f,1.0f,0.0f,1.0f,
        0.0f,1.0f,0.0f,1.0f,
        0.0f,1.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f,1.0f,
};

GLuint simpleCubeProgram;

GLint vertexLocation;
GLint vertexColourInput;

GLint projectionLocation;
GLint modelViewLocation;

float projectionMatrix[16];
float modelViewMatrix[16];
float angle = 0;


bool setupCube(int width,int heigth){
    LOGE("setupCube");
    simpleCubeProgram=createProgram(glVertextShader,glFragmentShader);

    if(simpleCubeProgram==0){
        LOGE("could not create program");
        return false;
    }

    vertexLocation=glGetAttribLocation(simpleCubeProgram,"vertexPosition");
    vertexColourInput=glGetAttribLocation(simpleCubeProgram,"vertexColour");

    projectionLocation=glGetUniformLocation(simpleCubeProgram,"projection");
    modelViewLocation=glGetUniformLocation(simpleCubeProgram,"modeView");

    /* Setup the perspective */
    //near和far是表示视点到远近投影平面的距离
    matrixPerspective(projectionMatrix,45,(float)width/(float)heigth,0.1f,100);

    glEnable(GL_DEPTH_TEST);

    glViewport(0,0,width,heigth);

    return true;
}

void renderCube(){
    LOGE("renderCube");

//    glClearColor(0.0f,0.0f,0.0f,1.0f);
//    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    //初始化单位矩阵
    matrixIdentityFunction(modelViewMatrix);

    //旋转xy轴
    matrixRotateX(modelViewMatrix,angle);
    matrixRotateY(modelViewMatrix,angle);

    //z轴方向移动
    matrixTranslate(modelViewMatrix,0.0f,0.0f,-10.0f);

    //导入程序到环境中
    glUseProgram(simpleCubeProgram);

    //设置位置数据
    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
    glEnableVertexAttribArray(vertexLocation);

    //设置颜色数据
    glVertexAttribPointer(vertexColourInput,3,GL_FLOAT,GL_FALSE,0,colour);
    glEnableVertexAttribArray(vertexColourInput);

    //设置矩阵数据
    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,projectionMatrix);
    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,modelViewMatrix);

    //36个点
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }

}


extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(simpleCubeInit)(JNIEnv *env, jclass type, jint width,jint height) {
    setupCube(width,height);
}

extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(simpleCubeRender)(JNIEnv *env, jclass type) {
    renderCube();
}

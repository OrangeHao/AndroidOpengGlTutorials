//
// Created by chenzihao on 2019/7/27.
//


#include <jni.h>
#include <string>


#include <android/log.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "matrix.h"
#include "gltools.h"

namespace {
    const char glVertextShader[]=
            "attribute vec4 vertexPosition;\n"
            "attribute vec3 vertexColour;\n"
            "\n"
            "//法线\n"
            "attribute vec3 vertextNormals;\n"
            "\n"
            "varying vec3 fragColour;\n"
            "uniform mat4 projection;\n"
            "uniform mat4 modeView;\n"
            "\n"
            "void main() {\n"
            "\n"
            "    //sence\n"
            "    vec3 transformedVertextNormal=normalize((modeView*vec4(vertextNormals,0.0)).xyz);\n"
            "    vec3 inverseLightDirection=normalize(vec3(0.0,1.0,1.0));\n"
            "    fragColour=vec3(0.0);\n"
            "\n"
            "    //diffuse 散射\n"
            "    vec3 difuseLightIntensity=vec3(1.0,1.0,1.0);\n"
            "    vec3 vertexDiffuseReflectionConstant=vertexColour;\n"
            "    float normalDotLight=max(0.0,dot(transformedVertextNormal,inverseLightDirection));\n"
            "    fragColour+=normalDotLight*vertexDiffuseReflectionConstant*difuseLightIntensity;\n"
            "\n"
            "    //ambient 环境\n"
            "    vec3 ambientLightIntensity=vec3(0.1,0.1,0.1);\n"
            "    vec3 vertextAmbientReflectionConstant=vertexColour;\n"
            "    fragColour+=vertextAmbientReflectionConstant*ambientLightIntensity;\n"
            "\n"
            "    //specular 镜面\n"
            "    vec3 inverseEyeDirection=normalize(vec3(0.0,0.0,1.0));\n"
            "    vec3 specularLightIntensity=vec3(1.0,1.0,1.0);\n"
            "    vec3 vertextSpecularReflectionConstant=vec3(1.0,1.0,1.0);\n"
            "    float shininess=2.0;\n"
            "\n"
            "    vec3 lightReflectionDirection=reflect(vec3(0)-inverseLightDirection,transformedVertextNormal);\n"
            "    float normalDoteReflection=max(0.0,dot(inverseEyeDirection,lightReflectionDirection));\n"
            "\n"
            "    fragColour+=pow(normalDoteReflection,shininess)*vertextSpecularReflectionConstant*specularLightIntensity;\n"
            "\n"
            "    clamp(fragColour,0.0,1.0);\n"
            "\n"
            "    gl_Position=projection*modeView*vertexPosition;\n"
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


    /* [Normals] */
    GLfloat normals[] = { 1.0f,  1.0f, -1.0f, /* Back. */
                          -1.0f,  1.0f, -1.0f,
                          1.0f, -1.0f, -1.0f,
                          -1.0f, -1.0f, -1.0f,
                          0.0f,  0.0f, -1.0f,
                          -1.0f,  1.0f,  1.0f, /* Front. */
                          1.0f,  1.0f,  1.0f,
                          -1.0f, -1.0f,  1.0f,
                          1.0f, -1.0f,  1.0f,
                          0.0f,  0.0f,  1.0f,
                          -1.0f,  1.0f, -1.0f, /* Left. */
                          -1.0f,  1.0f,  1.0f,
                          -1.0f, -1.0f, -1.0f,
                          -1.0f, -1.0f,  1.0f,
                          -1.0f,  0.0f,  0.0f,
                          1.0f,  1.0f,  1.0f, /* Right. */
                          1.0f,  1.0f, -1.0f,
                          1.0f, -1.0f,  1.0f,
                          1.0f, -1.0f, -1.0f,
                          1.0f,  0.0f,  0.0f,
                          -1.0f, -1.0f,  1.0f, /* Bottom. */
                          1.0f, -1.0f,  1.0f,
                          -1.0f, -1.0f, -1.0f,
                          1.0f, -1.0f, -1.0f,
                          0.0f, -1.0f,  0.0f,
                          -1.0f,  1.0f, -1.0f, /* Top. */
                          1.0f,  1.0f, -1.0f,
                          -1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,
                          0.0f,  1.0f,  0.0f
    };
/* [Normals] */

    GLuint simpleCubeProgram;

    GLint vertexLocation;
    GLint vertexColourInput;

    //法线位置
    GLint vertexNormalLocation;

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

        vertexNormalLocation = glGetAttribLocation(simpleCubeProgram, "vertextNormals");

        projectionLocation=glGetUniformLocation(simpleCubeProgram,"projection");
        modelViewLocation=glGetUniformLocation(simpleCubeProgram,"modeView");

        /* Setup the perspective */
        //near和far是表示视点到远近投影平面的距离
        //45是视锥张角的角度，也就是视角，越小物体越大
        matrixPerspective(projectionMatrix,45,(float)width/(float)heigth,0.5f,100);

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
//        matrixRotateX(modelViewMatrix,angle);
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

        /* [Upload vertex normals.] */
        glVertexAttribPointer(vertexNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, normals);
        glEnableVertexAttribArray(vertexNormalLocation);
        /* [Upload vertex normals.] */

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
}


extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(lightingCubeInit)(JNIEnv *env, jclass type, jint width,jint height) {
    setupCube(width,height);
}

extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(lightingCubeRender)(JNIEnv *env, jclass type) {
    renderCube();
}

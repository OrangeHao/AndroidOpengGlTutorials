//
// Created by chenzihao on 2019/7/23.
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
#include "Texture.cpp"


namespace{

    const char glVertexShader[] =
            "attribute vec4 vertexPosition;\n"
            "attribute vec2 vertexTextureCord;\n"
            "varying vec2 textureCord;\n"
            "uniform mat4 projection;\n"
            "uniform mat4 modeView;\n"
            "\n"
            "void main() {\n"
            "    gl_Position=projection*modeView*vertexPosition;\n"
            "    textureCord=vertexTextureCord;\n"
            "}";

    const char glFragmentShader[] =
            "precision mediump float;\n"
            "uniform sampler2D texture;\n"
            "varying vec2 textureCord;\n"
            "\n"
            "void main() {\n"
            "    gl_FragColor=texture2D(texture,textureCord);\n"
            "}";


    GLuint glProgram;

    GLuint vertexPosition;
    GLuint vertexTextureCord;

    GLint projection;
    GLint modeView;

    GLint texture;

    GLint textureId;

    float projectionMatrix[16];
    float modelViewMatrix[16];
    float angle = 0;

    bool setUpTextureCube(int width, int height) {
        glProgram = createProgram(glVertexShader, glFragmentShader);

        vertexPosition = glGetAttribLocation(glProgram, "vertexPosition");
        vertexTextureCord = glGetAttribLocation(glProgram, "vertexTextureCord");

        projection = glGetUniformLocation(glProgram, "projection");
        modeView = glGetUniformLocation(glProgram, "modeView");
        texture = glGetUniformLocation(glProgram, "texture");

        matrixPerspective(projectionMatrix, 45, (float) width / (float) height, 0.1, 100);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, width, height);

        textureId = loadSimpleTexture();
        return textureId != 0;

    }


//定义了8个点
    GLfloat cubeVertices[] = {
            -1.0f, 1.0f, 1.0f,    //正面左上0
            -1.0f, -1.0f, 1.0f,   //正面左下1
            1.0f, -1.0f, 1.0f,    //正面右下2
            1.0f, 1.0f, 1.0f,     //正面右上3
            -1.0f, 1.0f, -1.0f,    //反面左上4
            -1.0f, -1.0f, -1.0f,   //反面左下5
            1.0f, -1.0f, -1.0f,    //反面右下6
            1.0f, 1.0f, -1.0f,     //反面右上7
    };

//一个面由2个三角形组成，一个三角形3个点
    GLushort indices[] = {
            6, 7, 4, 6, 4, 5,    //后面
            6, 3, 7, 6, 2, 3,    //右面
            6, 5, 1, 6, 1, 2,    //下面
            0, 3, 2, 0, 2, 1,    //正面
            0, 1, 5, 0, 5, 4,    //左面
            0, 7, 3, 0, 4, 7,    //上面
    };


    GLfloat textureCords[] = {1.0f, 1.0f, /* Back. */
                              0.0f, 1.0f,
                              1.0f, 0.0f,
                              0.0f, 0.0f,
                              0.0f, 1.0f, /* Front. */
                              1.0f, 1.0f,
                              0.0f, 0.0f,
                              1.0f, 0.0f,
                              0.0f, 1.0f, /* Left. */
                              0.0f, 0.0f,
                              1.0f, 0.0f,
                              1.0f, 1.0f,
                              1.0f, 1.0f, /* Right. */
                              1.0f, 0.0f,
                              0.0f, 0.0f,
                              0.0f, 1.0f,
                              0.0f, 1.0f, /* Top. */
                              0.0f, 0.0f,
                              1.0f, 0.0f,
                              1.0f, 1.0f,
                              0.0f, 0.0f, /* Bottom. */
                              0.0f, 1.0f,
                              1.0f, 1.0f,
                              1.0f, 0.0f
    };


    void renderTextureCube() {

        glUseProgram(glProgram);

        //set modelview matrix
        matrixIdentityFunction(modelViewMatrix);
        matrixRotateY(modelViewMatrix, angle);
        matrixRotateX(modelViewMatrix, angle);
        matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f);

        //set cube location
        glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);
        glEnableVertexAttribArray(vertexPosition);

        //set texture location
        glVertexAttribPointer(vertexTextureCord, 2, GL_FLOAT, GL_FALSE, 0, textureCords);
        glEnableVertexAttribArray(vertexTextureCord);

        //set matrix
        glUniformMatrix4fv(projection, 1, GL_FALSE, projectionMatrix);
        glUniformMatrix4fv(modeView, 1, GL_FALSE, modelViewMatrix);

        //set sample texture unit
        glUniform1i(texture, 0);

        //draw
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);

        angle += 1;
        if (angle > 360) {
            angle -= 360;
        }
    }
}


extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(textureCubeInit)(JNIEnv *env, jclass type, jint width,
                               jint height) {
    setUpTextureCube(width,height);
}

extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(textureCubeRender)(JNIEnv *env, jclass type) {
    renderTextureCube();
}
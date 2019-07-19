//
// Created by chenzihao on 2019/7/18.
//

#include <jni.h>
#include <string>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <cstdio>
#include <cstdlib>

#include "gltools.h"


const char glVertexShader[] =
    "attribute vec4 vPosition;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vPosition;\n"
    "}\n";


const char glFragmentShader[] =
    "precision mediump float;\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4(0.0, 0.74, 1.0, 1.0);\n"
    "}\n";



GLuint simpleTriangleProgram;
GLuint vPosition;

bool setupGraphics(int w, int h) {
    simpleTriangleProgram = createProgram(glVertexShader, glFragmentShader);
    if (!simpleTriangleProgram) {
        LOGE ("Could not create program");
        return false;
    }
    vPosition = glGetAttribLocation(simpleTriangleProgram, "vPosition");
    glViewport(0, 0, w, h);
    return true;
}

const GLfloat triangleVertices[] = {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
};

void renderFrame() {
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(simpleTriangleProgram);

    //GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, triangleVertices);
    //启用数据
    glEnableVertexAttribArray(vPosition);

    //绘制，偏移，顶点数
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(init)(JNIEnv *env, jclass type, jint width, jint height) {
    setupGraphics(width, height);
}

extern "C"
JNIEXPORT void JNICALL
SHAPEJAVAPATH(step)(JNIEnv *env, jclass type) {
    renderFrame();
}

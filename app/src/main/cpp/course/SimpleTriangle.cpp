
#include "SimpleTriangle.h"
#include "Shader.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

const char glVertexShader[] =
        "#version 300 es\n"
        "layout (location = 0) in vec4 vPosition;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vPosition;\n"
        "}\n";


const char glFragmentShader[] =
        "#version 300 es\n"
        "precision mediump float;                     \n"
        "out vec4 fragColor;\n"
        "void main()\n"
        "{\n"
        "  fragColor = vec4(0.0, 0.74, 1.0, 1.0);\n"
        "}\n";


const GLfloat triangleVertices[] = {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
};

SimpleTriangle::SimpleTriangle() {

}

SimpleTriangle::~SimpleTriangle() {

}


void SimpleTriangle::init(int width,int height) {
    LOGE("SimpleTriangle jni");
    triangleShader= Shader(glVertexShader, glFragmentShader);
}


void SimpleTriangle::drawFrame() {
    LOGE("SimpleTriangle drawFrame");

    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    triangleShader.use();
    //GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, triangleVertices);
    //启用数据
    glEnableVertexAttribArray(0);

    //绘制，偏移，顶点数
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SimpleTriangle::destroy() {

}
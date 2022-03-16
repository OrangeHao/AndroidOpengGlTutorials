
#include "SimpleTriangle.h"
#include "Shader.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

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


const GLfloat triangleVertices[] = {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
};

SimpleTriangle::SimpleTriangle() {

}

SimpleTriangle::~SimpleTriangle() {

}


void SimpleTriangle::init() {
    LOGE("test jni");
    triangleShader= Shader(glVertexShader, glFragmentShader);
    vPosition = glGetAttribLocation(triangleShader.ID, "vPosition");
}


void SimpleTriangle::drawFrame() {
    triangleShader.use();

    //GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, triangleVertices);
    //启用数据
    glEnableVertexAttribArray(vPosition);

    //绘制，偏移，顶点数
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SimpleTriangle::destroy() {

}
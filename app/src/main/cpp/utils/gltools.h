//
// Created by chenzihao on 2019/7/19.
//

#ifndef ANDROIDOPENGGLTUTORIALS_GLTOOLS_H
#define ANDROIDOPENGGLTUTORIALS_GLTOOLS_H


#include <android/log.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cstdlib>
#include "LogUtil.h"

#define SHAPEJAVAPATH(path)Java_com_orange_androidopenggltutorials_render_shape_NativeLib_##path

class GLTools{

public:

    static void CheckGLError(const char *pGLOperation);

    static GLuint loadShader(GLenum shaderType, const char *shaderSource);

    static GLuint createProgram(const char *vertexSource, const char *fragmentSource);
};

#endif //ANDROIDOPENGGLTUTORIALS_GLTOOLS_H

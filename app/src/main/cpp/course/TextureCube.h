
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Shader.h"
#include "BaseCourse.h"
#include "LogUtil.h"


class TextureCube: public BaseCourse{

public:
    TextureCube(){};
    ~TextureCube(){};

    void init(int width,int height) {
        LOGD("TextureCube init");
        char vShader[] =
                "#version 300 es\n"
                "layout (location = 0) in vec4 vertexPosition;\n"
                "layout (location = 1) in vec2 vertexTextureCord;\n"
                "out vec2 textureCord;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modeView;\n"
                "\n"
                "void main() {\n"
                "    gl_Position=projection*modeView*vertexPosition;\n"
                "    textureCord=vertexTextureCord;\n"
                "}";

        char fragShader[] =
                "#version 300 es\n"
                "precision mediump float;"
                "uniform sampler2D simpleTexture;\n"
                "in vec2 textureCord;\n"
                "out vec4 fragColor;\n"
                "\n"
                "void main() {\n"
                "    fragColor=texture(simpleTexture,textureCord);\n"
                "}";

        textureCubeShader=Shader(vShader,fragShader);

        mWidth=width;
        mHeight=height;

        glEnable(GL_DEPTH_TEST);

        //创建纹理
        int textureId = loadSimpleTexture();
        if (textureId!=0){
            return;
        }
    };

    void drawFrame() {
        LOGD("LightingCube drawFrame");
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

        glm::mat4 view = glm::mat4(1.0f);
        view=glm::translate(view,glm::vec3(0.0f,0.0f,-10.0f));
        view=glm::rotate(view,glm::radians(angle),glm::vec3(1.0f, 0.0f, 0.0f));
        view=glm::rotate(view,glm::radians(angle*2),glm::vec3(0.0f, 1.0f, 0.0f));

        //导入程序到环境中
        glUseProgram(textureCubeShader.ID);

        //设置位置数据
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
        glEnableVertexAttribArray(0);


        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,textureCords);
        glEnableVertexAttribArray(1);


        //设置矩阵数据
        //near和far是表示视点到远近投影平面的距离
        //45是视锥张角的角度，也就是视角，越小物体越大
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.5f, 100.0f);
        textureCubeShader.setMat4("projection",projection);
        textureCubeShader.setMat4("modeView",view);

        textureCubeShader.setInt("simpleTexture",0);

        //36个点
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

        angle += 1;
        if (angle > 360)
        {
            angle -= 360;
        }
    };

    void destroy() {
        LOGD("textureCube destroy");
    };

private:

    Shader textureCubeShader;

    float angle = 0;

    int mWidth=0;
    int mHeight=0;


    GLuint loadSimpleTexture()
    {
        /* Texture Object Handle. */
        GLuint textureId;

        /* 3 x 3 Image,  R G B A Channels RAW Format. */
        GLubyte pixels[9 * 4] =
                {
                        18,  140, 171, 255, /* Some Colour Bottom Left. */
                        143, 143, 143, 255, /* Some Colour Bottom Middle. */
                        255, 255, 255, 255, /* Some Colour Bottom Right. */

                        255, 255, 0,   255, /* Yellow Middle Left. */
                        0,   255, 255, 255, /* Some Colour Middle. */
                        255, 0,   255, 255, /* Some Colour Middle Right. */

                        255, 0,   0,   255, /* Red Top Left. */
                        0,   255, 0,   255, /* Green Top Middle. */
                        0,   0,   255, 255, /* Blue Top Right. */
                };
        /* [includeTextureDefinition] */

        /* [placeTextureInMemory] */
        /* Use tightly packed data. */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Generate a texture object. */
        glGenTextures(1, &textureId);

        /* Activate a texture. */
        glActiveTexture(GL_TEXTURE0);

        /* Bind the texture object. */
        glBindTexture(GL_TEXTURE_2D, textureId);

        /* Load the texture. */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 3, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        /* Set the filtering mode. */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return textureId;
    }

};
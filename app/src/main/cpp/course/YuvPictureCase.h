
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <ImageDef.h>
#include "Shader.h"
#include "BaseCourse.h"
#include "LogUtil.h"

class YuvPicture : public BaseCourse {
public:
    YuvPicture() {}

    ~YuvPicture() {};

    void init(int width, int height) {
        char vShaderStr[] =
                "#version 300 es                            \n"
                "layout(location = 0) in vec4 a_position;   \n"
                "layout(location = 1) in vec2 a_texCoord;   \n"
                "out vec2 v_texCoord;                       \n"
                "void main()                                \n"
                "{                                          \n"
                "   gl_Position = a_position;               \n"
                "   v_texCoord = a_texCoord;                \n"
                "}                                          \n";

        char fShaderStr[] =
                "#version 300 es                                     \n"
                "precision mediump float;                            \n"
                "in vec2 v_texCoord;                                 \n"
                "layout(location = 0) out vec4 outColor;             \n"
                "uniform sampler2D y_texture;                        \n"
                "uniform sampler2D uv_texture;                        \n"
                "void main()                                         \n"
                "{                                                   \n"
                "	vec3 yuv;										\n"
                "   yuv.x = texture(y_texture, v_texCoord).r;  	\n"
                "   yuv.y = texture(uv_texture, v_texCoord).a-0.5;	\n"
                "   yuv.z = texture(uv_texture, v_texCoord).r-0.5;	\n"
                "	highp vec3 rgb = mat3( 1,       1,       	1,					\n"
                "               0, 		-0.344, 	1.770,					\n"
                "               1.403,  -0.714,       0) * yuv; 			\n"
                "	outColor = vec4(rgb, 1);						\n"
                "}                                                   \n";


        yuvShader = Shader(vShaderStr, fShaderStr);

        //create textures
        GLuint textureIds[2] = {0};
        glGenTextures(2, textureIds);
        m_yTextureId = textureIds[0];
        m_uvTextureId = textureIds[1];

    };

    void drawFrame() {
        if (yuvShader.ID == GL_NONE || m_yTextureId == GL_NONE || m_uvTextureId == GL_NONE) {
            return;
        }

        //upload Y plane data
        glBindTexture(GL_TEXTURE_2D, m_yTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width, m_RenderImage.height, 0,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);

        //update UV plane data
        glBindTexture(GL_TEXTURE_2D, m_uvTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_RenderImage.width >> 1,
                     m_RenderImage.height >> 1, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
                     m_RenderImage.ppPlane[1]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);

    };

    void destroy() {

    };


private:
    Shader yuvShader;

    GLuint m_yTextureId;
    GLuint m_uvTextureId;

    NativeImage m_RenderImage;
};
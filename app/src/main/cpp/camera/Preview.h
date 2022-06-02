
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Shader.h"
#include "BaseCourse.h"
#include "LogUtil.h"
#include "ImageDef.h"


static const float VERTICES_COORS[] =
        {
                -1.f, 1.f,
                -1.f, -1.f,
                1.f, 1.f,
                1.f, -1.f
        };

static const float TEXTURE_COORS[] =
        {
                0, 0,
                0, 1,
                1, 0,
                1, 1
        };

const char kVertexShader[] =
        "#version 100\n"
        "varying vec2 v_texcoord;\n"
        "attribute vec4 position;\n"
        "attribute vec2 texcoord;\n"
        "void main() {\n"
        "    v_texcoord = texcoord;\n"
        "    gl_Position = position;\n"
        "}";

// Pixel shader, YUV420 to RGB conversion.
const char kFragmentShader0[] =
        "#version 100\n \
                precision highp float; \
                varying vec2 v_texcoord;\
                uniform lowp sampler2D s_textureY;\
                uniform lowp sampler2D s_textureU;\
                uniform lowp sampler2D s_textureV;\
                void main() {\
                    float y, u, v, r, g, b;\
                    y = texture2D(s_textureY, v_texcoord).r;\
                    u = texture2D(s_textureU, v_texcoord).r;\
                    v = texture2D(s_textureV, v_texcoord).r;\
                    u = u - 0.5;\
                    v = v - 0.5;\
                    r = y + 1.403 * v;\
                    g = y - 0.344 * u - 0.714 * v;\
                    b = y + 1.770 * u;\
                    gl_FragColor = vec4(r, g, b, 1.0);\
                }";


class Preview{

public:
    Preview(): m_YTextureId(0),
               m_UTextureId(0),
               m_VTextureId(0),
               m_ViewportHeight(0),
               m_ViewportWidth(0),
               m_VertexCoorHandle(0),
               m_TextureCoorHandle(0){
        m_IsProgramChanged= false;
        memset(&m_RenderFrame, 0, sizeof(NativeImage));
    };
    ~Preview(){};



    /**
     * surface create的时候调用
     * @param width
     * @param height
     */
    void init() {
        LOGD("Preview init");

        createProgram();
        glEnable(GL_DEPTH_TEST);
        //创建纹理
    };

    void unInit(){
        LOGD("Preview unInit");
        NativeImageUtil::FreeNativeImage(&m_RenderFrame);
    }

    /**
     * 创建纹理单元
     * @return
     */
    bool createTextures(){
        GLsizei yWidth=static_cast<GLsizei>(m_RenderFrame.width);
        GLsizei yHeight=static_cast<GLsizei>(m_RenderFrame.height);
        //配置纹理0
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1,&m_YTextureId);
        glBindTexture(GL_TEXTURE_2D,m_YTextureId);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,yWidth,yHeight,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,NULL);
        if (!m_YTextureId) {
            GLTools::CheckGLError("GLByteFlowRender::CreateTextures Create Y texture");
            return false;
        }

        //配置纹理1
        GLsizei uWidth=static_cast<GLsizei>(m_RenderFrame.width/2);
        GLsizei uHeight=yHeight/2;
        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &m_UTextureId);
        glBindTexture(GL_TEXTURE_2D, m_UTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, uWidth, uHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                     NULL);
        if (!m_UTextureId) {
            GLTools::CheckGLError("GLByteFlowRender::CreateTextures Create U texture");
            return false;
        }

        //配置纹理2
        GLsizei vWidth=static_cast<GLsizei>(m_RenderFrame.width/2);
        GLsizei vHeight=yHeight/2;
        glActiveTexture(GL_TEXTURE2);
        glGenTextures(1, &m_VTextureId);
        glBindTexture(GL_TEXTURE_2D, m_VTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, vWidth, vHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,NULL);
        if (!m_VTextureId) {
            GLTools::CheckGLError("GLByteFlowRender::CreateTextures Create V texture");
            return false;
        }
        return true;
    }

    /**
     * 处理图像yuv纹理的函数
     * @return
     */
    bool updateTextures(){
        if (m_RenderFrame.ppPlane[0] == NULL) {
            LOGE("m_RenderFrame.ppPlane[0] == NULL");
            return false;
        }

        if (!m_YTextureId && !m_UTextureId && !m_VTextureId && !createTextures()) {
            LOGE("createTextures failed.");
            return false;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_YTextureId);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,
                     (GLsizei)m_RenderFrame.width,(GLsizei)m_RenderFrame.height,
                     0,GL_LUMINANCE,GL_UNSIGNED_BYTE,m_RenderFrame.ppPlane[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_UTextureId);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,
                     (GLsizei)m_RenderFrame.width>>1,(GLsizei)m_RenderFrame.height>>1,
                     0,GL_LUMINANCE,GL_UNSIGNED_BYTE,m_RenderFrame.ppPlane[1]);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,m_VTextureId);
        glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,
                     (GLsizei)m_RenderFrame.width>>1,(GLsizei)m_RenderFrame.height>>1,
                     0,GL_LUMINANCE,GL_UNSIGNED_BYTE,m_RenderFrame.ppPlane[2]);
        return true;
    }

    /**
     * 创建程序
     */
    int createProgram(){
        mPreviewShader=Shader(kVertexShader,kFragmentShader0);
        m_VertexCoorHandle = (GLuint) glGetAttribLocation(mPreviewShader.ID, "position");
        m_TextureCoorHandle = (GLuint) glGetAttribLocation(mPreviewShader.ID, "texcoord");
        return mPreviewShader.ID;
    }


    /**
     * opengl流程相关的函数
     * @return
     */
    GLuint useProgram(){
        if (!mPreviewShader.ID) {
            LOGE("GLByteFlowRender::UseProgram Could not use program.");
            return 0;
        }
        if (m_IsProgramChanged){
            glUseProgram(mPreviewShader.ID);
            GLTools::CheckGLError("GLByteFlowRender::UseProgram");

            glVertexAttribPointer(m_VertexCoorHandle, 2, GL_FLOAT, GL_FALSE, 2 * 4, VERTICES_COORS);
            glEnableVertexAttribArray(m_VertexCoorHandle);

            mPreviewShader.setInt("s_textureY",0);
            mPreviewShader.setInt("s_textureU",1);
            mPreviewShader.setInt("s_textureV",2);

            glVertexAttribPointer(m_TextureCoorHandle, 2, GL_FLOAT, GL_FALSE, 2 * 4, TEXTURE_COORS);
            glEnableVertexAttribArray(m_TextureCoorHandle);

            m_IsProgramChanged = false;
        }

        return mPreviewShader.ID;
    }

    /**
     * 绘制主函数
     */
    void drawFrame() {
        LOGD("preview drawFrame");
        glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);

        if (!updateTextures() || !useProgram()) {
            LOGD("GLByteFlowRender::OnDrawFrame skip frame");
            return;
        }
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    };


    /**
     * 更新帧数据
     * @param pImage
     */
    void updateFrame(NativeImage *pImage){
        if (pImage == nullptr) {
            return;
        }
        if (pImage->width!=m_RenderFrame.width || pImage->height!=m_RenderFrame.height){
            if (m_RenderFrame.ppPlane[0]!=NULL){
                NativeImageUtil::FreeNativeImage(&m_RenderFrame);
            }
            //将m_RenderFrame后面的内存初始化为0
            memset(&m_RenderFrame,0,sizeof(NativeImage));
            m_RenderFrame.width = pImage->width;
            m_RenderFrame.height = pImage->height;
            m_RenderFrame.format = pImage->format;
            NativeImageUtil::AllocNativeImage(&m_RenderFrame);
        }

        NativeImageUtil::CopyNativeImage(pImage, &m_RenderFrame);
    }

//    void onSurfaceCreate(){
//        LOGD("preview onSurfaceCreate");
//        if (!createProgram()) {
//            LOGE("GLByteFlowRender::OnSurfaceCreated create program fail.");
//        }
//    }

    void onSurfaceChange(int width, int height){
        LOGD("preview onSurfaceChange");
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        m_IsProgramChanged = true;
    }



    void destroy() {
        LOGD("Preview destroy");
    };


private:

    Shader mPreviewShader;

    GLuint m_VertexCoorHandle;
    GLuint m_TextureCoorHandle;

    volatile bool   m_IsProgramChanged;
    size_t          m_ViewportWidth;
    size_t          m_ViewportHeight;
    NativeImage     m_RenderFrame;

    GLuint m_YTextureId;
    GLuint m_UTextureId;
    GLuint m_VTextureId;

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
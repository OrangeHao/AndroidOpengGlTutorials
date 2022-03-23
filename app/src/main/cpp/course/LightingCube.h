
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Shader.h"
#include "BaseCourse.h"
#include "LogUtil.h"

/****
 * 光照立方体示例
 */
class LightingCube : public BaseCourse{
public:

    LightingCube(){};
    ~LightingCube(){};

    void init(int width,int height) {
            LOGD("LightingCube init");
        char vShader[]=
                "#version 300 es\n"
                "layout (location = 0) in vec4 vertexPosition;\n"
                "layout (location = 1) in vec3 vertexColour;\n"
                "\n"
                "//法线\n"
                "layout (location = 2) in vec3 vertextNormals;\n"
                "\n"
                "out vec3 fragColours;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modeView;\n"
                "\n"
                "void main() {\n"
                "\n"
                "    //sence\n"
                "    vec3 transformedVertextNormal=normalize((modeView*vec4(vertextNormals,0.0)).xyz);\n"
                "    vec3 inverseLightDirection=normalize(vec3(0.0,1.0,1.0));\n"
                "    fragColours=vec3(0.0);\n"
                "\n"
                "    //diffuse 散射\n"
                "    vec3 difuseLightIntensity=vec3(1.0,1.0,1.0);\n"
                "    vec3 vertexDiffuseReflectionConstant=vertexColour;\n"
                "    float normalDotLight=max(0.0,dot(transformedVertextNormal,inverseLightDirection));\n"
                "    fragColours+=normalDotLight*vertexDiffuseReflectionConstant*difuseLightIntensity;\n"
                "\n"
                "    //ambient 环境\n"
                "    vec3 ambientLightIntensity=vec3(0.1,0.1,0.1);\n"
                "    vec3 vertextAmbientReflectionConstant=vertexColour;\n"
                "    fragColours+=vertextAmbientReflectionConstant*ambientLightIntensity;\n"
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
                "    fragColours+=pow(normalDoteReflection,shininess)*vertextSpecularReflectionConstant*specularLightIntensity;\n"
                "\n"
                "    clamp(fragColours,0.0,1.0);\n"
                "\n"
                "    gl_Position=projection*modeView*vertexPosition;\n"
                "}";

        char fragShader[]=
                "#version 300 es\n"
                "precision mediump float;\n"
                "in vec3 fragColours;\n"
                "out vec4 fragColor;\n"
                "void main() {\n"
                "    fragColor=vec4(fragColours,1.0);\n"
                "}";

        lightingCubeShader=Shader(vShader,fragShader);

        mWidth=width;
        mHeight=height;

        glEnable(GL_DEPTH_TEST);
    };

    void drawFrame() {
            LOGD("LightingCube drawFrame");
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

            glm::mat4 view = glm::mat4(1.0f);
            view=glm::translate(view,glm::vec3(0.0f,0.0f,-10.0f));
            view=glm::rotate(view,glm::radians(angle),glm::vec3(1.0f, 0.0f, 0.0f));
            view=glm::rotate(view,glm::radians(angle*2),glm::vec3(0.0f, 1.0f, 0.0f));

            //导入程序到环境中
            glUseProgram(lightingCubeShader.ID);

            //设置位置数据
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
            glEnableVertexAttribArray(0);

            //设置颜色数据
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,colour);
            glEnableVertexAttribArray(1);

            /* [Upload vertex normals.] */
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, normals);
            glEnableVertexAttribArray(2);

            //设置矩阵数据
            //near和far是表示视点到远近投影平面的距离
            //45是视锥张角的角度，也就是视角，越小物体越大
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.5f, 100.0f);
            lightingCubeShader.setMat4("projection",projection);
            lightingCubeShader.setMat4("modeView",view);

            //36个点
            glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

            angle += 1;
            if (angle > 360)
            {
                    angle -= 360;
            }
    };

    void destroy() {
            LOGD("LightingCube destroy");
    };

private:

    Shader lightingCubeShader;

    float angle = 0;

    int mWidth=0;
    int mHeight=0;
};
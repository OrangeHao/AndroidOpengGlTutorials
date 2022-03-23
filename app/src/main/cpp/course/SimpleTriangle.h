
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Shader.h"
#include "BaseCourse.h"

class SimpleTriangle: public BaseCourse
{
public:

    SimpleTriangle();
    ~SimpleTriangle();

    virtual void init(int width,int height);
    virtual void drawFrame();
    virtual void destroy();

private:

    Shader triangleShader;

};
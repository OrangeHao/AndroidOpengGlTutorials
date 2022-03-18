

#ifndef ANDROID_OPENGL_TUTORIALS_BASE_COURSE
#define ANDROID_OPENGL_TUTORIALS_BASE_COURSE


class BaseCourse
{
public:

    BaseCourse(){

    }

    virtual ~BaseCourse(){

    }

    virtual void init(int width,int height)=0;
    virtual void drawFrame()=0;
    virtual void destroy()=0;

protected:

};


#endif
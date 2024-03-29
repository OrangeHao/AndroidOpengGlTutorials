
#ifndef ANDROID_OPENGL_TUTORIALS_BASE_CourseManager
#define ANDROID_OPENGL_TUTORIALS_BASE_CourseManager


#include "BaseCourse.h"

#define COURSE_ID_TRIANGLE  1
#define COURSE_ID_CUBE      2
#define COURSE_ID_LIGHTING_CUBE   3
#define COURSE_ID_TEXTURE_CUBE   4

class CourseManager{

public:

    CourseManager();
    ~CourseManager();

    void init();
    void setCourseById(int type);
    void initCourse(int width, int height);
    void drawFrame();

    static CourseManager* getInstance();
    static void destroy();

private:

    static CourseManager  *m_instance;
    BaseCourse* m_curCourse;

};


#endif
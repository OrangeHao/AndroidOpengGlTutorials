
#ifndef ANDROID_OPENGL_TUTORIALS_BASE_CourseManager
#define ANDROID_OPENGL_TUTORIALS_BASE_CourseManager


#include "BaseCourse.h"

#define COURSE_ID_TRIANGLE  1
#define COURSE_ID_CUBE      2

class CourseManager{

public:

    CourseManager();
    ~CourseManager();

    void init();
    void setCourseById(int type);
    void initCourse();
    void drawFrame();

    static CourseManager* getInstance();
    static void destroy();

private:

    static CourseManager  *m_instance;
    BaseCourse* m_curCourse;

};


#endif
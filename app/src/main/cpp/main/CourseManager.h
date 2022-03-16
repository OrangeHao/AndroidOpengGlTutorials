
#include "BaseCourse.h"

class CourseManager{

public:

    CourseManager();
    ~CourseManager();

    void init();
    void setCourseType(int type);
    void drawFrame();

    static CourseManager* getInstance();
    static void destroy();

private:

    static CourseManager* m_instance;
    BaseCourse* m_curCourse;

};

#include "CourseManager.h"
#include "SimpleTriangle.h"

CourseManager *CourseManager::m_instance= nullptr;

CourseManager::CourseManager() {
    m_curCourse=new SimpleTriangle();
}

CourseManager::~CourseManager() {
    if (m_curCourse){
        delete m_curCourse;
        m_curCourse= nullptr;
    }
}

CourseManager * CourseManager::getInstance() {
    LOGE("CourseManager getInstance");
    if(m_instance== nullptr){
        m_instance=new CourseManager;
    }
    return m_instance;
}

void CourseManager::destroy() {

}

void CourseManager::init() {

}

void CourseManager::setCourseById(int type) {
    LOGE("CourseManager setCourseById");
    switch (type) {
        case COURSE_ID_TRIANGLE:
            m_curCourse=new SimpleTriangle();
            break;
        case COURSE_ID_CUBE:
            m_curCourse=new SimpleTriangle();
            break;
        default:
            m_curCourse=new SimpleTriangle();
            break;
    }
}

void CourseManager::initCourse() {
    LOGE("CourseManager initCourse");
    if (m_curCourse== nullptr){
        m_curCourse=new SimpleTriangle();
    }
    m_curCourse->init();
}

void CourseManager::drawFrame() {
    LOGE("CourseManager drawFrame");
    if (m_curCourse!= nullptr){
        m_curCourse->drawFrame();
    }
}
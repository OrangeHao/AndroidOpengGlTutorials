
#include "CourseManager.h"
#include "SimpleTriangle.h"
#include "SimpleCube.h"
#include "LightingCube.h"
#include "TextureCube.h"
#include "Preview.h"
#include "LogUtil.h"

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
    LOGI("CourseManager getInstance");
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
    LOGI("CourseManager setCourseById");
    switch (type) {
        case COURSE_ID_TRIANGLE:
            m_curCourse=new SimpleTriangle();
            break;
        case COURSE_ID_CUBE:
            m_curCourse=new SimpleCube();
            break;
        case COURSE_ID_LIGHTING_CUBE:
            m_curCourse=new LightingCube();
            break;
        case COURSE_ID_TEXTURE_CUBE:
            m_curCourse=new TextureCube();
            break;
        default:
            m_curCourse=new SimpleTriangle();
            break;
    }

}

void CourseManager::initCourse(int width, int height) {
    LOGI("CourseManager initCourse");
    if (m_curCourse== nullptr){
        m_curCourse=new SimpleTriangle();
    }
    m_curCourse->init(width,height);
}

void CourseManager::drawFrame() {
    LOGI("CourseManager drawFrame");
    if (m_curCourse!= nullptr){
        m_curCourse->drawFrame();
    }
}
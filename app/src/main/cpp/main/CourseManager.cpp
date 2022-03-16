#include "CourseManager.h"



CourseManager::CourseManager() {

}

CourseManager::~CourseManager() {

}

CourseManager * CourseManager::getInstance() {
    if(m_instance== nullptr){
        m_instance=new CourseManager;
    }
    return m_instance;
}

void CourseManager::init() {

}
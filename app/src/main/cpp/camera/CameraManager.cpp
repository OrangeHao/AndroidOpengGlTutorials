
#include "CameraManager.h"


CameraManager *CameraManager::m_instance= nullptr;

CameraManager::CameraManager() {
    m_cameraPreview=new Preview();
}

CameraManager::~CameraManager() {
    if(m_cameraPreview){
        delete m_cameraPreview;
        m_cameraPreview= nullptr;
    }
}


CameraManager * CameraManager::getInstance() {
    if(m_instance== nullptr){
        m_instance=new CameraManager();
    }
    return m_instance;
}

void CameraManager::init() {
    if(m_cameraPreview== nullptr){
        return;
    }
    m_cameraPreview->init();
}

void CameraManager::onSurfaceChanged(int width, int height) {
    if(m_cameraPreview== nullptr){
        return;
    }
    m_cameraPreview->onSurfaceChange(width,height);
}


void CameraManager::UpdateFrame(int format, uint8_t *pBuffer, int width, int height) {
    if(m_cameraPreview== nullptr){
        return;
    }
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pBuffer;

    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }

    m_cameraPreview->updateFrame(&nativeImage);
}

void CameraManager::onDrawFrame() {
    if(m_cameraPreview== nullptr){
        return;
    }
    m_cameraPreview->drawFrame();
}


#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <cstdint>
#include <jni.h>
#include "Preview.h"
#include "ImageDef.h"


class CameraManager{

public:

    CameraManager();
    ~CameraManager();

    void init();
    void UpdateFrame(int format, uint8_t *pBuffer, int width, int height);
    void onSurfaceChanged(int width,int height);
    void onDrawFrame();

    static CameraManager* getInstance();
private:

    static CameraManager  *m_instance;
    Preview* m_cameraPreview;

};




#endif

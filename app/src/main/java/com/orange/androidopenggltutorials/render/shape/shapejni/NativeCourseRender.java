package com.orange.androidopenggltutorials.render.shape.shapejni;

import com.orange.androidopenggltutorials.render.BaseRender;
import com.orange.androidopenggltutorials.render.shape.NativeLib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * created by czh on 2022.3.18
 */
class NativeCourseRender extends BaseRender {

   public void setCourseById(int courseById){
      NativeLib.native_setCourseById(courseById);
   }

   @Override
   public void onSurfaceCreated(GL10 gl, EGLConfig config) {

   }

   @Override
   public void onSurfaceChanged(GL10 gl, int width, int height) {
      NativeLib.native_initCourse(width, height);
   }

   @Override
   public void onDrawFrame(GL10 gl) {
      NativeLib.native_drawCourseFrame();
   }
}

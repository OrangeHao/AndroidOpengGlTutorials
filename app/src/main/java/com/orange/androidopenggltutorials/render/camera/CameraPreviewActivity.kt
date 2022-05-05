package com.orange.androidopenggltutorials.render.camera

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.view.Surface
import android.view.TextureView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.orange.androidopenggltutorials.R

class CameraPreviewActivity : AppCompatActivity(),Camera2FrameCallback{

    private val REQUEST_PERMISSIONS = arrayOf(
        Manifest.permission.CAMERA,
        Manifest.permission.WRITE_EXTERNAL_STORAGE
    )
    private val CAMERA_PERMISSION_REQUEST_CODE = 1

    var mCamera2Wrapper:Camera2Wrapper?=null
    var mTextureView:TextureView?=null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_camera_preview)

        initView()
    }

    private fun initView(){
        mTextureView=findViewById(R.id.textureView)
        mTextureView!!.surfaceTextureListener=object :TextureView.SurfaceTextureListener{
            override fun onSurfaceTextureAvailable(
                surface: SurfaceTexture, width: Int, height: Int) {
                mCamera2Wrapper = Camera2Wrapper(this@CameraPreviewActivity, Surface(surface))
                if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                    mCamera2Wrapper!!.startCamera()
                } else {
                    ActivityCompat.requestPermissions(
                        this@CameraPreviewActivity,
                        REQUEST_PERMISSIONS,
                        CAMERA_PERMISSION_REQUEST_CODE
                    )
                }
            }

            override fun onSurfaceTextureSizeChanged(
                surface: SurfaceTexture, width: Int, height: Int) {}

            override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
                return false
            }

            override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {}
        }
    }

    override fun onResume() {
        super.onResume()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == CAMERA_PERMISSION_REQUEST_CODE) {
            if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                mCamera2Wrapper!!.startCamera()
            } else {
                Toast.makeText(this, "We need the camera permission.", Toast.LENGTH_SHORT).show()
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }

    }

    override fun onPause() {
        if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            mCamera2Wrapper!!.stopCamera()
        }
        super.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
    }


    override fun onPreviewFrame(data: ByteArray?, width: Int, height: Int) {

    }

    override fun onCaptureFrame(data: ByteArray?, width: Int, height: Int) {

    }


    private fun hasPermissionsGranted(permissions: Array<String>): Boolean {
        for (permission in permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                != PackageManager.PERMISSION_GRANTED) {
                return false
            }
        }
        return true
    }


}
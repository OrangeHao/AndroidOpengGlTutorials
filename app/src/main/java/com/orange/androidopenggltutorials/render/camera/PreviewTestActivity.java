package com.orange.androidopenggltutorials.render.camera;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;
import android.view.TextureView;
import android.widget.Toast;

import com.orange.androidopenggltutorials.R;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * @author 19b300
 */
public class PreviewTestActivity extends AppCompatActivity {

    private TextureView mTextureView;
    private String[] mCameraIDs;
    private String mCameraId;

    private CameraManager mCameraManager;
    private CameraDevice mCameraDevice;
    private CaptureRequest.Builder mPreviewRequestBuilder;
    private CameraCaptureSession mCameraCaptureSession;

    private int DEFAULT_CAMERA_ID = 0;
    private Handler mHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_preview_test);

        mHandler = new Handler(Looper.getMainLooper());
        initView();
        cameraInit();
    }

    private void initView() {
        mTextureView = findViewById(R.id.textureView);
        mTextureView.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surface, int width, int height) {
                try {
                    if (ActivityCompat.checkSelfPermission(PreviewTestActivity.this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                        Toast.makeText(PreviewTestActivity.this,"no permission",Toast.LENGTH_SHORT).show();
                        return;
                    }
                    mCameraManager.openCamera(mCameraId, mStateCallback, mHandler);
                } catch (CameraAccessException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surface, int width, int height) { }

            @Override
            public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surface) {
                return false; }

            @Override
            public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surface) { }
        });
    }

    private CameraDevice.StateCallback mStateCallback=new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            mCameraDevice=camera;

            SurfaceTexture surfaceTexture=mTextureView.getSurfaceTexture();
            Surface surface=new Surface(surfaceTexture);

            try {
                mPreviewRequestBuilder=mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);
                mPreviewRequestBuilder.addTarget(surface);
                mCameraDevice.createCaptureSession(Arrays.asList(surface),mSessionCallback,mHandler);
            } catch (CameraAccessException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice camera) {

        }

        @Override
        public void onError(@NonNull CameraDevice camera, int error) {

        }
    };

    private CameraCaptureSession.StateCallback mSessionCallback=new CameraCaptureSession.StateCallback() {
        @Override
        public void onConfigured(@NonNull CameraCaptureSession session) {
            mCameraCaptureSession=session;
            try {
                mCameraCaptureSession.setRepeatingRequest(mPreviewRequestBuilder.build(),null,mHandler);
            } catch (CameraAccessException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onConfigureFailed(@NonNull CameraCaptureSession session) { }
    };

    private void cameraInit(){
        mCameraManager=(CameraManager) getSystemService(CAMERA_SERVICE);
        try {
            mCameraIDs=mCameraManager.getCameraIdList();
            for (String id:mCameraIDs){
                Log.d("czh","camera id:"+id);
            }
            mCameraId=String.valueOf(DEFAULT_CAMERA_ID);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private boolean checkCameraIdSupport(String cameraId) {
        boolean isSupported = false;
        for (String id: mCameraIDs) {
            if (cameraId.equals(id)) {
                isSupported = true;
            }
        }
        return isSupported;
    }
}
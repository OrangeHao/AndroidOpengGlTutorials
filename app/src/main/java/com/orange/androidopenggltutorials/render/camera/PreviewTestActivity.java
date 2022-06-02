package com.orange.androidopenggltutorials.render.camera;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.Log;
import android.util.Size;
import android.view.Surface;
import android.view.TextureView;
import android.widget.ImageView;
import android.widget.Toast;

import com.orange.androidopenggltutorials.R;

import java.nio.ByteBuffer;
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
    private ImageReader mPreviewReader;
    /**用来产生一个handler，在线程里处理摄像头的内容；**/
    private HandlerThread mBackgroundThread;
    private Size previewSize;

    private ImageView mImageView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_preview_test);

        initHandler();
        initView();
    }

    private void initHandler() {
        mBackgroundThread = new HandlerThread("MyCamera2");
        mBackgroundThread.start();
        mHandler = new Handler(mBackgroundThread.getLooper());
    }

    private void initView() {
        mImageView=findViewById(R.id.imageView);
        mTextureView = findViewById(R.id.textureView);
        mTextureView.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surface, int width, int height) {
                openCamera(width,height);
            }

            @Override
            public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surface, int width, int height) {
            }

            @Override
            public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surface) {
                return false;
            }

            @Override
            public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surface) {
            }
        });
    }

    private void openCamera(int width, int height) {
        CameraManager cameraManager = (CameraManager) getSystemService(CAMERA_SERVICE);
        try {
            for (String cameraId : cameraManager.getCameraIdList()) {
                //描述相机设备的属性类
                CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(cameraId);
                //获取是前置还是后置摄像头
                Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                //使用后置摄像头
                if (facing != null && facing == CameraCharacteristics.LENS_FACING_BACK) {
                    StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
                    if (map != null) {
                        previewSize = CameraUtil.getOptimalSize(map.getOutputSizes(SurfaceTexture.class), width, height);
                        mCameraId = cameraId;
                    }
                }
            }

            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(PreviewTestActivity.this, "no permission", Toast.LENGTH_SHORT).show();
                return;
            }

            initImageReader();

            cameraManager.openCamera(mCameraId, mStateCallback, mHandler);
        } catch (CameraAccessException r) { }
    }

    private void initImageReader(){
        mPreviewReader=ImageReader.newInstance(previewSize.getWidth(),previewSize.getHeight(), ImageFormat.JPEG,2);
        mPreviewReader.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
            @Override
            public void onImageAvailable(ImageReader reader) {
                Image image=reader.acquireLatestImage();
                ByteBuffer byteBuffer=image.getPlanes()[0].getBuffer();
                byte[]bytes=new byte[byteBuffer.remaining()];
                byteBuffer.get(bytes);
                Log.d("czh","image data:"+bytes.length);
                Bitmap bitmap= BitmapFactory.decodeByteArray(bytes,0,bytes.length);
                mOnImageGetListener.newImage(bitmap);



                image.close();
            }
        },mHandler);
    }

    private CameraDevice.StateCallback mStateCallback=new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            mCameraDevice=camera;

            try {
                SurfaceTexture surfaceTexture=mTextureView.getSurfaceTexture();
                Surface surface=new Surface(surfaceTexture);
                mPreviewRequestBuilder=mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);
                mPreviewRequestBuilder.addTarget(surface);
                mPreviewRequestBuilder.addTarget(mPreviewReader.getSurface());

                mCameraDevice.createCaptureSession(Arrays.asList(surface,mPreviewReader.getSurface()),mSessionCallback,mHandler);
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


    private boolean checkCameraIdSupport(String cameraId) {
        boolean isSupported = false;
        for (String id: mCameraIDs) {
            if (cameraId.equals(id)) {
                isSupported = true;
            }
        }
        return isSupported;
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        stopHandlerThread();
    }

    private void stopHandlerThread() {
        if (mBackgroundThread != null) {
            mBackgroundThread.quitSafely();
            try {
                mBackgroundThread.join();
                mBackgroundThread = null;
                mHandler = null;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private onImageGetListener mOnImageGetListener=new onImageGetListener() {
        @Override
        public void newImage(Bitmap bitmap) {
            runOnUiThread(() -> mImageView.setImageBitmap(bitmap));
        }
    };
    public interface onImageGetListener{
        public void newImage(Bitmap bitmap);
    }
}
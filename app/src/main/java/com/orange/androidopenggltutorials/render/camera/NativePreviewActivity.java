package com.orange.androidopenggltutorials.render.camera;

import static com.orange.androidopenggltutorials.render.camera.NativeRenderer.IMAGE_FORMAT_I420;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
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
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.util.Size;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.orange.androidopenggltutorials.R;
import com.orange.androidopenggltutorials.render.shape.NativeLib;

import java.nio.ByteBuffer;
import java.util.Arrays;

/**
 * 使用opengl进行摄像头的预览
 * @author 19b300
 */
public class NativePreviewActivity extends AppCompatActivity {

    private Size DefaultPreviewSize = new Size(1280, 720);
    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.CAMERA,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
    };

    private static final int CAMERA_PERMISSION_REQUEST_CODE = 1;

    private Context mContext;

    private String[] mCameraIDs;
    private String mCameraId;

    private CameraDevice mCameraDevice;
    private CaptureRequest.Builder mPreviewRequestBuilder;
    private CameraCaptureSession mCameraCaptureSession;

    private int DEFAULT_CAMERA_ID = 0;

    private Handler mHandler;
    private ImageReader mPreviewReader;
    /**用来产生一个handler，在线程里处理摄像头的内容；**/
    private HandlerThread mBackgroundThread;
    private Size previewSize;

    private RelativeLayout mSurfaceContainer;
    private GLSurfaceView mGLSurfaceView;
    private NativeRenderer mNativeRenderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_preview);
        mContext=this;

        initHandler();
        initView();
    }

    private void initHandler() {
        mBackgroundThread = new HandlerThread("MyCamera2");
        mBackgroundThread.start();
        mHandler = new Handler(mBackgroundThread.getLooper());
    }

    private void initView() {
        mNativeRenderer=new NativeRenderer();
        //创建一个glSurfaceView
        mGLSurfaceView=new GLSurfaceView(this);
        mSurfaceContainer=findViewById(R.id.surface_root);
        RelativeLayout.LayoutParams lp=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT);
        //把glSurfaceView添加到activity的界面
        mSurfaceContainer.addView(mGLSurfaceView,lp);
        //renderer设置到glSurfaceView进行关联，后续操作renderer进行渲染
        mNativeRenderer.init(mGLSurfaceView);


        if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            openCamera();
        } else {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, CAMERA_PERMISSION_REQUEST_CODE);
        }

    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == CAMERA_PERMISSION_REQUEST_CODE) {
            if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                openCamera();
            } else {
                Toast.makeText(this, "We need the camera permission.", Toast.LENGTH_SHORT).show();
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    private void openCamera() {
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
                        previewSize = CameraUtil.getOptimalSize(map.getOutputSizes(SurfaceTexture.class),
                                DefaultPreviewSize.getWidth(), DefaultPreviewSize.getHeight());
                        mCameraId = cameraId;
                    }
                }
            }

            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(mContext, "no permission", Toast.LENGTH_SHORT).show();
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
                if (image != null) {
                    mNativeRenderer.setRenderFrame(IMAGE_FORMAT_I420, CameraUtil.YUV_420_888_data(image), image.getWidth(), image.getHeight());
                    mNativeRenderer.requestRender();
                    image.close();
                }
//                ByteBuffer byteBuffer=image.getPlanes()[0].getBuffer();
//                byte[]bytes=new byte[byteBuffer.remaining()];
//                byteBuffer.get(bytes);
//                Log.d("czh","image data:"+bytes.length);
//                image.close();
            }
        },mHandler);
    }

    private CameraDevice.StateCallback mStateCallback=new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            mCameraDevice=camera;
            try {
                mPreviewRequestBuilder=mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);
                mPreviewRequestBuilder.addTarget(mPreviewReader.getSurface());

                mCameraDevice.createCaptureSession(Arrays.asList(mPreviewReader.getSurface()),mSessionCallback,mHandler);
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


    protected boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }
}
package com.orange.androidopenggltutorials.render.shape

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.orange.androidopenggltutorials.R
import com.orange.androidopenggltutorials.render.shape.SimpleCubeJNI
import kotlinx.android.synthetic.main.activity_shape.*

class ShapeActivity : AppCompatActivity() {



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_shape)

        init()
    }

    private fun init(){
        switchBtn.setOnClickListener {
            glSurfaceView?.onPause()
            glSurfaceView?.setShape(SimpleCubeJNI::class.java)
            glSurfaceView?.onResume()
//            glSurfaceView?.requestRender()
        }
    }

    override fun onResume() {
        super.onResume()
        glSurfaceView?.onResume()
    }

    override fun onPause() {
        super.onPause()
        glSurfaceView?.onPause()
    }

}

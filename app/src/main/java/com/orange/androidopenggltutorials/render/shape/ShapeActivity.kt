package com.orange.androidopenggltutorials.render.shape

import android.opengl.GLSurfaceView
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.orange.androidopenggltutorials.render.shape.shapejni.SimpleCubeJNI
import kotlinx.android.synthetic.main.activity_shape.*
import com.lxj.xpopup.XPopup
import com.orange.androidopenggltutorials.R
import com.orange.androidopenggltutorials.render.BaseRender
import com.orange.androidopenggltutorials.render.shape.shapejni.TriangelJNI
import java.security.AccessController.getContext


class ShapeActivity : AppCompatActivity() {



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_shape)

        init()
    }

    private fun init(){
        switchBtn.setOnClickListener {
            showSelectDialog()
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


    private fun showSelectDialog(){
        XPopup.Builder(this)
            //.maxWidth(600)
            .asBottomList(
                getString(R.string.shape_selected), arrayOf(getString(R.string.shape_triangle),
                    getString(R.string.shape_cube), getString(R.string.shape_triangle),
                    getString(R.string.shape_triangle)))
            { position, text ->
                when (position){
                    0->{
                        updateGlsurfaceView(TriangelJNI::class.java,1)
                    }
                    1->{
                        updateGlsurfaceView(SimpleCubeJNI::class.java,2)
                    }
                    2->{

                    }
                    3->{

                    }
                }
            }
            .show()
    }

    private fun updateGlsurfaceView(clazz: Class<out BaseRender>,renderMode:Int){
        glSurfaceView?.onPause()
        glSurfaceView?.setShape(clazz)
        glSurfaceView?.renderMode=if (renderMode==1) GLSurfaceView.RENDERMODE_WHEN_DIRTY else GLSurfaceView.RENDERMODE_CONTINUOUSLY
        glSurfaceView?.onResume()
    }

}

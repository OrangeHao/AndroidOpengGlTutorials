package com.orange.androidopenggltutorials

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.orange.androidopenggltutorials.render.shape.NativeLib
import com.orange.androidopenggltutorials.render.shape.ShapeActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        initListener()

        Log.d("czh","from native:"+ NativeLib.getTestString())
    }


    private fun initListener(){
        btDraw.setOnClickListener {
            startActivity(Intent(this, ShapeActivity::class.java))
        }


    }
}

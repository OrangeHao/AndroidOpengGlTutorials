package com.orange.androidopenggltutorials;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.orange.androidopenggltutorials.render.camera.Camera2Wrapper;
import com.orange.androidopenggltutorials.render.camera.CameraPreviewActivity;
import com.orange.androidopenggltutorials.render.shape.ShapeActivity;

/**
 * created by czh on 2022.4.24
 * @author 19b300
 */
public class MainActivity extends AppCompatActivity {

   @Override
   protected void onCreate(@Nullable Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_main);
      initListener();
   }


   private void initListener(){
      findViewById(R.id.btDraw).setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            startActivity(new Intent(MainActivity.this, ShapeActivity.class));
         }
      });

      findViewById(R.id.btCamera).setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            startActivity(new Intent(MainActivity.this, CameraPreviewActivity.class));
         }
      });
   }
}

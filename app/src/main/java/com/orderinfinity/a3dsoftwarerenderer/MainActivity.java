package com.orderinfinity.a3dsoftwarerenderer;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Point;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.BoringLayout;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.View;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setImmersiveSticky();
        View decorView = getWindow().getDecorView();
        decorView.setOnSystemUiVisibilityChangeListener
                (new View.OnSystemUiVisibilityChangeListener()
                {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility)
                    {
                        setImmersiveSticky();
                    }
                });


        int width = 0, height = 0;
        try
        {
            DisplayMetrics metrics = new DisplayMetrics();
            Display display = getWindowManager().getDefaultDisplay();
            Method mGetRawH = null, mGetRawW = null;
            if (android.os.Build.VERSION.SDK_INT >= 17)
            {
                display.getRealMetrics(metrics);
                width = metrics.widthPixels;
                height = metrics.heightPixels;
            }
            else
            {
                mGetRawH = Display.class.getMethod("getRawHeight");
                mGetRawW = Display.class.getMethod("getRawWidth");

                width = (Integer) mGetRawW.invoke(display);
                height = (Integer) mGetRawH.invoke(display);
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            return;
        }

        setContentView(new SoftwareRendererView(this, width, height));
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        setImmersiveSticky();
    }

    @TargetApi(Build.VERSION_CODES.KITKAT)
    void setImmersiveSticky()
    {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }

    static
    {
        System.loadLibrary("software_renderer");
    }
}

class SoftwareRendererView extends View
{
    private Bitmap bitmap;

    private static native void init(Bitmap bitmap);
    private static native void render(Bitmap bitmap);

    public SoftwareRendererView(Context context, int width, int height)
    {
        super(context);
        bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        init(bitmap);
    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);

        render(bitmap);
        canvas.drawBitmap(bitmap,0.0f,0.0f,null);
        invalidate();
    }
}

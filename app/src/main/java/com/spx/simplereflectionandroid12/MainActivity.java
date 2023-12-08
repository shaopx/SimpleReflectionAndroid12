package com.spx.simplereflectionandroid12;

import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.spx.simplereflectionandroid12.R;
import com.spx.simplereflectionandroid12.databinding.ActivityMainBinding;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "FreeReflect_MainActivity";

    // Used to load the 'simplereflectionandroid14' library on application startup.
    static {
        System.loadLibrary("simplereflectionandroid14");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        findViewById(R.id.test_reflection).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
//                    Class<?> activityClass = Class.forName("dalvik.system.VMRuntime");
//                    Method method = activityClass.getDeclaredMethod("getRuntime");
//                    method.setAccessible(true);
//                    Object obj = method.invoke(activityClass, null);
                    Class<?> activityClass = Class.forName("dalvik.system.VMRuntime");
                    Method field = activityClass.getDeclaredMethod("setHiddenApiExemptions", String[].class);
                    field.setAccessible(true);
                    Log.i(TAG, "call success!!");
                    toast("反射调用hide接口成功");
                } catch (Throwable e) {
                    Log.e(TAG, "error:", e);
                    toast("error: " + e);
                }
            }
        });

        findViewById(R.id.enable_hide_reflection).setOnClickListener(v -> {
            int ret = enableFreeReflection(MainActivity.this.getApplication().getApplicationInfo().targetSdkVersion);
            toast("unseal result: " + ret);
        });
    }

    private void toast(String msg) {
        if (TextUtils.isEmpty(msg)) {
            return;
        }
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
        Log.i(TAG, msg);
    }


    /**
     * A native method that is implemented by the 'simplereflectionandroid14' native library,
     * which is packaged with this application.
     */
    public native int enableFreeReflection(int targetVersion);
}
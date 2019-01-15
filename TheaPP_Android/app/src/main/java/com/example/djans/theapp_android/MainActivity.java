package com.example.djans.theapp_android;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        WebView webViewTheaPP = (WebView)(findViewById(R.id.webViewTheaPP));
        webViewTheaPP.getSettings().setJavaScriptEnabled(true);
        webViewTheaPP.setWebViewClient(new WebViewClient());
        webViewTheaPP.loadUrl("file:///android_asset/index.html");
    }
}

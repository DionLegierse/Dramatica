package nl.blok_2.theapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.webkit.WebViewClient;
import android.webkit.WebView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        WebView webviewTheapp = (WebView)(findViewById(R.id.webViewTheapp));
        webviewTheapp.getSettings().setJavaScriptEnabled(true);
        webviewTheapp.setWebViewClient(new WebViewClient());
        webviewTheapp.loadUrl("file:///android_asset/index.html");
    }
}

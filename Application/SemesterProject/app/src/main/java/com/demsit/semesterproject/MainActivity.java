package com.demsit.semesterproject;

import android.content.DialogInterface;
import android.os.Bundle;

import androidx.annotation.NonNull;

import com.demsit.semesterproject.Fragments.HomeFragment;
import com.demsit.semesterproject.Fragments.SettingsFragment;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

import android.view.MenuItem;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    public static ArduinoConnection ac;
    private TextView mTextMessage;


    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            Fragment fragment = null;
            FragmentTransaction ft = getSupportFragmentManager().beginTransaction();
            Boolean find = true;
            switch (item.getItemId()) {
                case R.id.navigation_home:
                    fragment = new HomeFragment();
                    break;
                case R.id.navigation_settings:
                    fragment = new SettingsFragment();
                    break;
            }
            if(find){
                ft.replace(R.id.to_replace, fragment);
                ft.commit();
                return true;
            }return false;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextMessage = (TextView) findViewById(R.id.message);
        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
        FragmentTransaction ft = getSupportFragmentManager().beginTransaction();
        Fragment fragment = new HomeFragment();
        ft.replace(R.id.to_replace, fragment);
        setConnection();

        this.setTitle("B&H");


    }

    private void setConnection() {
        MainActivity activity = this;
        TextView text = new TextView(MainActivity.this);
        text.setText("Set the connection with the device");
        text.setPadding(20, 8, 20, 8);
        final AlertDialog.Builder ad = new AlertDialog.Builder(MainActivity.this, R.style.AlertDialog);
        ad.setView(text);
        ad.setCancelable(false);
        ad.setPositiveButton(getString(R.string.connection_button), new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                ac = ArduinoConnection.getInstance(activity);
                if(ac==null){
                    setConnection();
                }
            }
        });

        ad.show();


    }

    public void connectArduino(){
        ac = ArduinoConnection.getInstance(this);
    }

    public void fallingTrigerred(){
        TextView text = new TextView(MainActivity.this);
        text.setText("Press the button if you are ok");
        text.setPadding(20, 8, 20, 8);
        AlertDialog.Builder ad = new AlertDialog.Builder(MainActivity.this, R.style.AlertDialog);
        ad.setTitle(getString(R.string.alert_falling));
        ad.setView(text);
        ad.setCancelable(false);
        ad.setPositiveButton(getString(R.string.falling_false), new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                ac.send("15");
            }
        });

        ad.show();
    }

    public void getMessage(String message){

    }


    public void messageToSend(String s) {
        MainActivity activity = this;
        TextView text = new TextView(MainActivity.this);
        text.setText(s);
        text.setPadding(20, 8, 20, 8);
        final AlertDialog.Builder ad = new AlertDialog.Builder(MainActivity.this, R.style.AlertDialog);
        ad.setView(text);
        ad.setCancelable(false);
        ad.setPositiveButton(getString(R.string.ok), new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        ad.show();
    }
}

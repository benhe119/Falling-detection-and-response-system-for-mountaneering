package com.demsit.semesterproject.Fragments;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.Toast;

import com.demsit.semesterproject.MainActivity;
import com.demsit.semesterproject.R;


public class SettingsFragment extends Fragment {

    private View view;
    private MainActivity activity;
    private EditText settingsName;
    private EditText settingsEmergencyNumber;
    private EditText settingsBloodType;
    private Button updateName, updateNumber, updateBlood;
    private Switch fallingDetector;
    private String name, emergencyNumber, bloodType;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_settings, container, false);
        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        activity = (MainActivity)getActivity();
        settingsName = view.findViewById(R.id.settings_name);
        settingsEmergencyNumber = view.findViewById(R.id.settings_emergency_number);
        settingsBloodType = view.findViewById(R.id.settings_blood_type);
        fallingDetector = view.findViewById(R.id.falling_detector);
        updateName = view.findViewById(R.id.updateName);
        updateNumber = view.findViewById(R.id.updateNumber);
        updateBlood = view.findViewById(R.id.updateBloodType);
        updateName.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String messages[] = new String[3];
                String nameTemp = settingsName.getText().toString();
                if(!TextUtils.isEmpty(nameTemp) && !nameTemp.equals(name)){
                    name = nameTemp;
                    activity.ac.send("1;"+name);
                }
            }
        });
        updateNumber.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String numberTemp = settingsEmergencyNumber.getText().toString();
                if(!TextUtils.isEmpty(numberTemp) && !numberTemp.equals(emergencyNumber)){
                    emergencyNumber = numberTemp;
                    activity.ac.send("2;"+emergencyNumber);

                }
            }
        });
        updateBlood.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String bloodTpeTemp = settingsBloodType.getText().toString();
                if(!TextUtils.isEmpty(bloodTpeTemp) && !bloodTpeTemp.equals(bloodType)){
                    bloodType = bloodTpeTemp;
                    activity.ac.send("3;"+bloodType);
                }

            }
        });

        fallingDetector.setChecked(false);
        fallingDetector.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(!isChecked){
                    activity.ac.send("30");
                }else{
                    activity.ac.send("20");
                }
            }
        });
    }

}

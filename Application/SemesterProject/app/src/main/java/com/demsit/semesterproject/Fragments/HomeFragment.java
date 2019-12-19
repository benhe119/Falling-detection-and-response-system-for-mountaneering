package com.demsit.semesterproject.Fragments;


import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;


import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;


import com.demsit.semesterproject.ArduinoConnection;
import com.demsit.semesterproject.MainActivity;
import com.demsit.semesterproject.R;

public class HomeFragment extends Fragment {

    private View view;
    private ImageView connectionButton;
    private LinearLayout connectionInfo;
    private LinearLayout connectionDisabled;
    private LinearLayout connectionEnabled;
    private LinearLayout notificationBlock;
    private TextView notificationContent;
    private Button simulation;
    private ArduinoConnection ac;
    private MainActivity activity;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_home, container, false);
        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        connectionInfo = view.findViewById(R.id.connectionInfo);
        connectionButton = view.findViewById(R.id.connectionButton);
        connectionDisabled = view.findViewById(R.id.connectionDisabled);
        connectionEnabled = view.findViewById(R.id.connectionEnabled);
        simulation = view.findViewById(R.id.simulation_button);
        ac = MainActivity.ac;
        if(ac.btSocket!=null && ac.btSocket.isConnected()){
            connectionDisabled.setVisibility(View.GONE);
            connectionEnabled.setVisibility(View.VISIBLE);
        }else{
            connectionDisabled.setVisibility(View.VISIBLE);
            connectionEnabled.setVisibility(View.GONE);
        }
        connectionButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).connectArduino();
            }
        });
        simulation.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                activity.ac.send("10");
            }
        });
        activity = ((MainActivity)getActivity());


    }
}

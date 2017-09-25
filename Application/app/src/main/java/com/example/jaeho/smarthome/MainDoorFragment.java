package com.example.jaeho.smarthome;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

/**
 * Created by jaeho on 17. 7. 2.
 */

public class MainDoorFragment extends Fragment {

    Switch myswitch;
    LinearLayout auto_layout;
    TextView Autv, temp, humi;
    SeekBar aubar;
    Button bodysensor, doorbtn, ledonoff;

    int st = 1, i=0;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstancedState){

        View view = inflater.inflate(R.layout.maindoor,null);

        auto_layout = (LinearLayout)view.findViewById(R.id.D_cmd_layout_auto);

        Autv = (TextView)view.findViewById(R.id.D_Audata);
        //temp = (TextView)view.findViewById(R.id.D_temp);
        //humi = (TextView)view.findViewById(R.id.D_humi);

        aubar = (SeekBar)view.findViewById(R.id.D_autoBar);

        bodysensor = (Button)view.findViewById(R.id.D_bodysensor);
        doorbtn = (Button)view.findViewById(R.id.D_door);
        ledonoff = (Button)view.findViewById(R.id.D_ledonoff);

        myswitch = (Switch)view.findViewById(R.id.D_auto_switch);

        myswitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                if (isChecked) { ///not Auto -> Auto

                    myswitch.setText("Auto");

                    disableEnableControls(true,auto_layout);

                    st = 0;

                }
            }
        });


        myswitch.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if(!myswitch.isChecked()){

                    myswitch.setText("Auto");

                    disableEnableControls(true,auto_layout);

                    st = 0;

                    if (ledonoff.getText().equals("LED OFF")) {
                        ((MainActivity) getActivity()).formatString("0", 46);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("2", 46);
                    }

                    return true;
                }

                else {
                    myswitch.setText("Passive");

                    disableEnableControls(false, auto_layout);

                    st = 1;

                    if (ledonoff.getText().equals("LED OFF")) {
                        ((MainActivity) getActivity()).formatString("1", 46);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("3", 46);
                    }
                }

                return false;
            }
        });

        if (st == 1) {

            seekbarChangecControls(aubar,Autv,R.id.D_autoBar);

            buttonChangeControl(bodysensor);
            buttonChangeControl(doorbtn);
            buttonChangeControl(ledonoff);
        }

        return view;
    }

    public void buttonChangeControl(final Button button){

        button.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {

                i = 1-i;

                if (button.equals(bodysensor)) {

                    if(i==0) {

                        if (button.getText().equals("BodySensor ON")) {
                            button.setText("BodySensor OFF");
                            ((MainActivity)getActivity()).formatString("0",44);
                        }
                        else if(button.getText().equals("BodySensor OFF")){
                            button.setText("BodySensor ON");
                            ((MainActivity)getActivity()).formatString("1",44);
                        }
                    }

                    else {
                        if (button.getText().equals("BodySensor ON")) {
                            button.setText("BodySensor OFF");
                            ((MainActivity)getActivity()).formatString("0",44);
                        }
                        else if(button.getText().equals("BodySensor OFF")){
                            button.setText("BodySensor ON");
                            ((MainActivity)getActivity()).formatString("1",44);
                        }
                    }
                }

                else if(button.equals(doorbtn)){

                    if(i==0) {

                        if(button.getText().equals("Door Open")){
                            button.setText("Door Close");
                            ((MainActivity)getActivity()).formatString("0",45);
                        }
                        else if(button.getText().equals("Door Close")){
                            button.setText("Door Open");
                            ((MainActivity)getActivity()).formatString("1",45);
                        }
                    }

                    else {
                        if(button.getText().equals("Door Open")){
                            button.setText("Door Close");
                            ((MainActivity)getActivity()).formatString("0",45);
                        }
                        else if(button.getText().equals("Door Close")){
                            button.setText("Door Open");
                            ((MainActivity)getActivity()).formatString("1",45);
                        }
                    }
                }

                else if(button.equals(ledonoff)){

                    if(i==0) {

                        if(button.getText().equals("LED ON")){
                            if(myswitch.isChecked()){
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("0", 46);
                            }
                        }
                        else if(button.getText().equals("LED OFF")){
                            if(myswitch.isChecked()){
                                button.setText("LED ON");
                                ((MainActivity)getActivity()).formatString("2",46);
                            }
                        }
                    }

                    else {
                        if(button.getText().equals("LED ON")){
                            if(myswitch.isChecked()){
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("0", 46);
                            }
                        }
                        else if(button.getText().equals("LED OFF")){
                           if(myswitch.isChecked()){
                                button.setText("LED ON");
                                ((MainActivity)getActivity()).formatString("2",46);
                            }
                        }
                    }
                }
            }
        });
    }


    public void seekbarChangecControls(SeekBar seekbar, final TextView textview, final int id){

        textview.setText("0");

        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

                seekBar.getProgress();

                if(R.id.D_autoBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),47);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                seekBar.getProgress();
            }

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                textview.setText(""+ progress);
            }
        });

    }

    private void disableEnableControls(boolean enable, ViewGroup vg){

        for (int i = 0; i < vg.getChildCount(); i++){

            View child = vg.getChildAt(i);

            child.setEnabled(enable);

            if (child instanceof ViewGroup){
                disableEnableControls(enable, (ViewGroup)child);
            }
        }
    }
}

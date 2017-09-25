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
 * Created by jaeho on 17. 6. 29.
 */

public class Room2Fragment extends Fragment {

    Switch myswitch;
    LinearLayout mylayout,auto_layout;
    TextView Rtv, Gtv, Btv, Autv, temp, humi,fire;
    SeekBar redbar, greenbar, bluebar, aubar;
    Button doorbtn, winbtn, ledonoff;

    int st = 1, i=0;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstancedState){

        View view = inflater.inflate(R.layout.room2,null);

        mylayout = (LinearLayout) view.findViewById(R.id.R2_cmd_layout);
        auto_layout = (LinearLayout)view.findViewById(R.id.R2_cmd_layout_auto);

        Rtv = (TextView)view.findViewById(R.id.R2_Rdata);
        Gtv = (TextView)view.findViewById(R.id.R2_Gdata);
        Btv = (TextView)view.findViewById(R.id.R2_Bdata);
        Autv = (TextView)view.findViewById(R.id.R2_Audata);

        temp = (TextView)view.findViewById(R.id.R2_temp);
        humi = (TextView)view.findViewById(R.id.R2_humi);
        fire = (TextView)view.findViewById(R.id.R2_Fire);

        redbar = (SeekBar)view.findViewById(R.id.R2_redBar);
        greenbar = (SeekBar)view.findViewById(R.id.R2_greenBar);
        bluebar = (SeekBar)view.findViewById(R.id.R2_blueBar);
        aubar = (SeekBar)view.findViewById(R.id.R2_autoBar);

        doorbtn = (Button)view.findViewById(R.id.R2_door);
        winbtn = (Button)view.findViewById(R.id.R2_window);
        ledonoff = (Button)view.findViewById(R.id.R2_ledonoff);

        myswitch = (Switch)view.findViewById(R.id.R2_auto_switch);

        myswitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                if (isChecked) { ///not Auto -> Auto

                    myswitch.setText("Auto");

                    disableEnableControls(false, mylayout);
                    disableEnableControls(true,auto_layout);

                    st = 0;

                }

                else { ////Auto -> not Auto

                    myswitch.setText("Passive");

                    disableEnableControls(true, mylayout);
                    disableEnableControls(false, auto_layout);

                    st = 1;

                }
            }
        });

        myswitch.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if(!myswitch.isChecked()){

                    myswitch.setText("Auto");

                    disableEnableControls(false, mylayout);
                    disableEnableControls(true,auto_layout);

                    st = 0;

                    if (ledonoff.getText().equals("LED OFF")) {
                        ((MainActivity) getActivity()).formatString("0", 24);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("2", 24);
                    }

                    return true;
                }

                else {
                    myswitch.setText("Passive");

                    disableEnableControls(true, mylayout);
                    disableEnableControls(false, auto_layout);

                    st = 1;

                    if (ledonoff.getText().equals("LED OFF")) {
                        ((MainActivity) getActivity()).formatString("1", 24);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("3", 24);
                    }
                }

                return false;
            }
        });


        if (st == 1) {

            seekbarChangecControls(redbar, Rtv, R.id.R2_redBar);
            seekbarChangecControls(greenbar, Gtv,R.id.R2_greenBar);
            seekbarChangecControls(bluebar, Btv,R.id.R2_blueBar);
            seekbarChangecControls(aubar,Autv,R.id.R2_autoBar);

            buttonChangeControl(doorbtn);
            buttonChangeControl(winbtn);
            buttonChangeControl(ledonoff);
        }

        return view;
    }


    private void buttonChangeControl(final Button button){

        button.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {

                i = 1-i;

                if (button.equals(winbtn)) {

                    if(i==0) {

                        if (button.getText().equals("Window Open")) {
                            button.setText("Window Close");
                            ((MainActivity)getActivity()).formatString("0",22);
                        }
                        else if(button.getText().equals("Window Close")){
                            button.setText("Window Open");
                            ((MainActivity)getActivity()).formatString("1",22);
                        }
                    }

                    else {
                        if (button.getText().equals("Window Open")) {
                            button.setText("Window Close");
                            ((MainActivity)getActivity()).formatString("0",22);
                        }
                        else if(button.getText().equals("Window Close")){
                            button.setText("Window Open");
                            ((MainActivity)getActivity()).formatString("1",22);
                        }
                    }
                }

                else if(button.equals(doorbtn)){

                    if(i==0) {

                        if(button.getText().equals("Door Open")){
                            button.setText("Door Close");
                            ((MainActivity)getActivity()).formatString("0",23);
                        }
                        else if(button.getText().equals("Door Close")){
                            button.setText("Door Open");
                            ((MainActivity)getActivity()).formatString("1",23);
                        }
                    }

                    else {
                        if(button.getText().equals("Door Open")){
                            button.setText("Door Close");
                            ((MainActivity)getActivity()).formatString("0",23);
                        }
                        else if(button.getText().equals("Door Close")){
                            button.setText("Door Open");
                            ((MainActivity)getActivity()).formatString("1",23);
                        }
                    }
                }

                else if(button.equals(ledonoff)){

                    if(i==0) {

                        if(button.getText().equals("LED ON")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("1", 24);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("0", 24);
                            }
                        }

                        else if(button.getText().equals("LED OFF")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED ON");
                                ((MainActivity) getActivity()).formatString("3", 24);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED ON");
                                ((MainActivity)getActivity()).formatString("2",24);
                            }
                        }
                    }

                    else {
                        if(button.getText().equals("LED ON")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("1", 24);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("0", 24);
                            }
                        }

                        else if(button.getText().equals("LED OFF")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED ON");
                                ((MainActivity) getActivity()).formatString("3", 24);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED ON");
                                ((MainActivity)getActivity()).formatString("2",24);
                            }
                        }
                    }
                }
            }
        });
    }

    private void seekbarChangecControls(SeekBar seekbar, final TextView textview, final int id){

        textview.setText("0");

        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

                seekBar.getProgress();

                if(R.id.R2_redBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),25);
                else if(R.id.R2_greenBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),26);
                else if(R.id.R2_blueBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),27);
                else if(R.id.R2_autoBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),28);
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

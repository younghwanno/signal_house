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

public class KitchenFragment extends Fragment {

    Switch myswitch;
    LinearLayout mylayout,auto_layout;
    TextView Rtv, Gtv, Btv, Autv, temp, humi, lpg, fire;
    SeekBar redbar, greenbar, bluebar, aubar;
    Button winbtn1, winbtn2, ventilfan, ledonoff;

    int st = 1, i=0;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstancedState){

        View view = inflater.inflate(R.layout.kitchen,null);

        mylayout = (LinearLayout) view.findViewById(R.id.K_cmd_layout);
        auto_layout = (LinearLayout)view.findViewById(R.id.K_cmd_layout_auto);

        Rtv = (TextView)view.findViewById(R.id.K_Rdata);
        Gtv = (TextView)view.findViewById(R.id.K_Gdata);
        Btv = (TextView)view.findViewById(R.id.K_Bdata);
        Autv = (TextView)view.findViewById(R.id.K_Audata);

        temp = (TextView)view.findViewById(R.id.K_temp);
        humi = (TextView)view.findViewById(R.id.K_humi);
        lpg = (TextView)view.findViewById(R.id.K_LPG);
        fire = (TextView)view.findViewById(R.id.K_Fire);

        redbar = (SeekBar)view.findViewById(R.id.K_redBar);
        greenbar = (SeekBar)view.findViewById(R.id.K_greenBar);
        bluebar = (SeekBar)view.findViewById(R.id.K_blueBar);
        aubar = (SeekBar)view.findViewById(R.id.K_autoBar);

        winbtn1 = (Button)view.findViewById(R.id.K_window1);
        winbtn2 = (Button)view.findViewById(R.id.K_window2);
        ventilfan = (Button)view.findViewById(R.id.K_ventilfan);
        ledonoff = (Button)view.findViewById(R.id.K_ledonoff);

        myswitch = (Switch)view.findViewById(R.id.K_auto_switch);

        myswitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                if (isChecked) { ///not Auto -> Auto

                    myswitch.setText("Auto");

                    disableEnableControls(false, mylayout);
                    disableEnableControls(true,auto_layout);

                    st = 0;

                } else { ////Auto -> not Auto

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
                        ((MainActivity) getActivity()).formatString("0", 10);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("2", 10);
                    }

                    return true;
                }
                else {
                    myswitch.setText("Passive");

                    disableEnableControls(true, mylayout);
                    disableEnableControls(false, auto_layout);

                    st = 1;

                    if (ledonoff.getText().equals("LED OFF")) {
                        ((MainActivity) getActivity()).formatString("1", 10);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("3", 10);
                    }
                }

                return false;
            }
        });

        if (st == 1) {

            seekbarChangecControls(redbar, Rtv,R.id.K_redBar);
            seekbarChangecControls(greenbar, Gtv,R.id.K_greenBar);
            seekbarChangecControls(bluebar, Btv,R.id.K_blueBar);
            seekbarChangecControls(aubar,Autv,R.id.K_autoBar);

            buttonChangeControl(winbtn1);
            buttonChangeControl(winbtn2);
            buttonChangeControl(ventilfan);
            buttonChangeControl(ledonoff);
        }

        return view;
    }


    private void buttonChangeControl(final Button button){

        button.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View v) {

                i = 1-i;

                if (button.equals(winbtn1)) {

                    if(i==0) {

                        if (button.getText().equals("Window1 Open")) {
                            button.setText("Window1 Close");
                            ((MainActivity)getActivity()).formatString("0",7);
                        }
                        else if(button.getText().equals("Window1 Close")){
                            button.setText("Window1 Open");
                            ((MainActivity)getActivity()).formatString("1",7);
                        }
                    }
                    else {

                        if (button.getText().equals("Window1 Open")) {
                            button.setText("Window1 Close");
                            ((MainActivity)getActivity()).formatString("0",7);
                        }
                        else if(button.getText().equals("Window1 Close")){
                            button.setText("Window1 Open");
                            ((MainActivity)getActivity()).formatString("1",7);
                        }
                    }
                }

                else if(button.equals(winbtn2)){

                    if(i==0) {

                        if(button.getText().equals("Window2 Open")){
                            button.setText("Window2 Close");
                            ((MainActivity)getActivity()).formatString("0",8);
                        }
                        else if(button.getText().equals("Window2 Close")){
                            button.setText("Window2 Open");
                            ((MainActivity)getActivity()).formatString("1",8);
                        }
                    }

                    else {

                        if(button.getText().equals("Window2 Open")){
                            button.setText("Window2 Close");
                            ((MainActivity)getActivity()).formatString("0",8);
                        }
                        else if(button.getText().equals("Window2 Close")){
                            button.setText("Window2 Open");
                            ((MainActivity)getActivity()).formatString("1",8);
                        }
                    }
                }
                else if(button.equals(ventilfan)){

                    if(i==0){

                        if(button.getText().equals("Ventil Fan ON")){
                            button.setText("Ventil Fan OFF");
                            ((MainActivity)getActivity()).formatString("0",9);
                        }
                        else if(button.getText().equals("Ventil Fan OFF")){
                            button.setText("Ventil Fan ON");
                            ((MainActivity)getActivity()).formatString("1",9);
                        }
                    }

                    else {

                        if(button.getText().equals("Ventil Fan ON")){
                            button.setText("Ventil Fan OFF");
                            ((MainActivity)getActivity()).formatString("0",9);
                        }
                        else if(button.getText().equals("Ventil Fan OFF")){
                            button.setText("Ventil Fan ON");
                            ((MainActivity)getActivity()).formatString("1",9);
                        }
                    }

                }
                else if(button.equals(ledonoff)){

                    if(i==0)if(button.getText().equals("LED ON")){

                        if(!myswitch.isChecked()) {
                            button.setText("LED OFF");
                            ((MainActivity) getActivity()).formatString("1", 10);
                        }
                        else if(myswitch.isChecked()){
                            button.setText("LED OFF");
                            ((MainActivity) getActivity()).formatString("0", 10);
                        }
                    }

                    else if(button.getText().equals("LED OFF")){

                        if(!myswitch.isChecked()) {
                            button.setText("LED ON");
                            ((MainActivity) getActivity()).formatString("3", 10);
                        }
                        else if(myswitch.isChecked()){
                            button.setText("LED ON");
                            ((MainActivity)getActivity()).formatString("2",10);
                        }
                    }
                }

                else {

                    if(button.getText().equals("LED ON")){

                        if(!myswitch.isChecked()) {
                            button.setText("LED OFF");
                            ((MainActivity) getActivity()).formatString("1", 10);
                        }
                        else if(myswitch.isChecked()){
                            button.setText("LED OFF");
                            ((MainActivity) getActivity()).formatString("0", 10);
                        }
                    }

                    else if(button.getText().equals("LED OFF")){

                        if(!myswitch.isChecked()) {
                            button.setText("LED ON");
                            ((MainActivity) getActivity()).formatString("3", 10);
                        }
                        else if(myswitch.isChecked()){
                            button.setText("LED ON");
                            ((MainActivity)getActivity()).formatString("2",10);
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

                if(R.id.K_redBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),11);
                else if(R.id.K_greenBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),12);
                else if(R.id.K_blueBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),13);
                else if(R.id.K_autoBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),14);
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

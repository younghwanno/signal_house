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

public class ToiletFragment extends Fragment {

    Switch myswitch;
    LinearLayout mylayout,auto_layout;
    TextView Rtv, Gtv, Btv, Autv, temp, humi;
    SeekBar redbar, greenbar, bluebar, aubar;
    Button doorbtn, ventilfan, ledonoff;

    int st = 1, i=0;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstancedState){

        View view = inflater.inflate(R.layout.toilet,null);

        mylayout = (LinearLayout) view.findViewById(R.id.T_cmd_layout);
        auto_layout = (LinearLayout)view.findViewById(R.id.T_cmd_layout_auto);

        Rtv = (TextView)view.findViewById(R.id.T_Rdata);
        Gtv = (TextView)view.findViewById(R.id.T_Gdata);
        Btv = (TextView)view.findViewById(R.id.T_Bdata);
        Autv = (TextView)view.findViewById(R.id.T_Audata);

        temp = (TextView)view.findViewById(R.id.T_temp);
        humi = (TextView)view.findViewById(R.id.T_humi);

        redbar = (SeekBar)view.findViewById(R.id.T_redBar);
        greenbar = (SeekBar)view.findViewById(R.id.T_greenBar);
        bluebar = (SeekBar)view.findViewById(R.id.T_blueBar);
        aubar = (SeekBar)view.findViewById(R.id.T_autoBar);

        doorbtn = (Button)view.findViewById(R.id.T_door);
        ventilfan = (Button)view.findViewById(R.id.T_ventilfan);
        ledonoff = (Button)view.findViewById(R.id.T_ledonoff);

        myswitch = (Switch)view.findViewById(R.id.T_auto_switch);


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
                        ((MainActivity) getActivity()).formatString("0", 31);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("2", 31);
                    }

                    return true;
                }

                else {
                    myswitch.setText("Passive");

                    disableEnableControls(true, mylayout);
                    disableEnableControls(false, auto_layout);

                    st = 1;

                    if (ledonoff.getText().equals("LED OFF")) {
                        ((MainActivity) getActivity()).formatString("1", 31);
                    } else if (ledonoff.getText().equals("LED ON")) {
                        ((MainActivity) getActivity()).formatString("3", 31);
                    }
                }

                return false;
            }
        });


        if (st == 1) {

            seekbarChangecControls(redbar, Rtv,R.id.T_redBar);
            seekbarChangecControls(greenbar, Gtv,R.id.T_greenBar);
            seekbarChangecControls(bluebar, Btv,R.id.T_blueBar);
            seekbarChangecControls(aubar,Autv,R.id.T_autoBar);

            buttonChangeControl(doorbtn);
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

                if (button.equals(ventilfan)) {

                    if(i==0) {

                        if (button.getText().equals("Ventil Fan ON")) {
                            button.setText("Ventil Fan OFF");
                            ((MainActivity)getActivity()).formatString("0",29);
                        }
                        else if(button.getText().equals("Ventil Fan OFF")){
                            button.setText("Ventil Fan ON");
                            ((MainActivity)getActivity()).formatString("1",29);
                        }
                    }

                    else {
                        if (button.getText().equals("Ventil Fan ON")) {
                            button.setText("Ventil Fan OFF");
                            ((MainActivity)getActivity()).formatString("0",29);
                        }
                        else if(button.getText().equals("Ventil Fan OFF")){
                            button.setText("Ventil Fan ON");
                            ((MainActivity)getActivity()).formatString("1",29);
                        }
                    }
                }

                else if(button.equals(doorbtn)){

                    if(i==0) {

                        if(button.getText().equals("Door Open")){
                            button.setText("Door Close");
                            ((MainActivity)getActivity()).formatString("0",30);
                        }
                        else if(button.getText().equals("Door Close")){
                            button.setText("Door Open");
                            ((MainActivity)getActivity()).formatString("1",30);
                        }
                    }

                    else {
                        if(button.getText().equals("Door Open")){
                            button.setText("Door Close");
                            ((MainActivity)getActivity()).formatString("0",30);
                        }
                        else if(button.getText().equals("Door Close")){
                            button.setText("Door Open");
                            ((MainActivity)getActivity()).formatString("1",30);
                        }
                    }
                }

                else if(button.equals(ledonoff)){

                    if(i==0) {

                        if(button.getText().equals("LED ON")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("1", 31);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("0", 31);
                            }
                        }

                        else if(button.getText().equals("LED OFF")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED ON");
                                ((MainActivity) getActivity()).formatString("3", 31);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED ON");
                                ((MainActivity)getActivity()).formatString("2",31);
                            }
                        }
                    }

                    else {
                        if(button.getText().equals("LED ON")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("1", 31);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED OFF");
                                ((MainActivity) getActivity()).formatString("0", 31);
                            }
                        }

                        else if(button.getText().equals("LED OFF")){
                            if(!myswitch.isChecked()) {
                                button.setText("LED ON");
                                ((MainActivity) getActivity()).formatString("3", 31);
                            }
                            else if(myswitch.isChecked()){
                                button.setText("LED ON");
                                ((MainActivity)getActivity()).formatString("2",31);
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

                if(R.id.T_redBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),32);
                else if(R.id.T_greenBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),33);
                else if(R.id.T_blueBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),34);
                else if(R.id.T_autoBar == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),35);
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

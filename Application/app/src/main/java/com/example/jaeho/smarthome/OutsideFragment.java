package com.example.jaeho.smarthome;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

/**
 * Created by jaeho on 17. 6. 29.
 */

public class OutsideFragment extends Fragment {

    TextView temp, humi, battery, uv, dust, solardata;
    Button stob, stod, btod, carhousedoor;
    SeekBar solarangle;

    int i = 0;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstancedState){


        View view = inflater.inflate(R.layout.outside,null);

        temp = (TextView)view.findViewById(R.id.O_temp);
        humi = (TextView)view.findViewById(R.id.O_humi);
        battery = (TextView)view.findViewById(R.id.O_battery);
        uv = (TextView)view.findViewById(R.id.O_UV);
        dust = (TextView)view.findViewById(R.id.O_Dust);
        solardata = (TextView)view.findViewById(R.id.O_solordata);

        //stob = (Button)view.findViewById(R.id.O_StoB);
        //stod = (Button)view.findViewById(R.id.O_StoD);
        //btod = (Button)view.findViewById(R.id.O_BtoD);
        carhousedoor = (Button)view.findViewById(R.id.carhousedoor);

        solarangle = (SeekBar)view.findViewById(R.id.O_solorangle);

        //buttonChangeControl(stob);
        //buttonChangeControl(stod);
        //buttonChangeControl(btod);
        buttonChangeControl(carhousedoor);

        seekbarChangecControls(solarangle,solardata,R.id.O_solordata);

        return view;
    }

    public void buttonChangeControl(final Button button){

        button.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {

                i = 1-i;

                if (button.equals(stob)) {

                    if(i==0) {

                        if (button.getText().equals("S->B ON")) {
                            button.setText("S->B OFF");
                            ((MainActivity)getActivity()).formatString("0",48);
                        }
                        else if(button.getText().equals("S->B OFF")){
                            button.setText("S->B ON");
                            ((MainActivity)getActivity()).formatString("1",48);
                        }
                    }

                    else {
                        if (button.getText().equals("S->B ON")) {
                            button.setText("S->B OFF");
                            ((MainActivity)getActivity()).formatString("0",48);
                        }
                        else if(button.getText().equals("S->B OFF")){
                            button.setText("S->B ON");
                            ((MainActivity)getActivity()).formatString("1",48);
                        }
                    }
                }

                else if(button.equals(stod)){

                    if(i==0) {

                        if(button.getText().equals("S->D ON")){
                            button.setText("S->D OFF");
                            ((MainActivity)getActivity()).formatString("0",49);
                        }
                        else if(button.getText().equals("S->D OFF")){
                            button.setText("S->D ON");
                            ((MainActivity)getActivity()).formatString("1",49);
                        }
                    }

                    else {
                        if(button.getText().equals("S->D ON")){
                            button.setText("S->D OFF");
                            ((MainActivity)getActivity()).formatString("0",49);
                        }
                        else if(button.getText().equals("S->D OFF")){
                            button.setText("S->D ON");
                            ((MainActivity)getActivity()).formatString("1",49);
                        }
                    }
                }

                else if(button.equals(btod)){

                    if(i==0) {

                        if(button.getText().equals("B->D ON")){
                            button.setText("B->D OFF");
                            ((MainActivity)getActivity()).formatString("0",50);
                        }
                        else if(button.getText().equals("B->D OFF")){
                            button.setText("B->D ON");
                            ((MainActivity)getActivity()).formatString("1",50);
                        }
                    }

                    else {
                        if(button.getText().equals("B->D ON")){
                            button.setText("B->D OFF");
                            ((MainActivity)getActivity()).formatString("0",50);
                        }
                        else if(button.getText().equals("B->D OFF")){
                            button.setText("B->D ON");
                            ((MainActivity)getActivity()).formatString("1",50);
                        }
                    }
                }

                else if(button.equals(carhousedoor)){

                    if(i==0) {

                        if(button.getText().equals("Car House Door Open")){
                            button.setText("Car House Door Close");
                            ((MainActivity)getActivity()).formatString("0",57);
                        }
                        else if(button.getText().equals("Car House Door Close")){
                            button.setText("Car House Door Open");
                            ((MainActivity)getActivity()).formatString("1",57);
                        }
                    }

                    else {
                        if(button.getText().equals("Car House Door Open")){
                            button.setText("Car House Door Close");
                            ((MainActivity)getActivity()).formatString("0",57);
                        }
                        else if(button.getText().equals("Car House Door Close")){
                            button.setText("Car House Door Open");
                            ((MainActivity)getActivity()).formatString("1",57);
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

                if(R.id.O_solordata == id)
                    ((MainActivity)getActivity()).formatString(Integer.toString(seekBar.getProgress()),56);
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
}

package com.example.jaeho.smarthome;

import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TabHost;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {

    static final int SERVER_CONN_START = -3;
    static final int SERVER_CONN_FAILED = -4;
    static final int SERVER_CONN_SUCCESS = -5;
    static final int SERVER_CONN_END = -6;
    static final int SERVER_CONN_TIMEOUT = -7;
    static final int LOGIN_SUCCESS = -8;
    static final int LOGIN_FAIL = -9;

    mHandler handler;

    static Socket socket;
    static BufferedReader socket_in;
    static BufferedWriter socket_out;
    static PrintWriter p_socket_out;

    private final long FINISH_INTERVAL_TIME = 500;
    private long   backPressedTime = 0;

    public FragmentManager fragmentManager;
    public FragmentTransaction fragmentTransaction;


    LinearLayout Multi, Room1, Room2, Kitchen, Toilet, Maindoor;
    RelativeLayout Living;
    RelativeLayout intro, interFace;
    FrameLayout mainlayout;

    Switch myswitch;
    TextView weather;
    Button heatingPump, boiler, allDoor, allWindow;

    String split_String[] = new String[85];
    String receiveString = new String();

    int action;
    int i=0;

    public String myString[] = new String[85];
    public String yourString[] = new String[58];
    public String freeString[] = new String[58];


    StartIntro startIntroFragment = new StartIntro();
    LivingRoomFragment livingRoomFragment = new LivingRoomFragment();
    MultiRoomFragment multiRoomFragment = new MultiRoomFragment();
    Room1Fragment room1Fragment = new Room1Fragment();
    Room2Fragment room2Fragment = new Room2Fragment();
    KitchenFragment kitchenFragment = new KitchenFragment();
    ToiletFragment toiletFragment = new ToiletFragment();
    OutsideFragment commonDataFragment = new OutsideFragment();
    MainDoorFragment mainDoorFragment = new MainDoorFragment();
    OutsideFragment outsideFragment = new OutsideFragment();


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        handler = new mHandler();

        for(int i=0;i<yourString.length;i++) {
            yourString[i] = "0";
        }

        mainlayout = (FrameLayout)findViewById(R.id.mainlayout);
        Living = (RelativeLayout)findViewById(R.id.livingroom);
        intro = (RelativeLayout)findViewById(R.id.intro);
        interFace = (RelativeLayout)findViewById(R.id.interFace);
        Multi = (LinearLayout)findViewById(R.id.multi);
        Room1 = (LinearLayout)findViewById(R.id.room1);
        Room2 = (LinearLayout)findViewById(R.id.room2);
        Kitchen = (LinearLayout)findViewById(R.id.kitchen);
        Toilet = (LinearLayout)findViewById(R.id.toilet);
        Maindoor = (LinearLayout)findViewById(R.id.maindoor);

        weather = (TextView)findViewById(R.id.weather);
        myswitch = (Switch)findViewById(R.id.O_auto_switch);
        heatingPump = (Button)findViewById(R.id.O_heating_pump);
        boiler = (Button)findViewById(R.id.O_boiler);
        allDoor = (Button)findViewById(R.id.O_all_door);
        allWindow = (Button)findViewById(R.id.O_all_window);


        fragmentManager = getSupportFragmentManager();

        fragmentTransaction = fragmentManager.beginTransaction();

        fragmentTransaction.replace(R.id.intro,startIntroFragment);
        fragmentTransaction.replace(R.id.LV,livingRoomFragment);
        fragmentTransaction.replace(R.id.MU,multiRoomFragment);
        fragmentTransaction.replace(R.id.R1,room1Fragment);
        fragmentTransaction.replace(R.id.R2,room2Fragment);
        fragmentTransaction.replace(R.id.KT,kitchenFragment);
        fragmentTransaction.replace(R.id.TL,toiletFragment);
        fragmentTransaction.replace(R.id.OT,commonDataFragment);
        fragmentTransaction.replace(R.id.MD,mainDoorFragment);
        fragmentTransaction.replace(R.id.OT,outsideFragment);

        fragmentTransaction.commit();

        final TabHost tabHost1 = (TabHost)findViewById(R.id.tabHost1);

        tabHost1.setup();


        TabHost.TabSpec ts1 = tabHost1.newTabSpec("Tab Spec 1");
        ts1.setContent(R.id.LV);
        ts1.setIndicator("LR");
        tabHost1.addTab(ts1);

        TabHost.TabSpec ts2 = tabHost1.newTabSpec("Tab Spec 2");
        ts2.setContent(R.id.KT);
        ts2.setIndicator("KT");
        tabHost1.addTab(ts2);

        TabHost.TabSpec ts3 = tabHost1.newTabSpec("Tab Spec 3");
        ts3.setContent(R.id.R1);
        ts3.setIndicator("R1");
        tabHost1.addTab(ts3);

        TabHost.TabSpec ts4 = tabHost1.newTabSpec("Tab Spec 4");
        ts4.setContent(R.id.R2);
        ts4.setIndicator("R2");
        tabHost1.addTab(ts4);

        TabHost.TabSpec ts5 = tabHost1.newTabSpec("Tab Spec 5");
        ts5.setContent(R.id.TL);
        ts5.setIndicator("TL");
        tabHost1.addTab(ts5);

        TabHost.TabSpec ts6 = tabHost1.newTabSpec("Tab Spec 6");
        ts6.setContent(R.id.MU);
        ts6.setIndicator("MR");
        tabHost1.addTab(ts6);

        TabHost.TabSpec ts7 = tabHost1.newTabSpec("Tab Spec 7");
        ts7.setContent(R.id.MD);
        ts7.setIndicator("MD");
        tabHost1.addTab(ts7);

        TabHost.TabSpec ts8 = tabHost1.newTabSpec("Tab Spec 8");
        ts8.setContent(R.id.OT);
        ts8.setIndicator("OT");
        tabHost1.addTab(ts8);


        disableEnableControls(false,interFace);

        setOnTouch(Living,tabHost1);
        setOnTouch(Kitchen,tabHost1);
        setOnTouch(Room1,tabHost1);
        setOnTouch(Room2,tabHost1);
        setOnTouch(Toilet,tabHost1);
        setOnTouch(Multi,tabHost1);
        setOnTouch(Maindoor,tabHost1);



        myswitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

                if (isChecked) { ///not Auto -> Auto
                    myswitch.setText("Auto");
                }

                else { ////Auto -> not Auto
                    myswitch.setText("Passive");
                }
            }
        });


        myswitch.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(!myswitch.isChecked()){

                    myswitch.setText("Auto");
                    formatString("0", 51);

                    return true;
                }

                else {

                    myswitch.setText("Passive");
                    formatString("1", 51);

                }
                return false;
            }
        });


        buttonChangeControl(heatingPump);
        buttonChangeControl(boiler);
        buttonChangeControl(allDoor);
        buttonChangeControl(allWindow);

    }

    public void setOnTouch(final ViewGroup viewGroup, final TabHost tabHost){
        viewGroup.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                action = event.getAction();

                if(viewGroup == Living){
                    if( action == MotionEvent.ACTION_DOWN) {
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_Living));
                        tabHost.setCurrentTab(0);
                        Toast.makeText(MainActivity.this,"Living Room",Toast.LENGTH_SHORT).show();

                    }
                    else if(action == MotionEvent.ACTION_UP){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchOn_Living));
                    }
                }

                else if(viewGroup == Kitchen){
                    if( action == MotionEvent.ACTION_DOWN) {
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_Kitchen));
                        tabHost.setCurrentTab(1);
                        Toast.makeText(MainActivity.this,"Kitchen",Toast.LENGTH_SHORT).show();
                    }
                    else if(action == MotionEvent.ACTION_UP){
                        Kitchen.setBackgroundColor(getResources().getColor(R.color.touchOn_Kitchen));
                    }
                }

                else if(viewGroup == Room1){
                    if( action == MotionEvent.ACTION_DOWN) {
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_Room1));
                        tabHost.setCurrentTab(2);
                        Toast.makeText(MainActivity.this,"Room1",Toast.LENGTH_SHORT).show();
                    }
                    else if(action == MotionEvent.ACTION_UP){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchOn_Room1));
                    }
                }

                else if(viewGroup == Room2){
                    if( action == MotionEvent.ACTION_DOWN) {
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_Room2));
                        tabHost.setCurrentTab(3);
                        Toast.makeText(MainActivity.this,"Room2",Toast.LENGTH_SHORT).show();
                    }
                    else if(action == MotionEvent.ACTION_UP){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchOn_Room2));
                    }
                }

                else if(viewGroup == Toilet){
                    if( action == MotionEvent.ACTION_DOWN) {
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_Toilet));
                        tabHost.setCurrentTab(4);
                        Toast.makeText(MainActivity.this,"Toilet",Toast.LENGTH_SHORT).show();
                    }
                    else if(action == MotionEvent.ACTION_UP){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchOn_Toilet));
                    }
                }

                else if(viewGroup == Multi){
                    if( action == MotionEvent.ACTION_DOWN) {
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_multi));
                        tabHost.setCurrentTab(5);
                        Toast.makeText(MainActivity.this,"Multi Room",Toast.LENGTH_SHORT).show();

                    }
                    else if(action == MotionEvent.ACTION_UP){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchOn_Multi));
                    }
                }

                else if(viewGroup == Maindoor){
                    if(action == MotionEvent.ACTION_DOWN){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchDown_MainDoor));
                        tabHost.setCurrentTab(6);
                        Toast.makeText(MainActivity.this,"Main Door", Toast.LENGTH_SHORT).show();
                    }
                    else if(action == MotionEvent.ACTION_UP){
                        viewGroup.setBackgroundColor(getResources().getColor(R.color.touchOn_MainDoor));
                    }
                }

                return true;
            }
        });

    }

    public void buttonChangeControl(final Button button){

        button.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View v) {

                i = 1-i;

                if (button.equals(heatingPump)) {

                    if(i==0) {

                        if (button.getText().equals("Heating Pump ON")) {
                            button.setText("Heating Pump OFF");
                            formatString("0",52);
                        }

                        else if(button.getText().equals("Heating Pump OFF")){
                            button.setText("Heating Pump ON");
                            formatString("1",52);
                        }
                    }

                    else {

                        if (button.getText().equals("Heating Pump ON")) {
                            button.setText("Heating Pump OFF");
                            formatString("0",52);
                        }

                        else if(button.getText().equals("Heating Pump OFF")){
                            button.setText("Heating Pump ON");
                            formatString("1",52);
                        }
                    }
                }

                else if(button.equals(boiler)){

                    if(i==0) {

                        if(button.getText().equals("Boiler ON")){
                            button.setText("Boiler OFF");
                            formatString("0",53);
                        }

                        else if(button.getText().equals("Boiler OFF")){
                            button.setText("Boiler ON");
                            formatString("1",53);
                        }
                    }

                    else {

                        if(button.getText().equals("Boiler ON")){
                            button.setText("Boiler OFF");
                            formatString("0",53);
                        }

                        else if(button.getText().equals("Boiler OFF")){
                            button.setText("Boiler ON");
                            formatString("1",53);
                        }
                    }
                }

                else if(button.equals(allDoor)){

                    if(i==0) {

                        if(button.getText().equals("All Door Open")){
                            button.setText("All Door Close");
                            formatString("0",54);
                        }

                        else if(button.getText().equals("All Door Close")){
                            button.setText("All Door Open");
                            formatString("1",54);
                        }
                    }

                    else {

                        if(button.getText().equals("All Door Open")){
                            button.setText("All Door Close");
                            formatString("0",54);
                        }

                        else if(button.getText().equals("All Door Close")){
                            button.setText("All Door Open");
                            formatString("1",54);
                        }
                    }
                }

                else if(button.equals(allWindow)){

                    if(i==0) {

                        if(button.getText().equals("All Window Open")){
                            button.setText("All Window Close");
                            formatString("0",55);
                        }

                        else if(button.getText().equals("All Window Close")){
                            button.setText("All Window Open");
                            formatString("1",55);
                        }
                    }

                    else {

                        if(button.getText().equals("All Window Open")){
                            button.setText("All Window Close");
                            formatString("0",55);
                        }

                        else if(button.getText().equals("All Window Close")){
                            button.setText("All Window Open");
                            formatString("1",55);
                        }
                    }
                }
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

    public void formatString(String string, int index){

        yourString[index] = string;

        if(!freeString.equals(yourString)) {

            write(String.format("E,%s,%s,%s,%s,%s,%s,%s,%s," +
                            "%s,%s,%s,%s,%s,%s,%s,%s,%s," +
                            "%s,%s,%s,%s,%s,%s,%s,%s," +
                            "%s,%s,%s,%s,%s,%s,%s,%s," +
                            "%s,%s,%s,%s,%s,%s,%s,%s," +
                            "%s,%s,%s,%s,%s,%s,%s,%s,%s," +
                            "%s,%s,%s,%s,%s,%s,%s,%s,",

                    yourString[0], yourString[1], yourString[2], yourString[3], yourString[4], yourString[5], yourString[6], yourString[7],
                    yourString[8], yourString[9], yourString[10], yourString[11], yourString[12], yourString[13], yourString[14], yourString[15], yourString[16],
                    yourString[17], yourString[18], yourString[19], yourString[20], yourString[21], yourString[22], yourString[23], yourString[24],
                    yourString[25], yourString[26], yourString[27], yourString[28], yourString[29], yourString[30], yourString[31], yourString[32],
                    yourString[33], yourString[34], yourString[35], yourString[36], yourString[37], yourString[38], yourString[39], yourString[40],
                    yourString[41], yourString[42], yourString[43], yourString[44], yourString[45], yourString[46], yourString[47], yourString[48], yourString[49],
                    yourString[50], yourString[51], yourString[52], yourString[53], yourString[54], yourString[55],yourString[56],yourString[57])
            );

            freeString[index] = yourString[index];
        }
    }
    /*
        public void write(String string){
            if(string != null){
                p_socket_out.println(string);
                Log.d("Send",string);
            }
        }
    */
    public void write(final String string){

        Thread writer = new Thread(){

            @Override
            public void run(){

                try {
                    if (string != null) {
                        p_socket_out.println(string);
                        Log.d("Send", string);
                    }
                }catch (NullPointerException e){
                    e.printStackTrace();
                }
            }
        };

        writer.start();

    }

    class Reader extends Thread{
        public void run() {
            try {
                while (true) {
                    if (socket != null) {

                        if (socket_in != null) {

                            receiveString = socket_in.readLine();

                            if (receiveString != null) {
                                Log.d("receive String", "not null");
                            } else {
                                Log.d("receive String", "null");
                            }

                            Log.d("receive", receiveString);

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {

                                    split_String = receiveString.split(",");
                                    myString = split_String;

                                    ///////////////////////// Living Room  //////////////////////////////

                                    livingRoomFragment.temp.setText("Temp : " + myString[0]);
                                    livingRoomFragment.humi.setText("Humi : " + myString[1]);
                                    livingRoomFragment.co2.setText("CO2 : " + myString[2]);
                                    livingRoomFragment.smoke.setText("Smoke : " + myString[3]);


                                    if(myString[4].equals("0"))
                                        livingRoomFragment.fire.setText("Not Fired");

                                    else if(myString[4].equals("1"))
                                        livingRoomFragment.fire.setText("Fired!!!!");


                                    if(myString[5].equals("0")){
                                        livingRoomFragment.myswitch.setChecked(true);
                                        livingRoomFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if(myString[5].equals("1")){
                                        livingRoomFragment.myswitch.setChecked(false);
                                        livingRoomFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[5].equals("2")){
                                        livingRoomFragment.myswitch.setChecked(true);
                                        livingRoomFragment.ledonoff.setText("LED ON");
                                    }
                                    else if(myString[5].equals("3")){
                                        livingRoomFragment.myswitch.setChecked(false);
                                        livingRoomFragment.ledonoff.setText("LED ON");
                                    }


                                    livingRoomFragment.redbar.setProgress(Integer.parseInt(myString[6]));
                                    livingRoomFragment.greenbar.setProgress(Integer.parseInt(myString[7]));
                                    livingRoomFragment.bluebar.setProgress(Integer.parseInt(myString[8]));
                                    livingRoomFragment.aubar.setProgress(Integer.parseInt(myString[9]));


                                    if(myString[10].equals("0"))
                                        livingRoomFragment.winbtn1.setText("Window1 Close");
                                    else if(myString[10].equals("1"))
                                        livingRoomFragment.winbtn1.setText("Window1 Open");


                                    if(myString[11].equals("0"))
                                        livingRoomFragment.winbtn2.setText("Window2 Close");
                                    else if(myString[11].equals("1"))
                                        livingRoomFragment.winbtn2.setText("Window2 Open");

                                    /////////////////////////////////////////////////////////////////////////


                                    ///////////////////////////  Kitchen  //////////////////////////////////////
                                    kitchenFragment.temp.setText("Temp : " + myString[12]);
                                    kitchenFragment.humi.setText("Humi : " + myString[13]);
                                    kitchenFragment.lpg.setText("LPG : " + myString[14]);


                                    if(myString[15].equals("0"))
                                        kitchenFragment.fire.setText("Not Fired");

                                    else if(myString[15].equals("1"))
                                        kitchenFragment.fire.setText("Fired!!!!");


                                    if(myString[16].equals("0")){
                                        kitchenFragment.myswitch.setChecked(true);
                                        kitchenFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if(myString[16].equals("1")){
                                        kitchenFragment.myswitch.setChecked(false);
                                        kitchenFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[16].equals("2")){
                                        kitchenFragment.myswitch.setChecked(true);
                                        kitchenFragment.ledonoff.setText("LED ON");
                                    }
                                    else if(myString[16].equals("3")){
                                        kitchenFragment.myswitch.setChecked(false);
                                        kitchenFragment.ledonoff.setText("LED ON");
                                    }


                                    kitchenFragment.redbar.setProgress(Integer.parseInt(myString[17]));
                                    kitchenFragment.greenbar.setProgress(Integer.parseInt(myString[18]));
                                    kitchenFragment.bluebar.setProgress(Integer.parseInt(myString[19]));
                                    kitchenFragment.aubar.setProgress(Integer.parseInt(myString[20]));


                                    if(myString[21].equals("0"))
                                        kitchenFragment.winbtn1.setText("Window1 Close");
                                    else if(myString[21].equals("1"))
                                        kitchenFragment.winbtn1.setText("Window1 Open");


                                    if(myString[22].equals("0"))
                                        kitchenFragment.winbtn2.setText("Window2 Close");
                                    else if(myString[22].equals("1"))
                                        kitchenFragment.winbtn2.setText("Window2 Open");


                                    if(myString[23].equals("0"))
                                        kitchenFragment.ventilfan.setText("Ventil Fan OFF");
                                    else if(myString[23].equals("1"))
                                        kitchenFragment.ventilfan.setText("Ventil Fan ON");

                                    ///////////////////////////////////////////////////////////////////////////


                                    /////////////////////////// Room 1 //////////////////////////////////////

                                    room1Fragment.temp.setText("Temp : " + myString[24]);
                                    room1Fragment.humi.setText("Humi : " + myString[25]);


                                    if(myString[26].equals("0"))
                                        room1Fragment.fire.setText("Not Fired");
                                    else if(myString[26].equals("1"))
                                        room1Fragment.fire.setText("Fired!!!!");


                                    if(myString[27].equals("0")){
                                        room1Fragment.myswitch.setChecked(true);
                                        room1Fragment.ledonoff.setText("LED OFF");
                                    }
                                    else if(myString[27].equals("1")){
                                        room1Fragment.myswitch.setChecked(false);
                                        room1Fragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[27].equals("2")){
                                        room1Fragment.myswitch.setChecked(true);
                                        room1Fragment.ledonoff.setText("LED ON");
                                    }
                                    else if(myString[27].equals("3")){
                                        room1Fragment.myswitch.setChecked(false);
                                        room1Fragment.ledonoff.setText("LED ON");
                                    }


                                    room1Fragment.redbar.setProgress(Integer.parseInt(myString[28]));
                                    room1Fragment.greenbar.setProgress(Integer.parseInt(myString[29]));
                                    room1Fragment.bluebar.setProgress(Integer.parseInt(myString[30]));
                                    room1Fragment.aubar.setProgress(Integer.parseInt(myString[31]));


                                    if(myString[32].equals("0"))
                                        room1Fragment.winbtn.setText("Window Close");
                                    else if(myString[32].equals("1"))
                                        room1Fragment.winbtn.setText("Window Open");


                                    if(myString[33].equals("0"))
                                        room1Fragment.doorbtn.setText("Door Close");
                                    else if(myString[33].equals("1"))
                                        room1Fragment.doorbtn.setText("Door Open");

                                    /////////////////////////////////////////////////////////////////////////////


                                    /////////////////////////////  Room 2  /////////////////////////////////////

                                    room2Fragment.temp.setText("Temp : " + myString[34]);
                                    room2Fragment.humi.setText("Humi : " + myString[35]);


                                    if(myString[36].equals("0"))
                                        room2Fragment.fire.setText("Not Fired");
                                    else if(myString[36].equals("1"))
                                        room2Fragment.fire.setText("Fired!!!!");


                                    if(myString[37].equals("0")){
                                        room2Fragment.myswitch.setChecked(true);
                                        room2Fragment.ledonoff.setText("LED OFF");
                                    }
                                    else if(myString[37].equals("1")){
                                        room2Fragment.myswitch.setChecked(false);
                                        room2Fragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[37].equals("2")){
                                        room2Fragment.myswitch.setChecked(true);
                                        room2Fragment.ledonoff.setText("LED ON");
                                    }
                                    else if(myString[37].equals("3")){
                                        room2Fragment.myswitch.setChecked(false);
                                        room2Fragment.ledonoff.setText("LED ON");
                                    }


                                    room2Fragment.redbar.setProgress(Integer.parseInt(myString[38]));
                                    room2Fragment.greenbar.setProgress(Integer.parseInt(myString[39]));
                                    room2Fragment.bluebar.setProgress(Integer.parseInt(myString[40]));
                                    room2Fragment.aubar.setProgress(Integer.parseInt(myString[41]));


                                    if(myString[42].equals("0"))
                                        room2Fragment.winbtn.setText("Window Close");
                                    else if(myString[42].equals("1"))
                                        room2Fragment.winbtn.setText("Window Open");


                                    if(myString[43].equals("0"))
                                        room2Fragment.doorbtn.setText("Door Close");
                                    else if(myString[43].equals("1"))
                                        room2Fragment.doorbtn.setText("Door Open");

                                    //////////////////////////////////////////////////////////////////////


                                    ////////////////////  Toilet  ////////////////////////////////////////

                                    toiletFragment.temp.setText("Temp : " + myString[44]);
                                    toiletFragment.humi.setText("Humi : " + myString[45]);


                                    if(myString[46].equals("0")){
                                        toiletFragment.myswitch.setChecked(true);
                                        toiletFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if(myString[46].equals("1")){
                                        toiletFragment.myswitch.setChecked(false);
                                        toiletFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[46].equals("2")){
                                        toiletFragment.myswitch.setChecked(true);
                                        toiletFragment.ledonoff.setText("LED ON");
                                    }
                                    else if(myString[46].equals("3")){
                                        toiletFragment.myswitch.setChecked(false);
                                        toiletFragment.ledonoff.setText("LED ON");
                                    }


                                    toiletFragment.redbar.setProgress(Integer.parseInt(myString[47]));
                                    toiletFragment.greenbar.setProgress(Integer.parseInt(myString[48]));
                                    toiletFragment.bluebar.setProgress(Integer.parseInt(myString[49]));
                                    toiletFragment.aubar.setProgress(Integer.parseInt(myString[50]));


                                    if(myString[51].equals("0"))
                                        toiletFragment.ventilfan.setText("Ventil Fan OFF");
                                    else if(myString[51].equals("1"))
                                        toiletFragment.ventilfan.setText("Ventil Fan ON");


                                    if(myString[52].equals("0"))
                                        toiletFragment.doorbtn.setText("Door Close");
                                    else if(myString[52].equals("1"))
                                        toiletFragment.doorbtn.setText("Door Open");

                                    /////////////////////////////////////////////////////////////////////////////////

                                    ///////////////////////  MultiRoom  ////////////////////////////////////////////

                                    multiRoomFragment.temp.setText("Temp : " + myString[53]);
                                    multiRoomFragment.humi.setText("Humi : " + myString[54]);


                                    if(myString[55].equals("0")){
                                        multiRoomFragment.myswitch.setChecked(true);
                                        multiRoomFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if(myString[55].equals("1")){
                                        multiRoomFragment.myswitch.setChecked(false);
                                        multiRoomFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[55].equals("2")){
                                        multiRoomFragment.myswitch.setChecked(true);
                                        multiRoomFragment.ledonoff.setText("LED ON");
                                    }
                                    else if(myString[55].equals("3")){
                                        multiRoomFragment.myswitch.setChecked(false);
                                        multiRoomFragment.ledonoff.setText("LED ON");
                                    }


                                    multiRoomFragment.redbar.setProgress(Integer.parseInt(myString[56]));
                                    multiRoomFragment.greenbar.setProgress(Integer.parseInt(myString[57]));
                                    multiRoomFragment.bluebar.setProgress(Integer.parseInt(myString[58]));
                                    multiRoomFragment.aubar.setProgress(Integer.parseInt(myString[59]));


                                    if(myString[60].equals("0"))
                                        multiRoomFragment.winbtn1.setText("Window1 Close");
                                    else if(myString[60].equals("1"))
                                        multiRoomFragment.winbtn1.setText("Window1 Open");


                                    if(myString[61].equals("0"))
                                        multiRoomFragment.winbtn2.setText("Window2 Close");
                                    else if(myString[61].equals("1"))
                                        multiRoomFragment.winbtn2.setText("Window2 Open");


                                    if(myString[62].equals("0"))
                                        multiRoomFragment.doorbtn.setText("Door Close");
                                    else if(myString[62].equals("1"))
                                        multiRoomFragment.doorbtn.setText("Door Open");

                                    //////////////////////////////////////////////////////////////////////////////


                                    ///////////////////////  Main Door  ///////////////////////////////////////

                                    //mainDoorFragment.temp.setText("Temp : " + myString[63]);
                                    //mainDoorFragment.humi.setText("Humi : " + myString[64]);


                                    if(myString[65].equals("0")){
                                        mainDoorFragment.myswitch.setChecked(true);
                                        mainDoorFragment.ledonoff.setText("LED OFF");
                                    }
                                    else if (myString[65].equals("2")){
                                        mainDoorFragment.myswitch.setChecked(true);
                                        mainDoorFragment.ledonoff.setText("LED ON");
                                    }


                                    mainDoorFragment.aubar.setProgress(Integer.parseInt(myString[66]));


                                    if(myString[67].equals("0"))
                                        mainDoorFragment.bodysensor.setText("BodySensor OFF");
                                    else if(myString[67].equals("1"))
                                        mainDoorFragment.bodysensor.setText("BodySensor ON");


                                    if(myString[68].equals("0"))
                                        mainDoorFragment.doorbtn.setText("Door Close");
                                    else if(myString[68].equals("1"))
                                        mainDoorFragment.doorbtn.setText("Door Open");

                                    ///////////////////////////////////////////////////////////////////


                                    ///////////////////////  Out Side  ////////////////////////////

                                    outsideFragment.temp.setText("Temp : " + myString[69]);
                                    outsideFragment.humi.setText("Humi : " + myString[70]);
                                    outsideFragment.battery.setText("Battery : " + myString[71]);
                                    outsideFragment.uv.setText("UV : " + myString[72]);
                                    outsideFragment.dust.setText("Dust : " + myString[73]);

/*
                                    if(myString[74].equals("0"))
                                        outsideFragment.stob.setText("S->B OFF");
                                    else if(myString[74].equals("1"))
                                        outsideFragment.stob.setText("S->B ON");


                                    if(myString[75].equals("0"))
                                        outsideFragment.stod.setText("S->D OFF");
                                    else if(myString[75].equals("1"))
                                        outsideFragment.stod.setText("S->D ON");


                                    if(myString[76].equals("0"))
                                        outsideFragment.btod.setText("B->D OFF");
                                    else if(myString[76].equals("1"))
                                        outsideFragment.btod.setText("B->D ON");
*/

                                    if(myString[77].equals("0")){
                                        weather.setText("Weather : Rainning");
                                    }
                                    else if(myString[77].equals("1")){
                                        weather.setText("Weather : Sunny");
                                    }


                                    if(myString[78].equals("0")){
                                        myswitch.setChecked(true);
                                    }
                                    else if(myString[78].equals("1")){
                                        myswitch.setChecked(false);
                                    }


                                    if(myString[79].equals("0")){
                                        heatingPump.setText("Heating Pump OFF");
                                    }
                                    else if(myString[79].equals("1")){
                                        heatingPump.setText("Heating Pump ON");
                                    }


                                    if(myString[80].equals("0")){
                                        boiler.setText("Boiler OFF");
                                    }
                                    else if(myString[80].equals("1")){
                                        boiler.setText("Boiler ON");
                                    }


                                    if(myString[81].equals("0")){
                                        allDoor.setText("All Door Close");
                                    }
                                    else if(myString[81].equals("1")){
                                        allDoor.setText("All Door Open");
                                    }


                                    if(myString[82].equals("0")){
                                        allWindow.setText("All Window Close");
                                    }
                                    else if(myString[82].equals("1")){
                                        allWindow.setText("All Window Open");
                                    }


                                    outsideFragment.solarangle.setProgress(Integer.parseInt(myString[83]));


                                    if(myString[84].equals("0")){
                                        outsideFragment.carhousedoor.setText("Car House Door Close");
                                    }
                                    else if(myString[84].equals("1")){
                                        outsideFragment.carhousedoor.setText("Car House Door Open");
                                    }

                                    //////////////////////////////////////////////////////////////////////////

                                    livingRoomFragment.count = 0;

                                }
                            });
                        }
                    }
                }
            }catch (SocketException e){
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void connectSocket(final String ip, final String port, final String id, final String pw){
        Thread run = new Thread() {

            @Override
            public void run() {
                try {

                    socket = new Socket();

                    handler.sendEmptyMessage(SERVER_CONN_START);

                    InetSocketAddress socketAddr = new InetSocketAddress(ip,Integer.parseInt(port));


                    try {
                        socket.connect(socketAddr, 5000);
                    }catch (UnknownHostException e){
                        Log.d("Exception","UnknownHost");
                        handler.sendEmptyMessage(SERVER_CONN_FAILED);
                    }
                    catch(ConnectException e){
                        Log.d("Exception","connect");
                        handler.sendEmptyMessage(SERVER_CONN_FAILED);
                    }
                    catch (SocketTimeoutException e){
                        handler.sendEmptyMessage(SERVER_CONN_TIMEOUT);
                        Log.d("Exception","TimeOut");
                    }
                    catch (IOException e){
                        e.printStackTrace();
                    }


                    if (socket != null && socket.isConnected()) {

                        handler.sendEmptyMessage(SERVER_CONN_SUCCESS);

                        socket_out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
                        socket_in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        p_socket_out = new PrintWriter(socket_out, true);
                        p_socket_out.println(String.format("E,%s,%s,",id,pw));

                         String code = socket_in.readLine();

                        Log.d("connect1",code);

                        if(code.equals("success"))
                        {
                        FragmentTransaction tr = fragmentManager.beginTransaction();

                        if(!startIntroFragment.isHidden()){
                            tr.hide(startIntroFragment);

                        }
                        tr.commit();

                        handler.sendEmptyMessage(LOGIN_SUCCESS);

                        Reader reader = new Reader();
                        if(reader.isAlive()){
                            reader.isInterrupted();
                        }

                        reader.setDaemon(true);
                        reader.start();
                        }
                        else {
                          handler.sendEmptyMessage(LOGIN_FAIL);
                         Log.d("connect2", code);
                        }
                    }
                }catch (IOException e){
                    e.printStackTrace();
                }
            }
        };

        run.start();

        disableEnableControls(true,interFace);
    }


    public void closeConnection(){
        try{
            if(socket!= null && socket.isConnected()) {
                Log.d("socket","not null");

                disableEnableControls(false,interFace);
                socket.close();
                socket = null;
            }
            else
                Log.d("socket","null");

        }catch (IOException e){
            e.printStackTrace();
        }
    }


    @Override
    public void onBackPressed() {

        long tempTime = System.currentTimeMillis();
        long intervalTime = tempTime - backPressedTime;

        if (0 <= intervalTime && FINISH_INTERVAL_TIME >= intervalTime) {

            super.onBackPressed();
        }
        else
        {
            backPressedTime = tempTime;
            FragmentTransaction tr = fragmentManager.beginTransaction();

            if(startIntroFragment.isHidden()){

                tr.show(startIntroFragment);
                closeConnection();
            }
            else {
                super.onBackPressed();
            }

            tr.commit();
        }
    }

    class mHandler extends Handler {
        @Override
        public void handleMessage(final Message msg){
            super.handleMessage(msg);

            switch (msg.what){

                case SERVER_CONN_START:{
                    Toast.makeText(MainActivity.this, "Trying Connect", Toast.LENGTH_SHORT).show();
                    break;
                }
                case SERVER_CONN_FAILED: {
                    Toast.makeText(MainActivity.this, "Cannot Connect", Toast.LENGTH_SHORT).show();
                    break;
                }
                case SERVER_CONN_SUCCESS: {
                    Toast.makeText(MainActivity.this, "Connected", Toast.LENGTH_SHORT).show();
                    break;
                }
                case SERVER_CONN_END: {
                    Toast.makeText(MainActivity.this, "Disconnected", Toast.LENGTH_SHORT).show();
                    break;
                }
                case SERVER_CONN_TIMEOUT: {
                    Toast.makeText(MainActivity.this, "Not Response So Stop", Toast.LENGTH_SHORT).show();
                    break;
                }
                case LOGIN_SUCCESS:{
                    Toast.makeText(MainActivity.this, "Login Success", Toast.LENGTH_SHORT).show();
                    break;
                }
                case LOGIN_FAIL:{
                    Toast.makeText(MainActivity.this, "Login Fail", Toast.LENGTH_SHORT).show();
                    break;
                }
            }
        }
    };

    @Override
    protected void onStop(){
        super.onStop();
    }
}



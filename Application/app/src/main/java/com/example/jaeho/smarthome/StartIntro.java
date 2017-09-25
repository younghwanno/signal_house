package com.example.jaeho.smarthome;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Base64;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;

import java.nio.charset.Charset;
import java.security.MessageDigest;

/**
 * Created by jaeho on 17. 6. 30.
 */

public class StartIntro extends Fragment {

    RelativeLayout strintro;
    private EditText et_ip, et_port, et_id, et_pw;
    Button button;

    private String ip_input = new String();
    private String port_input = new String();
    private String id_input = new String();
    private String pw_input = new String();


    @Override
    public void onCreate(Bundle savedInstaceState) {
        super.onCreate(savedInstaceState);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstancedState){

        View view = inflater.inflate(R.layout.startintro,null);

        strintro = (RelativeLayout)view.findViewById(R.id.strintro);

        et_ip = (EditText)view.findViewById(R.id.ip);
        et_port = (EditText)view.findViewById(R.id.port);
        et_id = (EditText)view.findViewById(R.id.id);
        et_pw = (EditText)view.findViewById(R.id.pw);
        button = (Button) view.findViewById(R.id.connect);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                ip_input = et_ip.getText().toString();
                port_input = et_port.getText().toString();
                id_input = et_id.getText().toString();
                pw_input = et_pw.getText().toString();
                String hash_pw = getSha512(pw_input);

                ((MainActivity)getActivity()).connectSocket(ip_input, port_input, id_input, hash_pw);

                Log.d("PassWord",getSha512(pw_input));
            }
        });


        return view;
    }

    /////////////////패스워드 암호화 ////////////////
    private static String getSha512(String plainText) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] bytes = plainText.getBytes(Charset.forName("UTF-8"));
            md.update(bytes);
            return Base64.encodeToString(md.digest(),1);

        } catch (Exception e) {
            System.out.println("Sha256 error.");
            e.printStackTrace();
            return null;
        }
    }



}

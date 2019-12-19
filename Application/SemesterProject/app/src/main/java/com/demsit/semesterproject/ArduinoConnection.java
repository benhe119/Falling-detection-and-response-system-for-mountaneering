package com.demsit.semesterproject;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.UUID;



public class ArduinoConnection extends Thread{
    public static ArduinoConnection instance;
    public static final UUID mUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    public static  BluetoothSocket btSocket;
    public static InputStream in;
    public static OutputStream out;
    public static MainActivity context;


    public synchronized  static ArduinoConnection getInstance(MainActivity c) {
        context = c;
        if(instance == null && btSocket == null){
            setBluetoothConnection();
            if(btSocket!=null){
                if(btSocket.isConnected()){
                    instance = new ArduinoConnection();
                    instance.start();
                }
            }
        }
        return instance;
    }

    public void run(){
        while(!this.isInterrupted() && btSocket.isConnected()){
            if(in==null){
            }else{
                byte[] byteArray2 = new byte[256];
                try {
                    if(in.available()>=4){
                        in.read(byteArray2);
                        String message = new String(byteArray2, StandardCharsets.UTF_8).trim();
                        String code = message.split("\\;")[0];

                        if(code.equals("201")){
                            System.out.println("Name save in the board");
                            context.runOnUiThread(()->{
                                Toast.makeText(context.getApplicationContext(),
                                        "Name save in the board", Toast.LENGTH_SHORT).show();
                            });
                        }else if(code.equals("202")){
                            context.runOnUiThread(()->{
                                System.out.println("Phone number saved in the board");
                                Toast.makeText(context.getApplicationContext(),
                                        "Phone number saved in the board", Toast.LENGTH_SHORT).show();
                            });

                        }else if(code.equals("203")){
                            context.runOnUiThread(()->{
                                System.out.println("Type blood saved in the board");
                                Toast.makeText(context.getApplicationContext(), "Type blood saved in the board", Toast.LENGTH_SHORT).show();
                            });


                        }else if(code.equals("404")){
                            context.runOnUiThread(()->{
                                context.fallingTrigerred();
                            });
                        }else if(code.equals("500")){
                            String mess;
                            if(message.split("\\;").length>0){
                                mess = message.split("\\;")[1];
                            }else{
                                mess = "";
                            }

                            boolean end500 = false;
                            while(!end500){
                                byte[] byteArray3 = new byte[256];
                                try {
                                    in.read(byteArray3);
                                    String messbytes= new String(byteArray3, StandardCharsets.UTF_8).trim();
                                    mess+=messbytes.split("\\;")[0];
                                    if(messbytes.split("\\;").length>0){
                                        end500 = true;
                                        final String toSend = mess;
                                        context.runOnUiThread(()->{
                                            context.messageToSend(toSend);
                                        });

                                    }
                                }catch (IOException E){

                                }

                            }

                        }
                    }
                } catch (IOException e) {
                    System.out.println("A problem occured");
                    e.printStackTrace();
                }

            }
        }
    }

    public void send(String message){
        try{
            byte[] byteArray;
            byteArray = (message).getBytes();
            out.write(byteArray);
        }catch (IOException e){
            System.out.println("A problem occurred");
            e.printStackTrace();
        }
    }











    public static void setBluetoothConnection() {

        BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

        BluetoothDevice hc05 = btAdapter.getRemoteDevice("00:06:66:6B:B3:86");
        System.out.println(hc05.getName());
        btSocket = null;
        int counter = 0;
        do {
            try {
                btSocket = hc05.createRfcommSocketToServiceRecord(mUUID);
                System.out.println(btSocket);
                btSocket.connect();
                System.out.println(btSocket.isConnected());
            } catch (IOException e) {
                e.printStackTrace();
            }
            counter++;
        } while (!btSocket.isConnected() && counter < 3);
        if(btSocket.isConnected()){
            Toast.makeText(context, "The device is connected successfully", Toast.LENGTH_SHORT).show();

            try {
                in = btSocket.getInputStream();
                out = btSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }



}

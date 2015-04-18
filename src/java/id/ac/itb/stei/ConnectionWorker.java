/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package id.ac.itb.stei;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;

/**
 *
 * @author setyo.legowo
 */
public class ConnectionWorker implements Runnable {
    
    protected char[] buffer = new char[2048];
    protected Socket clientSocket = null;
    protected String serverText   = null;

    public ConnectionWorker(Socket clientSocket, String serverText) {
        this.clientSocket = clientSocket;
        this.serverText   = serverText;
    }

    @Override
    public void run() {
        try {
            int charsRead;
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream())
            );
            if((charsRead = in.read(buffer)) >= 0) {
                boolean error = false;
                String message = new String(buffer).substring(0, charsRead);
                String[] msg = message.split(" ");
                System.out.println("msg :" + message);
                DBFWContextHandler dbfw = new DBFWContextHandler(msg[1],
                        msg[2], msg[3]);
                if(msg[0].equalsIgnoreCase("1")) {
                    // nothing
                } else if(msg[0].equalsIgnoreCase("2")) {
                    for(int i=4; i < msg.length; i++){
                        dbfw.addColumn(msg[i]);
                    }
                } else
                    error = true;
                
                if(!error){
                    dbfw.run();
                    try (OutputStream output = clientSocket.getOutputStream()) {
                        output.write(("1 " + dbfw.getResult()).getBytes());
                    }
                } else {
                    try (OutputStream output = clientSocket.getOutputStream()) {
                        output.write("0 ERROR".getBytes());
                    }
                }
            }
        } catch (IOException e) {
        }
    }
    
}

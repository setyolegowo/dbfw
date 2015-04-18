/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package id.ac.itb.stei;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
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
            int charsRead = 0;
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream())
            );
            while((charsRead = in.read(buffer)) >= 0) {
                String message = new String(buffer).substring(0, charsRead);
                System.out.println("msg :"+message);
                break;
            }
            long time;
            try (OutputStream output = clientSocket.getOutputStream()) {
                time = System.currentTimeMillis();
                output.write(("HTTP/1.1 200 OK\n\nWorkerRunnable: " +
                        this.serverText + " - " + time).getBytes());
            }
            System.out.println("Request processed: " + time);
        } catch (IOException e) {
        }
    }
    
}

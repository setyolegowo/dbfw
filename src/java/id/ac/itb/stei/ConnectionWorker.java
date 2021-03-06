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
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 *
 * @author setyo.legowo
 */
public class ConnectionWorker implements Runnable {
    
    private final static Log log = LogFactory.getLog(ConnectionWorker.class);
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
                DBFWContextHandler dbfw = null;
                String message = new String(buffer).substring(0, charsRead);
                String[] msg = message.split(" ");
                log.info("msg :" + message);
                if(msg.length < 4)
                    error = true;
                else {
                    dbfw = new DBFWContextHandler(msg[0], msg[2], msg[3]);
                    dbfw.setIpclient(msg[1]);
                    for(int i=4; i < msg.length; i++){
                        dbfw.addColumn(msg[i]);
                    }
                }
                
                if(!error){
                    dbfw.run();
                    try (OutputStream output = clientSocket.getOutputStream()) {
                        output.write(("1 " + dbfw.getResult()).getBytes());
                        log.info("1 " + dbfw.getResult());
                    }
                } else {
                    try (OutputStream output = clientSocket.getOutputStream()) {
                        log.info("0 ERROR".getBytes());
                    }
                }
            }
        } catch (IOException e) {
            log.error(e);
        }
    }
    
}

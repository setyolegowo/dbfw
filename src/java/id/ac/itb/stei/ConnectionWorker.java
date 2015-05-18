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
                if(msg.length < 3)
                    error = true;
                else {
                    dbfw = new DBFWContextHandler(msg[0], msg[1], msg[2]);
                    dbfw.setIpclient(clientSocket.getRemoteSocketAddress().toString());
                    for(int i=3; i < msg.length; i++){
                        dbfw.addColumn(msg[i]);
                    }
                }
                
                if(!error){
                    dbfw.run();
                    try (OutputStream output = clientSocket.getOutputStream()) {
                        output.write(("1 " + dbfw.getResult()).getBytes());
                        log.error("msg : " + dbfw.getResult());
                    }
                } else {
                    try (OutputStream output = clientSocket.getOutputStream()) {
                        log.error("0 ERROR".getBytes());
                    }
                }
            }
        } catch (IOException e) {
            log.error(e);
        }
    }
    
}

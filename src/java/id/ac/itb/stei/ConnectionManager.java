/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package id.ac.itb.stei;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 *
 * @author setyo.legowo
 */
public class ConnectionManager implements Runnable {
    
    private static Log log = LogFactory.getLog(ConnectionManager.class);
    protected int          serverPort    = 6000;
    protected ServerSocket serverSocket  = null;
    protected boolean      isStopped     = false;
    protected Thread       runningThread = null;
    protected ExecutorService threadPool =
        Executors.newFixedThreadPool(10);
    
    public ConnectionManager() {}

    @Override
    public void run() {
        synchronized(this){
            this.runningThread = Thread.currentThread();
        }
        openServerSocket();
        while(! isStopped()){
            Socket clientSocket = null;
            try {
                clientSocket = this.serverSocket.accept();
            } catch (IOException e) {
                log.error(e);
                if(isStopped()) {
                    log.fatal("Server Stopped.");
                    break;
                }
                throw new RuntimeException(
                    "Error accepting client connection", e);
            }
            this.threadPool.execute(
                new ConnectionWorker(clientSocket, "Thread Pooled Server")
            );
        }
        this.threadPool.shutdown();
        log.info("Server Stopped.");
    }
    
    private synchronized boolean isStopped() {
        return this.isStopped;
    }
    
    public synchronized void stop(){
        this.isStopped = true;
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            throw new RuntimeException("Error closing server", e);
        }
    }
    
    private void openServerSocket() {
        try {
            this.serverSocket = new ServerSocket(this.serverPort);
        } catch (IOException e) {
            throw new RuntimeException("Cannot open port " + this.serverPort, e);
        }
    }
    
    public static void main(String[] args) {
        log.info("TEST LOG. HAHAHAHA");
        log.debug("DEBUG TEST LOG. HAHAHAHA");
        log.error("ERROR TEST LOG. HAHAHAHA");
        DBFWContextHandler.initBalana();
        ConnectionManager server = new ConnectionManager();
        new Thread(server).start();

        while(!Thread.currentThread().isInterrupted()){}
        server.stop();
    }
}

package commLinkProtocol;

import java.io.*;
import java.net.Socket;

import navalEngagements.*;

/**
 * 
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * 
 * @version 2.0 Demo Build
 * 
 * This class handles the protocol for communicating between the server and the client.
 *
 */

public class Communication implements Runnable {

    private BufferedReader in = null;
    private PrintWriter out = null;  	 
    
    private Socket socket;
    
    private Client client;    
    
    public Communication(Client client)    {
   	 
   	 this.client = client;   					 
   	 
    }
    
/**
 * Accepts the server IP and the port number and connets to the server
 */
    
    public int connect(String playerIP, int port) {
   	 
   	 try {
   								 
         this.socket = new Socket(playerIP, port);        		     	 
        	 
         System.out.println("Client Connected. Listening forever henceforth...\n");
         in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
   		 out = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));

         receiveMessage();
        		 
   	 
   	 } catch (IOException e) {
   		 
   		 System.err.print(e);
   		 System.err.print("Client: failed to listen\n");  
         return 1;    
   		 
   	 } catch (Exception e) {
   					 
   		 System.err.print(e);
   		 return 1;    
   		 
   	 }
   	 
   	 return 0;
   	 
    }
    
    /**
     * Starts a new thread to continuously receive messages
     */
    
    public void receiveMessage() {
   	 
   	 Thread thread = new Thread(this);
   	 thread.start();   			 
   	 
    }
    /**
     * Inputs String message and parses it by colons and semicolon
     */
    
    
    private void parseMessage(String message){ 
   	 
    	String[] parsedMessage = message.split("[:;]");
   	
   		System.out.println("Server-to-Client: " + message);
   	 
   		this.client.handleMessage(parsedMessage);
   		 
    }
    
    /**     
     * Sends the message to the server
     */

    public void sendMessage(String message){
   	 
    	try {

   		 System.out.println("Client-to-Server: " + message);
   		 
   		 this.out.println(message);
   		 this.out.flush();
   	 
   		 
   		 
   	 } catch (Exception e) {
   		 
   		 System.err.print(e);
   		 System.err.print("Client failed to send\n");
   		 
   	 }  	 
   	 
    }
    
    /**
     * Runs a thread and constantly waits for messages from the server
     */
    
    public void run () {
   	 
   	 
     	 try {     		
  		 
     		 while(true) {

         	   String message="";

         	   while (true) {
            		message = in.readLine();
            		 
            		if (message == null)
            			break;
            		else {
            			
            		parseMessage(message);
            			
            	}
            
            }
     		     				 

        }
     		 
    	} catch(Exception e) {
   		 
    		 System.err.print(e);
        	 System.err.print("Client: failed to receive\n");      	 
       	 
    	}
     	 
    }   					 

}
/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:07  sumedh
   Added the Nexus II source code files.

# Revision 1.1  1996/12/07  06:27:38  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:05:30  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/
// This class will handle the event generated when user presses "connect/dis
// connect" button. It will maintain all the state associated with a 
// connection ; 
// In particular, it will create reader,writer and demux threads and stop
// them when user pressed disconnect. It also sends the init and quit
// packet to the server 
// -- Rajeev Bector 11/26/96 

// package NexusII.networking ; 

import awtCommand.* ; 

import java.net.* ;
import java.awt.* ; 
import java.io.* ; 
import java.util.* ; 
import ACE.OS.*;
import ACE.SOCK_SAP.*;

public class connectionHandler implements Command,consts,Runnable { 

  // The socket for reading and writing 
  // private Socket nexusSocket;
  private SOCKConnector nexusConnector_ = new SOCKConnector();
  private SOCKStream nexusStream_  = new SOCKStream();

  // Threads which will do reading and writing from Socket 
  private Thread readerThread ; 
  private Thread writerThread ; 

  // Threads which demuxes the data from Write Queue 
  private Thread demuxThread; 

  // The socket
  // private Socket nexusSocket_ ; 

  // Data Streams which flow the data out 
  private DataInputStream is_ ; 
  private DataOutputStream os_ ; 

  // links to the outside world 
  private MT_Bounded_Queue read_q_ ; 
  private MT_Bounded_Queue write_q_ ; 
  
  Integer port_ ; 
  String  host_ ; 

  NexusClientApplet applet_ ; 
  // Constructor 
  public connectionHandler(String host,Integer port,NexusClientApplet applet) { 
    host_ = host ; 
    port_ = port ; 
    applet_ = applet ; 
  } // of constructor 

  // This will be called when the "connect/disconnet" button is pressed 

  public void execute(Object target, Event evt, Object what) { 
    // run in a separate thread 
    new Thread(this).start(); 
  }

  public synchronized void run() { 
    
    // If not connected -- then connect 
    if(!applet_.connected) { 

      // Initialize the queues now 
      read_q_ = new MT_Bounded_Queue(QUEUE_SIZE);
      write_q_ = new MT_Bounded_Queue(QUEUE_SIZE);

      // Try to connect to the server now 
	// nexusSocket_ = new Socket(host_,port_.intValue());
      try {
        nexusConnector_.connect(nexusStream_, host_, port_.intValue());
      }
      catch(SocketException s) { }
      catch(IOException i) { }
      // Attach Streams to read and write end os socket 
	os_ = new DataOutputStream(nexusStream_.outputStream());
	is_  = new DataInputStream(nexusStream_.inputStream());
      
      // do something now 
      
      // create and start the socket reader  first 
      readerThread = new socketReaderThread(read_q_,is_); 
      readerThread.start(); 
      
      // now create the writer also -- subclass of consumer 
      writerThread = new socketWriterThread(write_q_,os_);
      writerThread.start(); 
      // Have to send the init packet to the server 
      sayHelloToServer(); 

      
      // Do all the graphics needed and also init the join handlers etc.
      applet_.OnConnect();
	
      // make sure that when demux is created -- joinHandler is there ... !!

      // Create the demux here -- he'll spawn off the rooms 
      demuxThread = new nexusDemuxThread(read_q_,applet_.Mapper()); 
      demuxThread.start(); 
    
    } // of if !connected

    // if  connected already then I have to disconnect --- have to 
    // write code for this 
    else { 
      
      // first leave all the rooms 
      for(Enumeration e = applet_.Mapper().keys(); e.hasMoreElements();) { 
	String name = (String) e.nextElement() ; 
	applet_.SetRoomName(name); 
	applet_.nexusJoiner.execute(null,null,LEAVE_STR); 
      } 
      // send a quit packet to the server 
      sayByeToServer();
      
      // Have to send a leave for all the rooms and leave all the rooms 
      
      // stop the running threads
      readerThread.stop(); 
      demuxThread.stop(); 

      // Again meddle with graphics
      applet_.OnDisconnect();
      return ; 
    }
  } // of run 

  // This method returns the underlying socket descriptor 
  public Socket handle() {
    return nexusStream_.socket(); 
  }
  
  public DataInputStream getinStream() {
    return is_ ; 
  }
  
  public DataOutputStream getoutStream() {
    return os_ ;
  }
  
  public MT_Bounded_Queue reader_q() { 
    return read_q_ ; 
  }


  public MT_Bounded_Queue writer_q() { 
    if(write_q_ == null)
      System.out.println("Sending a null write_q");
    return write_q_ ; 
  }

  // This method will compose a hello packet and send it off -- cannot run 
  // this in a separate thread. Has to send this first surely to our TCP 
  // Connection 

  public void sayHelloToServer() {
    // hello doesnt have any body 
    byte[] packetHeader = new byte[PACKET_HEADER_LENGTH] ; 
    String clientID = new String(NexusClientApplet.myName);
    String roomID   = new String("ROOT");
    String contentType = new String("INIT");
    String contentLength  = new String("0");
    
    // Make a proper packet out of it 
    dataPacket initPacket = new dataPacket(clientID,roomID,contentType,contentLength); 
    
    // enqueue that packet for socketWriter to read 
    write_q_.nq(initPacket);
    
  }
  
  // This method send a Quit command to the server -- to say that it is 
  // pushing off 
  public void sayByeToServer() { 
    byte[] packetHeader = new byte[PACKET_HEADER_LENGTH] ; 
    String clientID = new String(NexusClientApplet.myName);
    String roomID   = new String("null");
    String contentType = new String("QUIT");
    String contentLength  = new String("0");
    
    // Make a proper packet out of it 
    dataPacket quitPacket = new dataPacket(clientID,roomID,contentType,contentLength); 
    
    // enqueue that packet for socketWriter to read 
    write_q_.nq(quitPacket);
  } // of sayByeToServer 
  
} // of class 






















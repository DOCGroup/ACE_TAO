/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:09  sumedh
   Added the Nexus II source code files.

# Revision 1.1  1996/12/07  06:27:38  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:06:22  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/

// This class will handle the event generated when user presses "join/leave"
// button. It will maintain all the state associated with all the rooms
// It'll also create and maintain the rooms 
// -- Rajeev Bector 11/26/96 

// package NexusII.networking ; 

// This is a SINGLETON 


import java.net.* ;
import java.util.* ; 
import java.awt.* ; 
import java.io.* ; 


//import NexusII.client.* ; 
//import NexusII.util.* ; 
//import NexusII.networking.* ; 

import awtCommand.* ; 

public class joinHandler implements Command,consts { 
  MT_Bounded_Queue write_q_ ; 
  Hashtable name_to_q_ ; 
  Hashtable name_to_handler_ ; 
  NexusClientApplet applet_ ; 

  // This will be called when the "join/leave" button is pressed 
  public joinHandler(MT_Bounded_Queue writer_q,NexusClientApplet applet) { 
    write_q_ = writer_q ; 
    applet_ = applet ; 
    // init. the Hashtables 
    name_to_q_ = new Hashtable(HASH_SIZE);
    name_to_handler_ = new Hashtable(HASH_SIZE); 

  } 

  public Hashtable Mapper() { 
    return name_to_q_ ; 
  }
  
  public void execute(Object target, Event evt, Object what) { 
    
    String roomName = applet_.GetRoomName() ; 
    if(roomName.length()==0) { 
      return ; 
    } 

    // if it is the join button
    if(((String)what).equalsIgnoreCase(JOIN_STR)) { 
      //if already joined -- return 
      if(name_to_q_.get(roomName) != null)
	return ; 
      
      // room doesnt exist 
      // Just in case the room name isn't listed
      applet_.AddRoomName(roomName);
      // create a new queue for the room to use 
	
      MT_Bounded_Queue q_ = new MT_Bounded_Queue(QUEUE_SIZE); 
      if(DEBUG) { 
	System.out.println("joinHandler: inserted the queue in hash ");
      }
      name_to_q_.put(roomName,q_); 
      roomHandler handler = new roomHandler(roomName,q_,write_q_,applet_);
      name_to_handler_.put(roomName,handler); 
      
      // send a request to the server 
      sayJoinToServer(roomName); 
      //  sendTestPacket(roomName,"URL","http://cumbia.cs.wustl.edu:4242/~sumedh/NexusII/NexusII.gif",q_);
    } // of join 

    else { 
      // its a "leave packet" 
      if(name_to_q_.get(roomName) == null)
	return ; 
      // remove the entry from the queue 
      name_to_q_.remove(roomName); 
      
      roomHandler handle = (roomHandler) name_to_handler_.get(roomName); 
      if(DEBUG)
	System.out.println("joinHandler:Stopiing the room thread" + roomName);
      handle.mystop();
      //    handle = null ; 

      // remove the entry from the queue 
      name_to_handler_.remove(roomName); 
      sayLeaveToServer(roomName); 
    } // of else 
  } // of  execute 
  
  private void sayJoinToServer(String roomName) { 
    //if (DEBUG)
    // System.out.println("In the say J to server...");
    byte[] packetHeader = new byte[PACKET_HEADER_LENGTH] ; 
    String clientID = new String(NexusClientApplet.myName);
    String roomID   = new String("ROOT");
    String contentType = new String("JOIN");
    String contentLength  = String.valueOf(roomName.length());
    String  content       = new String(roomName); 
    // Make a proper packet out of it 
    dataPacket joinPacket = new dataPacket(clientID,roomID,contentType,contentLength,content); 
    
    // enqueue that packet for socketWriter to read 
    write_q_.nq(joinPacket);
  } 
  
  private void sayLeaveToServer(String roomName) { 
    byte[] packetHeader = new byte[PACKET_HEADER_LENGTH] ; 
    String clientID = new String(NexusClientApplet.myName);
    String roomID   = new String("ROOT");
    String contentType = new String("LEAVE");
    String contentLength  = new String(Integer.toString(roomName.length()));
    String  content       = new String(roomName);     

    // Make a proper packet out of it 
    dataPacket leavePacket = new dataPacket(clientID,roomID,contentType,contentLength,content); 
    
    // enqueue that packet for socketWriter to read 
    write_q_.nq(leavePacket);
  } 


  public void sendTestPacket(String roomName,String type,String content,MT_Bounded_Queue readq) { 
    byte[] packetHeader = new byte[PACKET_HEADER_LENGTH] ; 
    String clientID = new String(NexusClientApplet.myName);
    String contentLength  = new String(Integer.toString(content.length()));


    // Make a proper packet out of it 
    dataPacket testPacket = new dataPacket(clientID,roomName,type,contentLength,content); 
    
    // enqueue that packet for socketWriter to read 
    readq.nq(testPacket);
  } 
  
  
}

// of class 

















/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:12  sumedh
   Added the Nexus II source code files.

# Revision 1.1  1996/12/07  06:27:38  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:09:22  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/
// The socketWriterThread is like a Consumer who reads from the write_q and
// puts it onto the socket. Thats it ... Big Deal. huh ! 

// This threads task is
// 1. Write things to SocketBuffer.
// 2. Ensure that Full Packets are sent 
// 3. Read the packet off the queue 

//package NexusII.networking ; 

// get hold of java classes

import java.io.* ; 

//import NexusII.client.* ; 
//import NexusII.util.* ; 

public class socketWriterThread extends Consumer implements Runnable,consts
{
  DataOutputStream os_ ; 

  // new constructor 
  // Pass the queue and outstream to the constructor 

  public socketWriterThread(MT_Bounded_Queue q, DataOutputStream os) {
    // call the super constructor 
    super(q);
    os_ = os ; 
  }

  // This will just override the run method and thats it 
  // I want to have my own run !

  public void run()  {
    // run indefinitely  -- i am a daemon anyway 
    if(DEBUG) {
      System.out.println("--- This is socketWriterThread --- \n");
    }
    boolean bye_sent = false ; 
    while(!bye_sent) { 

      // read the packet from the queue 
      dataPacket packet = (dataPacket) queue_.dq(); 
      if(DEBUG){
	System.out.println("socketWriterThread: I got something -- \n");
      }
       
      // read length bytes from packet 
      int len = packet.contentLength() ; 
      if(DEBUG) {
	System.out.println("socketWriterThread: length is " + new Integer(len));
      }

	
      int field_len = FIELD_LENGTH ; 

      // Allocate storage for holding the fields 

      byte[] packetHeader = new byte[PACKET_HEADER_LENGTH]; 
      byte[] packetBody = new byte[len] ;
      
      // Fill them 
      packet.bytize(packetHeader,packetBody); 

      // put it on the wire now -- 

      try {
	os_.write(packetHeader,0,PACKET_HEADER_LENGTH);
      } 
      catch (IOException e) {
	// do something here 
      }
      if(DEBUG){
	System.out.println("I wrote:" + new String(packetHeader,0));
      }

       try {
	os_.write(packetBody,0,len);
      }
      catch (IOException e) {
	// do something here 
      }
      if(DEBUG){
	System.out.println("I wrote:" + new String(packetBody,0));
      }
      // this is to handle if the user presses disconnect without leaving the
      // room 
      if(packet.contentType().equalsIgnoreCase("QUIT")) 
	 bye_sent = true ; 
      // go back to reading the queue now 
    }
    // of for(;;)
  } 
  // of method run 
  
}

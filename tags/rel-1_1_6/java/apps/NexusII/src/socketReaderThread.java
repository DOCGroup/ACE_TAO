/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:11  sumedh
   Added the Nexus II source code files.

# Revision 1.1  1996/12/07  06:27:38  rajeev
# Initial revision
#
# Revision 1.3  1996/12/03  01:35:37  rajeev
# fixed a big bug with respect to trim()
#
# Revision 1.2  1996/12/03  01:01:27  rajeev
# // fixed the bug at line 76
#
# Revision 1.1  1996/12/02  06:08:56  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/
// The socketReaderThread is like a Producer who reads from the socket and
// nqs it onto the queue. Thats it ... Big Deal. huh ! 

// This threads task is
// 1. Take things from SocketBuffer.
// 2. Ensure that Full Packets are read 
// 3. Put the packet onto the queue 

//package NexusII.networking ; 

// get hold of java classes
//import NexusII.client.* ; 
//import NexusII.util.* ; 

import java.io.* ; 


public class socketReaderThread extends Producer implements Runnable,consts
{
  DataInputStream is_ ; 
  
  // new constructor 
  // Pass the queue and socketid to the constructor 

  public socketReaderThread(MT_Bounded_Queue q, DataInputStream is) {
    // call the super constructor 
    super(q);
    is_ = is ; 
  }

  // This will just override the run method and thats it 
  // I want to have my own run !

  public void run()  {
    // run indefinitely  -- i am a daemon anyway 
    if(DEBUG) {
      System.out.println("--- This is socketReaderThread --- \n");
    }
    
    for(;;) {

      // read header bytes from stream 
      int field_len = FIELD_LENGTH ; 
      byte[] packetHeader = new byte[PACKET_HEADER_LENGTH];
      try {
	is_.readFully(packetHeader,0,PACKET_HEADER_LENGTH);
      } 
      catch (IOException e) {
	// do something here 
	System.out.println("socketReader: I/O exception in read: I am quitting");
	// what to do here -- right now quit
	return ; 
      }
      if(DEBUG){
	System.out.println("socketReader read:" + new String(packetHeader,0));
      }
      
      // take out the length field from this 
      String length = new String(packetHeader,0,DATA_LEN_OFFSET,field_len);
      
      // Read this much more from the socket 
      if(DEBUG) { 
	System.out.println("The length of packet is " + length);
      }

      Integer Test = new Integer(length.trim());
      int len = Test.intValue();
      if(DEBUG) {
	System.out.println("srt: attempting to read " + Test + " bytes ");
      }

      byte[] packetBody = new byte[len] ; 
      try {
	if(len != 0) 
	  is_.readFully(packetBody,0,len);
      }
      catch (IOException e) {
	// do something here 
	System.out.println("socketReader: I/O exception in read: I am quitting");
	// what to do here -- right now quit
	return ; 
      }
      
      // The header and the body are there now -- so make a packet
      dataPacket packet = new dataPacket(packetHeader,packetBody);
      if(DEBUG)
	System.out.println("srt: Now nq'ing item body " + packet.content() );
      queue_.nq(packet);
      if(DEBUG)
	System.out.println("srt: Done nq'ing..");
      
      // go back to reading the socket now 
    }
    // of for(;;)
  } 
  // of method run 
}

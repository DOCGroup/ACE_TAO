/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:10  sumedh
   Added the Nexus II source code files.

# Revision 1.1  1996/12/07  06:27:38  rajeev
# Initial revision
#
# Revision 1.2  1996/12/05  05:39:47  sumedh
# ok
#
# Revision 1.1  1996/12/02  06:08:20  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/

// Will take the packets from the input queue 
// Parse the packet 
// Call the room handler thread which Aravind will provide 

// Mapper object is the one which returns the room_q reference if you pass
// it the room number. Aravind has to implement this object called roomMapper
// which has an instance called mapper in the Applet context. This will be
// referred by nexusDemuxThread whenever it has anything to put in the room_q

// package NexusII.networking ; 

//import NexusII.util.*  ; 
//import NexusII.client.* ;

import java.util.* ; 
 
public class nexusDemuxThread extends Thread implements consts { 
  MT_Bounded_Queue q_ ; 
  dataPacket packet ; 
  Hashtable mapper_ ; 

  // constructor 
  public nexusDemuxThread(MT_Bounded_Queue read_q,Hashtable mapper) { 
    q_ = read_q ; 
    mapper_ = mapper ; 
  }

  public void run() { 
    // run till infinity 
    for(;;) { 
      if(DEBUG) { 
	System.out.println("Demux running ---- ");
      }
      // take out packets from the queue 
      // parse to find out which room they belong to 
      packet = ((dataPacket)q_.dq()); 
      if(DEBUG) { 
	System.out.println("Demux got something ---- ");
      } 
      
      String roomName = packet.destination().trim(); 
      if(DEBUG) {
        System.out.println("demux: destination is : " + roomName);
      }
      if(DEBUG) { 
	System.out.println("Hashtable size is " + new Integer(mapper_.size())); 
      } 
      // Get a reference to which queue to put it in 
      MT_Bounded_Queue room_q = (MT_Bounded_Queue)mapper_.get(roomName);
      
      // nq the packet on the room q 
      if(room_q != null) { 
	room_q.nq(packet); 
	System.out.println("deMux: I have enqueued it successfully");
      }	
      else
	{
	  System.out.println("demux: room_q was null: i couldnt do much");
	}
    } 
  }
  // of run 
}
// of class 
      
  

  

// 11/24/96 -- Rajeev Bector 

/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:06  sumedh
   Added the Nexus II source code files.

# Revision 1.1  1996/12/07  06:27:38  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:05:08  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/
// why not use java utilities !! 

import java.util.* ; 
import java.lang.* ; 
import java.awt.* ; 



//import NexusII.client.* ; 
//import NexusII.util.* ; 
//import NexusII.networking.* ; 



import awtCommand.* ; 

// This class takes a command parses it and packetizes it and puts it in the
// global send queue. It works in its own thread of control and after nqing
// its run() method falls off. It does its work and dies 
// Will act as an eventHandler for TextField most probably 

public class commandParser implements Command,consts,Runnable { 

  String command ; 
  MT_Bounded_Queue q_ ; 
  NexusClientApplet applet_ ; 
  // constructor

  public commandParser(MT_Bounded_Queue write_q,NexusClientApplet applet) {
    // create a reference to the write_q 
    q_ = write_q ; 
    // note the applet name 
    applet_ = applet ; 
    
  }
  

  public void execute(Object target, Event evt, Object what) { 
    // Get the text which was entered there -- 
      command = new String((String) what) ; 
      // run in a separate thread 
      Thread main = new Thread(this); 
      main.start(); 
  }

  // The parser etc. run here 
  public synchronized void run() { 
    String actual_command = new String(); 
    String data = new String(); 
    String data_len = new String(); 
    if(DEBUG) { 
      System.out.println("commandParser thread running --- \n");
    }
     
    // Do the parsing first 
    if(command.equalsIgnoreCase(LUSERS_STR)) { 
      actual_command = new String("LUSERS") ; 
      data = applet_.GetRoomName() ; 
      data_len = new String(String.valueOf(data.length()));  
      if(data.length() == 0)
	return ; 
    }

    else if(command.equalsIgnoreCase(LROOMS_STR)) { 
	actual_command = new String("LROOMS") ; 
	data = new String("") ; 
	data_len = new String("0") ; 
    }
	
    String room = new String("ROOT"); 

    if(DEBUG) { 
      System.out.println("The room  is " + room + "\n");
    }

    /* 
       StringBuffer databuffer = new StringBuffer();
       
       // Get the data 
       while(t.hasMoreTokens()) { 
       databuffer.append(t.nextToken());
       }
       String data = new String(databuffer); 
       if(DEBUG) { 
       System.out.println("The data  is " + data + "\n");
       }

    // data length 
    String data_len =  String.valueOf(data.length());
    // Now make a packet 
    */ 

    dataPacket pack = new dataPacket(NexusClientApplet.myName,room,actual_command,data_len,data);
    // enqueue it now 
    q_.nq(pack); 

  } // my job is over 
  
}
// of class 


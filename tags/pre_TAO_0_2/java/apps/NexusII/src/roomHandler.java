/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:11  sumedh
   Added the Nexus II source code files.

# Revision 1.4  1996/12/07  06:22:49  rajeev
# taking a backup !
#
# Revision 1.3  1996/12/06  06:20:07  rajeev
# Changes to include leave handler
#
# Revision 1.2  1996/12/06  04:49:44  rajeev
# working now hopefully !
#
# Revision 1.1  1996/12/06  03:44:32  rajeev
# Initial revision
#

*/

// per room 
import java.awt.* ; 
import awtCommand.* ; 
import java.io.* ; 
import java.net.* ; 
//import NexusII.client.* ;
//import NexusII.util.* ; 
//import NexusII.networking.*;

public class roomHandler implements Runnable,consts{ 
  
  String roomName_ ; 
  MT_Bounded_Queue read_q_ ; 
  MT_Bounded_Queue write_q_ ; 
  RoomFrame rf_;
  Thread  mainThread ; 
  NexusClientApplet applet_ ; 

  // I dont know what to do currently 
  public roomHandler(String roomName, MT_Bounded_Queue reader_q,MT_Bounded_Queue writer_q,NexusClientApplet applet) { 
    roomName_ = roomName ; 
    read_q_   = reader_q ; 
    write_q_  = writer_q ; 
    applet_ = applet ; 
    rf_ = new RoomFrame(write_q_,applet_,roomName_);
    // whenever this frame gets the mouse focus, handler will be called 
    FrameHandler fh_ = new FrameHandler(applet_); 
    rf_.setGotFocusCommand(fh_); 
    mainThread = new Thread(this); 
    mainThread.start(); 
  }


  public void mystop() { 
    // dispose the graphics part here 
    if(DEBUG)
      System.out.println("Roomhandlers stop called \n");
    rf_.hide(); 
    rf_.dispose();
    // no longer need the roomframe
    rf_ = null ; 
    mainThread.stop(); 
  }

  public void run() {
    if(DEBUG) {
      System.out.println("Room receiver running ---- ");
    }
    while(Thread.currentThread() == mainThread) { 
      // get the data packet
      dataPacket packet = (dataPacket)(read_q_.dq()) ; 
      if(DEBUG)
	System.out.println("Room - I got something\n");
      
      // if the packet is of text type -- display it 
      if(packet.contentType().trim().equalsIgnoreCase("TEXT")) { 
	rf_.addText(packet.content()); 
      } 
      // if the packet is a url address -- pull the image across 
      // this url has to be an image  as of now 
      if(packet.contentType().equalsIgnoreCase("URL")) { 
	Image im = null ; 
	try { 
	  im = applet_.getImage(new URL(packet.content())); 
	} catch (MalformedURLException i) { 
	  // nothing as of now 
	}
	// display the image
	rf_.updateImage(im); 
	
      }
      
      if (DEBUG)
	System.out.println("roomhandler" + roomName_ + " got data");
    }
  }
  
  public String toString() { 
    return roomName_ ; 
  }

}  // of class roomHandler 

// is executed when the room frame gets mouse focus 

class FrameHandler implements Command { 
  NexusClientApplet applet_ ;
  public FrameHandler(NexusClientApplet applet) { 
    applet_ = applet ; 
  } 

  public void execute(Object target, Event evt, Object what) { 
    if(target instanceof Frame) { 
    applet_.SetRoomName(((RoomFrame)target).getTitle()); 
    } 
  } 
}


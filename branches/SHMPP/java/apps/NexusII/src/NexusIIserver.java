// The Nexus II server.
// Sumedh Mungee <sumedh@cs.wustl.edu>

import java.net.*;
import java.io.*;
import java.util.*;
import ACE.OS.*;
import ACE.SOCK_SAP.*;

public class NexusIIserver implements consts {

  // Entry point into the server
  public static void main(String args[]) throws IOException {
    
    if(args.length != 1) {
      System.out.println("Usage: java NexusIIserver <port_num>");
      return;
    }

    // Create a "Hotel", which is a factory to generate Rooms
    // as and when required.
    RoomFactory Hotel = new RoomFactory();

    System.out.println("NexusII server booting");
    SOCKAcceptor sacceptor = new SOCKAcceptor( (new Integer(args[0])).intValue());

    System.out.println("NexusII is now accepting connections on port " + (new Integer(args[0])).intValue());
    
    for(;;) { 
      
      SOCKStream s = new SOCKStream();
      sacceptor.accept(s);
      // Accepted connection
      // construct a client handler..
      // Pass in the connected socket as an argument,
      // and a reference to the Hotel, in case it needs
      // to create rooms..
      // and away you go..
      new Thread(new ClientHandler(s, Hotel)).start();
      
    }
  }
}  


// This thread handles the clients..
// It uses 2 additional threads for read/write network operations 
// These threads are dedicated to read/write from the
// respective read/write queues.. 
class ClientHandler implements Runnable,consts {
  
  private SOCKStream s_;
  private RoomFactory Hotel_;
  private MT_Bounded_Queue rq_ = new MT_Bounded_Queue(QUEUE_SIZE);
  private MT_Bounded_Queue wq_ = new MT_Bounded_Queue(QUEUE_SIZE);
  private String my_name_;
  private Vector roomlist_ = new Vector();
  private boolean finished_ = false;
  private String init_image_;
  
  public ClientHandler(SOCKStream s, RoomFactory h) {
    s_ = s;
    Hotel_ = h;
    init_image_ = new String(System.getProperty("mywebaddress") + NEXUS_LOGO);
  }
  
  public void run()  {
      // Construct the reader/writer threads with the queues and the
      // corresponding socket data streams as parameters.
      Thread r_ = new socketReaderThread(rq_, new DataInputStream(s_.inputStream()));
      Thread w_ = new socketWriterThread(wq_, new DataOutputStream(s_.outputStream()));
      r_.start();
      w_.start(); 
      
      // now start parsing the messages, and take action..
      // todo: optimize the below..

      while(!finished_) {
	dataPacket d = (dataPacket) rq_.dq();
	if(d.contentType().startsWith("INIT"))
	  nexus_init(d);
	if(d.contentType().startsWith("JOIN"))
	  nexus_join(d);
	if(d.contentType().startsWith("LEAVE"))
	  nexus_leave(d);
	if(d.contentType().startsWith("QUIT"))
	  nexus_quit(d);
	if(d.contentType().startsWith("TEXT"))
	  nexus_text(d);
	if(d.contentType().startsWith("LUSERS"))
	  nexus_lusers(d);
	if(d.contentType().startsWith("LROOMS"))
	  nexus_lrooms(d);
	if(d.contentType().startsWith("NICK"))
	  nexus_nick(d);
	if(d.contentType().startsWith("URL"))
	  nexus_url(d);
      }
  }
  
  // The following classes implement the server functions..
  
  private void nexus_init(dataPacket packet) {
    my_name_ = new String(packet.clientName());
    wq_.nq(packet);
  }

  private void nexus_join(dataPacket packet) {
    Room r = Hotel_.getRoom(packet.content());
    if(r.checkClient(this)) 
      return;
    r.addClient(this);
    roomlist_.addElement(r);
    writeRoom(r, my_name_ + " has joined the room ");
    String contenttype = new String("url");
    dataPacket d = new dataPacket(my_name_, packet.content(), contenttype, (new Integer(init_image_.length())).toString() , init_image_);
    wq_.nq(d);
  }

  private void nexus_text(dataPacket packet) {
    Room r = Hotel_.getRoom(packet.destination());
    r.getQ().nq(packet);
  }
  
  private void nexus_lusers(dataPacket packet) {

    Room r = Hotel_.getRoom(packet.content());
    Enumeration e = r.clientList();
    StringBuffer sb = new StringBuffer();
    while(e.hasMoreElements()) 
      sb.append(" " + ((ClientHandler)e.nextElement()).getName() + " ");
    dataPacket d = new dataPacket(my_name_, packet.destination(), packet.contentType(), (new Integer(sb.length())).toString(), sb.toString());
    wq_.nq(d);
  }
  
  private void nexus_lrooms(dataPacket packet) {
    String s = Hotel_.listRooms();
    dataPacket d = new dataPacket(my_name_, packet.destination(), packet.contentType(), (new Integer(s.length())).toString(), s.toString());
    wq_.nq(d);
  }
    
  private void nexus_nick(dataPacket packet) {
    Enumeration e = roomlist_.elements();
    while(e.hasMoreElements()) 
      writeRoom((Room)e.nextElement(), my_name_ + " is now known as " + packet.content());

    my_name_ = new String(packet.content());
  }

  private void nexus_leave(dataPacket packet) {
    
    Room r = Hotel_.getRoom(packet.content());
    writeRoom(r, my_name_ + " has left the room " + packet.content());
    if(r.delClient(this)) Hotel_.delRoom(r);
    roomlist_.removeElement(r);
  }      
  
  private void nexus_quit(dataPacket packet) {
    
    Enumeration e = roomlist_.elements();
    while(e.hasMoreElements()) {
      Room r = (Room)e.nextElement();
      writeRoom(r, my_name_ + " has quit " );
      r.delClient(this);
    }
    finished_ = true;
  }

  private void nexus_url(dataPacket packet) {
    try {
      URL u = new URL(packet.content());

      // first extract the filename stripped of its path.
      int index = u.getFile().lastIndexOf("/");
      String infilename = u.getFile().substring(index + 1);

      // next construct the name of the temporary file
      String outfilename = (System.getProperty("mywebdir") + "_" + packet.destination() + "." + infilename);

      // now the temporary URL assigned to this request
      String imageURL = new String(System.getProperty("mywebaddress") + "_" + packet.destination() + "." + infilename);

      // Open temporary file for writing
      FileOutputStream fout = new FileOutputStream(outfilename);

      // Now contact alien ship
      InputStream i = u.openStream();
      byte[] buffer = new byte[1024];
      
      // And download the image
      for(;;) {
	int num = i.read(buffer);
	if(num < 0) 
	  break;
	fout.write(buffer, 0, num);
      }

      fout.close();
      i.close();
      
      // Get room for which this request was issued
      Room r = Hotel_.getRoom(packet.destination());
      
      // invalidate previous entry
      File f = new File(r.getLastImageName());
      if(f.exists()) f.delete();

      // add new image name
      r.putNextImageName(outfilename);
      writeRoom(r,"Asynchronously transferring image " + packet.content() + " from " + my_name_ );
      dataPacket d = new dataPacket(my_name_, packet.destination(), packet.contentType(), (new Integer(imageURL.length())).toString(), imageURL);
      r.getQ().nq(d); // multicast this imageURL onto the room..
    
    }
    catch(java.net.MalformedURLException ue) {
      System.out.println("warning:Invalid URL requested");
    }
    catch(java.io.IOException e) {
      System.out.println("warning: IOException occurred");
    }
    
  }

  // Sends a "system" message msg onto room r
  private void writeRoom(Room r, String msg) {
    StringBuffer sb = new StringBuffer();
    sb.append("==>");
    sb.append(msg);
    dataPacket d = new dataPacket(my_name_, r.getName() , "TEXT" , (new Integer(sb.length())).toString(), sb.toString());
    r.getQ().nq(d);
  }

  public String getName() {
    return my_name_;
  }

  public MT_Bounded_Queue getQ() {
    return wq_;
  }
  
}
// ----------------------------------------------------------------------
/** This class implements a room factory. getRoom returns an existing room,
  or else creates it and returns a reference to a new room.
  
**/ 
class RoomFactory implements consts {

  private Vector Hotel_; 
  public RoomFactory() { 
    Hotel_ = new Vector(); 
  } 
  public synchronized Room getRoom(String name) {
    Enumeration e = Hotel_.elements();
    while(e.hasMoreElements()) {
      Room r = (Room) e.nextElement();
      if(r.getName().equals(name)) 
 	return r;
    }
    addRoom(name);
    return getRoom(name);
  } 
  
  private synchronized void addRoom(String name) {
    Room r = new Room(name);
    Hotel_.addElement(r);
  }
  public synchronized void delRoom(Room r) {
    Hotel_.removeElement(r);
  }
  public synchronized String listRooms() {
    Enumeration e = Hotel_.elements();
    StringBuffer sb = new StringBuffer();
    while(e.hasMoreElements())
      sb.append(" " + ((Room)e.nextElement()).getName() + " ");
    return sb.toString();
  }
}  



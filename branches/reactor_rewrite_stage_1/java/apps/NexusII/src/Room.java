// RoomThread and Room implement the concept of a chat "room"
// Sumedh Mungee <sumedh@cs.wustl.edu>


import java.util.*;
import java.io.File;

// This class(&thread) is responsible for multicasting
// packets on its incoming "client" queues, onto one or
// more outgoing queues, which are picked up by the client.
class RoomThread implements Runnable, consts {

  private MT_Bounded_Queue rq_;
  private Vector clientlist_;

  public RoomThread(MT_Bounded_Queue rq, Vector clientlist) {
    rq_ = rq;
    clientlist_ = clientlist;
  }
  
  public void run() {
    for(;;) {
      dataPacket d = (dataPacket) rq_.dq(); // Extract packet
      Enumeration e = clientlist_.elements(); // Iterate over clients
      while(e.hasMoreElements()) 
        ((ClientHandler)e.nextElement()).getQ().nq(d); // Enqueue packet
    }
  }
}


public class Room implements consts {
  
  private String name_;         // name of this "room" 
  private String last_image_ = new String("NexusII.gif"); // filename of the last image broadcast
  private Thread roomthread_;
  private  MT_Bounded_Queue rq_ = new MT_Bounded_Queue();
  private Vector clientlist_ = new Vector();
  
  // Constructors
  public Room(String name) {
    int i;
    name_ = new String(name);
    roomthread_ = new Thread(new RoomThread(rq_, clientlist_));
    roomthread_.start();
  } 
 
  // Client management methods follow..

  public synchronized void addClient(ClientHandler client) {
    clientlist_.addElement(client);
  }
  // Returns true if this room has now become empty
  public synchronized boolean delClient(ClientHandler client) {
    clientlist_.removeElement(client);
    return clientlist_.isEmpty();
  }
  
  public synchronized boolean checkClient(ClientHandler client) {
    return clientlist_.contains(client);
  }
  
  public synchronized Enumeration clientList() {
    return clientlist_.elements();
  }

  public String getName() {
    return name_;
  }

  public MT_Bounded_Queue getQ() {
    return rq_;
  }

  public synchronized String getLastImageName() {
    return last_image_;
  }

  public synchronized void putNextImageName(String s) {
    last_image_ = s;
  }

  protected void finalize() {
    roomthread_.stop();
    File f = new File(last_image_);
    if(f.exists()) 
      f.delete();
    roomthread_ = null;
  }
}








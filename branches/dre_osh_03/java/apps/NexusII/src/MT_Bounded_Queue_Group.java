//package NexusII.server;

//import NexusII.util.*;
import java.util.*;


public class MT_Bounded_Queue_Group {

  public Vector q_group_ = new Vector();

  public MT_Bounded_Queue_Group(int num) { 
  
    q_group_ = new Vector(num);
  
  }

  public MT_Bounded_Queue_Group() {

    //  q_group_ = new Vector();

  }

  public String toString() {
  
    return q_group_.toString();

  }

  // always returns true
  public boolean addToGroup(MT_Bounded_Queue q) {
  
    q_group_.addElement(q);
    return true; // for now
  
  }
  

  // returns false if the q was not part of the group
  public boolean delFromGroup(MT_Bounded_Queue q) {

    return q_group_.removeElement(q);

  }


  public boolean checkInGroup(MT_Bounded_Queue q) {

    return q_group_.contains(q);

  }


    // returns number of q's on which data was successfully enqueued
  public int nq(Object o) {
    
    if(q_group_.isEmpty() ) 
      return 0;
    
    Enumeration e = q_group_.elements();
    
      int i = 0;
      while(e.hasMoreElements()) {
	
	MT_Bounded_Queue q = (MT_Bounded_Queue) e.nextElement();
	q.nq(o);
	i++;
	
      }
      
      return i;
  }
  
}





  

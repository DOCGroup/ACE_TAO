
// package NexusII.util ; 

// The minimun functionality to be associated with a queue 
interface Queue 
{
public void nq(Object item);
public Object dq();
public boolean is_full();
public boolean is_empty();
public static final int DEF_SIZE = 1; 
}; 

// Specific Conditions associated with MT_Bounded_Queue 
// Is true if there is space in the queue 

class Not_Full_Condition extends Timed_Wait 
{

public Not_Full_Condition(MT_Bounded_Queue q)
  {
   super(q);
  }
  
public synchronized boolean condition()
  {
    MT_Bounded_Queue mq_ = (MT_Bounded_Queue) object_;
    return !mq_.is_full ();
  }
  
}

// Is true if there's something in the queue 

class Not_Empty_Condition extends Timed_Wait
{

public Not_Empty_Condition(MT_Bounded_Queue q)
  {
    super(q); 
  }
  
public synchronized boolean condition()
  {
    // Delegate to the appropriate conditional
    // check on the MessageQueue.
    MT_Bounded_Queue mq_ = (MT_Bounded_Queue) object_;
    return !mq_.is_empty ();
  }

private MT_Bounded_Queue mq_ ; 
}




// Encapsulates a bounded - synchronized queue 

public class MT_Bounded_Queue implements Queue 
{


private Object[] queue_ ; 
private int front_  ; 
private int back_   ; 
private int max_size_ ; 
private int size_ ; 
private Not_Empty_Condition not_empty_condition_ ; 
private Not_Full_Condition not_full_condition_ ; 
private int nq_count_ ; 
private int dq_count_ ; 

// The counters counts the number of nq's and dq's operations made on this
// instance of the queue 

public int dq_count()
{
  return dq_count_ ; 
}

public int nq_count()
{
  return nq_count_ ; 
}

public MT_Bounded_Queue()
  {
    // 	call the other constructor with DEF_SIZE 
    this(DEF_SIZE);
  }
  
public MT_Bounded_Queue(int max_size)
  {
    this.front_ = 0 ; 
    this.back_  = 0 ; 
    this.max_size_  = max_size ; 
    this.size_ = 0 ; 
    // these are included for STATISTICS 
    this.nq_count_ = 0 ; 
    this.dq_count_ = 0 ; 
    this.queue_ = new Object[this.max_size_];
    not_full_condition_  = new Not_Full_Condition(this);
    not_empty_condition_ = new Not_Empty_Condition(this);
  }

// Blocking nq 
public synchronized void nq(Object item)
  {
    // Wait till the queue has space 
    while(is_full())
      {
	try {
	  wait();
	} catch (InterruptedException e) {}
      }

    // enqueue here 
    queue_[back_] = item ; 
    back_ = (back_ + 1) % max_size_  ; 
    size_++ ; 
    // One more enqueue operation has occured 
    nq_count_ ++ ; 
    // wakeup the sleeping guys 
    notifyAll();
  }

  // Timed nq 
// returns -1 if timed_out 
public synchronized int nq(Object item,long msec_timeout)
  {
    // Wait till the queue has space 
    try {
      not_full_condition_.timed_wait(msec_timeout);
    } catch (InterruptedException e) {}
    catch (TimeoutException t) 
      { 
	return -1 ; 
      }
  
  // enqueue here 
  queue_[back_] = item ; 
  back_ = (back_ + 1) % max_size_  ; 
  size_++ ; 
  	
  // One more enqueue operation has occured 
  nq_count_ ++ ; 	
  // wakeup the sleeping consumers 
  not_empty_condition_.broadcast ();
  return 0 ; 
  }




  // Blockin dq 
public synchronized Object dq()
  {
    // wait till the queue has something in it 
    while(is_empty())
      {
	try {
	  wait();
	} catch (InterruptedException e) {}
      }

    // dequeue here 
    Object return_object = queue_[front_] ; 
    front_ = (front_ + 1) % max_size_ ; 
    size_ -- ; 
    // One more enqueue operation has occured 
    dq_count_ ++ ; 
    //wake up the sleeping producers
    notifyAll(); 
    return return_object ; 
  }


  // Timed dq 

public synchronized Object dq(long msec_timeout)
  {
    // wait till the queue has something in it 
    try {
      not_empty_condition_.timed_wait(msec_timeout);
    } catch (InterruptedException e) {}
    catch (TimeoutException t)
      {
	return null; 
      }

    // dequeue here 
    Object return_object = queue_[front_] ; 
    front_ = (front_ + 1) % max_size_ ; 
    size_ -- ; 
    
    // One more enqueue operation has occured 
    dq_count_ ++ ; 

    //wake up the sleeping guys 
    not_full_condition_.broadcast(); 
    return return_object ; 
  }

public boolean is_empty()
  {
    if (size_ == 0)
      return true ;
    else
      return false ; 
  
  } 

public boolean is_full()
  {
    if (size_ == max_size_)
      return true ; 
    else
      return false ; 
  }

}






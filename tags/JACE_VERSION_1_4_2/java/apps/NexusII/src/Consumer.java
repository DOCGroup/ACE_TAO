
// This class encapsulates a Consumer. The consumer dq's from the queue 
// Supports both a timed and blocking consumer. 
// Each instance of this class creates a different thread of control. 
// On timeout, the producer returns. 
// The producer tries to dq "iteration" number of times, unless it times out

//package NexusII.util ; 

public class Consumer extends Thread 
{
public static final int DEFAULT_ITERATIONS = 1 ; 
public Consumer(MT_Bounded_Queue queue)
  {
    this.queue_ = queue ; 
    this.iterations_ = new Integer(DEFAULT_ITERATIONS); 
    this.time_out_ = -1 ; 
  }

public Consumer(MT_Bounded_Queue queue, String name)
  {
    super(name); 
    this.queue_ = queue ; 
    this.iterations_ = new Integer(DEFAULT_ITERATIONS); 
    this.time_out_ = -1 ; 
  }


public Consumer(MT_Bounded_Queue queue, String name, Integer iterations)
  {
    super(name); 
    this.queue_ = queue ; 
    this.iterations_ = iterations ; 
    this.time_out_ = -1 ; 
  }


public Consumer(MT_Bounded_Queue queue, String name, Integer iterations, long msec_timeout)
  {
    super(name); 
    this.queue_ = queue ; 
    this.iterations_ = iterations ; 
    this.time_out_ = msec_timeout ; 
  }



public void run()
  {	
    for(int i=0;i<iterations_.intValue();i++)
      {
	if(time_out_ < 0)
	  System.out.println(getName() + ": dequeued " + queue_.dq());
	else
	  {
	    Object err = queue_.dq(time_out_); 
	    if(err == null)
	      {
		System.out.println(getName() + ": Timedout\n");
		return ; 
	      }
	    
	    else
	      System.out.println(getName() + ": dequeued " + err);
	  }
      }
  }
  

protected MT_Bounded_Queue queue_ ; 
private Integer iterations_ ; 
private long time_out_ ; 
}
  

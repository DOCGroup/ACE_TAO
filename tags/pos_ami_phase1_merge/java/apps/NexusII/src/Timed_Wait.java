// package NexusII.util ; 

 
// Subclass the Exception class to get TimeoutException 
  
class TimeoutException extends Exception
{
  public TimeoutException()
  {
    super();
  }
  
  public TimeoutException(String s)
  {
    super(s);
  }

}

// Timed_wait class. This can used by enq and deq to do timed_waits 
public abstract class Timed_Wait
{
  // By default the object is itself 
public Timed_Wait ()
  {	
    object_  = this;
  }
  // If the calling class specifies objects, delegate to it. 
public Timed_Wait (Object obj)
  {
    object_ = obj;
  }

  // This is the object we delegate to if a 
  // subclass gives us a particular object, 
  // otherwise, we ``delegate'' to ourself 
  // (i.e., to this).
protected Object object_;
  
  // This hook method must be overridden 
  // by a subclass to provide the condition.

public abstract boolean condition ();

  // This will borrow the monitor lock from the calling class 

public final void timed_wait(long msec_timeout) 
throws InterruptedException, TimeoutException 
  {
    // wait if the condition is false 
    if (!condition())
      {
	long start = System.currentTimeMillis() ; 
	long wait_time = msec_timeout ; 
	
	for(;;)
	  {
	    // anyway have to wait atleast till waittime 
	    object_.wait(wait_time);
	    
	    // on coming out check for the condition again 
	    if(!condition())
	      {
		long now = System.currentTimeMillis() ; 
		long time_so_far = now - start ; 

		// if timed out 
		if(time_so_far >= msec_timeout)
		  throw new TimeoutException() ; 
		else
		  // retry !! we have some time left 
		  wait_time = msec_timeout - time_so_far ; 
	      }
	    else // the condition is true here 
	      break ; 
	  }
      }
  }
  
  // Notify all threads waiting on the object_.
public final void broadcast () 
  {
    object_.notifyAll ();
  }
}


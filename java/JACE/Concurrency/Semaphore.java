/*************************************************
 *
 * = PACKAGE
 *    JACE.Concurrency
 *
 * = FILENAME
 *    Semaphore.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Concurrency;

import java.util.*;
import JACE.ASX.*;

/**
 *     Implementation of Dijkstra's counting semaphore in java.
 * <P>
 *     <EM>This class does not support recursive semantics.</EM>
 */
public class Semaphore extends LockAdapter
{
  static class TimedWaitSAdapter extends JACE.ASX.TimedWait
  {
    TimedWaitSAdapter (Object obj)
    {
      super (obj);
    }
  
    // Check to see if there are any semaphores available.
    public boolean condition ()
    {
      return this.count_ > 0;
    }
  
    // Increment the count by one
    public void increment ()
    {
      this.count_++;
    }
  
    // Decrement the count by one
    public void decrement ()
    {
      this.count_--;
    }
  
    // Set the count
    public void count (int c)
    {
      this.count_ = c;
    }
  
    public int count ()
    {
      return this.count_;
    }
  
    private int count_ = 0;
  }

  /**
   * Create a Semaphore.
   *@param count semaphore count
   */
  public Semaphore (int c)
  {
    this.monitor_.count (c);
    this.owners_ = new Hashtable (c);
  }
  
  /**
   * Create a binary Semaphore.
   */
  public Semaphore ()
  {
    this.monitor_.count (1);
    this.owners_ = new Hashtable (1);
  }
  
  public synchronized int tryAcquire ()
  {
    if (this.monitor_.condition ()) {
      this.monitor_.decrement ();
      setOwner ();
      return AbstractLock.SUCCESS;
    } else
      return AbstractLock.FAILURE;
  }
  
  /**
   * Acquire the Semaphore.  Throws a TimeoutException if the semaphore
   * isn't acquired before the given absolute time.
   *@param tv time (TimeValue) to wait until before throwing a
   * TimeoutException (unless the semaphore is acquired before that)
   *@exception TimeoutException wait timed out exception
   *@exception InterruptedException exception during wait
   */
  public synchronized int acquire (TimeValue tv)
    throws TimeoutException, InterruptedException 
  {
    if (this.monitor_.condition ()) {
      this.monitor_.decrement ();
      setOwner ();
      return AbstractLock.SUCCESS;
    }

    numberOfWaiters_++;
    
    try {
      sleepHook ();
      this.monitor_.timedWait (tv);
    } finally {
      numberOfWaiters_--;
    }
    
    this.monitor_.decrement ();
    setOwner ();

    return AbstractLock.SLEEPHOOK;
  }
  
  public synchronized int release ()
  {
    if (!isOwner ())
      return AbstractLock.FAILURE;
    
    if (!signalNextRenewer ()) {
      this.monitor_.increment ();
      this.monitor_.signal ();
      clearOwner ();
    }
    
    return AbstractLock.SUCCESS;
  }
  
  /**
   * Checks any objects in the renewers queue, giving one of them
   * the lock if it is appropriate.  Assumes the synchronization
   * lock is already held.
   *
   *@return true if a renewer was signaled, else false
   */
  protected boolean signalNextRenewer ()
  {
    // First find the renewer with the minimum yieldTo count, processing
    // all of them along the way.
    if (this.renewers_.size() > 0) {
      
      RenewObject renewer = (RenewObject)renewers_.
	elementAt (renewers_.size () - 1);
      
      renewer.decrementYieldTo ();
      
      for (int i = this.renewers_.size() - 2; i >=0; i--) {
	
	RenewObject rwo = (RenewObject)renewers_.elementAt (i);
	
	rwo.decrementYieldTo ();
	
	renewer = renewer.min (rwo);
      }
      
      // If the renewer with the minimum yieldTo count has yielded to
      // enough threads, or if there are no waiting threads, it should
      // be signaled (thus, it wakes up and obtains the lock again).
      
      if (renewer.condition () || numberOfWaiters_ == 0) {
	// Note that we leave monitor_.inUse in the true state so
	// we are assured that only the renewer (and not another
	// Thread that does an acquire) will gain control.  This
	// is important since the renew method can't be synchronized
	// in its current implementation.
	renewers_.removeElement(renewer);

	synchronized (renewer) {
	  renewer.signal ();
	}
  
	return true;
      }
    }

    return false;
  }

  public int renew (int requeuePosition, 
		    JACE.ASX.TimeValue timeout) 
    throws InterruptedException,
	   TimeoutException
  {
    RenewObject rwo;

    synchronized (this) {

      if (!this.isOwner ())
	return AbstractLock.FAILURE;
      
      if (numberOfWaiters_ == 0 || 
	  requeuePosition == 0 ||
	  this.monitor_.condition ())
	return AbstractLock.SUCCESS;

      if (requeuePosition < 0 || requeuePosition > numberOfWaiters_)
	requeuePosition = numberOfWaiters_;
      
      rwo = new RenewObject (requeuePosition);

      this.release ();
      this.renewers_.addElement (rwo);
    }

    boolean exceptionOccured = true;
    try {
      synchronized (rwo) {
	rwo.timedWait (timeout);
	
	exceptionOccured = false;
      }
    } finally {
      synchronized (this) {

	if (exceptionOccured) {
	  if (!renewers_.removeElement (rwo)) {  
	    setOwner ();
	    release ();
	  } 
	} else {
	  setOwner();
	}
      }
    }
  
    // By this point, we should know that we have the lock.  The inUse
    // flag is never set to false in the release() call from the Thread
    // that gave us control.  That thread also set the owner value.
    
    return AbstractLock.SUCCESS;
  }

  protected boolean isOwner ()
  {
    return owners_.containsKey (accessorID());
  }

  protected void setOwner ()
  {
    owners_.put (accessorID(), this);
  }

  protected void clearOwner ()
  {
    owners_.remove (accessorID());
  }

  private TimedWaitSAdapter monitor_ = new TimedWaitSAdapter (this);
  // The monitor (adapter) to wait on

  private Hashtable owners_;
  private Vector renewers_ = new Vector ();
  private int numberOfWaiters_ = 0;
}

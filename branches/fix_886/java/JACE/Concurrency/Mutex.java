/*************************************************
 *
 * = PACKAGE
 *    JACE.Concurrency
 *
 * = FILENAME
 *    Mutex.java
 *
 *@author Prashant Jain
 *@author Everett Anderson
 *
 *************************************************/
package JACE.Concurrency;

import java.util.*;
import JACE.ASX.*;

/**
 *     Value added abstraction for mutex variable creation.
 *
 *     A mutex whose operations do not block forever and can time out.
 * <P>
 * <EM>This class does not support recursive semantics.</EM>
 */
public class Mutex extends LockAdapter
{
  /**
   * Acquire ownership of the lock, blocking indefinitely if necessary.  
   * <P>
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception InterruptedException indicates another thread has 
   *           interrupted this one during wait
   */
  public synchronized int acquire () throws InterruptedException
  {
    if (this.monitor_.condition ()) {
      this.monitor_.condition (false);
      setOwner ();
      return AbstractLock.SUCCESS;
    }

    this.numberOfWaiters_++;
    try {
      sleepHook ();
      this.monitor_.timedWait ();
    } finally {
      this.numberOfWaiters_--;
    }
    this.monitor_.condition (false);
    setOwner();
  
    return AbstractLock.SLEEPHOOK;
  }

  public int renew (int requeuePosition, 
		    TimeValue timeout) 
    throws InterruptedException,
	   TimeoutException
  {
    RenewObject rwo;

    synchronized (this) {

      if (!this.isOwner ())
	return AbstractLock.FAILURE;

      if (numberOfWaiters_ == 0 || requeuePosition == 0)
	return AbstractLock.SUCCESS;

      if (requeuePosition < 0 || requeuePosition > numberOfWaiters_)
	requeuePosition = numberOfWaiters_;
      
      rwo = new RenewObject (requeuePosition);
      
      this.release ();
      this.renewers_.addElement (rwo);
    }

    // We can't have the method synchronized, or synchronize on (this)
    // in here because then the Thread that was woken up won't be able
    // to continue its acquire.
    //
    // Normally when an exception occurs in timedWait, this thread just
    // needs to remove itself from the renewers queue.
    //
    // However, the following situation exists:
    //   Thread A is the current owner, and is doing processing in release()
    //   This thread generates a timeout exception in timedWait
    //   Thread A signals this thread to wake up and take ownership, and
    //     removes it from the queue.
    //   This thread never takes ownership -- the exception keeps going up.
    //
    // This could lead to other renewers waiting in limbo forever.
    //
    // Solution: If this thread has an exception and it looks like it
    // has been proclaimed the owner, then it calls release and lets
    // the exception continue.

    boolean exceptionOccured = true;
    try {
      synchronized (rwo) {
	rwo.timedWait (timeout);

	exceptionOccured = false;
      }
    } finally {
      if (exceptionOccured) {
	synchronized (this) {
	  if (!renewers_.removeElement (rwo)) {
	    setOwner ();
	    release ();
	  }
	}
      }
    }

    synchronized (this) {
      setOwner ();
    }

    // By this point, we should know that we have the lock.  The condition
    // flag is never set to true in the release() call from the Thread
    // that gave us control.

    return AbstractLock.SUCCESS;
  }
    
  public synchronized int tryAcquire () {
    if (this.monitor_.condition ()) {
      this.monitor_.condition (false);
      setOwner();
      return AbstractLock.SUCCESS;
    } else
      return AbstractLock.FAILURE;
  }

  public synchronized int acquire (TimeValue tv) 
    throws TimeoutException, InterruptedException 
  {
    if (this.monitor_.condition ()) {
      this.monitor_.condition (false);
      setOwner ();
      return AbstractLock.SUCCESS;
    }

    this.numberOfWaiters_++;
    try {
      sleepHook ();
      this.monitor_.timedWait (tv);
    } finally {
      this.numberOfWaiters_--;
    }
    this.monitor_.condition (false);
    setOwner();
    
    return AbstractLock.SLEEPHOOK;
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
	// Note that we leave monitor_.condition in the false state so
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

  public synchronized int release ()
  {
    if (!isOwner()) 
      return AbstractLock.FAILURE;

    if (!signalNextRenewer ()) {
      // Do a normal release if there are no threads waiting to renew
      // or no such threads are ready to renew.
      this.monitor_.condition (true);
      this.monitor_.signal ();
    }
    
    return AbstractLock.SUCCESS;
  }

  /**
   * Monitor used to signal whether or not this Mutex is available.
   */
  protected WaitObject monitor_ = new WaitObject (true, this);
  // The monitor (adapter) to wait on

  /**
   * Queue of waiting renewers.
   */
  protected Vector renewers_ = new Vector ();

  /**
   * Number of waiting threads.
   */
  protected int numberOfWaiters_ = 0;
}

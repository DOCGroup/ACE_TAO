/*************************************************
 *
 * = PACKAGE
 *    JACE.Concurrency
 *
 * = FILENAME
 *    Condition.java
 *
 *@author Irfan Pyarali
 *
 *************************************************/
package JACE.Concurrency;

import JACE.ASX.TimeoutException;
import JACE.ASX.TimeValue;

/**
 *     Abstraction for <em>traditional</em> 
 *     condition variable
 * <P>
 *     This condition variable allows the use of one 
 *     mutex between multiple conditions. 
 *     This implementation is based on the C++ version of ACE.
 */
public class Condition
{
  /** 
   * Default constructor
   *@param Mutex for synchronization
   */
  public Condition (Mutex mutex)
  {
    mutex_ = mutex;
  }
  
  /**
   * Wait for condition to become signaled. 
   *@exception InterruptedException exception during wait
   */
  public void Wait () 
    throws InterruptedException
  {
    waiters_++;

    try
      {
	mutex_.release();
	synchronized (waitObject_) {
	  waitObject_.wait ();
	}
	mutex_.acquire ();
      }
    finally 
      {
	waiters_--;
      }
  }
  
  /**
   * TimedWait for condition to become signaled.  Note that the
   * given TimeValue is an absolute time, not a relative time.
   *
   *@param tv Absolute time to wait until before timing out
   *@exception TimeoutException wait timed out exception
   *@exception InterruptedException exception during wait
   */
  public void Wait (TimeValue tv) 
      throws TimeoutException, InterruptedException
  {
    waiters_++;

    try
      {
	mutex_.release();

	synchronized (waitObject_) {
	  long start = System.currentTimeMillis();
	  long waitTime = tv.getMilliTime() - start;
	  if (waitTime < 1)
	    throw new TimeoutException ();
	  waitObject_.wait (waitTime);
	}

	mutex_.acquire (tv);
      }
    finally 
      {
	waiters_--;
      }
  }
  
  /**
   * Signal condition. Wake one waiter (if any).
   */
  public void signal ()
  {
    synchronized (waitObject_) {
      waitObject_.notify ();
    }
  }

  /**
   * Signal condition. Wake up all waiters (if any).
   */
  public void broadcast ()
  {
    synchronized (waitObject_) {
      waitObject_.notifyAll ();
    }
  }

  /** 
   * Accessor to lock
   *@return Mutex
   */
  public Mutex mutex ()
  {
    return mutex_;
  }

  private int waiters_;  
  private Object waitObject_ = new Object ();
  private Mutex mutex_;
}

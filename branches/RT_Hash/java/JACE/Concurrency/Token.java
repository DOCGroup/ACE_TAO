/*************************************************
 *
 * = PACKAGE
 *    JACE.Concurrency
 *
 * = FILENAME
 *    Token.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Concurrency;

import java.util.*;
import JACE.ASX.*;

import JACE.OS.*;

/**
 *     Class that acquires, renews, and releases a synchronization
 *     token that is serviced in strict FIFO ordering.
 * <P>
 *     This is a general-purpose synchronization mechanism that offers
 *     several benefits. For example, it implements "recursive mutex"
 *     semantics, where a thread that owns the token can reacquire it
 *     without deadlocking.  In addition, threads that are blocked
 *     awaiting the token are serviced in strict FIFO order as other
 *     threads release the token. The solution makes use of the
 *     Specific Notification pattern presented by Tom Cargill in
 *     "Specific Notification for Java Thread Synchronization," PLoP96.
 *
 * <P>
 *     This class DOES support recursive semantics.
 */
public class Token extends LockAdapter
{
  /**
   * Acquire ownership of the lock, blocking indefinitely if necessary.  
   * <P>
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception InterruptedException indicates another thread has 
   *           interrupted this one during wait
   */
  public int acquire () throws InterruptedException
  {
    try
      {
	return this.acquire (null);
      }
    catch (TimeoutException e)
      {
	// This really shouldn't happen since we are supposed to
	// block.
	return AbstractLock.FAILURE;
      }
  }

  /**
   * Acquire the token by the given absolute time time-out.  The
   * method uses synchronized blocks internally to avoid race conditions.
   *@param     timeout time to wait until before throwing a
   *           TimeoutException (unless the token is acquired before that).
   *           Performs a blocking acquire if the given timeout is null.
   *@return    AbstractLock.SUCCESS if acquires without calling <sleepHook>
   *           AbstractLock.SLEEPHOOK if <sleepHook> is called.
   *           AbstractLock.FAILURE if failure occurs 
   *@exception TimeoutException if time-out occurs
   *@exception InterruptedException exception during wait
   */
  public int acquire (TimeValue timeout) throws TimeoutException,
  InterruptedException
  {
    int result = AbstractLock.SUCCESS;
    WaitObject snl = new WaitObject ();
    boolean mustWait;
    synchronized (snl)
      {
	synchronized (this)
	  {
	    mustWait = !this.snq_.isEmpty ();
	      
	    if (mustWait && isOwner ())
	      {
		// I am the one who has the token. So just increment
		// the nesting level
		this.nestingLevel_++;
		return AbstractLock.SUCCESS;
	      }
	    // Add local lock to the queue
	    this.snq_.addElement (snl);
	  }
	if (mustWait)
	  {
	    result = AbstractLock.SLEEPHOOK;
	    sleepHook();

	    boolean exceptionOccured = true;
	    try {
	      snl.timedWait(timeout);
	      exceptionOccured = false;
	    } finally {
	      if (exceptionOccured) {
		synchronized (this) {
		  if (!snq_.removeElement (snl)) {
		    setOwner ();
		    release ();
		  }
		}
	      }
	    }
	  }
	  
	// Set the owner of the token
	synchronized (this) {
	  setOwner();
	}
      }

    return result;
  }

  /**
   * Try to acquire the token. Implements a non-blocking acquire.
   *
   *@return   AbstractLock.SUCCESS if acquires
   *          AbstractLock.FAILURE if failure occurs
   */
  public synchronized int tryAcquire ()
  {
    int result = AbstractLock.SUCCESS;

    if (this.snq_.isEmpty ())
      {
	// No one has the token, so acquire it
	this.snq_.addElement (new WaitObject ());
	  
	setOwner();
      }
    else if (isOwner())
      {
	this.nestingLevel_++;
      }
    // Someone else has the token.
    else
      {
	// Would have to block to acquire the token, so return
	// failure.
	result = AbstractLock.FAILURE;
      }
    return result;
  }

  /**
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token.  If the given TimeValue
   * is null, it's the same as calling renew(int requeuePosition).
   *@param requeuePosition Position in the queue where to insert the
   * lock. If requeuePosition == -1 and there are other threads
   * waiting to obtain the token we are queued at the end of the list
   * of waiters.  If requeuePosition > -1 then it indicates how many
   * entries to skip over before inserting our thread into the list of
   * waiters (e.g.,requeuePosition == 0 means "insert at front of the
   * queue").
   *@param timeout Throw a TimeoutException if the token isn't renewed
   * before this absolute time timeout.
   *@return    AbstractLock.SUCCESS if renewed the lock
   *           AbstractLock.FAILURE if failure occurs
   *@exception TimeoutException exception if timeout occurs
   *@exception InterruptedException exception during wait
   */
  public int renew (int requeuePosition, TimeValue timeout) 
    throws TimeoutException, InterruptedException
  {
    WaitObject snl = null;
    int saveNestingLevel = 0;

    synchronized (this)
      {
	if (!isOwner ()) 
	  return AbstractLock.FAILURE;

        // Check if there is a thread waiting to acquire the token. If
        // not or if requeuePosition == 0, then we don't do anything
        // and we simply keep the token. 
        if (this.snq_.size () > 1 && requeuePosition != 0)
	  {
            // Save the nesting level
            saveNestingLevel = this.nestingLevel_;
            this.nestingLevel_ = 0;
            
            // Reinsert ourselves at requeuePosition in the queue
            snl = (WaitObject) this.snq_.firstElement ();
            this.snq_.removeElementAt (0);
            
            if (requeuePosition < 0)
	      this.snq_.addElement (snl);  // Insert at end
            else
	      this.snq_.insertElementAt (snl, Math.min(requeuePosition,
						       this.snq_.size()));
            
            synchronized (this.snq_.firstElement ())
	      {
                // Notify the first waiting thread in the queue
                WaitObject obj = (WaitObject) this.snq_.firstElement ();
                // Set its condition to be true so that it falls out
                // of the for loop
                obj.condition (true);
                // Now signal the thread
                obj.signal ();
	      }    
	  }
      }
    
    // Check if we reinserted the lock in the queue and therefore need
    // to do a wait 
    if (snl != null)
      {
        synchronized (snl)
	  {
            // Set the condition to be false so that we can begin the
            // wait 
            snl.condition (false);
	    // Wait until the given absolute time (or until notified
	    // if the timeout is null)
	    
	    boolean exceptionOccured = true;
	    try {

	      snl.timedWait (timeout);

	      exceptionOccured = false;

	    } finally {
	      if (exceptionOccured) {
		synchronized (this) {
		  if (!snq_.removeElement (snl)) {
		    setOwner ();
		    release ();
		  }
		}
	      }
	    }
	  }

	synchronized (this) {
	  // Restore the nesting level and current owner of the lock
	  this.nestingLevel_ = saveNestingLevel;
	  
	  // Set the owner of the token
	  setOwner();
	}
      }

    return AbstractLock.SUCCESS;
  }

  /**
   * Release the token.  It is safe for non-owners to call
   * this.
   *@return    AbstractLock.SUCCESS on success
   *           AbstractLock.FAILURE on failure (for instance, a non-owner
   *           calling release)
   */
  public synchronized int release ()
  {
    if (!isOwner())
      return AbstractLock.FAILURE;
    
    // Check if nestingLevel > 0 and if so, decrement it
    if (this.nestingLevel_ > 0)
      this.nestingLevel_--;
    else
      {
	clearOwner ();
	this.snq_.removeElementAt (0);
	if (!this.snq_.isEmpty ())
	  {
	    synchronized (this.snq_.firstElement ())
	      {
		// Notify the first waiting thread in the queue
		WaitObject obj = (WaitObject) this.snq_.firstElement ();
		// Set its condition to be true so that it falls out
		// of the for loop
		obj.condition (true);
		// Now signal the thread
		obj.signal ();
	      }
	  } 
      }
    
    return AbstractLock.SUCCESS;
  }
  
  private Vector snq_ = new Vector ();
  // Vector of lock objects
  
  private int nestingLevel_ = 0;
  // Current Nesting Level
}

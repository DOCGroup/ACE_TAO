/*************************************************
 *
 * = PACKAGE
 *    ACE.Concurrency
 *
 * = FILENAME
 *    Token.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.Concurrency;

import java.util.*;
import ACE.ASX.*;

class WaitObject extends TimedWait
{
  public boolean condition ()
  {
    return this.condition_;
  }
  
  public void condition (boolean c)
  {
    this.condition_ = c;
  }

  private boolean condition_ = false;
}

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Class that acquires, renews, and releases a synchronization
 *     token that is serviced in strict FIFO ordering.
 *
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *     This is a general-purpose synchronization mechanism that offers
 *     several benefits. For example, it implements "recursive mutex"
 *     semantics, where a thread that owns the token can reacquire it
 *     without deadlocking.  In addition, threads that are blocked
 *     awaiting the token are serviced in strict FIFO order as other
 *     threads release the token. The solution makes use of the
 *     Specific Notification pattern presented by Tom Cargill in
 *     "Specific Notification for Java Thread Synchronization," PLoP96.
 *</blockquote>
 */
public class Token
{
  /**
   * Acquire the token. Note that this will block. The method uses
   * synchronized blocks internally to avoid race conditions.
   *@return 0 if acquires without calling <sleepHook>
   * 1 if <sleepHook> is called.
   * -1 if failure occurs
   *@exception InterruptedException exception during wait
   */
  public int acquire () throws InterruptedException
    {
      try
	{
	  return this.acquire (new TimeValue ());
	}
      catch (TimeoutException e)
	{
	  // This really shouldn't happen since we are supposed to
	  // block.
	  return -1;
	}
    }

  /**
   * Acquire the token. Wait for timeout amount of time. The method
   * uses synchronized blocks internally to avoid race conditions. 
   *@param timeout Amount of time to wait for in trying to acquire the
   * token.
   *@return 0 if acquires without calling <sleepHook>
   * 1 if <sleepHook> is called.
   * -1 if failure occurs
   *@exception TimeoutException exception if timeout occurs
   *@exception InterruptedException exception during wait
   */
  public int acquire (TimeValue timeout) throws InterruptedException, TimeoutException
    {
      int result = 0;
      WaitObject snl = new WaitObject ();
      boolean mustWait;
      synchronized (snl)
	{
	  synchronized (this)
	    {
	      mustWait = !this.snq_.isEmpty ();
	      if (mustWait &&
		  Thread.currentThread ().toString ().compareTo (this.owner_) == 0)
		{
		  // I am the one who has the token. So just increment
		  // the nesting level
		  this.nestingLevel_++;
		  return result;
		}
	      // Add local lock to the queue
	      this.snq_.addElement (snl);
	    }
	  if (mustWait)
	    {
	      result = 1;
	      // Call sleep hook
	      sleepHook ();
	      snl.timedWait (timeout);  // Do a blocking wait
	    }
	  // Set the owner of the token
	  this.owner_ = Thread.currentThread ().toString ();
	}
      return result;
    }

  /**
   * Try to acquire the token. Implements a non-blocking acquire.
   *@return 0 if acquires without calling <sleepHook>
   * 1 if <sleepHook> is called.
   * -1 if failure occurs
   */
  public synchronized int tryAcquire ()
    {
      int result = 0;
      if (!this.snq_.isEmpty ())
	{
	  // No one has the token, so acquire it
	  this.snq_.addElement (new WaitObject ());
	}
      // Check if I am the one holding the token.
      else if (Thread.currentThread ().toString ().compareTo (this.owner_) == 0)
	{
	  this.nestingLevel_++;
	}
      // Someone else has the token.
      else
	{
	  // Will have to block to acquire the token, so call
	  // sleepHook and return 
	  sleepHook ();
	  result = 1;
	}
      return result;
    }

  /**
   * Method that is called before a thread goes to sleep in an
   * acquire(). This should be overridden by a subclass to define 
   * the appropriate behavior before acquire() goes to sleep.
   * By default, this is a no-op.
   */
  public void sleepHook ()
  {
  }

  /**
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token. 
   *@param requeuePosition Position in the queue where to insert the
   * lock. If requeuePosition == -1 and there are other threads
   * waiting to obtain the token we are queued at the end of the list
   * of waiters.  If requeuePosition > -1 then it indicates how many
   * entries to skip over before inserting our thread into the list of
   * waiters (e.g.,requeuePosition == 0 means "insert at front of the
   * queue"). 
   *@exception InterruptedException exception during wait
   */
  public void renew (int requeuePosition) throws InterruptedException
  {
    try
      {
	this.renew (requeuePosition, new TimeValue ());
      }
    catch (TimeoutException e)
      {
	// This really shouldn't happen since we are supposed to
	// block.
      }
  }

  /**
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token. 
   *@param requeuePosition Position in the queue where to insert the
   * lock. If requeuePosition == -1 and there are other threads
   * waiting to obtain the token we are queued at the end of the list
   * of waiters.  If requeuePosition > -1 then it indicates how many
   * entries to skip over before inserting our thread into the list of
   * waiters (e.g.,requeuePosition == 0 means "insert at front of the
   * queue").
   *@param timeout Amount of time to wait for in trying to acquire the
   * token.
   *@exception TimeoutException exception if timeout occurs
   *@exception InterruptedException exception during wait
   */
  public void renew (int requeuePosition, TimeValue timeout) 
    throws InterruptedException, TimeoutException
  {
    WaitObject snl = null;
    int saveNestingLevel = 0;

    synchronized (this)
      {
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
	      this.snq_.insertElementAt (snl, requeuePosition);

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
	    // Do a blocking wait
	    snl.timedWait (timeout);
	  }
	// Restore the nesting level and current owner of the lock
	this.nestingLevel_ = saveNestingLevel;
	this.owner_ = Thread.currentThread ().toString ();
      }
  }

  /**
   * Release the token.
   */
  public synchronized void release ()
  {
    // Check if nestingLevel > 0 and if so, decrement it
    if (this.nestingLevel_ > 0)
      this.nestingLevel_--;
    else
      {
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
  }

  private Vector snq_ = new Vector ();
  // Vector of lock objects

  private int nestingLevel_ = 0;
  // Current Nesting Level

  private String owner_ = null;
  // Current owner of the token.
}

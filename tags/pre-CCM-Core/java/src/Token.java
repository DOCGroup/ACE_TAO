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
   * synchronized blocks internally to avoid race conditions.  It
   * ignores thread interrupts.
   *@return 0 if acquires without calling <sleepHook>
   * 1 if <sleepHook> is called.
   * -1 if failure occurs (should never happen)
   */
  public int acquire ()
    {
      try
	{
	    return this.acquire (null);
	}
      catch (TimeoutException e)
	{
	  // This really shouldn't happen since we are supposed to
	  // block.
	  return -1;
	}
    }

  /**
   * Acquire the token.  Returns failure   
   * Throws a TimeoutException if the token isn't acquired before the
   * given absolute time timeout.
   *@param timeout time (TimeValue) to wait until before throwing a
   * TimeoutException (unless the token is acquired before that).
   * Performs a blocking acquire if the given timeout is null.
   *@return 0 if acquires without calling <sleepHook>
   * 1 if <sleepHook> is called.
   * -1 if failure occurs (timeout)
   */
  public int acquire (TimeValue timeout) throws TimeoutException
    {
      int result = 0;
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
		  return 0;
		}
	      // Add local lock to the queue
	      this.snq_.addElement (snl);
	    }
	  if (mustWait)
	  {
	      result = 1;
	      sleepHook();

	      while (mustWait) { 
		  try {
		      snl.timedWait(timeout);
		      mustWait = false;
		  } catch (InterruptedException e) {
		      // must keep waiting
		  }
	      }
	  }

	  // Set the owner of the token
	  setOwner();
	}
      return result;
    }

  /**
   * Try to acquire the token. Implements a non-blocking acquire.
   *@return 0 if acquires without calling <sleepHook>
   * -1 if failure occurs
   */
  public synchronized int tryAcquire ()
    {
      int result = 0;

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
	  result = -1;
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
   * other threads waiting to get the token.  This blocks until it
   * can regain the token.
   *@param requeuePosition Position in the queue where to insert the
   * lock. If requeuePosition == -1 and there are other threads
   * waiting to obtain the token we are queued at the end of the list
   * of waiters.  If requeuePosition > -1 then it indicates how many
   * entries to skip over before inserting our thread into the list of
   * waiters (e.g.,requeuePosition == 0 means "insert at front of the
   * queue"). 
   */
  public void renew (int requeuePosition)
  {
    try
      {
	this.renew (requeuePosition, null);
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
   *@exception TimeoutException exception if timeout occurs
   */
  public void renew (int requeuePosition, TimeValue timeout) 
    throws TimeoutException
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
	    boolean mustWait = true;
	    while (mustWait) {
		try {
		    snl.timedWait (timeout);
		    mustWait = false;
		} catch (InterruptedException e) { 
		    // must keep waiting
		}
	    }
        }
        // Restore the nesting level and current owner of the lock
        this.nestingLevel_ = saveNestingLevel;

        // Set the owner of the token
        setOwner();
    }
  }

  /**
   * Release the token.  It is safe for non-owners to call
   * this.
   */
  public synchronized void release ()
  {
    if (!isOwner())
        return;

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

  // The next two methods allow subclasses to change the behavior of the
  // checking and setting the Object owner_ member variable.  The default
  // is to use the current Thread's toString() as the Object.
  protected void setOwner() {
      this.owner_ = Thread.currentThread().toString();
  }

  protected boolean isOwner() {
      return Thread.currentThread().toString().equals(this.owner_);
  }

  private Vector snq_ = new Vector ();
  // Vector of lock objects

  private int nestingLevel_ = 0;
  // Current Nesting Level

  private Object owner_ = null;
  // Current owner of the token.  The setOwner() and isOwner()
  // methods provide subclasses with the ability to change the
  // behavior.  The default is to use the Thread.toString().
}

/*************************************************
 *
 * = PACKAGE
 *    JACE.Concurrency
 *
 * = FILENAME
 *    Lock.java
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.Concurrency;

import JACE.ASX.*;

/**
 *      Abstract adapter class which provides useful default implementations
 *      for several methods in the AbstractLock interface, as well as 
 *      protected helper functions for making sure only the owner
 *      can perform certain operations.
 *
 *@see  JACE.Concurrency.AbstractLock
 */
public abstract class LockAdapter implements AbstractLock
{
  /**
   * Default implementation that calls acquire (TimeValue) with a null
   * timeout.
   *
   *@see AbstractLock#acquire
   */
  public int acquire () throws InterruptedException
  {
    try {
      return acquire (null);
    } catch (TimeoutException e) {
      // This should never happen
      return AbstractLock.FAILURE;
    }
  }

  /**
   * Acquire ownership of the lock by the given absolute time time-out.
   * A value of null for the timeout parameter results in a blocking
   * acquire.
   * A value of TimeValue.zero throws a TimeoutException if the
   * acquire would block.
   * <P>
   *@param timeout  absolute time by which the lock must be acquired
   *@return         appropriate Lock return value (AbstractLock.FAILURE, 
   *                AbstractLock.SUCCESS or AbstractLock.SLEEPHOOK)
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@exception      InterruptedException indicates another thread has 
   *                interrupted this one during wait
   *@see AbstractLock#tryAcquire
   */
  public abstract int acquire (TimeValue timeout) 
    throws TimeoutException, InterruptedException;

  /**
   * Default implementation that calls acquireRead (TimeValue) with a
   * null timeout.
   *
   *@see AbstractLock#acquireRead
   */
  public int acquireRead () throws InterruptedException
  {
    try {
      return acquireRead (null);
    } catch (TimeoutException e) {
      // This should never happen
    }

    return AbstractLock.FAILURE;
  }

  /**
   * Default implementation that calls acquire (TimeValue).
   *
   *@see AbstractLock#acquireRead(TimeValue)
   */
  public int acquireRead (TimeValue timeout) 
    throws TimeoutException, InterruptedException
  {
    return acquire (timeout);
  }

  /**
   * Default implementation that calls acquire with a null
   * timeout.
   *
   *@see AbstractLock#acquireWrite
   */
  public int acquireWrite () throws InterruptedException
  {
    try {
      return acquire (null);
    } catch (TimeoutException e) {
      // This should never happen
    }

    return AbstractLock.FAILURE;
  }

  /**
   * Default implementation that calls acquire (TimeValue).
   *
   *@see AbstractLock#acquireWrite(TimeValue)
   */
  public int acquireWrite (TimeValue timeout) 
    throws TimeoutException, InterruptedException
  {
    return acquire (timeout);
  }
  
  /**
   * Default implementation that calls renew (int, TimeValue) with
   * a null timeout.
   *
   *@see AbstractLock#renew(int)
   */
  public int renew (int requeuePosition) throws InterruptedException
  {
    try 
      {
	return renew (requeuePosition, null);
      } catch (TimeoutException e) {
	// Note that this should never happen since we requested a
	// blocking acquire.
	return AbstractLock.FAILURE;
      }
  }

  /**
   * Give up the lock to some waiting threads (if any), then reacquire
   * by the given absolute time time-out.
   * <P>
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token. 
   * <P>
   * A value of null for the timeout should indicate a blocking renew.
   * <P>
   *@param     requeuePosition position in the waiters queue to insert
   *           this thread.  If this value is -1 and there are other
   *           threads waiting to obtain the token, this thread is queued
   *           at the end.  If this value is greater than -1, then it
   *           indicates how many entries to skip over before inserting
   *           our thread into the queue.  (For example, if it is 0,
   *           this thread is put at the front of the queue.)  If this 
   *           value is greater than the number of waiters, this thread is
   *           simply put at the end of the current waiters queue.
   * 
   *@param     timeout absolute time by which the lock must be reacquired
   *
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@exception      InterruptedException indicates another thread has 
   *                interrupted this one during wait
   */
  public abstract int renew (int requeuePosition,
			     TimeValue timeout)
			     throws TimeoutException,
				    InterruptedException;

  /**
   * Default implementation that calls tryAcquire ().
   *
   *@see AbstractLock#tryAcquireRead
   */
  public int tryAcquireRead ()
  {
    return tryAcquire ();
  }

  /**
   * Default implementation that calls tryAcquire ().
   *
   *@see AbstractLock#tryAcquireWrite
   */
  public int tryAcquireWrite ()
  {
    return tryAcquire ();
  }
  /** 
   * Try to acquire the lock without blocking.  
   * <P>
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   */
  public abstract int tryAcquire ();

  /**
   * Default implementation as a no-op.
   *
   *@see AbstractLock#sleepHook
   */
  public void sleepHook ()
  {
  }

  /** 
   * Release ownership of this lock.  
   * <P>
   *@return    appropriate AbstractLock return value 
   *           (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   */
  public abstract int release ();

  /**
   * Obtains an Object which uniquely identifies the current accessor
   * (usually a thread).  This is used to make sure only an owner can
   * perform certain operations like release.  Subclasses can redefine
   * the behavior as necessary, such as in the Token service where it is
   * defined to be the client ID sent by the proxy.
   * <P>
   * When using Java 1.2 or later, it might be more efficient to use
   * ThreadLocal and an Integer for the ID.  The current default 
   * implementation returns the Thread.currentThread () reference.
   *
   *@return Object representing a unique ID for this accessor
   */
  protected Object accessorID ()
  {
    return Thread.currentThread();
  }

  /**
   * Check to see if the current accessor is the (or a) owner of this
   * lock.
   */
  protected boolean isOwner() 
  {
    return accessorID().equals(this.owner_);
  }

  /**
   * Set the current accessor to be the (or a) owner of this lock.
   */
  protected void setOwner()
  {
    this.owner_ = accessorID();
  }

  /**
   * Make sure that this accessor is no longer the (or a) owner of this
   * lock.
   */
  protected void clearOwner()
  {
    this.owner_ = null;
  }

  /**
   * Reference to the accessorID of the owner.
   */
  private Object owner_;
}

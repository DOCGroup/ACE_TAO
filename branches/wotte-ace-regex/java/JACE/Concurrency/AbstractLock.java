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
 *      Interface for any Java ACE synchronization mechanism.
 * <P>
 *      Defines the interface for Token, Mutex, RWMutex, Semaphore,
 *      and the RemoteLock proxies in the Token service, as well as
 *      the possible constant return values.
 * <P>
 *      Methods which take TimeValue timeouts can throw
 *      JACE.ASX.TimeoutExceptions.  The locks should continue to
 *      function properly after a thread times out or is interrupted.
 *      <em>Also note that the timeouts are absolute time-of-day
 *      values, not relative times.</em>
 * <P>
 *      An AbstractLock.FAILURE can be returned for an undefined type of
 *      failure.
 * <P>
 *      You can assume that 
 *      AbstractLock.FAILURE < AbstractLock.SUCCESS < AbstractLock.SLEEPHOOK
 * <P>
 *      Any method can throw a LockException, providing a way to return
 *      unusual error cases in future types of locks (such as the Token
 *      service).
 * <P>
 *      It is safe to call release () in a finally block, since it will
 *      return FAILURE if the accessing thread is not the owner.
 *
 */
public interface AbstractLock
{
  /**
   * Generic failure indication, used as a return value.
   */
  public static final int FAILURE = -1;

  /**
   * Success indication, used as a return value.
   */
  int SUCCESS = 0;
  
  /**
   * Success indication, but notes that the thread had to sleep
   * to complete it (and it called the sleep hook).  Used as a
   * return value.
   */
  int SLEEPHOOK = 1;

  
  /**
   * Acquire ownership of the lock, blocking indefinitely if necessary.  
   * <P>
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException special exception defined by a later 
   *           implementation
   *@exception InterruptedException indicates another thread has 
   *           interrupted this one during wait
   */
  public int acquire () throws LockException, InterruptedException;

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
   *@exception      LockException special exception defined by a later
   *                implementation
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@exception      InterruptedException indicates another thread has 
   *                interrupted this one during wait
   *@see AbstractLock#tryAcquire
   */
  public int acquire (TimeValue timeout)
    throws LockException, TimeoutException, InterruptedException;

  /**
   * Acquire a read lock, blocking indefinitely if necessary.  This can
   * be used to implement Reader-Writer locks in which multiple readers
   * may have simultaneous access.
   * <P>
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException special exception defined by a later 
   *           implementation
   *@exception InterruptedException indicates another thread has 
   *           interrupted this one during wait
   */
  public int acquireRead () throws LockException, InterruptedException;
  
  /**
   * Acquire a read lock by the given absolute time time-out.  This can 
   * be used to implement Reader-Writer locks in which multiple readers
   * may have simultaneous access.  
   * <P>
   *@param timeout  absolute time by which the lock must be acquired
   *@return         appropriate lock return value (AbstractLock.FAILURE, 
   *                AbstractLock.SUCCESS or AbstractLock.SLEEPHOOK)
   *@exception      LockException special exception defined by a later
   *                implementation
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@exception      InterruptedException indicates another thread has 
   *                interrupted this one during wait
   *@see AbstractLock#tryAcquireRead
   */
  public int acquireRead (TimeValue timeout) 
    throws LockException, TimeoutException, InterruptedException;

  /**
   * Acquire a write lock, blocking indefinitely if necessary.  This can
   * be used to implement Reader-Writer locks in which a writer has 
   * exclusive access.  
   * <P>
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException special exception defined by a later 
   *           implementation
   *@exception InterruptedException indicates another thread has 
   *           interrupted this one during wait
   */
  public int acquireWrite () throws LockException, InterruptedException;

  /** 
   * Acquire a write lock by the given absolute time time-out.  This can
   * be used to implement Reader-Writer locks in which a writer has
   * exclusive access.  
   * <P>
   *@param timeout  absolute time by which the lock must be acquired
   *@return         appropriate AbstractLock return value (AbstractLock.FAILURE, 
   *                AbstractLock.SUCCESS or AbstractLock.SLEEPHOOK)
   *@exception      LockException special exception defined by a later
   *                implementation
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@exception      InterruptedException indicates another thread has 
   *                interrupted this one during wait
   *@see AbstractLock#tryAcquireWrite
   */
  public int acquireWrite (TimeValue timeout) 
    throws LockException, TimeoutException, InterruptedException;
  
  /**
   * Give up the lock to some number of waiting threads (if any), then 
   * reacquire, blocking indefinitely if necessary.
   * <P>
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token. 
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
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException special exception defined by a later 
   *           implementation
   *@exception InterruptedException indicates another thread has 
   *           interrupted this one during wait
   */
  public int renew (int requeuePosition) throws LockException,
        InterruptedException;

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
   *@exception      LockException special exception defined by a later
   *                implementation
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@exception      InterruptedException indicates another thread has 
   *                interrupted this one during wait
   */
  public int renew (int requeuePosition, TimeValue timeout) 
    throws LockException, TimeoutException, InterruptedException;

  /** 
   * Try to acquire the lock without blocking.  
   * <P>
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException special exception defined by a later
   *                implementation
   */
  public int tryAcquire () throws LockException;

  /** 
   * Try to acquire a read lock without blocking.  
   * <P>
   *@see            #acquireRead
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException special exception defined by a later
   *                implementation
   */
  public int tryAcquireRead () throws LockException;

  /**
   * Try to acquire a write lock without blocking.  
   *<P>
   *@see            #acquireWrite
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException special exception defined by a later
   *                implementation
   */
  public int tryAcquireWrite () throws LockException;

  /**
   * Method that is called before a thread goes to sleep in an
   * acquire. This should be overridden by a subclass to define 
   * the appropriate behavior.
   */
  public void sleepHook ();

  /** 
   * Release ownership of this lock.  
   * <P>
   *@return    appropriate AbstractLock return value 
   *           (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception LockException special exception defined by a later
   *           implementation
   */
  public int release () throws LockException;
}

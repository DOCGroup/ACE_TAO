package JACE.Concurrency;

import java.util.*;
import JACE.ASX.*;

/**
 * A read/write lock allows multiple
 * readers or a single writer to access the guarded element.
 * <P>
 * <EM>This class does not support recursive semantics.</EM>
 */
public class RWMutex extends LockAdapter
{
  public synchronized int tryAcquire () 
  {
    if (referenceCount_ == 0) {
      referenceCount_ = -1;
      setOwner ();
      return AbstractLock.SUCCESS;
    } else
      return AbstractLock.FAILURE;
  }

  public synchronized int tryAcquireRead () 
  {
    if (referenceCount_ > -1 && waiters_.size () == 0) {
      referenceCount_++;
      setOwner ();
      return AbstractLock.SUCCESS;
    } else
      return AbstractLock.FAILURE;
  }

  public int acquire(TimeValue timeout) 
    throws TimeoutException, InterruptedException
  {
    return acquireWrite(timeout);
  }

  public void waitUntilIsOwner (RWWaitObject waitObj, TimeValue timeout)
    throws TimeoutException, InterruptedException
  {
    boolean exceptionOccured = true;
    try {
      sleepHook ();
      synchronized (waitObj) {
	waitObj.timedWait (timeout);
      }
      exceptionOccured = false;
    } finally {
      
      synchronized (this) {

	if (exceptionOccured) {
	  if (!waiters_.removeElement (waitObj)) {  
	    setOwner ();
	    release ();
	  } 
	} else
	  setOwner();
      }
    }
  }
  
  public int acquireRead(TimeValue timeout)
    throws TimeoutException, InterruptedException
  {
    RWWaitObject waitObj = null;

    synchronized (this) {

      if (referenceCount_ > -1 && waiters_.size () == 0) {
	referenceCount_++;
	setOwner ();
	return AbstractLock.SUCCESS;
      }

      waitObj = new RWWaitObject (true);

      waiters_.addElement (waitObj);
    }

    waitUntilIsOwner (waitObj, timeout);

    return AbstractLock.SLEEPHOOK;
  }
  
  public int acquireWrite(TimeValue timeout)
    throws TimeoutException, InterruptedException
  {
    RWWaitObject waitObj = null;

    synchronized (this) {
      
      if (referenceCount_ == 0) {
	referenceCount_ = -1;
	setOwner ();
	return AbstractLock.SUCCESS;
      }
      
      waitObj = new RWWaitObject (false);

      waiters_.addElement (waitObj);
    }

    waitUntilIsOwner (waitObj, timeout);

    // When the writer gets here, it has been cleared to go by
    // whatever thread specifically gave control to this writer in
    // release.  The referenceCount_ and numberOfWaitingWriters_
    // variables are also adjusted by the releasing thread since
    // it already has a synchronization lock.  Not doing that,
    // and then having another synchronized (this) block in here
    // could lead to a situation in which another thread sneaks
    // in inbetween when this thread leaves timedWait and goes to
    // adjust them.

    return AbstractLock.SLEEPHOOK;
  }


  public synchronized int release ()
  {
    if (!isOwner ())
      return AbstractLock.FAILURE;

    clearOwner ();

    // Releasing a reader.
    if (referenceCount_ > 0) {
      referenceCount_--;
      
      if (referenceCount_ != 0) 
	return AbstractLock.SUCCESS;

    } else {
      // releasing a writer
      referenceCount_ = 0;
    }
    
    if (waiters_.size () == 0)
      return AbstractLock.SUCCESS;

    if (releaseFirstReaders () == 0) {
      RWWaitObject waitObj = (RWWaitObject)waiters_.firstElement ();
      waiters_.removeElementAt (0);
      
      referenceCount_ = -1;
      
      waitObj.condition (true);
      synchronized (waitObj) {
	waitObj.signal ();
      }
    }

    return AbstractLock.SUCCESS;
  }
   
  // Releases all waiting readers up to the first waiting writer
  // or the end of the queue.  Returns the number of readers
  // released.
  protected int releaseFirstReaders ()
  {
    int releasedReaders = 0;
    
    do {
      
      RWWaitObject waitObj = (RWWaitObject)waiters_.firstElement ();
      if (!waitObj.isReader ())
	break;

      waiters_.removeElementAt (0);

      referenceCount_++;
      releasedReaders++;
      
      waitObj.condition (true);
      synchronized (waitObj) {
	waitObj.signal ();
      }

    } while (waiters_.size () > 0);

    return releasedReaders;
  }

  public int renew (int requeuePosition,
		    JACE.ASX.TimeValue timeout)
    throws InterruptedException,
	   TimeoutException
  {
    RWWaitObject waitObj = null;

    synchronized (this) {

      if (!isOwner ())
	return AbstractLock.FAILURE;

      if (requeuePosition == 0 || waiters_.size () == 0) 
	return AbstractLock.SUCCESS;

      waitObj = new RWWaitObject (referenceCount_ > 0);

      if (requeuePosition < 0 || requeuePosition > waiters_.size ()) {
	requeuePosition = waiters_.size ();
      }

      waiters_.insertElementAt (waitObj, requeuePosition);

      release ();
    }
    
    waitUntilIsOwner (waitObj, timeout);

    // When the writer gets here, it has been cleared to go by
    // whatever thread specifically gave control to this writer in
    // release.  The referenceCount_ and numberOfWaitingWriters_
    // variables are also adjusted by the releasing thread since
    // it already has a synchronization lock.  Not doing that,
    // and then having another synchronized (this) block in here
    // could lead to a situation in which another thread sneaks
    // in inbetween when this thread leaves timedWait and goes to
    // adjust them.

    return AbstractLock.SUCCESS;
  }

  static class RWWaitObject extends WaitObject
  {
    public RWWaitObject (boolean isReader)
    {
      isReader_ = isReader;
    }

    public boolean isReader ()
    {
      return isReader_;
    }

    private boolean isReader_ = false;
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

  private Vector waiters_ = new Vector ();

  private int referenceCount_ = 0;
  // Value is -1 if writer has the lock, else this keeps track of the
  // number of readers holding the lock.  

  private Hashtable owners_ = new Hashtable ();

  private int nestingLevel_ = 0;
}


/*************************************************
 *
 * = PACKAGE
 *    ACE.Concurrency
 *
 * = FILENAME
 *    RWMutex.java
 *
 *@author Irfan Pyarali
 *
 *************************************************/

package ACE.Concurrency;

import ACE.OS.*;

/*******************************************************************************
* <HR>
* <B> Description </B>
* <BR>
* This class increments a read/write lock. A read/write lock allows multiple
* readers or a single writer to access the guarded element.
* This implementation is based on the C++ version of ACE.
* </PRE><P><HR>
* <B> Notes </B>
* <UL>
* <LI> This class does not support recursive semantics
* </UL>
*******************************************************************************/
public class RWMutex
{
  /**
   * Acquires the write lock
   * @exception InterruptedException Lock acquisition interrupted
   **/
  public void acquire()
    throws InterruptedException
  {
    acquireWrite();
  }

  /**
   * Acquires the read lock
   * @exception InterruptedException Lock acquisition interrupted
   **/
  public void acquireRead()
    throws InterruptedException
  {
    // make sure we start with no exception
    InterruptedException exception_ = null;
    
    // grab lock
    lock_.acquire ();
    
    // Give preference to writers who are waiting.
    while (referenceCount_ < 0 || numberOfWaitingWriters_ > 0)
      {
	numberOfWaitingReaders_++;
	try 
	  {
	    waitingReaders_.Wait ();
	  }
	catch (InterruptedException exception)
	  {
	    // cache exception
	    exception_ = exception;
	  }
	numberOfWaitingReaders_--;
      }
    
    if (exception_ == null)
      // No errors
      referenceCount_++;

    // make sure this is released in all cases
    lock_.release ();

    if (exception_ != null)
      // error: propogate
      throw exception_;
  }
  
  /**
   * Acquires the write lock
   * @exception InterruptedException Lock acquisition interrupted
   **/
  public void acquireWrite()
    throws InterruptedException
  {
    // make sure we start with no exception
    InterruptedException exception_ = null;
    
    // grab lock
    lock_.acquire ();
    
    // Give preference to writers who are waiting.
    while (referenceCount_ != 0)
      {
	numberOfWaitingWriters_++;
	try 
	  {
	    waitingWriters_.Wait ();
	  }
	catch (InterruptedException exception)
	  {
	    // cache exception
	    exception_ = exception;
	  }
	numberOfWaitingWriters_--;
      }
    
    if (exception_ == null)
      // No errors
      referenceCount_ = -1;

    // make sure this is released in all cases
    lock_.release ();

    if (exception_ != null)
      // error: propogate
      throw exception_;
  }

  /**
   * Release held lock
   * @exception InterruptedException Lock acquisition interrupted
   **/
  public void release()
    throws InterruptedException
  {
    lock_.acquire ();
    
    // Releasing a reader.
    if (referenceCount_ > 0) 
      referenceCount_--;
    else 
      // Releasing a writer.
      if (referenceCount_ == -1) 
	referenceCount_ = 0;
    
    // Give preference to writers over readers...
    if (numberOfWaitingWriters_ > 0)
      {
	waitingWriters_.signal ();
      }
    else if (numberOfWaitingReaders_ > 0)
      {
	waitingReaders_.broadcast ();
      }
	
    
    lock_.release ();
  }

  private Mutex lock_ = new Mutex (); 
  // Serialize access to internal state.
 
  private Condition waitingReaders_ = new Condition (lock_);
  // Reader threads waiting to acquire the lock.
 
  private int numberOfWaitingReaders_;
  // Number of waiting readers.
 
  private Condition waitingWriters_ = new Condition (lock_);
  // Writer threads waiting to acquire the lock.
 
  private int numberOfWaitingWriters_ = 0;
  // Number of waiting writers.
 
  private int referenceCount_ = 0;
  // Value is -1 if writer has the lock, else this keeps track of the
  // number of readers holding the lock.
}


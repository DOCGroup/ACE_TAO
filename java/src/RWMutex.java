/*************************************************
 *
 * = PACKAGE
 *    ACE.Concurrency
 *
 * = FILENAME
 *    RWMutex.java
 *
 *@author Ross Dargahi (rossd@krinfo.com) and Prashant Jain
 *
 *************************************************/
package ACE.Concurrency;

/*******************************************************************************
* <HR>
* <B> Description </B>
* <BR>
* This class increments a read/write lock. A read/write lock allows multiple
* readers or a single writer to access the guarded element.
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
  public synchronized void acquireRead()
       throws InterruptedException
  {
    // Wait till there is an active writer, wait.
    while (this.mWriterActive_)
      wait();

    this.mNumReaders_++;
  }

  /**
   * Acquires the write lock
   * @exception InterruptedException Lock acquisition interrupted
   **/
  public synchronized void acquireWrite()
       throws InterruptedException
  {
    // If there is an active writer before us, then wait for it to finish
    // before proceeding
    while (this.mWriterActive_)
      wait();
    
    // Set the writer active flag to true, then wait for all readers to finish
    // with the lock. Note that no new readers will be able to grab the lock
    // since they will be blocking on the writer active flag in acquireRead()
    this.mWriterActive_ = true;
    
    while (this.mNumReaders_ > 0)
      wait();
   
    this.mWriterHoldsLock_ = true;
  }

  /**
   * Release held lock
   * @exception InterruptedException Lock acquisition interrupted
   **/
  public synchronized void release()
  {
    if (this.mWriterHoldsLock_)
      {
	this.mWriterActive_ = false;
	this.mWriterHoldsLock_ = false;
      }
    else
      {
	this.mNumReaders_--;
      }

    notifyAll();

  }

  private int mNumReaders_;
  // Current number of readers
    
  private boolean mWriterActive_;
  // If true, a writer is active

  private boolean mWriterHoldsLock_;
  // If true, a writer holds the lock
}


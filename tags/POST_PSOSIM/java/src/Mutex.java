/*************************************************
 *
 * = PACKAGE
 *    JACE.Concurrency
 *
 * = FILENAME
 *    Mutex.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Concurrency;

import java.util.*;
import JACE.ASX.*;

class TimedWaitMAdapter extends TimedWait
{
  TimedWaitMAdapter (Object obj)
  {
    super (obj);
  }

  // Check to see if the lock is currently held or not.
  public boolean condition ()
  {
    return !this.inUse_;
  }

  // Acquire/Release the lock
  public void inUse (boolean c)
  {
    this.inUse_ = c;
  }

  private boolean inUse_ = false;
  // The actual lock
}


/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Value added abstraction for mutex variable creation.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *     A timed mutex, <em>i.e.</em> a mutex whose operations do not
 * block forever and can <q>time out</q>.
 *</blockquote>
 */
public class Mutex
{
  /**
   * Acquire the mutex. Note that this will block.
   *@exception InterruptedException exception during wait
   */
  public synchronized void acquire () throws InterruptedException
    {
      this.monitor_.timedWait ();
      this.monitor_.inUse (true);
    }

  /**
   * Acquire the mutex. Note that the call will return if <timeout>
   * amount of time expires.
   *@param tv amount of time (TimeValue) to wait before returning
   * (unless operation completes before)
   *@exception TimeoutException wait timed out exception
   *@exception InterruptedException exception during wait
   */
  public synchronized void acquire (TimeValue tv) throws
  TimeoutException, InterruptedException 
    {
      this.monitor_.timedWait (tv);
      this.monitor_.inUse (true);
    }

  /**
   * Release the mutex.
   */
  public synchronized void release ()
    {
      this.monitor_.inUse (false);
      this.monitor_.signal ();
    }

  private TimedWaitMAdapter monitor_ = new TimedWaitMAdapter (this);
  // The monitor (adapter) to wait on
}

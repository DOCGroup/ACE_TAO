/*************************************************
 *
 * = PACKAGE
 *    ACE.Concurrency
 *
 * = FILENAME
 *    Semaphore.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.Concurrency;

import java.util.*;
import ACE.ASX.*;

class TimedWaitSAdapter extends ACE.ASX.TimedWait
{
  TimedWaitSAdapter (Object obj)
  {
    super (obj);
  }

  // Check to see if there are any semaphores available.
  public boolean condition ()
  {
    return this.count_ > 0;
  }

  // Increment the count by one
  public void increment ()
  {
    this.count_++;
  }

  // Decrement the count by one
  public void decrement ()
  {
    this.count_--;
  }

  // Set the count
  public void count (int c)
  {
    this.count_ = c;
  }

  private int count_ = 0;
}

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *     Implementation of Dijkstra's counting semaphore in java.
 */
public class Semaphore
{  
  /**
   * Create a Semaphore.
   *@param count semaphore count
   */
  public Semaphore (int c)
    {
      this.monitor_.count (c);
    }

  /**
   * Acquire the Semaphore. Note that this will block.
   *@exception InterruptedException exception during wait
   */
  public synchronized void acquire () throws InterruptedException
    {
      this.monitor_.timedWait ();
      this.monitor_.decrement ();
    }

  /**
   * Acquire the Semaphore.  Note that the call will return if <timeout>
   * amount of time expires.
   *@param tv amount of time (TimeValue) to wait before returning
   * (unless operation completes before)
   *@exception TimeoutException wait timed out exception
   *@exception InterruptedException exception during wait
   */
  public synchronized void acquire (TimeValue tv)
      throws ACE.ASX.TimeoutException, InterruptedException 
    {
      this.monitor_.timedWait (tv);
      this.monitor_.decrement ();
    }

  /**
   * Release the Semaphore.
   */
  public synchronized void release ()
    {
      this.monitor_.increment ();
      this.monitor_.signal ();
    }

  private TimedWaitSAdapter monitor_ = new TimedWaitSAdapter (this);
  // The monitor (adapter) to wait on
}

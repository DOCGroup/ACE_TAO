/*************************************************
 *
 * = PACKAGE
 *    ACE.Concurrency
 *
 * = FILENAME
 *    ThreadManager.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.Concurrency;

import java.util.*;
import ACE.OS.*;

public class ThreadManager
{ 
  /**
   * Default constructor
   */
  public ThreadManager ()
    {
      this (ACE.DEFAULT_THREAD_GROUP_NAME);
    }

  /**
   * Create a Thread Manager.
   *@param groupName name of the thread group that the Thread Manager
   * will manage
   */
  public ThreadManager (String groupName)
    {
      this.thrGrp_ = new ThreadGroup (groupName);
      if (this.thrGrp_ == null)
	ACE.ERROR ("Thread group create failed");
    }
  
  /** 
   * Create a new thread.
   *@param thr the caller whose run method will be invoked when the
   * thread has been spawned
   *@param daemon flag indicating whether the thread should be
   * spawned off as a daemon thread
   */
  public void spawn (Runnable thr,
		     boolean daemon)
    {
      Thread t = new Thread (this.thrGrp_, thr);
      if (daemon)  // Set the thread to be a daemon thread
	t.setDaemon (true);
      t.start ();
    }

  /** 
   * Create a new thread and also give it a name.
   *@param thr the caller whose run method will be invoked when the
   * thread has been spawned
   *@param threadName the name of the new thread
   *@param daemon flag indicating whether the thread should be
   * spawned off as a daemon thread
   */
  public void spawn (Runnable thr,
		     String threadName,
		     boolean daemon)
    {
      Thread t = new Thread (this.thrGrp_, thr, threadName);
      if (daemon)  // Set the thread to be a daemon thread
	t.setDaemon (true);
      t.start ();
    }


  /** 
   * Create <n> new threads.
   *@param n the number of threads to spawn
   *@param thr the caller whose run method will be invoked by each of
   * the <n> threads
   *@param daemon flag indicating whether the threads should be
   * spawned off as daemon threads 
   */
  public void spawnN (int n, 
		      Runnable thr,
		      boolean daemon)
    {
      // Spawn off all the threads.
      for (int i = 0; i < n; i++)
	{
	  this.spawn (thr, daemon);
	}
    }

  /**
   * Get the thread group containing all the threads. Note that the
   * thread group can be used to get information regarding number of
   * active threads as well as to suspend/resume all the threads in
   * the group. 
   *@return the thread group that contains all the threads managed by
   * the Thread Manager
   */
  public ThreadGroup thrGrp ()
    {
      return this.thrGrp_;
    }

  private ThreadGroup thrGrp_;
  // Thread Group that contains all the spawned threads

}

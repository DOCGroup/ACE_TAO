/*************************************************
 *
 * = PACKAGE
 *    ACE.ASX
 *
 * = FILENAME
 *    Task.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.ASX;

import ACE.OS.*;
import ACE.Reactor.*;
import ACE.Concurrency.*;

public abstract class Task implements Runnable, EventHandler
{
  // = Initialization/termination methods.

  /**
   * Initialize a Task. Note, we allocate a message queue ourselves.
   */
  public Task ()
    {
      this.msgQueue_ = new MessageQueue ();
      this.thrMgr_ = null;
    }

  /**
   * Initialize a Task. Note, we use the message queue and thread
   * manager supplied by the user.
   *@param mq Message Queue to hold list of messages on the Task
   *@param thrMgr Thread Manager that manages all the spawned threads
   */
  public Task (MessageQueue mq,
	       ThreadManager thrMgr)
    {
      this.msgQueue_ = mq;
      this.thrMgr_ = thrMgr;
    }

  /**
   * Not meant to be invoked by the user directly!. This needs to be
   * in the public interface in order to get invoked by Thread
   * class.
   */
  public void run ()
    {
      this.svc ();
    }

  // = Initialization and termination hooks (note that these *must* be
  // defined by subclasses).

  /**
   * Hook called to open a Task. 
   *@param obj used to pass arbitrary information
   */
  public abstract int open (Object obj);

  /**
   * Hook called to close a Task.
   */
  public abstract int close (long flags);

  // = Immediate and deferred processing methods, respectively.

  /**
   * Transfer a message into the queue to handle immediate
   * processing.
   *@param mb Message Block to handle immediately
   *@param tv amount of time to wait for
   */
  public abstract int put (MessageBlock mb, TimeValue tv);

  /** 
   * Run by a daemon thread to handle deferred processing. Note, that
   * to do anything useful, this method should be overriden by the
   * subclass.
   *@return default implementation always returns 0.
   */
  public int svc ()
    {
      return 0;
    }

  // = Active object method.

  /**
   * Turn the task into an active object. That is, having <nThreads>
   * separate threads of control that all invoke Task::svc.
   *@param flags Task Flags
   *@param nThreads number of threads to spawn
   *@param forceActive whether to force creation of new threads or not
   *@return -1 if failure occurs, 1 if Task is already an active
   * object and <forceActive> is false (doesn't *not* create a new
   * thread in this case), and 0 if Task was not already an active
   * object and a thread is created successfully or thread is an active
   * object and <forceActive> is true.
   */
  public synchronized int activate (long flags, int nThreads, boolean forceActive)
    {
       // Create a Thread Manager if we do not already have one
       if (this.thrMgr_ == null)
	 this.thrMgr_ = new ThreadManager ();

       if (this.thrCount () > 0 && forceActive == false)
	 return 1; // Already active.
       this.flags_ = flags;

       if (ACE.BIT_ENABLED (flags, TaskFlags.THR_DAEMON))
	 this.thrMgr_.spawnN (nThreads, this, true); // Spawn off all threads as daemon threads
       else        // Spawn off all threads as normal threads
	 this.thrMgr_.spawnN (nThreads, this, false);

       return 0;
    }
  
  // = Suspend/resume a Task

  /**
   * Suspend a task.
   */
  public synchronized void suspend ()
    {
      // Suspend all threads
      if (this.thrMgr_ != null)
	this.thrMgr_.thrGrp ().suspend ();
    }

  /** 
   * Resume a suspended task.
   */
  public synchronized void resume ()
    {
      // Resume all threads
      if (this.thrMgr_ != null)
	this.thrMgr_.thrGrp ().resume ();
    }

  /**
   * Get the current group name.
   *@return name of the current thread group
   */
  public synchronized String grpName ()
    {
      if (this.thrMgr_ != null)
	return this.thrMgr_.thrGrp ().getName ();
      else
	return null;
    }

  /**
   * Get the message queue associated with this task.
   *@return the message queue associated with this task.
   */
  public MessageQueue msgQueue ()
    {
      return this.msgQueue_;
    }

  /**
   * Set the message queue associated with this task.
   *@param mq Message Queue to use with this Task.
   */
  public void msgQueue (MessageQueue mq)
    {
      this.msgQueue_ = mq;
    }

  /**
   * Get the number of threads currently running within the Task.
   *@return the number of threads currently running within the Task.
   * 0 if we're a passive object, else > 0.
   */
  public synchronized int thrCount ()
    {
      if (this.thrMgr_ != null)
	return this.thrMgr_.thrGrp ().activeCount ();
      else
	return 0;
    }

  // = Message queue manipulation methods.

  /**
   * Insert message into the message queue.
   *@param mb Message Block to insert into the Message Queue
   *@param tv amount of time to wait for
   */
  protected int putq (MessageBlock mb, TimeValue tv) throws InterruptedException
    {
      return this.msgQueue_.enqueueTail (mb, tv);
    }

  /**
   * Extract the first message from the queue. Note that the call is blocking.
   *@return the first Message Block from the Message Queue.
   *@param tv amount of time to wait for
   */
  protected MessageBlock getq (TimeValue tv) throws InterruptedException
    {
      return this.msgQueue_.dequeueHead (tv);
    }

  /**
   * Return a message back to the queue.
   *@param mb Message Block to return back to the Message Queue
   *@param tv amount of time to wait for
   */
  protected int ungetq (MessageBlock mb, TimeValue tv) throws InterruptedException
    {
      return this.msgQueue_.enqueueHead (mb, tv);
    }

  /**
   * Transfer message to the adjacent ACETask in an ACEStream.
   *@param mb Message Block to transfer to the adjacent Task
   *@param tv amount of time to wait for
   *@return -1 if there is no adjacent Task, else the return value of
   * trying to put the Message Block on that Task's Message Queue.
   */
  protected int putNext (MessageBlock mb, TimeValue tv)
    {
      return this.next_ == null ? -1 : this.next_.put (mb, tv);
    }

  /**
   * Turn the message back around. Puts the message in the sibling's
   * Message Queue.
   *@param mb Message Block to put into sibling's Message Queue
   *@param tv amount of time to wait for
   *@return -1 if there is no adjacent Task to the sibling, else the
   * return value of trying to put the Message Block on sibling's
   * Message Queue. 
   */
  protected int reply (MessageBlock mb, TimeValue tv)
    {
      return this.sibling ().putNext (mb, tv);
    }

  // = ACE_Task utility routines to identify names et al.

  /**
   * Get the name of the enclosing Module. (NOT YET IMPLEMENTED)
   *@return the name of the enclosing Module if there's one associated
   * with the Task, else null.
   */
  protected String name ()
    {
      // Needs to be implemented once Module class has been implemeted
      return null;
    }

  /**
   * Get the Task's sibling. (NOT YET IMPLEMENTED)
  *@return the Task's sibling if there's one associated with the
  * Task's Module, else null.
  */
  protected Task sibling ()
    {
      // Needs to be implemented once Module class has been implemeted
      return null;      
    }

  /**
   * Check if queue is a reader.
   *@return true if queue is a reader, else false.
   */
  protected boolean isReader ()
    {
      return (ACE.BIT_ENABLED (this.flags_, TaskFlags.ACE_READER));
    }

  /**
   * Check if queue is a writer.
   *@return true if queue is a writer, else false.
   */
  protected boolean isWriter ()
    {
      return (ACE.BIT_DISABLED (this.flags_, TaskFlags.ACE_READER));
    }

  // = Pointers to next ACE_Queue (if ACE is part of an ACE_Stream).

  /**
   * Get next Task pointer.
   *@return pointer to the next Task
   */
  protected Task next ()
    {
      return this.next_;
    }

  /**
   * Set next Task pointer.
   *@param task next task pointer
   */
  protected void next (Task task)
    {
      this.next_ = task;
    }

  // Special routines corresponding to certain message types.

  /**
   * Flush the Message Queue
   *@return 0 if Message Queue is null, 1 if flush succeeds, -1 if
   * ACE_FLUSHALL bit is not enabled in flags.
   */
  protected int flush (long flag)
    {
      if (ACE.BIT_ENABLED (flag, TaskFlags.ACE_FLUSHALL))
	return (this.msgQueue_ == null ? 0 : 1);
      else
	return -1;
    }

  private ThreadManager thrMgr_ = null;
  // Thread_Manager that manages all the spawned threads

  private long flags_;
  // Task flags.

  private MessageQueue msgQueue_;
  // List of messages on the Task..

  private Task next_;
  // Adjacent ACE_Task.

}

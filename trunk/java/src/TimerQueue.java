/*************************************************
 *
 * = PACKAGE
 *    JACE.Reactor
 *
 * = FILENAME
 *    TimerQueue.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Reactor;

import java.util.*;
import JACE.ASX.*;

class TimerNode
{
  public TimerNode (EventHandler handler,
		    Object arg,
		    TimeValue timerValue,
		    TimeValue interval,
		    TimerNode next,
		    int timerId)
  {
    this.handler_ = handler;
    this.arg_ = arg;
    this.timerValue_ = timerValue;
    this.interval_ = interval;
    this.next_ = next;
    this.timerId_ = timerId;
  }

  public EventHandler handler_;
  // Handler to invoke <handleTimeout> on when a timeout occurs.

  public Object arg_;
  // Argument to pass to <handleTimeout>.

  public TimeValue timerValue_;
  // Time until the timer expires.

  public TimeValue interval_;
  // If this is a periodic timer this holds the time until the next
  // timeout.

  public TimerNode next_;
  // Pointer to next timer.
  
  public int timerId_;
  // Id of this timer (used to cancel timers before they expire).
}

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
 *     Class that provides an interface to timers. 
 *
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *     This is a simple implementation that keeps a linked list of
 *     absolute timers. It allows multiple timers to be scheduled
 *     and returns a timer id for each timer scheduled. In addition,
 *     it allows periodic timers to be scheduled.
 *</blockquote>
 */
public class TimerQueue implements Runnable
{
  /**
   * Constructor.
   *@param createInternalThread flag specifying whether to create an
   * internal thread that runs the event loop. If it is true, a thread
   * is spawned and it runs the event loop, handling all timeout
   * events. If it is false, the caller is then responsible for calling
   * handleEvents () to run the event loop.
   */
  public TimerQueue (boolean createInternalThread)
  {
    this.eventLoopRunning_ = false;
    if (createInternalThread)
      new Thread (this).start ();
  }
  
  /**
   * The thread run method. Do *NOT* call this method! It gets called
   * automatically. 
   */
  public void run ()
  {
    this.handleEvents ();
  }

  /**
   * Handle timeout events. This forms the event loop and takes care
   * of all scheduling. This method should only be called if the Timer
   * Queue was constructed with the value of createInternalThread as
   * false.
   */
  public void handleEvents ()
  {
    if (!this.eventLoopRunning_)
      {
	// Set the flag indicating that the event loop is now running
	this.eventLoopRunning_ = true;

	TimeValue timeout = null;
	TimeValue earliest = null;

	for (;;)
	  {
	    synchronized (this.obj_) 
	      {
		earliest = this.earliestTime ();
		if (earliest != null)
		  timeout = TimeValue.minus (earliest, TimeValue.getTimeOfDay ());
		else
		  timeout = new TimeValue ();
		try
		  {
		    // Extract the earliest time from the queue and do a timed wait
		    this.obj_.timedWait (timeout);
		
		    // We have been notified. Check to see if we need to
		    // restart the wait with a different timeout
		    if (this.reset_)
		      {
			this.reset_ = false;
			this.obj_.condition (false);
			timeout = TimeValue.minus (this.earliestTime (), TimeValue.getTimeOfDay ());
		      }
		  }
		catch (TimeoutException e)
		  {
		    // Timeout occurred. Call handleTimeout on appropriate
		    // Event Handlers
		    this.dispatchHandlers ();
		  }
		catch (InterruptedException e)
		  {
		  }
	      }
	  }
      }
  }

  /**
   * Check if the queue is empty.
   *@return true if queue is empty, else false.
   */
  boolean isEmpty ()
  {
    return this.head_ == null;
  }

  /**
   * Get the node of the earliest node in the TimerQueue.
   *@return the time of the earlier node in the TimerQueue.
   */
  TimeValue earliestTime ()
  {
    synchronized (this.obj_)
      {
	if (!this.isEmpty ())
	  return this.head_.timerValue_;
	else
	  return null;
      }
  }

  /**
   * Schedule an <EventHandler> that will expire after <delta> amount
   * of time.  If it expires then <obj> is passed in as the value to
   * the <EventHandler>'s <handleTimeout> callback method. This method
   * returns a timer id that uniquely identifies the timer and can be
   * used to cancel the timer before it expires.
   *@param handler Event Handler that is to be scheduled with the timer
   *@param obj Object that is passed back to the Event Handler when
   * timeout occurs (Asynchronous Completion Token)
   *@param delta amount of time for which to schedule the timer
   *@return id of the timer scheduled
   */
  public int scheduleTimer (EventHandler handler,
			     Object obj,
			     TimeValue delta)
  {
    return this.scheduleTimer (handler, obj, delta, TimeValue.zero);
  }

  /**
   * Schedule an <EventHandler> that will expire after <delta> amount
   * of time.  If it expires then <obj> is passed in as the value to
   * the <EventHandler>'s <handleTimeout> callback method.  If
   * <interval> is != to <TimeValue.zero> then it is used to
   * reschedule the <EventHandler> automatically.  This method
   * returns a timer id that uniquely identifies the timer and can be
   * used to cancel the timer before it expires.
   *@param handler Event Handler that is to be scheduled with the timer
   *@param arg Object that is passed back to the Event Handler when
   * timeout occurs (Asynchronous Completion Token)
   *@param timeout amount of time for which to schedule the timer
   *@param interval amount of time to use to reschedule the timer
   *@return id of the timer scheduled
   */
  public int scheduleTimer (EventHandler handler,
			    Object arg,
			    TimeValue timeout,
			    TimeValue interval)
  {
    // Increment the sequence number (it will wrap around).
    this.timerId_++;
    TimeValue futureTime = TimeValue.plus (timeout, TimeValue.getTimeOfDay ());
    TimerNode node = new TimerNode (handler, 
				    arg,
				    futureTime,
				    interval, 
				    null,
				    this.timerId_);
    synchronized (this.obj_)
      {
	// Check if event loop is running. If it is not, then we can
	// just place it at the appropriate place in the queue and
	// don't need to do any notification. If event loop is
	// running, then check if the node is the first node in the
	// queue (either because the queue is empty or because the
	// time for the node is earlier than the currently scheduled
	// timer node).
	if (this.eventLoopRunning_ &&
	    (this.isEmpty () || futureTime.lessThan (this.earliestTime ())))
	  {
	    // Insert the node into (the beginning of) the queue to be
	    // scheduled. 
	    this.reschedule (node);

	    // Notify the waiting thread so that it can reschedule
	    // using the earliest timeout
	    this.obj_.notify ();
	  }
	else // Place in the appropriate position in the queue.
	  {
	    this.reschedule (node);
	  }
      }
    return this.timerId_;
  }


  /**
   * Cancel the single timer associated with <timerId>.
   *@param timerId id of the timer that needs to be cancelled.
   *@return Object that was passed in when timer was scheduled
   * (Asynchronous Completion Token). 
   */
  public Object cancelTimer (int timerId)
  {
    TimerNode prev = null;
    TimerNode curr = null;

    synchronized (this.obj_)
      {
	// Try to locate the TimerNode that matches the timerId.
	for (curr = this.head_; 
	     curr != null && curr.timerId_ != timerId;
	     curr = curr.next_)
	  prev = curr;

	if (curr != null)
	  {
	    if (prev == null)
	      this.head_ = curr.next_;
	    else
	      prev.next_ = curr.next_;

	    return curr.arg_;
	  }
      }
    return null;
  }

  /**
   * Cancel all timers associated with <Event Handler>.
   *@param handler Event Handler whose associated timers need to be cancelled.
   */
  public void cancelTimer (EventHandler handler)
  {
    TimerNode prev = null;
    TimerNode curr = this.head_;

    synchronized (this.obj_)
      {
	while (curr != null)
	  {
	    if (curr.handler_ == handler)
	      {
		if (prev == null)
		  {
		    this.head_ = curr.next_;
		    curr = this.head_;
		  }
		else
		  {
		    prev.next_ = curr.next_;
		    curr = prev.next_;
		  }
	      }
	    else
	      {
		prev = curr;
		curr = curr.next_;
	      }
	  }
      }
  }

  // Call handleTimeout() on all handlers whose timers have expired.
  private void dispatchHandlers ()
  {
    TimeValue currentTime = TimeValue.getTimeOfDay ();

    for (;;)
      {
	if (this.isEmpty () || this.earliestTime ().greaterThan (currentTime))
	  break; // There aren't any more timers eligible to expire.

	TimerNode expired = this.head_;
	EventHandler handler = expired.handler_;
	Object arg = expired.arg_;
	int result;
	
	this.head_ = this.head_.next_;
	
	// Check whether this is an interval timer.
	if (expired.interval_.greaterThan (TimeValue.zero))
	  {
	    // Make sure that we skip past values that have already
	    // "expired".
	    do
	      expired.timerValue_.plusEquals (expired.interval_);
	    while (expired.timerValue_.lessThanEqual (currentTime));
		   
	    // Since this is an interval timer, we need to reschedule
	    // it.
	    this.reschedule (expired);
	  }
	
	// Perform the callback.
	result = handler.handleTimeout (currentTime, arg);
	
	if (result == -1)
	  this.cancelTimer (handler);
      }
  }

  // Reschedule a TimerNode by inserting it at the appropriate
  // position in the queue.
  private void reschedule (TimerNode expired)
  {
    if (this.isEmpty () || 
	expired.timerValue_.lessThan (this.earliestTime ()))
      {
	expired.next_ = this.head_;
	this.head_ = expired;
	// Set the condition to true so that the waiting thread can be
	// notified and it can reschedule.
	this.obj_.condition (true);
	this.reset_ = true;
      }
    else
      {
	TimerNode prev = this.head_;
	TimerNode after  = this.head_.next_;

	// Locate the proper position in the queue. 

	while (after != null 
	       && expired.timerValue_.greaterThan (after.timerValue_))
	  {
	    prev = after;
	    after  = after.next_;
	  }

	expired.next_ = after;
	prev.next_  = expired;
      }
  }

  private WaitObject obj_ = new WaitObject ();
  // Synchronization object (as well as object to use to do wait on)

  private TimerNode head_; 
  // Pointer to linked list of TimerHandles. 

  private int timerId_;
  // Keeps track of the timer id that uniquely identifies each timer.
  // This id can be used to cancel a timer via the <cancel (int)>
  // method.

  private boolean reset_;
  // Flag indicating whether to start the wait again

  private boolean eventLoopRunning_;
  // Flag indicating whether the event loop is running or not
}


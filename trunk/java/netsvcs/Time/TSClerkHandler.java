/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Time
 *
 * = FILENAME
 *    TS_Clerk_Handler.java
 *
 *@author Prashant Jain, Everett Anderson
 *
 *************************************************/
package netsvcs.Time;

import java.io.*;
import java.util.Date;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.TimeValue;

/**
 * <hr>
 * <p><h2>SYNOPSIS</h2>
 *
 * <blockquote>Requests the remote time on a server at regular
 * intervals.</blockquote>
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>TSClerkHandlers are created by a TSClerkProcessor.  There
 * is one handler for each server that the Clerk Processor factors into
 * its calculations.</blockquote>
 */
public class TSClerkHandler extends SvcHandler
{
  /**
   * Constructor.  TSClerkProcessor specifies the server machine and
   * port, as well as the interval at which to make the query.
   */
  public TSClerkHandler (String hostname, 
			 int port,
			 TimerQueue tq, 
			 int timeout,
			 TSClerkProcessor parent)
  {

    this.hostname_ = hostname;
    this.port_ = port;
    this.tq_ = tq;
    this.timeout_ = timeout;

    this.initialTimeout_ = timeout;
    this.processor_ = parent;

    this.sendMsg_ = new String ("TIME_UPDATE_REQUEST\n");
  }
  
  /**
   * Called to start this handler in a new thread.  This only
   * does it when the state of the handler is INITIALIZING.
   */
  public int open (Object obj)
  {
    if (this.state_ != RECONNECTING) {

      Thread test = new Thread(this);

      new Thread (this).start ();

    }
    return 0;
  }

  /**
   * Accessor - return the host name of the server
   */
  public String hostname ()
  {
    return this.hostname_;
  }

  /**
   * Accessor - return the port used to contact the server
   */
  public int port ()
  {
    return this.port_;
  }

  /**
   * Accessor - returns the difference between the local time and
   * the remote server. 
   */
  public long delta ()
  {
    return this.delta_;
  }

  /**
   * Called when the thread starts.  Schedules itself with the
   * timer queue.
   */
  public void run ()
  {
    this.timerId_ = this.tq_.scheduleTimer (this,
					    null,
					    new TimeValue (this.timeout_),
					    new TimeValue (this.timeout_));

  }

  /**
   * Accessor - return the state
   */
  public int state() 
  {
    return this.state_;
  }

  /**
   * Sets the state of the handler
   */
  public void state(int newState)
  {
    this.state_ = newState;
  }

  /**
   * Provides a new time out interval (exponentially increasing) so
   * that if the server doesn't respond, we don't keep trying to
   * reconnect as often.  Maximum value is 5 minutes.
   */
  public int recalculateTimeout() 
  {
    this.timeout_ *= 2;

    if (this.timeout_ > this.max_timeout_)
      this.timeout_ = max_timeout_;

    return this.timeout_;
  }

  /**
   * Start the recovery from a server disconnection by closing the
   * port and recalculating the timeout value.
   */
  protected void errorRecovery()
  {
    ACE.DEBUG("Time Service failure with server " + this.hostname_);

    this.timeout_ = this.recalculateTimeout();

    this.reschedule();
  }

  /**
   * Removes this handler from the timer queue, and reschedules it
   * (presumably with a new timeout value)
   */
  public void reschedule()
    {
      this.tq_.cancelTimer(this);

      this.timerId_ = this.tq_.scheduleTimer (this,
					      null,
					      new TimeValue (this.timeout_),
					      new TimeValue (this.timeout_));
    }

  /**
   * Called back by the timer queue.  If the handler isn't connected,
   * it tries to reconnect to the server.  Otherwise, it requests
   * the remote time.  The server is said to have disconnected when
   * an exception is thrown in the socket system, or the result is
   * a string with length <= 0.
   */
  public int handleTimeout (TimeValue tv, Object obj)
  {
    if (this.state_ != CONNECTED) {

      this.processor_.initiateConnection(this);

      // If still not connected
      if (this.state_ != CONNECTED) { 

	// used to set state to reconnecting here
	this.state_ = RECONNECTING;

	// Reschedule to try again later
	this.errorRecovery();
	return 0;
      } 

      // If connected, poll the server at the requested intervals
      this.resetTimeout();
    }

    StringBuffer ack = new StringBuffer ();
    int ackLen;
    try
      {
	// Used to calculate the turn-around time
	long sendTime = System.currentTimeMillis();

	this.peer ().send (this.sendMsg_);
	ackLen = this.peer ().recv (ack);

	long recvTime = System.currentTimeMillis();

	if (ackLen <= 0) {
	  this.state_ = DISCONNECTED;
	  return -1;

	} else {   

	  long delta = (new Long(ack.toString())).longValue() - recvTime;

	  delta += (recvTime - sendTime) / 2;

	  this.delta_ = delta;
	}

      }
    catch (NullPointerException e)
      {
	ACE.ERROR ("connection reset by peer");
	this.state_ = DISCONNECTED;
	return -1;
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
	this.state_ = DISCONNECTED;
	return -1;
      }

    return 0;
  }
  
  /**
   * Resets the timer interval to be the one supplied to the
   * constructor.
   */
  public void resetTimeout()
    {
      this.timeout_ = this.initialTimeout_;

      this.reschedule();
    }
    
  private TSClerkProcessor processor_;
  // Reference used to re-establish connections

  public static final int MAX_RETRY_TIMEOUT = 300;
  // Wait at most 5 minutes before trying to reconnect

  // States
  public static final int CONNECTED = 0;
  public static final int DISCONNECTED = 1;
  public static final int RECONNECTING = 2;

  // If there has been a failure, try reconnecting
  // at least every MAX_RETRY_TIMEOUT seconds
  private int max_timeout_ = MAX_RETRY_TIMEOUT;

  // State of the handler
  private int state_ = DISCONNECTED;

  // Difference between the remote time and the local time.
  private long delta_ = 0;

  // Name of the remote host
  private String hostname_;

  // Port used for the connection
  private int port_;

  // Current timer interval
  private int timeout_;

  // Reference to the Clerk Processor's timer queue
  private TimerQueue tq_;

  // Message to send for a time update
  private String sendMsg_;

  // ID of the handler in the queue
  private int timerId_;

  // Desired time interval to receive updates
  private int initialTimeout_;

}

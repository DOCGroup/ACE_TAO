/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Time
 *
 * = FILENAME
 *    TSClerkProcessor.java
 *
 *@author Prashant Jain, Everett Anderson
 *
 *************************************************/
package netsvcs.Time;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.TimeValue;

/**
 *
 * <hr>
 * <p><h2>SYNOPSIS</h2>
 *
 * <blockquote>Monitors a specified port (default 7989) and launches
 * TSClerkHandlers when connections are made.  The handlers communicate
 * with servers and calculate the difference between the server time
 * and local time.  The Clerk Processor averages these differences
 * and reports them to clients.</blockquote>
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>This doesn't actually change the system clock, but it
 * provides the average of the differences of the local and server
 * times.  A client could use this information to adjust the clock, or
 * just use the midpoint to determine the correct network time.</blockquote>
 *
 */
public class TSClerkProcessor extends Connector implements Runnable
{
  /**
   * Default constructor
   */
  public TSClerkProcessor ()
  {
    this.serverArray_ = new Vector ();

  }

  /**
   * Parse the command line, setup the TSRequestAcceptor, and run
   * the Clerk Processor in its own thread.
   */
  public int init (String [] args)
    {
      // Parse arguments
      this.parseArgs (args);
      
      TSRequestAcceptor ra = new TSRequestAcceptor (this);
      ra.init (args);

      // Run in own thread of control so that we don't block the caller
      new Thread (this).start ();
      return 0;
    }


  /**
   * Makes connections to the servers, schedules itself for intervals
   * to update the delta time.
   */
  public void run ()
  {

    // Set up connections with all servers
    Enumeration table = this.serverArray_.elements ();
    while (table.hasMoreElements ())
      {
	this.initiateConnection((TSClerkHandler)table.nextElement());
      }

    // Set up timer
    this.timer_id_ = this.tq_.scheduleTimer (this,
					     null,
					     new TimeValue (this.timeout_),
					     new TimeValue (this.timeout_));
  }

  /**
   * Makes connections to the servers.
   */
  public void initiateConnection (TSClerkHandler handler)
  {
    this.open (handler.hostname(), handler.port());

    try
      {
	// Connect to the server
	this.connect (handler);

	// Set the state of the Clerk Handler so it queries the
	// server at intervals.
	handler.state(TSClerkHandler.CONNECTED);
	
      }
    catch (UnknownHostException e)
      {
	ACE.ERROR (e);
      }
    catch (SocketException e)
      {
	ACE.ERROR ("Connection refused");
      }
    catch (InstantiationException e)
      {
	ACE.ERROR (e);
      }
    catch (IllegalAccessException e)
      {
	ACE.ERROR (e);
      }
    catch (IOException e)
      {
	ACE.ERROR (e);
      }
  }


  /**
   *
   * Called by the timer queue.  Calls updateTime().
   */
  public int handleTimeout (TimeValue tv, Object obj)
  {
    return this.updateTime ();
  }

  /**
   * Calculates the delta time by averaging the results from
   * Clerk Handler delta()'s.  It only includes handlers whose
   * state is currently CONNECTED.  If they're not connected, it
   * reschedules them to begin the error correction process of
   * trying to reconnect to the server (possible synch problems?).
   */
  protected int updateTime ()
  {
    TSClerkHandler handler;
    int count = 0;
    long totalDeltaTime = 0;

    Enumeration table = this.serverArray_.elements ();

    while (table.hasMoreElements ())
      {
	handler = (TSClerkHandler) table.nextElement ();

	if (handler.state() != TSClerkHandler.CONNECTED) {

	  // Reconnecting state means we don't need to put
	  // it in the timer queue again
	  if (handler.state() == TSClerkHandler.RECONNECTING)
	    continue;
	  else
	  if (handler.state() == TSClerkHandler.DISCONNECTED) 
	    handler.state(TSClerkHandler.RECONNECTING);

	  handler.errorRecovery();
	  continue;
	}

	long delta = handler.delta();

	ACE.DEBUG(handler.hostname() + ": " + delta);

	totalDeltaTime += delta;
	count++;
      }

    if (count > 0) {

      this.timeDelta_ = totalDeltaTime / count;

      ACE.DEBUG("Average deviation: " + totalDeltaTime/count);

    } else

      this.timeDelta_ = 0;

    return 0;
  }

  /**
   * Return the delta time.
   */
  public long getDelta()
    {
      return this.timeDelta_;
    }

  /**
   * Parse the command line.  Watches for -t <time> and
   * -h <machine:port> switches.  Must specify time
   * value before host switches!
   */
  protected void parseArgs (String args[])
  {
    String s;
    GetOpt opt = new GetOpt (args, "t:h:");
    for (int c; (c = opt.next ()) != -1; )
      {
	switch (c)
	  {
	  case 't':
	    s = opt.optarg ();
	    this.timeout_ = (new Integer (s)).intValue ();
	    break;
	  case 'h':
	    s = opt.optarg ();
	    this.addNewHandler (s);
	    break;
	  default:
	    ACE.ERROR ("Bad command line argument: " + c);

	    ACE.ERROR ("Valid arguments: -t <timeout> -h <hostname>:<port> -h ...");
	    break;
	  }
      }
  }
  
  /**
   *
   * Creates a new Clerk Handler and adds it to the serverArray_
   */
  private void addNewHandler (String s)
  {
    StringTokenizer tokens = new StringTokenizer (s, ":");
    String hostname = tokens.nextToken ();

    int port = (new Integer (tokens.nextToken ())).intValue ();
    
    // Create new handler and add it to array of servers
    this.serverArray_.addElement (new TSClerkHandler (hostname, 
						      port,
						      this.tq_,
						      this.timeout_,
						      this));
  }
  
  // Vector of TSClerkHandlers, one for each server
  private Vector serverArray_;

  // Default interval at which to update the time
  private int timeout_ = 1000;

  // Timer queue which calls handleTimeout when the Clerk Processor
  // is supposed to update the time.
  private TimerQueue tq_ = new TimerQueue (true);

  // Clerk Processor ID in the timer queue
  private int timer_id_;

  // Average of the differences of the local and server times.
  private long timeDelta_;
}

package JACE.netsvcs.Time;

import java.net.*;
import java.io.*;
import java.util.*;

import JACE.ASX.TimeValue;
import JACE.Connection.*;
import JACE.OS.*;
import JACE.Reactor.*;
import JACE.Misc.*;

/**
 * Clerk used to query a number of time servers, compute the average
 * of the time differences, and report it with a sequence number.  This
 * can be used to adjust the current local time accordingly.
 * <P>
 * <B>Valid command line arguments:</B>
 * <PRE>
 *   -h (host name:port)   Specify a time server to contact
 *   -t (time in seconds)  Specify how often to query the servers
 *                         (Defaults to five minutes)
 *   -d                    Enable debugging messages
 * </PRE>
 */
public class TSClerkProcessor implements EventHandler, Runnable
{
  /**
   * Prints out the valid command line arguments.  See the class
   * description for more information.  
   */
  public void printUsage ()
  {
    ACE.ERROR ("Valid options:");
    ACE.ERROR ("-h <host name>:<port>  Specify a time server to contact");
    ACE.ERROR ("-t <time in seconds>   How often to query the servers");
    ACE.ERROR ("-d                     Enable debugging messages");
  }

  /**
   * Parses the command line arguments.  See the class description
   * for more information.
   */
  protected int parseArgs (String args[])
  {
    GetOpt opt = new GetOpt (args, "h:t:d", true);
    for (int c; (c = opt.next ()) != -1; )
      {
	switch (c)
	  {
	    // Specify a hostname:port pair to query
	  case 'h':
	    if (newHandler (opt.optarg ()) == -1) {
	      printUsage ();
	      return -1;
	    }
	    break;
	    // Specify time interval to query servers
	  case 't':
	    int sec = Integer.parseInt (opt.optarg ());
	    updateInterval_ = new TimeValue (sec);
	    break;
	  case 'd':
	    ACE.enableDebugging ();
	    ACE.DEBUG ("Debugging is enabled");
	    break;
	  default:
	    ACE.ERROR ("Unknown argument: " + (char)c);
	    printUsage ();
	    return -1;
	  }
      }
    return 0;
  }

  /**
   * Safely shut down the clerk and all its handlers.
   */
  public synchronized void close ()
  {
    if (!done_) {
      done_ = true;
      tq_.cancelTimer (this);

      for (int i = 0; i < handlerSet_.size (); i++) {
	TSClerkHandler h = (TSClerkHandler)handlerSet_.elementAt (i);
	
	h.close ();
      }
    }
  }

  /**
   * Called by the JVM when the clerk is run in its own thread.  If the
   * TimerQueue provided to (or created by) this TSClerkProcessor isn't
   * running its event loop, it will be run in this thread (by calling
   * handleEvents ()).
   *
   *@see JACE.Reactor.TimerQueue
   */
  public void run ()
  {
    if (handlerSet_.size () == 0) {
      ACE.DEBUG ("No servers are registered.  Call init first.");
      return;
    }

    if (!tq_.eventLoopRunning ()) 
      tq_.handleEvents ();
  }

  /**
   * Initialize this TSClerkProcessor with command line arguments.  See
   * the class description for more information.  This also schedules
   * a timeout with the timer queue for when to query the servers.
   *
   *@return -1 on failure, 0 on success
   */
  public int init (String args[]) 
  {
    if (args.length < 2) {
      printUsage ();
      return -1;
    }

    if (parseArgs (args) == -1) 
      return -1;

    if (handlerSet_.size () == 0) {
      ACE.ERROR ("No servers are registered.");
      done_ = true;
      return -1;
    }

    if (tq_ == null) 
      tq_ = new TimerQueue (true);

    tq_.scheduleTimer (this,
		       "Time Service Processor",
		       TimeValue.zero,
		       updateInterval_);

    return 0;
  }

  /**
   * Called by TSClerkHandler instances when they need to connect
   * (or reconnect) to a server.  This uses Connector to make the
   * connection.
   *
   *@param handler TSClerkHandler to connect to the server
   *@param host name of the service
   *@param port port to connect to on the server
   */
  void connectHandler (TSClerkHandler handler,
		       String host,
		       int port)
  {
    // Don't let handlers reconnect if we are in the process of closing
    if (done_)
      return;

    ACE.DEBUG ("Connecting handler to " + host + " on port " + port);
    try {

      Connector c = new Connector ();
      c.open (host, port);
      c.connect (handler);

    } catch (UnknownHostException e) {
      synchronized (this) {
	handlerSet_.removeElement (handler);
      }
      ACE.ERROR (e);
    } catch (SocketException e) {
      ACE.ERROR (e);
    } catch (InstantiationException e) {
      ACE.ERROR (e);
    } catch (IllegalAccessException e) {
      ACE.ERROR (e);
    } catch (IOException e) {
      ACE.ERROR (e);
    }
  }

  /**
   * Create a new TSClerkHandler for the given (host name):(port)
   * combination.  See the class description for more information about
   * providing a host names and ports on the command line.
   *
   *@param hostAndPort String with the host name and port separated by
   *                   a colon.
   *@return -1 on failure, 0 on success
   */
  protected int newHandler (String hostAndPort)
  {
    int colon = hostAndPort.lastIndexOf (':');

    if (colon < 1) {
      ACE.ERROR ("Invalid -h <host>:<port> parameter: " + hostAndPort);
      return -1;
    }

    int port = Integer.parseInt (hostAndPort.substring (colon + 1));
    String host = hostAndPort.substring (0, colon);

    ACE.DEBUG ("New handler for server " + host + " on port " + port);

    TSClerkHandler handler = new TSClerkHandler (this, host, port);
    handlerSet_.addElement (handler);

    return 0;
  }

  /**
   * Have each TSClerkHandler query its time server, average the results,
   * and set the timeStatus accordingly.  This is called by the
   * TimerQueue when appropriate.  The interval can be specified on the
   * command line.
   */
  public synchronized int handleTimeout (TimeValue tv, Object obj)
  {
    if (done_)
      return -1;

    // Increment the sequence number
    int sequenceNumber = status_.sequenceNumber () + 1;
    Enumeration handlers = handlerSet_.elements ();

    long total = 0;
    int count = 0;

    // Use each handler to query its server, collecting the time
    // difference information.
    while (handlers.hasMoreElements ()) {
      TSClerkHandler h = (TSClerkHandler)handlers.nextElement ();

      if (h.sendRequest () < 0)
	continue;

      total += h.delta ();
      count++;
    }

    if (count == 0) {
      ACE.ERROR ("Could not reach any time servers, will keep trying.");      
      return 0;
    }

    timeStatus (new TimeInfo (sequenceNumber, total / count));

    ACE.DEBUG ("Status: " + timeStatus ());

    return 0;
  }

  /**
   * Return the current sequence number and time difference pair.
   */
  public synchronized TimeInfo timeStatus ()
  {
    return status_;
  }

  /**
   * Set the current sequence number and time difference pair.
   */
  protected synchronized void timeStatus (TimeInfo status)
  {
    status_ = status;
  }

  /**
   * Default constructor.  Results in this TSClerkProcessor creating
   * a new timer queue which runs in its own thread.  Thus, this
   * TSClerkProcessor runs in its own thread.
   */
  public TSClerkProcessor ()
  {
    // Effectively runs in its own thread because of the timer queue
  }

  /**
   * Constructor allowing the timer queue to be specified.  If the timer
   * queue isn't already running, the caller is responsible for calling
   * handleEvents to start the clerk.  Be careful since the querying
   * process for the servers may take a while.
   *
   *@param queue TimerQueue to register with
   */
  public TSClerkProcessor (TimerQueue queue)
  {
    tq_ = queue;
  }

  private boolean done_ = false;

  // List of the TSClerkHandlers this uses to maintain its
  // server connections.
  private Vector handlerSet_ = new Vector ();
  private TimerQueue tq_ = null;

  // Default is every five minutes
  private TimeValue updateInterval_ = new TimeValue (300, 0);

  TimeInfo status_ = new TimeInfo ();
}

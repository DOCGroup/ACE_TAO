/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Time
 *
 * = FILENAME
 *    TSRequestAcceptor.java
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

/**
 * <hr>
 * <p><h2>SYNOPSIS</h2>
 *
 * <blockquote>Monitors a specified port (default 7990) and launches 
 * TSRequestHandlers when connections are made.  The handlers
 * report the average deviation from the local time when input
 * is received to their sockets.</blockquote>
 * 
 * <p><h2>DESCRIPTION</h2>
 * <blockquote>This is in place of the shared memory system used in C++ ACE.  
 * The clients need to request the correct time range from the Clerk, so
 * they can do it with <a href="ACE.SOCK_SAP.SOCKStream.html#_top_">
 * <tt>sockets</tt></a>.  An instance of TSRequestAcceptor is created
 * and initialized in TSClerkProcessor init().  This should be the
 * only place it's used.</blockquote>
 *
 * @see ACE.SOCK_SAP.SOCKAcceptor,ACE.netsvcs.Time.TSClerkProcessor
 */

public class TSRequestAcceptor extends Acceptor implements Runnable
{
  /**
   * Create an instance of TSRequestAcceptor.  Default constructor.
   */
  public TSRequestAcceptor (TSClerkProcessor parent) 
    {
      this.parent_ = parent;
    }

  /**
   *
   * Process command line arguments (port), and start this instance
   * in its own thread.
   *
   */
  public int init(String [] args)
    {
      this.parseArgs (args);

      new Thread (this).start();
      return 0;
    }

  /**
   *
   * Called when the thread starts.  Open the port and accept
   * connections.
   */
  public void run ()
    {
      try {
	this.open (this.port_);
	while (true)
	  this.accept();
      }
    catch (SocketException e)
      {
	ACE.ERROR (e);
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

    System.err.println("Stopped accepting");
    }

  /**
   *
   * Parse the command line.  This only looks for -p <port number>.
   *
   */
  protected void parseArgs (String args[])
    {
      String s;
      GetOpt opt = new GetOpt (args, "p:");

      for (int c; (c = opt.next ()) != -1; )
	{
	  switch (c)
	    {
	    case 'p':
	      s = opt.optarg ();
	      this.port_ = (new Integer (s)).intValue ();
	      break;
	    default:
	      ACE.ERROR("Invalid argument: " + c);
	      break;
	    }
	}
    }

  /**
   *
   * Modifies to behavior of Acceptor accept() so the TSClerkProcessor
   * reference can be passed to the TSRequestHandler.
   *
   */

  protected SvcHandler makeSvcHandler ()
        throws InstantiationException, IllegalAccessException
  {
    return (SvcHandler) new TSRequestHandler(parent_);
  }

  // Port to monitor
  private int port_ = 7990;

  // Reference to the Clerk Processor (which holds the time value)
  private TSClerkProcessor parent_;
};
     

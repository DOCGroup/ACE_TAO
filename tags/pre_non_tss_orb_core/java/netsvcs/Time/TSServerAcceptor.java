/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Time
 *
 * = FILENAME
 *    TS_Server_Acceptor.java
 *
 *@author Prashant Jain
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
 * <p><h2>DESCRIPTION</h2>
 *
 * Acceptor: listens to a port and launches TSServerHandlers
 * when connections are made.
 *
 * @see netsvcs.Time.TSServerHandler, JACE.Connection.Acceptor
 */
public class TSServerAcceptor extends Acceptor implements Runnable
{
  // Run this in its own thread
  public int init (String [] args)
    {
      // Parse arguments
      this.parseArgs (args);

      // Run in own thread of control so that we don't block the caller
      new Thread (this).start ();
      return 0;
    }

  // Create a TSServerHandler for each client that wants to connect
  public void run ()
  {
    try
      {
	this.setHandlerFactory (Class.forName ("netsvcs.Time.TSServerHandler"));
	this.open (this.port_);
	while (true)
	  this.accept ();
      }
    catch (ClassNotFoundException e)
      {
	ACE.ERROR (e);
      }
    catch (SocketException e)
      {
	ACE.ERROR ("Socket Exception: " + e);
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

  // Process the command line
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
	    ACE.ERROR ("Unknown argument: " + c);
	    break;
	  }
      }
  }

  private int port_ = 7989;
}


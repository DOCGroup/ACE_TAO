/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    ServerLoggingAcceptor.java
 *
 *@author Chris Cleeland, Everett Anderson
 *
 *************************************************/
package netsvcs.Logger;

import JACE.SOCK_SAP.*;
import JACE.Connection.*;
import JACE.OS.*;
import java.io.*;
import java.net.*;
import JACE.Misc.*;

/**
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>
 * Acceptor:  Listens on a specified port and launches ServerLoggingHandlers
 * in response to requests.  A LogMessageReceiver can be specified on the
 * command line to change the way the logging service processes messages.
 *
 * @see netsvcs.Logger.ServerLoggingHandler, netsvcs.Logger.LogMessageReceiver
 */
public class ServerLoggingAcceptor extends Acceptor implements Runnable
{
  /** Main function to bootstrap the process
   *
   *  Command line arguments:
   *
   *  -p <port>         Select a port for listening for requests
   *  -r <class name>   Specify a LogMessageReceiver (make sure it's a full class name)
   * 
   */
  public static void main (String [] args)
    {
      ServerLoggingAcceptor sla = new ServerLoggingAcceptor();

      sla.init(args);
    }

  /** 
   * Receives the command line and launches its own thread
   */
  public int init (String [] args)
    {
      this.parseArgs(args);

      // If the user didn't specify a LogMessageReceiver, use the default
      // (which just calls a print method on LogMessage)
      if (this.receiver_ == null)
	this.receiver_ = new DefaultLMR();

      new Thread (this).start();
      return 0;
    }

  /**
   * Specify what LogMessageReceiver to use 
   */
  public void setLMR(LogMessageReceiver receiver)
    {
      this.receiver_ = receiver;
    }

  /** 
   * Accessor for the LogMessageReceiver
   */
  public LogMessageReceiver getLMR ()
    {
      return this.receiver_;
    }

  /** 
   * Create a new ServerLoggingHandler 
   */
  protected SvcHandler makeSvcHandler ()
    throws InstantiationException, IllegalAccessException
    {
      return new netsvcs.Logger.ServerLoggingHandler (this.receiver_);
    }

  /**
   * Run forever accepting new connections
   */
  public void run ()
    {
      try {

	this.open (this.port_);
	while (true) 
	  this.accept();  

      } catch (SocketException e)
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

      ACE.ERROR("ServerLoggingAcceptor has exited");
    }

  /**
   * Process the command line
   */
  protected void parseArgs (String args[])
  {
    String s;
    GetOpt opt = new GetOpt (args, "p:r:");
    for (int c; (c = opt.next ()) != -1; )
      {
	switch (c)
	  {
	  case 'p':
	    s = opt.optarg ();
	    this.port_ = (new Integer (s)).intValue ();
	    break;
	  case 'r':
	    // Load the LMR with the given name
	    s = new String(opt.optarg ());
	    Class LMRfactory;
	    try {
	      LMRfactory = Class.forName(s);

	      receiver_ = (LogMessageReceiver)LMRfactory.newInstance();

	    } catch (ClassNotFoundException e) {
	      ACE.ERROR("Unable to find LMR factory: " + e);
	    } catch (InstantiationException e) {
	      ACE.ERROR("Creating LMR: " + e);
	    } catch (IllegalAccessException e) {
	      ACE.ERROR("Creating LMR: " + e);
	    }
	    // Any of the above exceptions will result in just using the
	    // default LMR
	    break;
	  default:
	    ACE.ERROR ("Unknown argument: " + c);
	    ACE.ERROR ("Valid args: -p <port> -r <LogMessageReceiver name>");
	    break;
	  }
      }
  }

  private int port_ = ACE.DEFAULT_SERVER_PORT;
  private LogMessageReceiver receiver_ = null;
};







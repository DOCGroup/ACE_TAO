package JACE.netsvcs.Logger;

import java.util.*;
import java.io.*;
import java.net.*;
import JACE.SOCK_SAP.*;
import JACE.Connection.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.netsvcs.Server;

/**
 * Server for the logging service.  Sets the default logging strategy
 * to StderrStrategy so that logging requests are printed on the
 * System.err stream.  Other strategies can be specified on the
 * command line.  
 * <P>
 * <B>Valid command line arguments:</B>
 * <PRE>
 *   -r (class name)  Specify a LoggingStrategy
 *                    (Default is StdErrStrategy)
 *   -p (port)        Port to listen on for clients
 *   -d               Enable debugging messages
 *   -a (class name)  Specify ActivateStrategy
 *                    (Default is thread per connection)
 * </PRE>
 *
 *@see LoggingStrategy
 *@see StderrStrategy
 */
public class ServerLoggingAcceptor extends Server
{
  /**
   * Default constructor.  Sets the default LoggingStrategy to
   * StderrStrategy.
   */
  public ServerLoggingAcceptor ()
  {
    name ("Logging Service");
    logStrategy_ = new StderrStrategy ();
  }

  /**
   * Simple main program for running the logging service without the
   * service configurator.
   *
   *@param args command line arguments
   */
  public static void main (String [] args)
  {
    ServerLoggingAcceptor sla = new ServerLoggingAcceptor();
    
    sla.init(args);
  }

  /** 
   * Accessor for the LoggingStrategy
   */
  public LoggingStrategy loggingStrategy ()
  {
    return this.logStrategy_;
  }

  /**
   * Creates a new ServerLoggingHandler instance.
   */
  protected SvcHandler makeSvcHandler ()
  {
    return new ServerLoggingHandler ();
  }

  /**
   * Prints out the valid command line arguments.  See the class
   * description for more information.  Called by Server.init when
   * parseArgs returns -1.
   */
  protected void printUsage ()
  {
    ACE.ERROR ("Valid options:\n");
    ACE.ERROR ("-r <class name>  Specify a LoggingStrategy");
    ACE.ERROR ("                 (Default is StdErrStrategy");
    ACE.ERROR ("-p <port>        Port to listen on for clients");
    ACE.ERROR ("-d               Enable debugging messages");
    ACE.ERROR ("-a <class name>  Specify ActivateStrategy");
    ACE.ERROR ("                 (Default is single threaded");
  }
  
  /**
   * Parses the command line arguments.  See the class description
   * for more information.
   *
   *@param args command line arguments
   *@return -1 on failure, 0 on success
   */
  protected int parseArgs (String args[])
  {
    String s;
    Object strategy;
    GetOpt opt = new GetOpt (args, "p:r:da:", true);
    int c = 0;
   
    try {

      while ((c = opt.next ()) != -1)
	{
	  switch (c)
	    {
	    case 'd':
	      ACE.enableDebugging ();
	      ACE.DEBUG ("Debugging is enabled");
	      break;
	    case 'p':
	      if (!port (opt.optarg ()))
		return -1;
	      break;
	    case 'a':
	      strategy = newStrategyInstance (opt.optarg (),
					      "ActivateStrategy");
	      if (strategy == null)
		return -1;

	      activateStrategy ((ActivateStrategy) strategy);
	      break;
	    case 'r':
	      // Load the Strategy with the given name
	      strategy = newStrategyInstance (opt.optarg (),
					      "LoggingStrategy");
	      if (strategy == null) 
		return -1;
	      
	      logStrategy_ = (LoggingStrategy)strategy;
	      break;
	    default:
	      ACE.ERROR ("Unknown argument: " + c);
	      return -1;
	    }
	}
    } catch (ArrayIndexOutOfBoundsException e) {
      ACE.ERROR ("Option -" + (char)c + " requires an argument");
      return -1;
    }

    return 0;
  }

  private LoggingStrategy logStrategy_;
}

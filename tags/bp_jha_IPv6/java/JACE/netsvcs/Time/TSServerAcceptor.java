package JACE.netsvcs.Time;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.TimeValue;
import JACE.Concurrency.ThreadManager;
import JACE.netsvcs.Server;

/**
 * Server for the time service.  Creates TSServerHandlers as necessary
 * to handle the requests.
 * <P>
 * <B>Valid command line arguments:</B>
 * <PRE>
 *    -p (port)        Port to listen on for clients");
 *    -d               Enable debugging messages");
 *    -a (class name)  Specify ActivateStrategy");
 *                     (Default is multi-threaded");
 * </PRE>
 *
 */
public class TSServerAcceptor extends Server
{
  public TSServerAcceptor ()
  {
    // Set the name in case we're not using the service configurator
    name ("Time Service");
  }

  /**
   * Simple main program for running the logging service without the
   * service configurator.
   *
   *@param args command line arguments
   */
  public static void main (String [] args)
  {
    // Simple main program to get things rolling
    TSServerAcceptor ta = new TSServerAcceptor();

    ta.init (args);
  }

  /**
   * Creates a new TSServerHandler instance.
   */  
  protected SvcHandler makeSvcHandler ()
  {
    return new TSServerHandler ();
  }
  
  /**
   * Prints out the valid command line arguments.  See the class
   * description for more information.  Called by Server.init when
   * parseArgs returns -1.
   */
  protected void printUsage ()
  {
    ACE.ERROR ("Valid options:\n");
    ACE.ERROR ("-p <port>        Port to listen on for clients");
    ACE.ERROR ("-d               Enable debugging messages");
    ACE.ERROR ("-a <class name>  Specify ActivateStrategy");
    ACE.ERROR ("                 (Default is multi-threaded");
  }

  /**
   * Parses the command line arguments.  See the class description
   * for more information.
   *
   *@param args command line arguments
   *@return -1 on failure, 0 on success
   */
  protected int parseArgs (String [] args) 
  {
    int c = 0;
    GetOpt opt = new GetOpt(args, "p:da:", true);
    
    try {

      while ((c = opt.next ()) != -1) {
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
	    Object strategy = newStrategyInstance (opt.optarg (),
						   "ActivateStrategy");
	    if (strategy == null)
	      return -1;
	    
	    activateStrategy ((ActivateStrategy) strategy);
	    break;
	  default:
	    ACE.ERROR("Unknown argument: " + (char)c);
	    return -1;
	  }
      }
    } catch (ArrayIndexOutOfBoundsException e) {
      ACE.ERROR ("Option -" + (char)c + " requires an argument");
      return -1;
    }

    return 0;
  }
}

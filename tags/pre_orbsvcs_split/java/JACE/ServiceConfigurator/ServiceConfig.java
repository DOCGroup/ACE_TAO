/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceConfig.java
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
import java.net.*;
import JACE.OS.*;
import JACE.Misc.*;

/**
 *     Provide the base class that supplies common server daemon
 *     operations.  Also provides a global point for interacting with
 *     the service repository.  Services can be suspended, resumed,
 *     removed, and reloaded.
 *     <P>
 *     ServiceConfig files contain lines of the following forms:
 *
 * <PRE>
 *  load (Service Name) (Class Name) (Type) "argument list" (opt URL path)
 *
 *  resume (Service Name)
 *  suspend (Service Name)
 *  remove (Service Name)
 *
 * where (Type) is currently only ServiceObject or Service_Object 
 * [Note:  This is not enforced by the ServiceConfig program, but
 * something must be there.]
 * </PRE>
 * <P>
 * <B>Valid command line arguments:</B>
 * <PRE>
 *    -f (filename)     Specify name of the service config file
 *    -d                Enable debugging
 *    -p (URL path)     If not found locally, search this URL path
 *                      for classes (multiple -p options allowed)
 * </PRE>
 *
 *@author Prashant Jain
 *@author Everett Anderson
 */
public class ServiceConfig
{
  /** Begins the process of loading a service configurator file:
   *  parses the command line and calls processDirectives
   *
   *@param args command line arguments to the Service Configurator
   *@exception FileNotFoundException Couldn't find service config file
   *@exception IOException Problem reading or parsing the config file
   *@exception ClassNotFoundException Couldn't find a certain class
   *@exception IllegalAccessException Inappropriate method call on a class
   *@exception InstantiationException Couldn't create a certain class instance
   *@return -1 on failure, 0 on success
   */
  public static int open (String [] args) 
    throws FileNotFoundException, IOException, ClassNotFoundException, 
	   IllegalAccessException, InstantiationException
  {
    // Parse the command line
    if (ServiceConfig.parseArgs (args) < 0) {
      printUsage ();
      return -1;
    }

    return ServiceConfig.processDirectives ();
  }

  protected static void printUsage ()
  {
    ACE.ERROR ("Valid options:\n");
    ACE.ERROR ("-f <filename>     Specify name of the service config file");
    ACE.ERROR ("-d                Enable debugging");
    ACE.ERROR ("-p <URL path>     If not found locally, search this URL path");
    ACE.ERROR ("                  for classes (multiple -p options allowed)");
  }

  /** Parse the command line.
   * <P>
   * Currently, valid command line options are the following:
   * <PRE>
   * -d             Debug mode
   * -f (filename)  Load services in the given file
   * -p (URL path)  If not found locally, search this URL path for classes
   *                (there can be multiple -p URL options)
   * </PRE>
   *@param args command line arguments
   *@exception ClassNotFoundException Couldn't find a specified Service
   *@exception IllegalAccessException Inappropriate method call on a Service
   *@exception InstantiationException Couldn't create a Service instance
   */
  protected static int parseArgs (String [] args) 
    throws ClassNotFoundException, IllegalAccessException,
	   InstantiationException
  {
    GetOpt getopt = new GetOpt (args, "df:p:", true);
    for (int c; (c = getopt.next ()) != -1; )
      switch (c)
	{
	case 'p':
	  // Specify a URL path
	  try {
	    ServiceConfig.loader_.addURLPath (getopt.optarg ());
	  } catch (MalformedURLException e) {
	    ACE.ERROR ("Invalid URL: " + getopt.optarg ());
	  }
	  break;
	  /*
	    Not supported:
	    
	    case 'b':
	      ServiceConfig.beADaemon_ = true;
	      break;
	    case 'n':
	      break;
	  */
	case 'd': 
	  ACE.enableDebugging ();
	  ACE.DEBUG ("Debugging is enabled");
	  break;
	case 'f':
	  // Specify the file name of the config file
	  ServiceConfig.serviceConfigFile_ = getopt.optarg ();
	  break;
	default:
	  ACE.ERROR ((char ) c + " is not a ServiceConfig option");
	  return -1;
	}

    return 0;
  }

  /** 
   * Uses the Service Repository to suspend a service with the given name.
   *
   *@param name name of the Service to suspend
   *@return -1 on failure, 0 on success
   */
  public static int suspend (String name)
  {
    return ServiceConfig.svcRep_.suspend (name);
  }

  /**
   * Uses the Service Repository to resume a service with the given name.
   *
   *@param name name of the Service to resume
   *@return -1 on failure, 0 on success
   */
  public static int resume (String name)
  {
    return ServiceConfig.svcRep_.resume (name);
  }

  /**
   * Removes the specified Service from the Service Repository.
   *
   *@param name name of the Service to remove
   *@return -1 on failure, 0 on success
   */
  public static int remove (String name)
  {
    return ServiceConfig.svcRep_.remove (name);
  }

  /**
   * Returns the info String from the specified Service.
   *
   *@param name name of the Service to query
   *@return information about the Service
   */
  public static String info (String name)
  {
    return ServiceConfig.svcRep_.info (name);
  }

  /**
   * Call fini on the specified Service.
   *
   *@param name name of the Service to shut down
   *@return -1 on failure, 0 on success
   */
  public static int fini (String name)
  {
    return ServiceConfig.svcRep_.fini (name);
  }

  /**
   * Call init on the specified Service.  This is only needed
   * when a user wants to add a Service to the repository without
   * using service config files.  
   *
   *@param name name of the Service to initialize
   *@param args command line arguments to pass to the Service
   *@return -1 on failure, 0 on success
   *@see ServiceConfig#insert
   */
  public static int init (String name, String [] args) 
  { 
    return ServiceConfig.svcRep_.init (name, args);
  }

  /**
   * Adds the given Service to the Service Repository.  This is
   * only needed when a user wants to add a Service to the repository
   * without using service config files.
   *
   *@param service Service to insert
   *@see ServiceConfig#init
   */
  public static void insert (Service service)
  {
    ServiceConfig.svcRep_.insert (service);
  }

  /**
   * Check to see if the specified Service is suspended.
   *
   *@param name name of the Service to check
   *@return true if it is suspended, else false
   */
  public static boolean suspended (String name)
  {
    return ServiceConfig.svcRep_.suspended (name);
  }

  /**
   * Check to see if the repository contains a service with the given
   * name.
   *
   *@param name name of the Service to check
   *@return true if it is in the repository, else false
   */
  public static boolean contains (String name)
  {
    return (ServiceConfig.svcRep_.find (name) == null ? false : true);
  }

  /**
   * Get an Enumeration of all of the names of the Services in the
   * repository.
   *
   *@return Enumeration of Service names
   */
  public static Enumeration serviceNames ()
  {
    return ServiceConfig.svcRep_.serviceNames ();
  }

  /**
   * Get an Enumeration of all Services in the repository.
   *
   *@return Enumeration of Services
   */
  public static Enumeration services ()
  {
    return ServiceConfig.svcRep_.services ();
  }

  /** 
   * Load a service of the given name, type, and arguments, then 
   * initialize it with the given arguments.
   *
   * Should be aware that this could also throw a class cast exception if
   * the author of the service didn't implement the Service interface.
   *
   *@param serviceName name of the service to load
   *@param className class name to load
   *@param args command line arguments to initialize the service
   *@exception ClassNotFoundException couldn't find the specified class file
   *@exception IllegalAccessException problem calling a method on the service
   *@exception InstantiationException problem creating an instance
   *@exception IOException problem reading the class file
   *@return -1 on failure, 0 on success
   */
  public static int load (String serviceName,
			  String className,
			  String [] args) 
    throws ClassNotFoundException, IllegalAccessException,
	   InstantiationException, IOException 
  {
    Class svcClass = ServiceConfig.loader_.loadClass (className);

    Service svc = (Service)(svcClass.newInstance());

    svc.name (serviceName);

    ServiceConfig.svcRep_.insert (svc);

    return ServiceConfig.svcRep_.init (serviceName, args);
  }

  /**
   * Same as load (String, String, String[]) but creates the
   * String array from the given args parameter using the space
   * character as a delimiter.
   *
   */
  public static int load (String serviceName, 
			  String className,
			  String args)
    throws ClassNotFoundException, IllegalAccessException,
	   InstantiationException, IOException 
  {
    return ServiceConfig.load (serviceName,
			       className,
			       OS.createStringArray (args, " "));
  }

  /**
   * Parse a service configurator file, creating classes as necessary. 
   *
   *@return -1 on failure, 0 on success
   *@exception FileNotFoundException Couldn't find the file 
   *(default "svc.conf")
   *@exception IOException Problem reading/parsing 
   *@exception ClassNotFoundException Couldn't find a certain class
   *@exception IllegalAccessException Inappropriate method call
   *@exception InstantiationException Couldn't create a class instance
   */
  protected static int processDirectives () 
    throws FileNotFoundException, IOException, ClassNotFoundException, 
	   IllegalAccessException, InstantiationException 
  {
    ACE.DEBUG("Processing directives in file " + 
	      ServiceConfig.serviceConfigFile_);

    File configFile = new File (ServiceConfig.serviceConfigFile_);
    
    // Check if file exists and is a normal file
    if (!configFile.exists () || !configFile.isFile ())
      throw new FileNotFoundException ("File " + 
				       ServiceConfig.serviceConfigFile_ + 
				       " not found");
    
    // Check if the file is readable
    if (!configFile.canRead ())
      throw new IOException ("File " + 
			     ServiceConfig.serviceConfigFile_ + 
			     " not readable");

    // Set up the stream
    FileInputStream fileIn = new FileInputStream (configFile);

    // Parse the file
    Reader r = new BufferedReader(new InputStreamReader(fileIn));
    StreamTokenizer in = new StreamTokenizer (r);

    // Set characters in ASCII range 32 to 47, ASCII range 91 to 96,
    // and ASCII range 123 to 126 as ordinary characters
    in.wordChars ('.', '/');  // ASCII range 32 to 47
    in.wordChars (':', '@');  // ASCII range 58 to 64
    in.wordChars ('[', '`');  // ASCII range 91 to 96
    in.wordChars ('{', '~');  // ASCII range 123 to 126
    in.quoteChar ('\"');
    in.commentChar ('#');
    in.eolIsSignificant(true);
    in.lowerCaseMode(false);

    Vector strs = new Vector();
    int lineNumber = 1;

    while (in.nextToken () != StreamTokenizer.TT_EOF) {

      if (in.ttype == StreamTokenizer.TT_EOL) {

	String command = ((String)strs.elementAt(COMMAND_NAME)).toLowerCase();
	String name = (String)strs.elementAt(SERVICE_NAME);

	if (strs.size() == 2) {
	  if (command.equals("remove"))
	    ServiceConfig.remove(name);
	  else
	  if (command.equals("suspend"))
	    ServiceConfig.suspend(name);
	  else
	  if (command.equals("resume"))
	    ServiceConfig.resume(name);
	} else {

	  if (!command.equals("load")) 
	    throw new IOException ("Unknown command: " +
				   command +
				   " at line " +
				   lineNumber);

	  if (strs.size() < 5)
	    throw new IOException ("Line " + lineNumber +
				   " requires 5 values, not " + 
				   strs.size());

	  // Type is currently unenforced since everything must descend
	  // from Service anyway.
	  //String type = (String)strs.elementAt (SERVICE_TYPE);
	  String className = (String)strs.elementAt (CLASS_NAME);
	  
	  if (strs.size () > CODE_BASE) {
	    try {
	      String url = (String)strs.elementAt (CODE_BASE);

	      ServiceConfig.loader_.addURLPath (url);

	    } catch (MalformedURLException e) {
	      ACE.ERROR("Bad code base on line " + lineNumber);
	    }
	  }

	  ServiceConfig.load(name, 
			     className, 
			     (String)strs.elementAt(ARGS));
	}

	lineNumber++;
	strs.removeAllElements();
	continue;
      }

      // Skip over non-strings
      if ((in.ttype == StreamTokenizer.TT_WORD) ||
	  (in.ttype == '\"'))
	strs.addElement(new String(in.sval));
      else
	ACE.DEBUG ("Invalid string on line " + lineNumber +
		   " element " + strs.size() +
		   " type " + in.ttype);
    }

    return 0;
  }

  /**
   * Close all services.
   */
  public static void close ()
  {
    ServiceConfig.svcRep_.close ();
  }

  /**
   * Return a reference to the ServiceLoader used to load
   * classes.
   *
   *@return ServiceLoader used internally by ServiceConfig.
   */
  public static ServiceLoader loader () 
  {
    return ServiceConfig.loader_;
  }

  /**
   * Specify the ServiceLoader to be used by ServiceConfig.
   *
   *@param loader ServiceLoader instance to use (should support remote
   * loading)
   */
  public static void loader (ServiceLoader loader)
  {
    ServiceConfig.loader_ = loader;
  }

  // Set by command line options
  private static boolean beADaemon_ = false;

  private static String serviceConfigFile_ = "svc.conf";

  private static ServiceRepository svcRep_ = new ServiceRepository ();
  private static ServiceLoader loader_ = new ServiceLoader ();

  // Order for the commands on a line in the config file
  private final static int COMMAND_NAME = 0;
  private final static int SERVICE_NAME = 1;
  private final static int CLASS_NAME = 2;
  private final static int SERVICE_TYPE = 3;
  private final static int ARGS = 4;
  private final static int CODE_BASE = 5;
}

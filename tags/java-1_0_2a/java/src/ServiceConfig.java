/*************************************************
 *
 * = PACKAGE
 *    ACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceConfig.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.ServiceConfigurator;

import java.io.*;
import ACE.OS.*;
import ACE.Misc.*;

/**
 * <hr>
 * <p><b>TITLE</b><br>
 *     Provide the base class that supplies common server daemon
 *     operations.
 */
public class ServiceConfig
{
  public static int open (String [] args) throws
  FileNotFoundException, IOException, ClassNotFoundException,
    IllegalAccessException, InstantiationException
  {
    ServiceConfig.parseArgs (args);
    if (ServiceConfig.svcRep_ == null)
      ServiceConfig.svcRep_ = new ServiceRepository ();
    return ServiceConfig.processDirectives ();
  }

  protected static int processDirectives () throws
  FileNotFoundException, IOException, ClassNotFoundException,
    IllegalAccessException, InstantiationException 
  {
    File configFile = new File (ServiceConfig.serviceConfigFile_);
    
    // Check if file exists and is a normal file
    if (!configFile.exists () || !configFile.isFile ())
      throw new FileNotFoundException ("File " + ServiceConfig.serviceConfigFile_ + " not found");
    
    // Check if the file is readable
    if (!configFile.canRead ())
      throw new IOException ("File " + ServiceConfig.serviceConfigFile_ + " not readable");

    // Set up the stream
    FileInputStream fileIn = new FileInputStream (configFile);

    // Parse the file
    StreamTokenizer in = new StreamTokenizer (fileIn);

    // Set '#' as comment character to be ignored and set '/' as
    // ordinary character (was original comment character) 
    in.commentChar ('#');
    in.ordinaryChar ('/');

    // Set characters in ASCII range 33 to 47 and ASCII range 91 to 96
    // as ordinary characters
    in.wordChars ('!', '/');  // ASCII range 33 to 47
    in.wordChars ('[', '`');  // ASCII range 91 to 96

    String className = null;
    String classType = null;
    String args = null;
    // Create a state machine
    int state = ServiceConfig.CLASS_NAME;

    while (in.nextToken () != StreamTokenizer.TT_EOF)
      {
	switch (state)
	  {
	  case ServiceConfig.CLASS_NAME:
	    if (in.ttype == StreamTokenizer.TT_WORD)
	      className = in.sval;
	    else
	      throw new IOException ("Illegal CLASS NAME argument in file " + ServiceConfig.serviceConfigFile_);
	    state = ServiceConfig.CLASS_TYPE;
	    break;
	  case ServiceConfig.CLASS_TYPE:
	    if (in.ttype == StreamTokenizer.TT_WORD)
	      classType = in.sval;
	    else
	      throw new IOException ("Illegal CLASS TYPE argument in file " + ServiceConfig.serviceConfigFile_);
	    state = ServiceConfig.ARGS;
	    // Set space to be an ordinary character to allow
	    // arguments to be parsed in
	    in.wordChars (' ', ' ');
	    break;
	  case ServiceConfig.ARGS:
	    if (in.ttype == StreamTokenizer.TT_WORD)
	      {
		args = in.sval;
		// Load the class
		Class c = ServiceConfig.svcRep_.load (className);

		// Note that c should be defined else an exception
		// would have been thrown by now
		
		// Figure out the type of the class, create an
		// instance and then invoke the initialization method
		if (classType.equals ("ServiceObject"))
		  {
		    ServiceObject svcObj = (ServiceObject) c.newInstance ();

		    // Create an array of String from args String
		    String [] argArray = OS.createStringArray (args, " ");
		    
		    // Call init on the Service Object passing in arguments
		    svcObj.init (argArray);
		  }
		else
		  throw new IOException ("Illegal CLASS TYPE argument in file " + ServiceConfig.serviceConfigFile_);
	      }
	    else
	      throw new IOException ("Illegal ARGS argument in file " + ServiceConfig.serviceConfigFile_);
	    state = ServiceConfig.CLASS_NAME;
	    // Set space back to whitespace-character to extract the
	    // next token
	    in.whitespaceChars (' ', ' ');
	    break;
	  default:
	    throw new IOException ("Illegal argument in file " + ServiceConfig.serviceConfigFile_);
	  }
      }
    return 0;
  }

  protected static void parseArgs (String [] args)
  {
    GetOpt getopt = new GetOpt (args, "bdnf:");
    for (int c; (c = getopt.next ()) != -1; )
      switch (c)
	{
	case 'b':
	  // Note: not supported yet!
	  ServiceConfig.beADaemon_ = true;
	  break;
	case 'd': 
	  ServiceConfig.debug_ = true;
	  break;
	case 'n':
	  ServiceConfig.noDefaults_ = true;
	  break;
	case 'f':
	  ServiceConfig.serviceConfigFile_ = getopt.optarg ();
	  break;
	default:
	  ACE.ERROR ((char ) c + " is not a ServiceConfig option");
	  break;
	}
  }

  // Set by command line options
  private static boolean beADaemon_ = false;
  private static boolean debug_ = false;
  private static boolean noDefaults_ = false;
  private static String serviceConfigFile_ = "svc.conf";
  private static ServiceRepository svcRep_ = null;

  // States for the state-machine used in parsing the config file
  private final static int CLASS_NAME = 0;
  private final static int CLASS_TYPE = 1;
  private final static int ARGS = 2;
}

/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Naming
 *
 * = FILENAME
 *    NameAcceptor.java
 *
 *************************************************/
package JACE.netsvcs.Naming;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.TimeValue;
import JACE.netsvcs.Server;

/**
 * Server for the naming service.  
 * Listens on the specified port (command line option) and launches
 * NameHandlers when connections are made.  
 * <P>
 * The hash table for the mapping and a timer queue are created here.
 * Periodically, if it has been changed, the mapping is saved to a file.
 * If the data file exists at load time, it is read from disk.  Currently,
 * the service stores the entire mapping in one Hashtable (which is probably
 * kept in memory at all times).
 * <P>
 * <B>Valid command line arguments:</B>
 * <PRE>
 *    -f (file name)    File name of the database
 *                      (Default is namedata.dat)
 *    -p (port number)  Port to listen on for clients
 *    -d                Enable debugging
 *    -t (time sec)     How often to save the database (default 60 sec)
 *    -a (class name)   Specify ActivateStrategy
 *                      (Default is multi-threaded)
 * </PRE>
 *
 *@see NameHandler
 *
 *@author Everett Anderson
 *
 */
public class NameAcceptor extends Server
{
  /**
   * Constructor
   */
  public NameAcceptor ()
  {
    // Set the name in case we are not using the service
    // configurator
    name ("Naming Service");

    // Create the hash table and timer queue
    this.mapping_ = new Hashtable ();
    this.tq_ = new TimerQueue (true);
  }

  /**
   * Simple main program.  See the class description for more
   * information about command line arguments.
   */
  public static void main (String [] args)
  {
    // Simple main program to get things rolling
    NameAcceptor na = new NameAcceptor();

    na.init(args);
  }

  /**
   * Check to see if the mapping has been modified since the last
   * save.
   */
  synchronized boolean modifiedMapping ()
  {
    return mappingWasModified_;
  }

  /**
   * Set the modified state of the mapping.
   */
  synchronized void modifiedMapping (boolean value)
  {
    mappingWasModified_ = value;
  }

  /**
   * Cancels the timer which was used to save the mapping, then delegates
   * to Server.fini ().
   *
   *@return -1 on failure, 0 on success
   */
  public int fini ()
  {
    if (!done () && tq_ != null) 
      tq_.cancelTimer (this);

    return super.fini ();
  }

  /**
   * Read the data file (if it exists) and schedule a periodic timer
   * to save it at intervals.  At the end, this delegates to
   * Server.initialize () (which currently sets the default
   * activation scheme if it wasn't defined on the command line).
   *
   *@see Server#initialize
   *@return -1 on failure, 0 on success
   */
  protected int initialize ()
  {
    this.loadTable ();

    this.tq_.scheduleTimer (this,
			    null,
			    new TimeValue (this.updateInterval_),
			    new TimeValue (this.updateInterval_));

    // Use whatever default ActivateStrategy is defined in the
    // Server class (unless specified in parseArgs)
    return super.initialize ();
  }

  /**
   * Create a new NameHandler instance.
   */
  protected SvcHandler makeSvcHandler ()
  {
    return new NameHandler (mapping_);
  }

  /**
   * Prints out the valid command line arguments.  See the class
   * description for more information.  Called by Server.init when
   * parseArgs returns -1.
   */
  protected void printUsage ()
  {
    ACE.ERROR ("Valid options:\n");
    ACE.ERROR ("-f <file name>    File name of the database");
    ACE.ERROR ("                  (Default is namedata.dat)");
    ACE.ERROR ("-p <port number>  Port to listen on for clients");
    ACE.ERROR ("-d                Enable debugging");
    ACE.ERROR ("-t <time sec>     How often to save the database");
    ACE.ERROR ("                  (Default is 60 seconds)");
    ACE.ERROR ("-a <class name>   Specify ActivateStrategy");
    ACE.ERROR ("                  (Default is multi-threaded");
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
    String s;
    GetOpt opt = new GetOpt (args, "p:f:t:da:", true);

    try {

      while ((c = opt.next ()) != -1) {
	switch (c)
	  {
	  case 'f':
	    this.filename_ = opt.optarg ();
	    break;
	  case 't':
	    try {
	      this.updateInterval_ = Integer.parseInt (opt.optarg ());
	    } catch (NumberFormatException e) {
	      ACE.ERROR ("Invalid interval specified: " + e.getMessage ());
	      return -1;
	    }
	    break;
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
	    ACE.ERROR ("Unknown argument: " + (char)c);
	    return -1;
	  }
      }
    } catch (ArrayIndexOutOfBoundsException e) {
      ACE.ERROR ("Option -" + (char)c + " requires an argument");
      return -1;
    }
    
    return 0;
  }

  /**
   * Loads the hash table into memory from the specified
   * file.  Uses ObjectInputStream.
   */
  protected void loadTable ()
  {
    File file = new File(this.filename_);
    FileInputStream fis;
    ObjectInputStream ois;

    Hashtable ht = null;

    try {

      if ((file.exists()) && (file.canRead())) {

	fis = new FileInputStream (file);

	ois = new ObjectInputStream(fis);

	ht = (Hashtable)ois.readObject();
      } else 
	return;
    } catch (ClassNotFoundException e) {
      ACE.ERROR(e);
    } catch (StreamCorruptedException e) {
      ACE.ERROR(e);
    } catch (SecurityException e) {
      ACE.ERROR(e);
    } catch (IOException e) {
      ACE.ERROR(e);
    }

    if (ht != null) 
      this.mapping_ = ht;

  }

  /**
   * Writes the table out to the specified file if it has been modified.
   */
  protected void saveTable ()
  {
    if (!modifiedMapping ())
      return;

    FileOutputStream fos;
    ObjectOutputStream oos;

    try {

      fos = new FileOutputStream(this.filename_);
      oos = new ObjectOutputStream(fos);

      synchronized (this.mapping_) {
	oos.writeObject(this.mapping_);

	modifiedMapping (false);
      }

      oos.flush();

      oos.close();

    } catch (OptionalDataException e) {
      ACE.ERROR(e);
    } catch (NotSerializableException e) {
      ACE.ERROR(e);
    } catch (IOException e) {
      ACE.ERROR(e);
    }
  }

  /**
   * Call back for the TimerQueue.  This calls the method to save the
   * hash table.  The default time out is 60 seconds.
   */
  public int handleTimeout (TimeValue tv, Object obj)
  {
    this.saveTable();

    return 0;
  }

  // Mapping data structure
  Hashtable mapping_ = null;

  // Default file name
  String filename_ = "namedata.dat";

  // How often to save the table (seconds)
  int updateInterval_ = 60;

  // Calls handleTimeout at updateInterval_ intervals
  TimerQueue tq_ = null;

  boolean mappingWasModified_ = false;
}


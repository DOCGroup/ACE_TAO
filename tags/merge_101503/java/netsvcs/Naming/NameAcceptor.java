/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Naming
 *
 * = FILENAME
 *    NameAcceptor.java
 *
 * Listens on the specified port (command line option) and launches
 * NameHandlers when connections are made.  Each NameHandler runs in
 * its own thread.
 *
 * The hash table for the mapping and a timer queue are created here.
 * Periodically the mapping is written out to a file.
 *
 * A small main program is included to start things off.  If the
 * data file exists, it is read into memory.  Currently the service
 * stores the entire mapping in memory at all times.  The mapping is
 * dumped to a file at regular intervals.
 *
 *@see netsvcs.Naming.NameHandler
 *
 *@author Everett Anderson
 *
 *************************************************/
package netsvcs.Naming;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.TimeValue;

public class NameAcceptor extends Acceptor implements Runnable
{
  /**
   * Constructor
   */
  public NameAcceptor ()
    {
      super();

      // Create the hash table and timer queue
      this.mapping_ = new Hashtable();
      this.tq_ = new TimerQueue(true);
    }

  /**
   * Simple main program.  Command line options are
   * described under parseArgs.
   */
  public static void main (String [] args)
    {
      // Simple main program to get things rolling
      NameAcceptor na = new NameAcceptor();

      na.init(args);
    }


  /**
   * Close the socket when shutting down
   */
  public int fini ()
  {
    try 
      {
	this.done_ = true;
	this.sockAcceptor_.close();
      } 
    catch (IOException e) 
      {
	ACE.ERROR("" + e);
	return -1;
      }

    return 0;
  }

  /**
   * Stops accepting when suspended 
   */
  public int suspend()
  {
    this.suspended_ = true;
    return 0;
  }

  /**
   * Resumes accepting 
   */
  public int resume()
  {
    this.suspended_ = false;
    return 0;
  }


  /**
   * Runs this instance in its own thread
   */
  public int init (String [] args)
    {
      // Parse arguments
      this.parseArgs (args);

      System.out.println("Starting naming service on port: " + this.port_);

      // Run in own thread of control so that we don't block the caller
      new Thread (this).start();

      return 0;
    }

  /**
   * 
   * Main loop:  launches NameHandlers in separate threads whenever a
   * connection request is made
   */
  public void run ()
  {
    // Load the hash table from disk
    this.loadTable();

    // Schedule to write out the memory copy of the hash table at regular
    // intervals
    this.tq_.scheduleTimer(this, 
			   null, 
			   new TimeValue(this.updateInterval_), 
			   new TimeValue(this.updateInterval_));

    try
      {
	// Create new NameHandlers as requests come in
	this.open (this.port_);
	while (!this.done_) {

	  if (!this.suspended_)
	    this.accept ();
	}
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
  }

  /**
   * Create a new NameHandler
   */
  protected SvcHandler makeSvcHandler ()
    throws InstantiationException, IllegalAccessException
    {
      return new netsvcs.Naming.NameHandler (this.mapping_);
    }

  /** 
   * Process the command line.  The following options are available:
   *
   * -p <port>       Port number for listening
   * -f <filename>   Name of the database file
   * -t <time>       Mapping write-out time interval (in seconds)
   *
   */
  protected void parseArgs (String args[])
  {
    String s;
    GetOpt opt = new GetOpt (args, "p:f:t:");
    for (int c; (c = opt.next ()) != -1; )
      {
	switch (c)
	  {
	    // Specify port
	  case 'p':
	    s = opt.optarg ();
	    this.port_ = (new Integer (s)).intValue ();
	    break;
	    // Specify file name of the database
	  case 'f':
	    s = opt.optarg ();
	    this.filename_ = new String(s);
	    break;
	    // Specify time interval to write out the table
	  case 't':
	    s = opt.optarg ();
	    this.updateInterval_ = (new Integer (s)).intValue();
	    break;
	  default:
	    ACE.ERROR ("Unknown argument: " + c);
	    break;
	  }
      }
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
   * Writes the table out to the specified file.
   */
  protected void saveTable ()
    {
      FileOutputStream fos;
      ObjectOutputStream oos;

      try {

	fos = new FileOutputStream(this.filename_);
	oos = new ObjectOutputStream(fos);

	oos.writeObject(this.mapping_);

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

  // Port to listen on
  private int port_ = ACE.DEFAULT_SERVER_PORT;

  // Mapping data structure
  Hashtable mapping_ = null;

  // Default file name
  String filename_ = "namedata.dat";

  // How often to save the table (seconds)
  int updateInterval_ = 60;

  // Calls handleTimeout at updateInterval_ intervals
  TimerQueue tq_ = null;

  boolean done_ = false;
  boolean suspended_ = false;

}


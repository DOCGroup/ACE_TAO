/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    ServerLoggingHandler.java
 *
 *@author Chris Cleeland, Everett Anderson
 *
 *************************************************/
package netsvcs.Logger;

import JACE.SOCK_SAP.*;
import JACE.Connection.*;
import JACE.OS.*;
import java.util.*;
import java.io.*;

/**
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>
 * Created by ServerLoggingAcceptor every time a client connects.  This reads 
 * a logging statement passes it to the LogMessageReceiver for processing.
 * </blockquote>
 *
 * @see netsvcs.Logger.ServerLoggingAcceptor
 */
public class ServerLoggingHandler extends SvcHandler
{
  // Processes log messages
  private LogMessageReceiver receiver_;

  /**
   * Constructor
   *
   *@param receiver    LogMessageReceiver that handles what to do with a message
   */
  public ServerLoggingHandler (LogMessageReceiver receiver)
  {
    super();
    this.receiver_ = receiver;
  }

  /**
   * Start this handler in its own thread
   */
  public int open(Object obj)
  {
    new Thread (this).start();
    return 0;
  }

  /**
   * Accessor: get the host name of the connected client
   */
  protected String hostName ()
  {
    return new String(this.peer().socket().getInetAddress().getHostName());
  }

  /**
   * Receive input from the client, and send it to the LMR.  This is the
   * main loop for this thread.
   */
  public void run()
  {
    DataInputStream dis = new DataInputStream(this.peer().inputStream());

    for (;;)
      {
	// Messages arrive in the ACE.LogRecord format
	//
	// Hey!  We need exception catching in here too!
	try
	  {
	    // Reconstitute a log message from the wire
	    LogRecord rec = new LogRecord();
	
	    rec.streamInFrom(dis);
	
	    // Give the record to the log processor
	    this.receiver_.logRecord(this.hostName(),
				     rec);
	  }
	catch (EOFException eof)
	  {
	    try { 
	      this.stream_.close(); 
	    } catch (IOException n) { }

	    return;
	  }
	catch (IOException ioe)
	  {
	    ACE.ERROR(Thread.currentThread().getName()
			       + ": "
			       + ioe);
	  }
      }
  }
};


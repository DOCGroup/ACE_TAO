/*************************************************
 *
 *@author Chris Cleeland
 *
 * What we really need to define is a run() (or whatever
 * the Thread class has as its method to execute code), and
 * have that do the usual delegated work of handle_input.
 * We also need to figure out the best place to close the
 * the socket, which probably ISN'T the finalizer.
 *
 *************************************************/

//package ACE.SimpleLogger;

import JACE.SOCK_SAP.*;
import java.util.*;
import java.io.*;

// Should this extend or simply be handed to a Thread instance to
// be run?
public class LoggingHandler extends Thread
{
  private SOCKStream cliStream_;

  /**
   * Create a default Logging Handler
   */
  public  LoggingHandler()
  {
    this(new SOCKStream());
  }

  /**
   * Create a LoggingHandler with an existing stream
   */
  public LoggingHandler(SOCKStream aStream)
  {
    cliStream_ = aStream;
    setName();
  }

  private void setName()
  {
    int portnum = ((cliStream_.socket() == null)
		   ? 0
		   : cliStream_.socket().getLocalPort());
    this.setName("LoggingHandler#" + portnum);
  }

  /**
   * Start
   */ 
  public void open()
  {
    this.start();
  }

  /**
   */
  public SOCKStream stream()
  {
    return cliStream_;
  }

  /**
   * Handle logging events
   */
  public void run()
  {
    DataInputStream dis = (DataInputStream) cliStream_.inputStream();

    for (;;)
      {
	// Messages arrive in the following format:
	// o 4 byte length (network format)
	// o message, in ACE.LogRecord format
	//
	// Hey!  We need exception catching in here too!
	try
	  {
	    // Reconstitute a log message from the wire
	    LogRecord rec = new LogRecord();

	    // We don't really need this, because
	    // the object already knows how to
	    // extract itself properly.  However,
	    // in order to interoperate with the
	    // C++ version, this must be extracted.
	    // Plus, it makes a convenient way to
	    // check everything.
	    int length = dis.readInt();
	
	    rec.streamInFrom(dis);
	
	    if (rec.length() == length)
	      {
		rec.print(cliStream_.socket().getInetAddress().getHostName(),
			  true, System.out);
		System.out.flush();
	      }
	    else
	      {
		System.err.println("Logging_Handler: Length error receiving logging message\n");
	      }
	  }
	catch (EOFException eof)
	  {
	    System.err.println(Thread.currentThread().getName()
			       + ": end-of-file condition found; terminating.");
	    try { cliStream_.close(); } catch (IOException n) { }
	    this.stop();
	  }
	catch (IOException ioe)
	  {
	    System.err.println(Thread.currentThread().getName()
			       + ": IOException received -- "
			       + ioe.getMessage());
	  }
      }
  }
};

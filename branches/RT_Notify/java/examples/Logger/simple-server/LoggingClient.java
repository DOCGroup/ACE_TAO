/**
 * Main class that acts as an example logging client.
 */

import java.io.*;
import java.net.*;
import JACE.SOCK_SAP.*;
import LogRecord;
import LoggerConstants;

public class LoggingClient implements Runnable
{
  private String loggerHost_;
  private int port_;
  private int maxIter_;
  private static final int DEFAULT_ITERATIONS = 10;
  
  public static void main(String[] args)
  {
    // Really need to put code in here to parse options
    int iter;
    int port;
    String host;

	iter = (args.length > 0) ? Integer.parseInt(args[0])
          : DEFAULT_ITERATIONS;
	port = (args.length > 1) ? Integer.parseInt(args[1])
	  : LoggerConstants.DEFAULT_SERVER_PORT;
	host = (args.length > 2) ? args[2]
	  : LoggerConstants.DEFAULT_SERVER_HOSTNAME;

    LoggingClient lc = new LoggingClient(iter, port, host);
    lc.run();
  }

  public LoggingClient()
  {

    this(DEFAULT_ITERATIONS,
	 LoggerConstants.DEFAULT_SERVER_PORT,
	 LoggerConstants.DEFAULT_SERVER_HOSTNAME);
  }

  public LoggingClient(int iterations, int thePort, String theHost)
  {
    maxIter_ = iterations;
    port_ = thePort;
    loggerHost_ = theHost;
  }

  public void run()
  {
    SOCKStream logger = new SOCKStream();
    SOCKConnector connector = new SOCKConnector();
    // INETAddr addr = new INETAddr(port_, loggerHost_);

    LogRecord rec = new LogRecord(9, 2, 0);

    try
      {
        connector.connect(logger, loggerHost_, port_);

	int oneSecond = 1000;
	// Currently SOCKStream uses DataInputStream for its input stream,
	// and PrintStream for its output stream.  It probably ought to use
	// DataOutputStream for the output stream for symmetry, or at least
	// provide a mechanism for changing the type of the filter stream
	// used (which might be better in the long run...give it the class
	// id).
	BufferedOutputStream bos = new BufferedOutputStream((OutputStream) logger.outputStream(), LogRecord.MAXLOGMSGLEN);
	DataOutputStream dos = new DataOutputStream(bos);

	for (int i = 0; i < maxIter_; i++)
	  {
	    // Need to overload LogRecord.msgData to take a String
	    // argument so that it's easy to create instances with text
	    // inside.
	    rec.msgData("message = " + i);
	    try
	      {
		dos.writeInt(rec.length());
		rec.streamOutTo(dos);
		bos.flush();
		rec.print("localhost", true, System.err);
	      }
	    catch (IOException ex) { }

	    try
	      {
		Thread.sleep(oneSecond);
	      }
	    catch (InterruptedException ex) { }
	  }

	try { logger.close(); } catch (IOException ex) { }

      }
    catch (SocketException ex)
      {
        System.err.println("socket exception: " + ex);
      }
    catch (IOException ex)
      {
        System.err.println("io exception: " + ex);
      }

  }
}

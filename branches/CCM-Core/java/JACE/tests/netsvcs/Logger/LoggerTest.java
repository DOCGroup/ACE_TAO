/*************************************************
 *
 * = FILENAME
 *    LoggerTest.java
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.tests.netsvcs.Logger;

import JACE.SOCK_SAP.*;
import java.io.*; 
import java.net.*;
import JACE.OS.*;
import JACE.netsvcs.Logger.*;

/**
 *
 * This is a simple test log client very similar to the direct_logging 
 * client of C++ ACE.  The logging service should correctly receive 
 * messages from both the C++ and Java version.
 *
 * @see JACE.netsvcs.Logger.ServerLoggingAcceptor
 */
public class LoggerTest {

  /** Command line:  <hostname> [<port>]
   *
   * Creates a "hello world" log message and sends it to the server logging 
   * service.
   */
  public static void main(String args[])
    {
      if (args.length != 2) {
	System.err.println("Use:  LoggerTest <host name> [<port>]");
	System.exit(0);
      }

      ACE.enableDebugging ();

      // Set the port
      int port = args.length > 1 ? 
	(new Integer(args[1])).intValue() : ACE.DEFAULT_SERVER_PORT;

      SOCKStream cli_stream = new SOCKStream();
      INETAddr remote_addr;
      String host;

      // Try to find the host
      try {

	host = args[0];

	remote_addr = new INETAddr(port, host);

      } catch (UnknownHostException uhe) {
	ACE.ERROR("UnknownHostException " + uhe);
	return;
      }

      System.out.println("Connecting to " + host + " on port " + port);

      SOCKConnector con = new SOCKConnector();

      try {

	// Connect to the service
	con.connect(cli_stream, remote_addr);

      } catch (SocketException se) {
	
	ACE.ERROR("Socket Exception " + se);
	return;

      } catch (IOException ie) {

	ACE.ERROR("IOException " + ie);
	return;
      }


      // Send a message with priority 4, the current time,
      // and 0 for the process ID.
      LogRecord record = new LogRecord(4,
				       System.currentTimeMillis(),
				       0);

      // Set the text of the message
      record.msgData("hello world");

      try {

	// Send it
	record.streamOutTo(cli_stream.socket().getOutputStream ());

	// Close the socket
	cli_stream.close();

      } catch (IOException ie) {

	ACE.ERROR("" + ie);
	return;
      }
    }
};


      

	

	

	
      

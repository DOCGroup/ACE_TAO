/*************************************************
 *
 * = FILENAME
 *    LoggerTest.java
 *
 *@author Everett Anderson
 *
 *************************************************/
import JACE.SOCK_SAP.*;
import java.io.*; 
import java.net.*;
import JACE.OS.*;
import netsvcs.Logger.LogRecord;

/**
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>
 * This is an example/test log client very similar to the direct_logging client of
 * C++ ACE.  The server logging service should correctly receive messages from both
 * of these examples.
 * </blockquote>
 *
 * @see netsvcs.Logger.ServerLoggingAcceptor, netsvcs.Logger.LogRecord
 */
public class LoggerTest {

  // Command line:  <port> <hostname>
  //
  // Creates a "hello world" log message and sends it to the server logging service.
  // If no command line arguments are given, it uses ACE.DEFAULT_SERVER_PORT, and the
  // current machine for the service location.
  //
  public static void main(String args[])
    {
      int port = args.length > 0 ? (new Integer(args[0])).intValue() : ACE.DEFAULT_SERVER_PORT;

      SOCKStream cli_stream = new SOCKStream();
      INETAddr remote_addr;
      String host;

      try {

	host = args.length > 1 ? args[1] : InetAddress.getLocalHost().getHostName();

	remote_addr = new INETAddr(port, host)
;
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

      // Set the message
      record.msgData("hello world");

      try {

	// Get a transmission system from the socket
	OutputStream os = cli_stream.socket().getOutputStream();
	DataOutputStream dos = new DataOutputStream(os);

	// Send it
	record.streamOutTo(dos);

	// Close the socket
	cli_stream.close();

      } catch (IOException ie) {

	ACE.ERROR("IOException, loop: " + ie);
	return;
      }
    }
};


      

	

	

	
      

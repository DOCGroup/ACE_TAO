/*************************************************
 *
 * = PACKAGE
 *    JACE.SOCK_SAP
 *
 * = FILENAME
 *    SOCKConnector.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.SOCK_SAP;

import java.io.*;
import java.net.*;
import JACE.OS.*;

/**
 *     Defines an active connection factory for the socket wrappers. 
 */
public class SOCKConnector
{
  // = Initialization

  /**
   * Create a SOCKConnector. Do nothing constructor. Allows user to
   * call connect() later.
   */
  public SOCKConnector ()
    {
      // Do nothing constructor
    }

  /**
   * Create a SOCKConnector and connect to the server.
   *@param sockStream SOCK Stream to use for the connection
   *@param hostname hostname of the server
   *@param port port number to connect with server at
   */
  public SOCKConnector (SOCKStream sockStream,
			 String hostname,
			 int port) throws SocketException, IOException
    {
      this.connect (sockStream,
		    hostname,
		    port);
    }

  /**
   * Connect to the server.
   *@param sockStream SOCK Stream to use for the connection
   *@param hostname hostname of the server
   *@param port port number to connect with server at
   */
  public void connect (SOCKStream sockStream,
		       String hostname,
		       int port) throws SocketException, IOException
    {
      sockStream.socket (new Socket (hostname, port));
    }

  /**
   * Connect to the server.
   *@param sockStream SOCK Stream to use for the connection
   *@param addr INETAddr instance specifying host/port
   */
  public void connect (SOCKStream sockStream,
		       INETAddr addr) throws SocketException, IOException
    {
      sockStream.socket (new Socket (addr.getHostName(),
				     addr.getPortNumber()));
    }
}

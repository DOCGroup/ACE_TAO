/*************************************************
 *
 * = PACKAGE
 *    ACE.Connection
 *
 * = FILENAME
 *    AcceptStrategy.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.Connection;

import java.io.*;
import java.net.*;
import ACE.SOCK_SAP.*;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 * Interface for specifying a passive connection
 * acceptance strategy for a 
 * <a href="ACE.Connection.SvcHandler.html"><tt>SvcHandler</tt></a>
 * .
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *
 *<blockquote>
 * This class provides a strategy that manages passive
 * connection setup for an application, and can be extended
 * to define new strategies.
 *</blockquote>
 *
 * @see SvcHandler
 * @see Acceptor
 */

public class AcceptStrategy
{
  /**
   * Create an instance of Accept Strategy.
   *@param port port number where the server will listen for connections
   *@exception IOException couldn't open port
   */
  AcceptStrategy (int port) throws IOException
  {
    this.open (port);
  }

  /**
   * Initialize AcceptStrategy.
   *@param port port number where the server will listen for connections
   *@exception IOException couldn't open port
   */
  public void open (int port) throws IOException
  {
    // Create a new SOCK_Acceptor to accept client connections
    this.sockAcceptor_ = new SOCKAcceptor (port);
  }

  /**
   * Accept connections into the SvcHandler. Note that subclasses
   * should overwrite this method to provide a different accept
   * strategy.
   *@param sh Svc Handler in which to accept the connection
   *@exception SocketException
   *@exception IOException
   *@return 0
   */
  public int acceptSvcHandler (SvcHandler sh) throws
  SocketException, IOException
    {
      // Create a new stream
      SOCKStream sockStream = new SOCKStream ();
      
      // Block in accept. Returns when a connection shows up
      this.sockAcceptor_.accept (sockStream);
      
      // Set the streams for the new handler
      sh.setHandle (sockStream);
      return 0;
    }

  // Our connection acceptance factory
  private SOCKAcceptor sockAcceptor_;

}

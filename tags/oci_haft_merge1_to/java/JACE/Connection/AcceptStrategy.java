/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    AcceptStrategy.java
 *
 *@author Prashant Jain
 *@author Everett Anderson
 *
 *************************************************/
package JACE.Connection;

import java.io.*;
import java.net.*;
import JACE.SOCK_SAP.*;

/**
 * Interface for specifying a passive connection
 * acceptance strategy for a 
 * <a href="ACE.Connection.SvcHandler.html"><tt>SvcHandler</tt></a>
 * .
 * <P>
 * This class provides a strategy that manages passive
 * connection setup for an application, and can be extended
 * to define new strategies.
 * <P>
 *
 * The default implementation delegates to a generic Acceptor.
 *
 *@see SvcHandler
 *@see Acceptor
 */
public class AcceptStrategy
{
  /**
   * Create an instance of AcceptStrategy that delegates to the given
   * Acceptor.
   *@param port port number where the server will listen for connections
   *@param peer Acceptor instance to delegate to
   */
  AcceptStrategy (int port, Acceptor peer) throws IOException
  {
    this.acceptor_ = peer;
    this.open (port);
  }

  /**
   * Create an instance of Accept Strategy that delegates to Acceptor.
   *@param port port number where the server will listen for connections
   *@exception IOException couldn't open port
   */
  AcceptStrategy (int port) throws IOException
  {
    this.acceptor_ = new Acceptor ();
    this.open (port);
  }

  /**
   * Initialize AcceptStrategy.
   *@param port port number where the server will listen for connections
   *@exception IOException couldn't open port
   */
  public void open (int port) throws IOException
  {
    this.acceptor_.open (port);
  }

  /**
   * Accept connections into the SvcHandler. Note that subclasses
   * should overwrite this method to provide a different accept
   * strategy.
   *@param sh Svc Handler in which to accept the connection
   *@exception SocketException Socket error
   *@exception IOException Socket error
   *@return 0
   */
  public int acceptSvcHandler (SvcHandler sh) throws
  SocketException, IOException
    {
      return this.acceptor_.acceptSvcHandler (sh);
    }

  // The Acceptor we delegate to (if any)
  private Acceptor acceptor_;
}

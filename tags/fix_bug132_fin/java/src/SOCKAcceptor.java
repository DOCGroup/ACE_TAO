/*************************************************
 *
 * = PACKAGE
 *    JACE.SOCK_SAP
 *
 * = FILENAME
 *    SOCKAcceptor.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.SOCK_SAP;

import java.io.*;
import java.net.*;
import JACE.OS.*;

/**
 * <hr>
 * <p><b>TITLE</b><br>
 *     Defines the format and interface for an ACE.SOCKAcceptor.
 */
public class SOCKAcceptor
{
  // = Initialization

  /**
   * Create a SOCKAcceptor. Do nothing constructor. Allows user to
   * call open() later and pass in the port number.
   */
  public SOCKAcceptor ()
    {
    }

  /**
   * Create a SOCKAcceptor.
   *@param port port number where the server will listen for connections
   */
  public SOCKAcceptor (int port) throws IOException
    {
      this.open (port);
    }

  /**
   * Create socket to listen for connections on.
   *@param port port number where the server will listen for connections
   */
  public void open(int port) throws IOException
    {
      // Close old socket (if there is one)
      this.close ();

      // Create a new server socket
      this.listenSocket_ = new ServerSocket (port);
      //      ACE.DEBUG ("Server listening on port " + port);
    }

  /**
   * Close the socket and do any necessary cleanup.
   */
  public void close () throws IOException
    {
      if (this.listenSocket_ != null)
	{
	  this.listenSocket_.close ();
	  this.listenSocket_ = null;
	}
    }

  /**
   * Accept a connection. The streams are set when the method returns.
   *@param sockStream SOCK Stream to use for the connection
   */
  public void accept (SOCKStream sockStream) throws SocketException, IOException
    {
      // Block in accept. Returns when a connection shows up and sets
      // the streams
      sockStream.socket (this.listenSocket_.accept ());
      ACE.DEBUG ("Accepted connection from " +
		 sockStream.socket ().getInetAddress ());
    }

  /**
   * Get the underlying listen socket.
   *@return the underlying listen socket
   */
  public ServerSocket listenSocket ()
  {
    return this.listenSocket_;
  }

  /**
   * Set the underlying listen socket.
   *@param s the underlying listen socket
   */
  public void listenSocket (ServerSocket s)
  {
    this.listenSocket_ = s;
  }

  /**
   * Clean up when the garbage collector gets run (if at all). Note
   * that there is no guarantee that finalize () will get called.
   *@exception Throwable (Probably IOException from the socket level)
   */
  protected void finalize () throws Throwable
    {
      super.finalize ();
      this.close ();
    }

  // Socket on which listen for connections (by default initialized to
  // null)
  private ServerSocket listenSocket_;
}

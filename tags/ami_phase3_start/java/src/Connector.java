/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    Connector.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Connection;

import java.io.*;
import java.net.*;
import JACE.OS.*;
import JACE.SOCK_SAP.*;
import JACE.ServiceConfigurator.*;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 * Abstract factory for connecting a
 * (<a href="JACE.Connection.SvcHandler.html"><tt>SvcHandler</tt></a>),
 * to an application.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 * Implements the basic strategy for actively establishing connections
 * with applications.  The <tt>Connector</tt> establishes the connection,
 * passing it on to a <tt>SvcHandler</tt> instance, and handing over
 * control to that instance.
 *<p>
 * TCP is the transport mechanism used, via
 * <a href="JACE.SOCK_SAP.SOCKConnector.html#_top_"><tt>SOCKConnector</tt></a>.
 *</blockquote>
 *
 *<h2>NOTES</h2>
 *<blockquote>
 * This class, as currently implemented, does not work like its C++ counterpart.
 * Future versions are expected to rectify this discrepancy.
 *</blockquote>
 *
 *@see SOCKConnector
 *@see SvcHandler
 */
public class Connector extends ServiceObject
{
  /**
   * Create a Connector. Do nothing constructor. Allows user to
   * call <a href="#open(java.lang.String)">open</a>() later.
   */
  public Connector ()
    {
    }

  /**
   * Create a Connector passing in server hostname and port
   * number, effectively shorthand for calling
   * <a href="#open(java.lang.String)">open</a>().
   *@param hostname server hostname
   *@param port server port number
   */
  public Connector (String hostname, int port)
    {
      this.open (hostname, port);
    }

  /**
   * Initialize the Connector passing in server hostname and port
   * number.  Note that no connection attempt is made.
   *@param hostname server hostname
   *@param port server port number
   */
  public void open (String hostname, int port)
    {
      this.hostname_ = hostname;
      this.port_ = port;
    }

  /**
   * Connect to the server.
   *@param sh Svc Handler to use to handle the connection
   *@exception UnknownHostException Bad host
   *@exception SocketException Socket error
   *@exception InstantiationException Couldn't create new SOCKConnector
   *@exception IllegalAccessException No strategy available
   *@exception IOException Socket error
   */
  public void connect (SvcHandler sh) throws UnknownHostException, 
      SocketException,
      InstantiationException, 
      IllegalAccessException, 
      IOException
  {
    // Make a connection using the appropriate Connection_Strategy
    this.connectSvcHandler (sh);

    // Activate the Svc_Handler using the appropriate Activation_Strategy
    this.activateSvcHandler (sh);
  }

  /**
   * Bridge method for making a new connection. The default behavior
   * creates a new SOCKConnector and then calls setHandle() on the
   * <SvcHandler> that was passed in. Subclasses can override this
   * strategy, if needed.
   *@param sh Svc Handler to use to handle the connection
   *@return 0
   *@exception SocketException Socket error
   *@exception IOException Socket error
   */
  protected int connectSvcHandler (SvcHandler sh) throws
  SocketException, IOException
  {
    // Create a new stream
    SOCKStream sockStream = new SOCKStream ();

    // Create a SOCK_Connector (note the constructor does the connect for us)
    this.sockConnector_ = new SOCKConnector (sockStream,
					       this.hostname_,
					       this.port_);
    ACE.DEBUG ("Connected to " +
	       sockStream.socket ().getInetAddress ());
    
    // Set the streams for the new handler
    sh.setHandle (sockStream);
    return 0;
  }
  
  /**
   * Bridge method for activating a <SvcHandler>.  The default
   * behavior of this method is to activate the <SvcHandler> by
   * calling its open() method (which allows the SVC_HANDLER to define
   * its own concurrency strategy).  However, subclasses can override
   * this strategy to do more sophisticated concurrency activations.
   *@param sh Svc Handler to activate
   *@return 0
   */
  protected int activateSvcHandler (SvcHandler sh)
    {
      sh.open (null);
      return 0;
    }

  
  // Port server is listening on
  private int port_;

  // Server hostname
  private String hostname_;

  // Our connection factory
  private SOCKConnector sockConnector_;
}

/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    Acceptor.java
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
 * <p><h2>SYNOPSIS</h2>
 *
 * <blockquote>Abstract factory for creating a service handler
 * (<a href="ACE.Connection.SvcHandler.html"><tt>SvcHandler</tt></a>),
 * accepting into the
 * <a href="ACE.Connection.SvcHandler.html"><tt>SvcHandler</tt></a>, and activating the
 * <a href="ACE.Connection.SvcHandler.html"><tt>SvcHandler</tt></a>.</blockquote>
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>Implements the basic strategy for passively establishing
 * connections with applications.  The <tt>Acceptor</tt>
 * is a factory for <tt>SvcHandler</tt> instances, and, by default
 * generates a new <tt>SvcHandler</tt> instance for each connection
 * esablished.</blockquote>
 *
 * <p>
 *
 * <blockquote> The user of this class <em>must</em> provide a
 * reference to a handler factory prior to calling <a
 * href="#accept()"><tt>accept</tt></a>, or an exception will be
 * thrown.  The handler factory is identified by the meta-class for
 * the <tt>SvcHandler</tt>, and is typically obtained by calling <a
 * href="java.lang.Class#classForName(java.lang.String)"><tt>Class.classForName("SvcHandler")</tt></a>.
 * </blockquote>
 *
 * <p>
 *
 * <blockquote> TCP is the transport mechanism used, via
 * <a href="ACE.SOCK_SAP.SOCKAcceptor.html#_top_"><tt>SOCKAcceptor</tt></a>,
 * <em>et.al.</em> The SvcHandler is instantiated with a concrete type
 * that performs the application-specific service. </blockquote>
 *
 * <h2>NOTES</h2>
 *
 * <blockquote> This class is not directly related to the
 * <tt>AcceptorStrategy</tt> class.</blockquote>
 *
 *
 * @see java.lang.Class,ACE.Connection.SvcHandler,ACE.SOCK_SAP.SOCKAcceptor */
public class Acceptor extends ServiceObject
{
  /**
   * Create an instance of Acceptor. Default constructor.  Note that if
   * an instance is created via this method, <tt>setHandlerFactory</tt>
   * must be called prior to using <tt>accept</tt>.
   *
   * @see ACE.Connection.Acceptor.setHandlerFactory
   */
  public Acceptor ()
  {
  }
  
  /**
   * Create an instance of Acceptor.
   *@param handlerFactory meta-class reference used to create
   * an instance of a SvcHandler when a connection is accepted
   * (typically obtained by calling <tt>Class.classForName</tt>).
   *
   *@see java.lang.Class.classForName
   */
  public Acceptor (Class handlerFactory)
  {
    this.handlerFactory_ = handlerFactory;
  }

  /**
   * Set the handler factory. This is provided to aid the default
   * no-arg constructor.
   *@param handlerFactory meta-class reference used to create
   * an instance of a SvcHandler when a connection is accepted
   * (typically obtained by calling <tt>Class.classForName</tt>).
   *
   *@see java.lang.Class.classForName
   */
  public void setHandlerFactory (Class handlerFactory)
  {
    this.handlerFactory_ = handlerFactory;
  }

  /**
   * Initialize the Acceptor.
   *@param port TCP port number where the Acceptor will listen for connections
   *@exception IOException socket level exception
   */
  public void open (int port) throws IOException
  {
    this.sockAcceptor_ = new SOCKAcceptor (port);
  }

  /**
   * Template method for accepting connections. Delegates operational
   * activities to the following bridge methods:
   * <ul>
   * <li><tt>makeSvcHandler</tt></li>
   * <li><tt>acceptSvcHandler</tt></li>
   * <li><tt>activateSvcHandler</tt></li>
   * </ul>
   *
   * <p>
   *
   * The method first obtains a <tt>SvcHandler</tt> via
   * <tt>makeSvcHandler</tt>, accepts the connection <q>into</q> the
   * handler using <tt>acceptSvcHandler</tt>, and finally turns over
   * control to the handler with <tt>activateSvcHandler</tt>.
   *
   *@exception SocketException socket level error
   *@exception InstantiationException <tt>makeSvcHandler</tt> failure
   *@exception IllegalAccessException <tt>makeSvcHandler</tt> failure
   *@exception IOException socket level error
   */
  public void accept () throws SocketException, 
    InstantiationException, 
    IllegalAccessException,
    IOException
  {

    // Create a Svc_Handler using the appropriate Creation_Strategy
    SvcHandler sh = this.makeSvcHandler ();

    // Accept a connection into the SvcHandler using the appropriate
    // Accept_Strategy 
    this.acceptSvcHandler (sh);

    // Activate the SvcHandler using the appropriate ActivationStrategy
    this.activateSvcHandler (sh);
  }

  /**
   * Bridge method for creating a <tt>SvcHandler</tt>. The default is to
   * create a new <SvcHandler>.  However, subclasses can override this
   * policy to perform <SvcHandler> creation in any way that they like
   * (such as creating subclass instances of <SvcHandler>, using a
   * singleton, etc.)
   *@return a new instance of the SvcHandler
   *@exception InstantiationException could not create new SvcHandler
   *@exception IllegalAccessException no SvcHandler factory provided
   */
  protected SvcHandler makeSvcHandler ()
        throws InstantiationException, IllegalAccessException
  {
    // Create a new handler for the connection
    return (SvcHandler) handlerFactory_.newInstance ();
  }

  /**
   * Bridge method for accepting the new connection into the
   * <tt>SvcHandler</tt>.  The default behavior delegates the work to
   * <tt>SOCKAcceptor.accept</tt>. However, subclasses can override this
   * strategy.
   *@param sh SvcHandler in which to accept the connection
   *@return 0
   *@exception SocketException socket level error
   *@exception IOException socket level error
   */
  protected int acceptSvcHandler (SvcHandler sh)
       throws SocketException, IOException
  {
    // Create a new stream
    SOCKStream sockStream = new SOCKStream ();

    // Block in accept. Returns when a connection shows up
    this.sockAcceptor_.accept (sockStream);

    // Set the streams for the new handler
    sh.setHandle (sockStream);
    return 0;
  }
  
  /**
   * Bridge method for activating a <tt>SvcHandler</tt>.  The default
   * behavior of this method is to activate the <tt>SvcHandler</tt> by
   * calling its open() method (which allows the <tt>SvcHandler</tt> to
   * define its own concurrency strategy).  However, subclasses can
   * override this strategy to do more sophisticated concurrency
   * activations.
   *@param sh SvcHandler to activate
   *@return 0
   */
  protected int activateSvcHandler (SvcHandler sh)
  {
    sh.open (null);
    return 0;
  }

  // Handler class that should be instantiated when a connection is
  // made with a client
  private Class handlerFactory_;

  // Our connection acceptance factory
  private SOCKAcceptor sockAcceptor_;
}

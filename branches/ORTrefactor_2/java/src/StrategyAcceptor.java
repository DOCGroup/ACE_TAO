/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    StrategyAcceptor.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Connection;

import java.io.*;
import java.net.*;
import JACE.OS.*;
import JACE.SOCK_SAP.*;

public class StrategyAcceptor
{
  /**
   * Create an instance of StrategyAcceptor.
   *@param handlerFactory Svc Handler factory that is used to create
   * an instance of a Svc Handler when a connection is accepted.
   */
  public StrategyAcceptor (Class handlerFactory)
    {
      this (handlerFactory, null, null, null);
    }

  /**
   * Create an instance of StrategyAcceptor. Use the creation
   * strategy and the handlerFactory passed in to creae a new instance
   * of the Svc Handler.
   *@param handlerFactory Svc Handler factory that is used to create
   * an instance of a Svc Handler when a connection is accepted.
   *@param creStrategy Creation strategy to use to create a new
   * instance of the Svc Handler.
   *@param acceptStrategy Accept strategy to use to accept a new
   * connection into the Svc Handler.
   *@param activateStrategy Activate strategy to use to activate the
   * instance of the Svc Handler.
   */
  public StrategyAcceptor (Class handlerFactory,
			    CreationStrategy creStrategy,
			    AcceptStrategy acceptStrategy,
			    ActivateStrategy activateStrategy)
    {
      // Cache everything
      this.handlerFactory_ = handlerFactory;
      this.creStrategy_ = creStrategy;
      this.acceptStrategy_ = acceptStrategy;
      this.activateStrategy_ = activateStrategy;
    }

  /**
   * Initialize the Strategy Acceptor. The method creates the
   * appropriate strategies as needed.
   *@param port port number where the server will listen for connections
   *@exception IOException Socket level error
   */
  public void open (int port) throws IOException
    {
      if (this.creStrategy_ == null)
	this.creStrategy_ = new CreationStrategy (this.handlerFactory_);
      if (this.acceptStrategy_ == null)
	this.acceptStrategy_ = new AcceptStrategy (port);
      else
	this.acceptStrategy_.open (port);
      if (this.activateStrategy_ == null)
	this.activateStrategy_ = new ActivateStrategy ();
    }

  /**
   * Accept a connection using the appropriate strategies.
   *
   *@exception SocketException Socket level error
   *@exception InstantiationException Problem creating a handler
   *@exception IllegalAccessException No strategy available
   *@exception IOException Socket level error
   */
  public void accept () throws SocketException, 
      InstantiationException, 
      IllegalAccessException,
      IOException
  {
    // Create a Svc_Handler using the appropriate Creation_Strategy
    SvcHandler sh = this.makeSvcHandler ();

    // Accept a connection into the Svc_Handler
    this.acceptSvcHandler (sh);

    // Activate the Svc_Handler
    this.activateSvcHandler (sh);
  }

  /**
   * Bridge method for creating a SvcHandler.  The strategy for
   * creating a SvcHandler is configured into the Acceptor via it's
   * creStrategy_.  If no strategy is passed in, the default behavior
   * of this method is to use the default CreationStrategy.
   *@return a new instance of the Svc Handler
   *@exception InstantiationException Couldn't create SvcHandler
   *@exception IllegalAccessException No strategy available
   */
  protected SvcHandler makeSvcHandler () throws InstantiationException, IllegalAccessException
    {
      // Create a new handler for the connection
      return this.creStrategy_.makeSvcHandler ();
    }


  /**
   * Bridge method for accepting the new connection into the
   * <SvcHandler>.  The strategy for accepting into a SvcHandler is
   * configured into the Acceptor via it's acceptStrategy_.  If no
   * strategy is passed in, the default behavior of this method is to
   * use the default AcceptStrategy. 
   *@param sh Svc Handler in which to accept the connection
   *@return result of accepting a connection using the accept strategy
   *@exception SocketException Socket level error
   *@exception IOException Socket level error
   */
  protected int acceptSvcHandler (SvcHandler sh) throws SocketException, IOException
  {
    // Delegate responsibility to the appropriate strategy
    return this.acceptStrategy_.acceptSvcHandler (sh);
  }
  
  /**
   * Bridge method for activating a <SvcHandler>.  The strategy for
   * activating a SvcHandler is configured into the Acceptor via it's
   * activateStrategy_. If no strategy is passed in, the default
   * behavior of this method is to use the default ActivateStrategy.
   *@param sh Svc Handler to activate
   *@return result of activating the Svc Handler
   */
  protected int activateSvcHandler (SvcHandler sh)
    {
      // Delegate responsibility to the appropriate strategy
      return this.activateStrategy_.activateSvcHandler (sh);
    }

  // Handler class that should be instantiated when a connection is
  // made with a client
  private Class handlerFactory_;

  // Creation Strategy
  private CreationStrategy creStrategy_;

  // Accept Strategy
  private AcceptStrategy acceptStrategy_;

  // Activation Strategy
  private ActivateStrategy activateStrategy_;
}

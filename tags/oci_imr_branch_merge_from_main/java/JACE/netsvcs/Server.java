package JACE.netsvcs;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Misc.GetOpt;
import JACE.ServiceConfigurator.Service;

/**
 * Abstract class providing default implementations for several
 * Service methods.  Currently, all the network services
 * descend from this class.  The real work for a service is done
 * by a Handler.  
 * <P>
 * Inner classes are provided for thread per connection
 * and single threaded server activation strategies.  Currently,
 * specifying a single threaded strategy means that the server will
 * disconnect the client after handling one request.  Acceptor and
 * EventHandler may be changed later to incorporate handleInput to
 * address this.  Thus, the default activation strategy is thread
 * per connection.
 *
 *@see Handler
 *@see JACE.Connection.ActivateStrategy
 *@author Everett Anderson
 */
public abstract class Server extends Acceptor implements Runnable
{
  /**
   * Safely shuts down all the handlers as well as the accepting socket.
   *
   *@return -1 on failure, 0 on success
   */
  public synchronized int fini ()
  {
    if (!done ()) {
      ACE.DEBUG ("Shutting down " + name ());
      try {
	this.done_ = true;
	for (int i = handlers_.size () - 1; i >= 0; i--)
	  ((Handler)handlers_.elementAt (i)).close ();

	this.sockAcceptor_.close();
      } catch (IOException e) {
	ACE.ERROR(e);
	return -1;
      }
    }      
    
    return 0;
  }

  /**
   * Returns information about the state of the service such as
   * suspended, not running, or running.
   */
  public String info ()
  {
    if (suspended ())
      return "suspended";
    else
    if (done ())
      return "not running";
    else
      return "running on port " + port_;
  }
  
  /**
   * Provided for extra initialization in subclasses after the
   * command line arguments have been parsed but before starting the
   * service.  This is a good place to set the default ActivateStrategy
   * since you can make sure it wasn't set in parseArgs.  The default
   * implementation sets the strategy to Server.ThreadPerConnection.
   *
   *@return -1 on error, 0 on success
   */
  protected int initialize ()
  {
    if (activateStrategy_ == null)
      activateStrategy (new Server.ThreadPerConnection ());

    return 0;
  }

  /**
   * Template method for initialization.  Calls parseArgs, initialize,
   * sets the done() state to false, and starts this Server in its own
   * thread.  If parseArgs fails, this calls printUsage.
   *
   *@param args command line arguments
   *@return -1 on failure, 0 on success
   */
  public int init (String [] args)
  {
    // Parse arguments
    if (this.parseArgs (args) == -1) {
      printUsage ();
      return -1;
    }

    if (initialize () < 0) {
      ACE.ERROR (name () + " failed initializing");
      return -1;
    }

    ACE.DEBUG ("Using " + activateStrategy_.getClass().getName ());
    ACE.DEBUG ("Starting " + name () + " on port: " + this.port_);

    done_ = false;

    // Run in own thread of control so that we don't block the caller
    new Thread (this).start();
    return 0;
  }

  /**
   * Called by the JVM when this Server starts running in its own
   * thread.
   */
  public void run ()
  {
    try {
      this.open (this.port_);
      while (!this.done ()) {
	this.accept ();
      }
    } catch (InstantiationException e) {
      ACE.ERROR (e);
    } catch (IllegalAccessException e) {
      ACE.ERROR (e);
    } catch (IOException e) {
      if (!done ())
	ACE.ERROR (e);
    } finally {
      fini ();
    }
  }

  /**
   * Calls the appropriate activation strategy with the given
   * service handler.  This assumes the SvcHandler is an instance
   * of type Handler, and sets its parent accordingly.
   *
   *@param sh SvcHandler (assumed to be a Handler) to activate
   *@return -1 on failure, 0 on success
   */
  protected int activateSvcHandler (SvcHandler sh)
  {
    if (done ())
      return -1;
    
    addHandler (sh);
    ((Handler)sh).parent (this);

    while (suspended () && !done ())
      Thread.yield ();

    if (activateStrategy_.activateSvcHandler (sh) != 0) {
      removeHandler (sh);
      return -1;
    }

    return 0;
  }
  
  /**
   * Add the given SvcHandler to this Servers list of handlers.
   * @param sh service handler to add (assumed to be a Handler)
   */
  protected void addHandler (SvcHandler sh)
  {
    handlers_.addElement (sh);
  }

  /**
   * Called by Handler instances during their close () method.
   *@param sh service handler to remove
   */
  public void removeHandler (SvcHandler sh)
  {
    handlers_.removeElement (sh);
  }
  
  /**
   * Parses the command line arguments.  Subclasses must override
   * this.
   *
   *@param args command line arguments
   *@return -1 on failure, 0 on success
   */
  protected abstract int parseArgs (String [] args);

  /**
   * Create the appropriate Handler.  Subclasses must override this,
   * returning a new instance of the proper subclass of Handler.
   *
   *@return new Handler instance
   */
  protected abstract SvcHandler makeSvcHandler ();

  /**
   * Print out the correct syntax and meaning of the command line
   * arguments.
   */
  protected abstract void printUsage ();

  /**
   * Set the ActivateStrategy for handlers.
   *
   *@param strategy new ActivateStrategy to use
   *@see JACE.Connection.ActivateStrategy
   */
  protected void activateStrategy (ActivateStrategy strategy)
  {
    activateStrategy_ = strategy;
  }

  /**
   * Return the current ActivateStrategy for handlers.
   *
   *@return current ActivateStrategy instance
   */
  protected ActivateStrategy activateStrategy ()
  {
    return activateStrategy_;
  }

  /**
   * Check to see if this Server has been shut down.
   */
  protected synchronized boolean done ()
  {
    return done_;
  }

  /**
   * Useful method for subclasses when parsing the port command
   * line option.  
   *
   *@param port String gathered from the command line representing the port
   *@return false if there was an error, true if successful
   */
  protected boolean port (String port)
  {
    try {
      
      this.port_ = Integer.parseInt (port);

    } catch (NumberFormatException e) {
      ACE.ERROR("Invalid port specified: " + e.getMessage ());
      return false;
    } catch (ArrayIndexOutOfBoundsException e) {
      ACE.ERROR("Port option requires an argument");
      return false;
    }

    return true;
  }

  /**
   * Useful method for subclasses when trying to load and instantiate
   * a certain class from a command line argument.  This can be used
   * when a possible command line argument is what kind of activation
   * strategy is used for handlers.
   *
   *@param classname name of the class to load and create an instance of
   *@param descrption descrption of what type of class it is
   *@return null if failed loading, a new instance of the class on success
   */
  protected Object newStrategyInstance (String classname,
					String description)
  {
    try {
      Class factory = Class.forName (classname);

      return factory.newInstance ();
    
    } catch (ClassNotFoundException e) {
      ACE.ERROR("Unable to find " + description + ": " 
		+ e.getMessage ());
    } catch (InstantiationException e) {
      ACE.ERROR ("Instantiating " + description + ": " 
		 + e.getMessage ());
    } catch (IllegalAccessException e) {
      ACE.ERROR ("Illegal access on " + description + ": " 
		 + e.getMessage ());
    }    

    return null;
  }

  /**
   * Shuts down the Server if it wasn't already done
   */
  protected void finalize () throws Throwable
  {
    fini ();
  }

  private boolean done_ = true;

  /**
   * List of currently active Handlers
   */
  protected Vector handlers_ = new Vector ();
  private ActivateStrategy activateStrategy_ = null;

  /**
   * Activation strategy in which each Handler is run in its own
   * Thread.
   */
  public static class ThreadPerConnection extends ActivateStrategy
  {
    /**
     * Opens the given service handler, and runs it in its own
     * Thread.
     *@param sh service handler to activate
     *@return -1 on failure, 0 on success
     */
    public int activateSvcHandler (SvcHandler sh)
    {
      if (sh.open (null) < 0)
	return -1;

      new Thread (sh).start ();
      return 0;
    }
  }

  /**
   * Activation strategy in which all Handlers are run in the
   * Server Thread in sequence.  This assumes that the given
   * SvcHandler is a Handler instance. 
   */
  public static class SingleThreaded extends ActivateStrategy
  {
    /**
     * Opens the given service handler, calls Handler.handleRequest, and
     * then Handler.close before returning.
     *@param sh service handler to activate (assumed to be a Handler)
     *@return -1 on failure, 0 on success
     */
    public int activateSvcHandler (SvcHandler sh)
    {
      if (sh.open (null) < 0)
	return -1;

      ((Handler)sh).handleRequest ();
      ((Handler)sh).close ();

      return 0;
    }
  }
}

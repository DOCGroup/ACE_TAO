package JACE.netsvcs;

import java.io.*;
import java.net.*;
import JACE.OS.*;
import JACE.Connection.*;

/**
 * Abstract class representing a handler for a Server.  Provides
 * default implementations and template methods.
 *
 *@see Server
 *@author Everett Anderson
 */
public abstract class Handler extends SvcHandler
{
  /**
   * Initialize this Handler.  The default implementation sets the
   * done flag to false.
   *
   *@return -1 on failure, 0 on success
   */
  public int open (Object obj)
  {
    done_ = false;
    return 0;
  }

  /**
   * Shut down this handler.  Default implementation calls close ().
   */
  public int close (long flags)
  {
    return close ();
  }

  /**
   * Shut down this handler, setting the done flag, and removing it
   * from the parent Server's handler list.
   */
  public synchronized int close ()
  {
    if (!done ()) {
      try {
	done_ = true;
	parent_.removeHandler (this);
	peer ().close ();
      } catch (IOException e) {
	return -1;
      }
    }

    return 0;
  }

  /**
   * Returns the name of the host that is connected to this handler,
   * or null if not connected.
   */
  public String hostName ()
  {
    if (done ())
      return null;
    else 
      return this.peer().socket().getInetAddress().getHostName();
  }

  /**
   * Process a single request and handle any errors.  The default
   * implementation calls handleRequest with an Object from
   * newRequest ().
   */
  public void handleRequest ()
  {
    handleRequest (newRequest ());
  }

  /**
   * Process a single request and handle any errors.  The default
   * implementation calls processRequest with the given request
   * Object, and then handles exceptions appropriately.  Subclasses
   * normally just implement processRequest rather than override
   * this method.
   *
   *@param request request to process
   */
  public void handleRequest (Object request)
  {
    try {

      processRequest (request);

    } catch (NullPointerException e) {
      if (!done ()) {
	ACE.ERROR("Failure: " + e);
	close ();
      }
    } catch (SocketException e) {
      if (!done ()) {
	ACE.DEBUG (hostName () + " disconnected");
	close ();
      }
    } catch (EOFException e) {
      if (!done ()) {
	ACE.DEBUG (hostName () + " disconnected");
	close ();
      }
    } catch (IOException e) {
      if (!done ()) {
	ACE.ERROR ("Lost connection: " + e);
	close ();
      }
    }
  }

  /**
   * Process a single request (including reading it from the wire) 
   * without handling errors.  Subclasses must define the behavior.
   *
   *@param requestObject request to process
   *@exception SocketException problem with the socket
   *@exception EOFException end of connection, 
   *           usually means client disconnected
   *@exception IOException error during transmission
   */
  protected abstract void processRequest (Object requestObject) 
    throws SocketException, EOFException, IOException;

  /**
   * Returns a new instance of a request object.  Subclasses must
   * define the behavior.
   */
  public abstract Object newRequest ();
  
  /**
   * Called by the JVM when a Handler is run in its own Thread.  The
   * default implementation creates a single request object which is
   * reused during multiple handleRequest calls.  The loop exits
   * when the Handler's done() method returns true.
   */
  public void run()
  {
    Object request = newRequest ();

    while (!done ()) 
      handleRequest (request);

    close ();
  }

  /**
   * Set the Server parent of this Handler.
   */
  public void parent (Server parent)
  {
    parent_ = parent;
  }

  /**
   * Return the Server parent of this Handler.
   */
  public Server parent ()
  {
    return parent_;
  }

  /**
   * Check to see if this Handler should shut down.
   */
  protected synchronized boolean done ()
  {
    return done_;
  }

  /**
   * Closes the handler, freeing resources.
   */
  protected void finalize () throws Throwable
  {
    close ();
  }

  private boolean done_ = true;
  private Server parent_;
}

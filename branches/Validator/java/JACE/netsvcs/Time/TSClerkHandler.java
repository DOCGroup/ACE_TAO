package JACE.netsvcs.Time;

import java.io.*;
import java.net.*;

import JACE.OS.*;
import JACE.Connection.*;
import JACE.ASX.*;

/**
 * Requests a time update from a time server.  This is used by the
 * TSClerkProcessor to query a server.
 */
public class TSClerkHandler extends SvcHandler
{
  /**
   * Constructor.
   *
   *@param parent TSClerkProcessor which is creating this instance
   *@param host name of the machine this handler is connected to
   *@param port port on the time server to connect to
   */
  public TSClerkHandler (TSClerkProcessor parent,
			 String host,
			 int port)
  {
    parent_ = parent;
    host_ = host;
    port_ = port;
  }

  /**
   * Initialize this handler.  Called automatically by Connector when a
   * successful connection is made.
   *
   *@return -1 on failure, 0 on success
   */
  public int open (Object obj)
  {
    ACE.DEBUG ("Successful connection to " + host ());
    connected (true);
    return 0;
  }

  /**
   * Safely shut down this handler, closing the socket.
   *
   *@return -1 on failure, 0 on success
   */
  public synchronized int close ()
  {
    if (!connected ())
      return 0;

    ACE.DEBUG ("Shutting down connection to " + host ());
    try {
      peer ().close ();
    } catch (IOException e) {
      return -1;
    } finally {
      connected (false);
    }

    return 0;
  }

  /**
   * Accessor for the port number of the server.
   */
  public int port ()
  {
    return port_;
  }

  /**
   * Accessor for the host name of the server.
   */
  public String host ()
  {
    return host_;
  }

  /**
   * Check to see if this handler is currently connected to a server.
   */
  public synchronized boolean connected ()
  {
    return connected_;
  }

  /**
   * Set the connected state.
   *
   *@param state true if connected, false if not
   */
  protected synchronized void connected (boolean state)
  {
    connected_ = state;
  }

  /**
   * (Isn't used, just fulfills the interface.  Returns -1 by
   * default)
   */
  public int handleTimeout (TimeValue tv, Object obj)
  {
    return -1;
  }

  /**
   * (Isn't used, just fulfills the interface.)
   */
  public void run ()
  {
    ACE.ERROR ("TSClerkHandler is not setup to run in its own thread");
  }

  /**
   * Sends a request to the server and waits for a reply.  This is called
   * by TSClerkProcessor.
   *
   *@return -1 on failure, 0 on success
   */
  public int sendRequest ()
  {
    // Ask the clerk processor to connect this handler if it isn't
    // already.  Thus, it tries to reconnect if the server has gone
    // down.
    if (!connected ()) 
      parent_.connectHandler (this, host_, port_);

    TimeRequest request = new TimeRequest ();

    long start, stop;
    try {

      start = System.currentTimeMillis ();
      request.streamOutTo (peer().outputStream ());
      
      request.streamInFrom (peer().inputStream ());
      stop = System.currentTimeMillis ();

    } catch (NullPointerException e) {
      close ();
      return -1;
    } catch (IOException e) {
      close ();
      return -1;
    }

    // Compute the difference in the local time and the server time
    // (in seconds)   
    long difference = request.time () - (stop / 1000);

    // Calculate the transmission time (in seconds)
    long oneWayTime = (stop - start) / 2000;

    difference += oneWayTime;

    /*
    ACE.DEBUG (host() + " reports:");
    ACE.DEBUG ("    time difference: " + difference);
    ACE.DEBUG ("    trans. delay: " + oneWayTime);
    */

    // Set the time difference for this handler
    delta (difference);

    return 0;
  }

  /**
   * Returns the current time difference between local time and
   * the server (in seconds).
   */
  public synchronized long delta ()
  {
    return delta_;
  }

  /**
   * Sets the current time difference between local time and the
   * server (in seconds).
   */
  protected synchronized void delta (long delta)
  {
    delta_ = delta;
  }

  private long delta_;
  private TSClerkProcessor parent_;
  private boolean connected_ = false;
  private String host_;
  private int port_;
}

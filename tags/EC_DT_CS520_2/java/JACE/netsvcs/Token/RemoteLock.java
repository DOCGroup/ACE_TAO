package JACE.netsvcs.Token;

import java.io.*;
import JACE.Concurrency.*;
import JACE.ASX.*;
import JACE.Connection.*;
import JACE.OS.*;

/**
 * Proxy used by clients to connect to the token service.  This 
 * implements the AbstractLock interface, so can be used like any
 * other synchronization mechanism.  The user can either use this
 * class directly, or use a proxy which already inputs its type.
 * <P>
 * Currently, a separate instance (and thus a separate socket connection)
 * must be used for each thread which accesses the service.  The token
 * service itself could handle multiple client IDs and token names per
 * connection with the following requirement -- since the service blocks
 * in its operations, a shadow mutex would have to be used in the proxy.
 * <P>
 * It would be best if the user called the close () method after finishing
 * up with a RemoteLock, but that is not absolutely necessary.  The socket
 * will be closed when the JVM exits or finalize is called.  (That will also
 * free the actual token in the token service in case release was never
 * called.)
 * <P>
 * The SLEEPHOOK result is never returned, only SUCCESS or FAILURE.  (C++
 * version doesn't seem to indicate the sleep hook result.)
 *
 *@see MutexHandler
 *@see RWMutexHandler
 *@see JACE.Concurrency.AbstractLock
 *
 *@author Everett Anderson
 */
public class RemoteLock extends SvcHandler implements AbstractLock
{
  /**
   * Accessor for the token name. 
   *
   *@return name of the token
   */
  public String tokenName ()
  {
    return request_.tokenName ();
  }

  /**
   * Set the name of the token.
   */
  public void tokenName (String name)
  {
    request_.tokenName (name);
  }

  /**
   * Accessor for the client ID.
   */
  public String clientID ()
  {
    return request_.clientID ();
  }

  /**
   * Set the client ID.
   */
  public void clientID (String clientID)
  {
    request_.clientID (clientID);
  }

  /**
   * Constructor.
   *
   *@see LockTypes
   *@param tokenType  type of token to create in the token service
   *@param proxyType  type of proxy to define this RemoteLock as
   *@param tokenName  name of the token to connect to in the token service
   *@param clientID   clientID to use to refer to this client
   *@param host       host name of the token service
   *@param port       port to connect to for the token service
   */
  public RemoteLock (int tokenType,
		     int proxyType,
		     String tokenName,
		     String clientID,
		     String host,
		     int port)
  {
    host_ = host;
    port_ = port;

    // Only allocates one reply and one request
    reply_ = new TokenReply ();

    request_ = new TokenRequest (tokenType,
				 proxyType,
				 0,
				 tokenName,
				 clientID);
  }

  /**
   * Reconnect this proxy to the token service. 
   *
   *@exception LockException problem occured in reconnecting
   */
  protected void reconnect () throws LockException
  {
    Connector c = new Connector ();
    c.open (host_, port_);

    try {
      c.connect (this);
    } catch (InstantiationException e) {
      throw new LockException (e.getMessage());
    } catch (IllegalAccessException e) {
      throw new LockException (e.getMessage());
    } catch (IOException e) {
      throw new LockException (e.getMessage());
    }
  }

  /**
   * Check to see if this RemoteLock is connected.
   */
  public boolean connected ()
  {
    return connected_;
  }

  /**
   * Initialize this RemoteLock.  Called by Connector.
   */
  public int open (Object obj)
  {
    connected_ = true;
    return 0;
  }

  /**
   * Shut down the connection to the server.  Current implementation
   * calls close ().
   */
  public int close (long flags)
  {
    return close ();
  }

  /**
   * Shut down the connection to the server and mark this lock
   * as disconnected.
   */
  public int close ()
  {
    if (connected ()) {
      try {
	connected_ = false;
	peer ().close ();
      } catch (IOException e) {
	return -1;
      }
    }
    
    return 0;
  }

  /**
   * Send the given request to the token service, throwing a 
   * LockException on error.
   */
  protected void sendRequest (TokenRequest request) throws LockException
  {
    try {
      if (!connected ())
	reconnect ();

      request.streamOutTo (peer ().dataOutputStream ());

    } catch (IOException e) {
      close ();
      throw new LockException (e.getMessage ());
    }
  }

  /**
   * Receive a reply from the token service, throwing a LockException
   * on error.
   */
  protected void receiveReply (TokenReply reply) throws LockException
  {
    if (!connected ())
      throw new LockException ("Proxy wasn't connected, any replies lost");

    try {

      reply.streamInFrom (peer ().dataInputStream ());

    } catch (IOException e) {
      close ();
      throw new LockException (e.getMessage ());
    }
  }

  /**
   * For errors that shouldn't generate exceptions, return the
   * appropriate result code as defined in AbstractLock.
   *
   *@return AbstractLock.SUCCESS or AbstractLock.FAILURE
   */
  protected int processErrno (TokenReply reply)
  {
    switch (reply.errno ())
      {
      case TokenReply.NO_ERRORS:
	return AbstractLock.SUCCESS;
      case TokenReply.EIO:
	close ();
	return AbstractLock.FAILURE;
      default:
	return AbstractLock.FAILURE;
      }
  }

  /**
   * Make a request to the token service with the given operation
   * type and arguments.
   *
   *@see LockOperations
   *@see LockTypes
   *@param operationType type of operation to perform
   *@param proxyType type of proxy this is
   *@param requeuePosition put this owner at this position in the
   *                       waiting queue (only makes sense if the
   *                       operation is renew)
   *@return AbstractLock.SUCCESS or AbstractLock.FAILURE
   *@exception LockException remote access error occured
   */
  protected int makeRequest (int operationType,
			     int proxyType,
			     int requeuePosition)
    throws LockException
  {
    request_.operationType (operationType);
    request_.proxyType (proxyType);
    request_.requeuePosition (requeuePosition);
    request_.useTimeout (false);

    sendRequest (request_);
    receiveReply (reply_);
    
    // make sure that if someone does send a magic cookie arg back,
    // to keep it going
    request_.arg (reply_.arg ());

    return processErrno (reply_);
  }

  /**
   * Make a request to the token service with the given arguments
   * that must be performed by the given absolute time timeout.
   * Currently, the timeout is managed by the remote service.
   *
   *@see LockOperations
   *@see LockTypes
   *@param operationType type of operation to perform
   *@param proxyType type of proxy this is
   *@param requeuePosition put this owner at this position in the
   *                       waiting queue (only makes sense if the
   *                       operation is renew)
   *@param timeout absolute time timeout to accomplish the operation by
   *@return AbstractLock.SUCCESS or AbstractLock.FAILURE
   *@exception LockException remote access error occured
   */
  protected int makeRequest (int operationType,
			     int proxyType,
			     int requeuePosition,
			     TimeValue timeout)
    throws LockException, TimeoutException
  {
    request_.operationType (operationType);
    request_.proxyType (proxyType);
    request_.requeuePosition (requeuePosition);
    request_.useTimeout (timeout);

    sendRequest (request_);
    receiveReply (reply_);

    request_.arg (reply_.arg ());

    if (reply_.errno () == TokenReply.ETIME)
      throw new TimeoutException (timeout, "Remote Lock");

    return processErrno (reply_);
  }

  /**
   * Acquire ownership of the lock, blocking indefinitely if necessary.  
   * <P>
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException a remote error occured
   */
  public int acquire () throws LockException
  {
    return makeRequest (LockOperations.ACQUIRE, 0, 0);
  }

  /**
   * Acquire ownership of the lock by the given absolute time time-out.
   * A value of null for the timeout parameter results in a blocking
   * acquire.
   * A value of TimeValue.zero throws a TimeoutException if the
   * acquire would block.
   * <P>
   *@param timeout  absolute time by which the lock must be acquired
   *@return         appropriate Lock return value (AbstractLock.FAILURE, 
   *                AbstractLock.SUCCESS or AbstractLock.SLEEPHOOK)
   *@exception      LockException a remote error occured
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@see #tryAcquire
   */
  public int acquire (TimeValue timeout)
    throws LockException, TimeoutException
  {
    return makeRequest (LockOperations.ACQUIRE, 0, 0, timeout);
  }
    
  /**
   * Acquire a read lock, blocking indefinitely if necessary. 
   *
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException a remote error occured
   */
  public int acquireRead () throws LockException
  {
    return makeRequest (LockOperations.ACQUIRE,
			LockTypes.READ_LOCK_PROXY,
			0);
  }

  /**
   * Acquire a read lock by the given absolute time time-out.  
   *
   *@param timeout  absolute time by which the lock must be acquired
   *@return         appropriate lock return value (AbstractLock.FAILURE, 
   *                AbstractLock.SUCCESS or AbstractLock.SLEEPHOOK)
   *@exception      LockException a remote error occured
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@see #tryAcquireRead
   */
  public int acquireRead (TimeValue timeout) 
    throws LockException, TimeoutException
  {
    return makeRequest (LockOperations.ACQUIRE,
			LockTypes.READ_LOCK_PROXY,
			0,
			timeout);
  }

  /**
   * Acquire a write lock, blocking indefinitely if necessary. 
   *
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException a remote error occured
   */
  public int acquireWrite () 
    throws LockException
  {
    return makeRequest (LockOperations.ACQUIRE,
			LockTypes.WRITE_LOCK_PROXY,
			0);
  }

  /**
   * Acquire a write lock by the given absolute time time-out.  
   *
   *@param timeout  absolute time by which the lock must be acquired
   *@return         appropriate lock return value (AbstractLock.FAILURE, 
   *                AbstractLock.SUCCESS or AbstractLock.SLEEPHOOK)
   *@exception      LockException a remote error occured
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   *@see #tryAcquireWrite
   */
  public int acquireWrite (TimeValue timeout) 
    throws LockException, TimeoutException
  {
    return makeRequest (LockOperations.ACQUIRE,
			LockTypes.WRITE_LOCK_PROXY,
			0,
			timeout);
  }

  
  /**
   * Give up the lock to some number of waiting threads (if any), then 
   * reacquire, blocking indefinitely if necessary.
   * <P>
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token. 
   * <P>
   *@param     requeuePosition position in the waiters queue to insert
   *           this thread.  If this value is -1 and there are other
   *           threads waiting to obtain the token, this thread is queued
   *           at the end.  If this value is greater than -1, then it
   *           indicates how many entries to skip over before inserting
   *           our thread into the queue.  (For example, if it is 0,
   *           this thread is put at the front of the queue.)  If this 
   *           value is greater than the number of waiters, this thread is
   *           simply put at the end of the current waiters queue.
   *@return    AbstractLock.FAILURE or AbstractLock.SUCCESS
   *@exception LockException a remote error occured
   */
  public int renew (int requeuePosition) 
    throws LockException 
  {
    return makeRequest (LockOperations.RENEW,
			0,
			requeuePosition);
  }

  /**
   * Give up the lock to some waiting threads (if any), then reacquire
   * by the given absolute time time-out.
   * <P>
   * An optimized method that efficiently reacquires the token if no
   * other threads are waiting.  This is useful for situations where
   * you don't want to degrade the quality of service if there are
   * other threads waiting to get the token. 
   * <P>
   * A value of null for the timeout indicates a blocking renew.
   * <P>
   *@param     requeuePosition position in the waiters queue to insert
   *           this thread.  If this value is -1 and there are other
   *           threads waiting to obtain the token, this thread is queued
   *           at the end.  If this value is greater than -1, then it
   *           indicates how many entries to skip over before inserting
   *           our thread into the queue.  (For example, if it is 0,
   *           this thread is put at the front of the queue.)  If this 
   *           value is greater than the number of waiters, this thread is
   *           simply put at the end of the current waiters queue.
   * 
   *@param     timeout absolute time by which the lock must be reacquired
   *
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException a remote error occured
   *@exception      JACE.ASX.TimeoutException thrown when the lock is not
   *                obtained by the desired time
   */
  public int renew (int requeuePosition, TimeValue timeout) 
    throws LockException, TimeoutException
  {
    return makeRequest (LockOperations.RENEW,
			0,
			requeuePosition,
			timeout);
  }

  /** 
   * Try to acquire the lock without blocking.  
   * <P>
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException a remote error occured
   */
  public int tryAcquire () throws LockException
  {
    return makeRequest (LockOperations.TRY_ACQUIRE, 0, 0);
  }

  /** 
   * Try to acquire a read lock without blocking.  
   * <P>
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException a remote error occured
   */
  public int tryAcquireRead () throws LockException
  {
    return makeRequest (LockOperations.TRY_ACQUIRE,
			LockTypes.READ_LOCK_PROXY,
			0);
  }

  /** 
   * Try to acquire a write lock without blocking.  
   * 
   *@return         appropriate AbstractLock return value 
   *                (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception      LockException a remote error occured
   */
  public int tryAcquireWrite () throws LockException
  {
    return makeRequest (LockOperations.TRY_ACQUIRE,
			LockTypes.WRITE_LOCK_PROXY,
			0);
  }

  /** 
   * Release ownership of this lock.  
   * 
   *@return    appropriate AbstractLock return value 
   *           (AbstractLock.FAILURE or AbstractLock.SUCCESS)
   *@exception LockException a remote error occured
   */
  public int release () throws LockException
  {
    return makeRequest (LockOperations.RELEASE, 0, 0);
  }

  /**
   * Closes the connection to the server (if it is still open).
   */
  protected void finalize () throws Throwable
  {
    close ();
  }

  /**
   * No-op implementation for the sleep hook (unused).
   */
  public void sleepHook () {}

  /** Status of whether this RemoteLock is connected to the server or not */
  protected boolean connected_ = false;

  /** Request object for transmissions to the server */
  protected TokenRequest request_;

  /** Reply object for receiving transmissions from the server */
  protected TokenReply reply_;

  /** Host name of the token service */
  protected String host_;

  /** Port number of the token service */
  protected int port_;
}

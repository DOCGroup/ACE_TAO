package JACE.netsvcs.Token;

import java.util.*;
import JACE.ASX.*;
import JACE.OS.*;
import JACE.Concurrency.*;

/**
 * LockHandler implementation for any AbstractLock.
 * <P>
 * Provides the dispatching to appropriate methods on an AbstractLock
 * as requests come in.
 */
public class LockHandlerAdapter implements LockHandler
{
  /**
   * Constructor taking an AbstractLock to use as the locking
   * mechanism the requests work on.
   */
  public LockHandlerAdapter (AbstractLock lock)
  {
    lock_ = lock;
  }

  /**
   * Default constructor.
   */
  public LockHandlerAdapter ()
  {
    lock_ = null;
  }

  /**
   * Dispatch the request according to its type, calling the
   * appropriate methods on the AbstractLock member.
   *
   *@param caller TokenRequestHandler which called handleRequest (unused)
   *@param request request to process
   *@return appropriate reply to send to the client
   */
  public TokenReply handleRequest (TokenRequestHandler caller,
				   TokenRequest request)
  {
    String client = request.clientID ();
    String token = request.tokenName ();
    TokenReply result = null;

    // Dispatch according to operation type
    switch (request.operationType ())
      {
      case LockOperations.ACQUIRE:
	ACE.DEBUG (client + " begins ACQUIRE for " + token);
	result = acquireDispatcher (request);
	break;
      case LockOperations.RELEASE:
	ACE.DEBUG (client + " begins RELEASE for " + token);
	result = release (request);
	break;
      case LockOperations.RENEW:
	ACE.DEBUG (client + " begins RENEW for " + token);
	result = renew (request);
	break;
      case LockOperations.REMOVE:
	ACE.DEBUG (client + " begins REMOVE for " + token);
	result = remove (request);
	break;
      case LockOperations.TRY_ACQUIRE:
	ACE.DEBUG (client + " begins TRY_ACQUIRE for " + token);
	result = tryAcquireDispatcher (request);
	break;
      default:
	ACE.ERROR ("Unknown operation: " + request.operationType ());
	break;
      }

    ACE.DEBUG (client + " result: " + result);

    return result;
  }
  
  /**
   * Create a TimeValue from the given request's timeout information.  Note
   * that the time in the request is an absolute time timeout.
   *
   *@param request request to obtain the timeout info from
   *@return null if useTimeout is false, otherwise a TimeValue
   *        representing the appropriate time period
   */
  protected TimeValue getTimeout (TokenRequest request)
  {
    if (request.useTimeout ()) 
      return new TimeValue (request.sec (),
			    request.usec () * 1000);
    else
      return null;
  }

  /**
   * Call acquireWrite on the lock, returning its return value.
   *
   *@see AbstractLock#acquireWrite
   *@return value from the lock's operation
   */
  protected int acquireWrite (TokenRequest request, TimeValue timeout)
    throws LockException, TimeoutException, InterruptedException
  {
    int result;

    if (timeout != null)
      result = lock_.acquireWrite (timeout);
    else
      result = lock_.acquireWrite ();
    
    return result;
  }

  /**
   * Call acquireRead on the lock, returning its return value.
   *
   *@see AbstractLock#acquireRead
   *@return value from the lock's operation
   */
  protected int acquireRead (TokenRequest request, TimeValue timeout)
    throws LockException, TimeoutException, InterruptedException
  {
    int result;

    if (timeout != null)
      result = lock_.acquireRead (timeout);
    else
      result = lock_.acquireRead ();

    return result;
  }

  /**
   * Call acquire on the lock, returning its return value.
   *
   *@see AbstractLock#acquire
   *@return value from the lock's operation
   */
  protected int acquire (TokenRequest request, TimeValue timeout)
    throws LockException, TimeoutException, InterruptedException
  {
    int result;

    if (timeout != null)
      result = lock_.acquire (timeout);
    else
      result = lock_.acquire ();

    return result;
  }

  /**
   * Dispatch to the appropriate acquire method.  In C++ ACE, when
   * the type is LockTypes.RWLOCK and the proxy type is
   * LockTypes.WRITE_LOCK_PROXY, then this calls acquireWrite.
   * If it's RWLOCK and the proxy is READ_LOCK_PROXY, it calls
   * acquireRead.  In the normal case, it just calls acquire.
   *
   *@return reply to be sent back to the client (values for errno
   *        include constants in TokenReply such as EFAULT, ETIME,
   *        EINTR, or NO_ERRORS)
   */
  protected TokenReply acquireDispatcher (TokenRequest request)
  {
    int result;
    TimeValue timeout = getTimeout (request);
    
    try {

      /*
	ACE specifies that when requesting a reader lock, the
	token type will be RWLOCK and the proxy type is 0.
	When it's a writer lock, the proxy type is 1.
      */
      if (request.tokenType () == LockTypes.RWLOCK) {
	if (request.proxyType () == LockTypes.READ_LOCK_PROXY)
	  result = acquireRead (request, timeout);
	else
	  result = acquireWrite (request, timeout);
      } else
	result = acquire (request, timeout);
  
    } catch (LockException e) {
      return new TokenReply (TokenReply.EFAULT,
			     request.arg ());
    } catch (TimeoutException e) {
      return new TokenReply (TokenReply.ETIME,
			     request.arg ());
    } catch (InterruptedException e) {
      return new TokenReply (TokenReply.EINTR,
			     request.arg ());
    }
    
    if (result == AbstractLock.FAILURE) {
      return new TokenReply (TokenReply.EFAULT,
			     request.arg ());
    } else {
      return new TokenReply (TokenReply.NO_ERRORS, 
			     request.arg ());
    }
  }

  /**
   * Process a release request and construct a reply.  The values
   * for errno include TokenReply constants EFAULT, EACCES, or
   * NO_ERRORS.
   */
  protected TokenReply release (TokenRequest request)
  {
    int result;

    try {
      result = lock_.release ();
    } catch (LockException e) {
      return new TokenReply (TokenReply.EFAULT,
			     request.arg ());
    }

    if (result == AbstractLock.FAILURE) {
      return new TokenReply (TokenReply.EACCES,
			     request.arg ());
    } else {
      return new TokenReply (TokenReply.NO_ERRORS,
			     request.arg ());
    }
  }

  /**
   * Process a renew request and construct a reply.  The values for
   * errno include TokenReply constants EFAULT, ETIME, EINTR, EACCES,
   * or NO_ERRORS.
   */
  protected TokenReply renew (TokenRequest request)
  {
    int result = AbstractLock.FAILURE;
    TimeValue timeout = getTimeout (request);

    try {

      if (timeout != null) {
	result = lock_.renew (request.requeuePosition (),
			     timeout);
      } else {
	result = lock_.renew (request.requeuePosition ());
      }

    } catch (LockException e) {
      return new TokenReply (TokenReply.EFAULT,
			     request.arg ());
    } catch (TimeoutException e) {
      return new TokenReply (TokenReply.ETIME,
			     request.arg ());
    } catch (InterruptedException e) {
      return new TokenReply (TokenReply.EINTR,
			     request.arg ());
    }

    if (result == AbstractLock.FAILURE) {
      return new TokenReply (TokenReply.EACCES,
			     request.arg ());
    } else {
      return new TokenReply (TokenReply.NO_ERRORS, 
			     request.arg ());
    }
  }

  /**
   * Process a remove request and construct a reply.  This currently
   * is not supported in the normal AbstractLock interface, so the
   * default implementation returns a reply with errno set to
   * TokenReply.ENOTSUP.
   */
  protected TokenReply remove (TokenRequest request)
  {
    ACE.ERROR ("Remove is unimplemented");
    return new TokenReply (TokenReply.ENOTSUP,
			   request.arg ());
  }

  /**
   * Call tryAcquireWrite on the lock, returning the result.
   */
  protected int tryAcquireWrite (TokenRequest request)
    throws LockException
  {
    return lock_.tryAcquireWrite ();
  }

  /**
   * Call tryAcquireRead on the lock, returning the result.
   */
  protected int tryAcquireRead (TokenRequest request)
    throws LockException
  {
    return lock_.tryAcquireRead ();
  }

  /**
   * Call tryAcquire on the lock, returning the result.
   */
  protected int tryAcquire (TokenRequest request) throws LockException
  {
    return lock_.tryAcquire ();
  }

  /**
   * Dispatch to the appropriate tryAcquire method.  In C++ ACE, when
   * the type is LockTypes.RWLOCK and the proxy type is
   * LockTypes.WRITE_LOCK_PROXY, then this calls acquireWrite.
   * If it's RWLOCK and the proxy is READ_LOCK_PROXY, it calls
   * acquireRead.  In the normal case, it just calls acquire.
   *
   *@return reply to be sent back to the client (values for errno
   *        include constants in TokenReply such as EFAULT, 
   *        EWOULDBLOCK, or NO_ERRORS).
   */
  protected TokenReply tryAcquireDispatcher (TokenRequest request)
  {
    int result;

    try {

      /*
	ACE specifies that when requesting a reader lock, the
	token type will be RWLOCK and the proxy type is 0.
	When it's a writer lock, the proxy type is 1.
      */
      if (request.tokenType () == LockTypes.RWLOCK) {
	if (request.proxyType () == LockTypes.READ_LOCK_PROXY)
	  result = tryAcquireRead (request);
	else
	  result = tryAcquireWrite (request);
      } else
	result = tryAcquire (request);
  
    } catch (LockException e) {
      return new TokenReply (TokenReply.EFAULT,
			     request.arg ());
    }

    if (result == AbstractLock.FAILURE) {
      return new TokenReply (TokenReply.EWOULDBLOCK,
			     request.arg ());
    } else {
      return new TokenReply (TokenReply.NO_ERRORS, 
			     request.arg ());
    }
  }

  /**
   * Abandon any claim the specified client has on the lock.
   *
   *@param clientID identification of the client
   */
  public void abandonLock (String clientID)
  {
    ACE.DEBUG (clientID + " abandoning lock");
    try {
      int nesting_level = 0;
      while (lock_.release () != AbstractLock.FAILURE) 
	{
	  nesting_level++;
	  // Loop until not the owner in case the lock
	  // supports nested acquires
	}
      if (nesting_level == 0) 
	ACE.DEBUG (clientID + " was not the owner");
      else
	ACE.DEBUG (clientID + " had " + nesting_level + " locks");
    } catch (LockException e) {
      ACE.ERROR ("While abandoning lock: " + e.getMessage ());
      // Don't need to send a reply to the client
    }
  }

  protected AbstractLock lock_;
}

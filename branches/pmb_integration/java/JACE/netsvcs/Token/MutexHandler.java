package JACE.netsvcs.Token;

import JACE.Concurrency.*;

/**
 * LockHandler implementation for a mutex lock.  
 * <P>
 * Currently, this uses JACE.Concurrency.Token as the actual lock since
 * it supports nested acquires.
 *
 *@see LockHandler
 */
public class MutexHandler extends LockHandlerAdapter
{
  // Uses token since it supports nested acquires.
  static class ExtendedMutex extends Token
  {
    // This is so that we don't make any assumptions about previous
    // implementations of LockAdapter, and enable owner checking with
    // the client ID from TokenRequest.  The thread name is set in
    // handleRequest.
    protected Object accessorID ()
    {
      return Thread.currentThread().getName();
    }
  }

  /**
   * Default constructor.
   */
  public MutexHandler ()
  {
    super (new ExtendedMutex ());
  }

  public TokenReply handleRequest (TokenRequestHandler caller,
				   TokenRequest request)
  {
    Thread.currentThread().setName (request.clientID ());
    
    return super.handleRequest (caller, request);
  }

  public void abandonLock (String clientID)
  {
    Thread.currentThread().setName (clientID);

    super.abandonLock (clientID);
  }
}


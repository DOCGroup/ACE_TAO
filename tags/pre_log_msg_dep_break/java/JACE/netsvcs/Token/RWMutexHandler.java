package JACE.netsvcs.Token;

import JACE.Concurrency.*;

/**
 * LockHandler implementation for a reader/writer mutex lock.  
 * <P>
 * Since it uses RWMutex as the actual lock, it doesn't support
 * nested acquires.
 *
 *@see LockHandler
 */
public class RWMutexHandler extends LockHandlerAdapter
{
  static class ExtendedRWMutex extends RWMutex
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
  public RWMutexHandler ()
  {
    super (new ExtendedRWMutex ());
  }

  public TokenReply handleRequest (TokenRequestHandler caller,
				   TokenRequest request)
  {
    // Set the name of this thread to the client ID to perform
    // proper owner checking.
    Thread.currentThread().setName (request.clientID ());
    
    // process the request
    return super.handleRequest (caller, request);
  }

  public void abandonLock (String clientID)
  {
    // Set the name of this thread to the client ID to perform
    // proper owner checking.
    Thread.currentThread().setName (clientID);

    super.abandonLock (clientID);
  }
}

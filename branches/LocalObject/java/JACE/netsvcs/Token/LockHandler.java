package JACE.netsvcs.Token;

/**
 * Defines a handler for a certain type of lock.  This allows new types
 * of synchronization mechanisms to be added to the Token service without
 * any modification of existing code.   Implementing class instances that
 * are registered (via the command line or another way)
 * with the token service can be created as requests for that type of
 * lock come into the service.
 *
 *@see LockHandlerAdapter
 *@see MutexHandler
 *@author Everett Anderson
 */
public interface LockHandler
{
  /**
   * Process a given TokenRequest and construct the appropriate
   * reply.  The request has already been read from the connection,
   * and the reply will be sent without the LockHandler having to
   * worry about the details.
   *
   *@param caller TokenRequestHandler which is accessing this LockHandler
   *@param request request read from the connection
   *@return appropriate TokenReply (success, failure, etc)
   */
  TokenReply handleRequest(TokenRequestHandler caller,
			   TokenRequest request);

  /**
   * Release any claim the client represented with the given ID
   * has on this handler's lock.  This is used when a client
   * disconnects.
   *
   *@param clientID ID of the client whose claims to abandon
   */
  void abandonLock (String clientID);
}

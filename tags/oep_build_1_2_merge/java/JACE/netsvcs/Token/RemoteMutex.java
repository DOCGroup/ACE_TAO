package JACE.netsvcs.Token;

/**
 * Proxy used by clients for accessing a mutex at the token service.
 */
public class RemoteMutex extends RemoteLock
{
  /**
   * Constructor.
   *
   *@param tokenName name of the mutex to access
   *@param clientID identification of this client
   *@param host host of the token service
   *@param port port number of the token service
   */
  public RemoteMutex (String tokenName,
		      String clientID,
		      String host,
		      int port)
  {
    super (LockTypes.MUTEX,
	   0,
	   tokenName,
	   clientID,
	   host,
	   port);
  }
}

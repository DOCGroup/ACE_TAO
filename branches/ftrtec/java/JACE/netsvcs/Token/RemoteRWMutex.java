package JACE.netsvcs.Token;

/**
 * Proxy used by clients for accessing a reader/writer mutex
 *  at the token service.
 */
public class RemoteRWMutex extends RemoteLock
{
  /**
   * Constructor.
   *
   *@param tokenName name of the reader/writer lock to access
   *@param clientID identification of this client
   *@param host host of the token service
   *@param port port number of the token service
   */
  public RemoteRWMutex (String tokenName,
			String clientID,
			String host,
			int port)
  {
    super (LockTypes.RWLOCK,
	   0,
	   tokenName,
	   clientID,
	   host,
	   port);
  }
}

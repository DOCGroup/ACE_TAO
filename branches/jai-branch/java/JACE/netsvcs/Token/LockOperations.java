package JACE.netsvcs.Token;

/**
 * Constants defining the operation types available on a lock.  
 * For information on specifying a read/write style lock, see LockTypes.
 *
 *@see LockTypes
 */
public interface LockOperations
{
  int ACQUIRE = 0;
  int RELEASE = 1;
  int RENEW = 2;
  int REMOVE = 3;
  int TRY_ACQUIRE = 4;
}

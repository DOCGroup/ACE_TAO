package JACE.netsvcs.Token;

/**
 * Constants related to the default lock and proxy types.  New types
 * of LockHandlers and lock types can be created and registered with
 * the token service on the command line without modifying this.
 * <P>
 * C++ ACE handles RWLOCK in this way:
 * When a request comes through for a RWLOCK, the proxy type is
 * 0 for a read lock request and 1 for a write lock request.
 */
public interface LockTypes
{
  int MUTEX = 0;
  int RWLOCK = 1;
  
  int READ_LOCK_PROXY = 0;
  int WRITE_LOCK_PROXY = 1;
}

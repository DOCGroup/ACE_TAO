// $Id$

#include "ace/Handle_Ops.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Handle_Ops.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, Handle_Ops, "$Id$")

ACE_HANDLE
ACE_Handle_Ops::handle_timed_open (ACE_Time_Value *timeout,
                                   const ACE_TCHAR *name,
                                   int flags,
                                   int perms)
{
  ACE_TRACE ("ACE_Handle_Ops::handle_timed_open");

  if (timeout != 0)
    {
      // Open the named pipe or file using non-blocking mode...
      ACE_HANDLE handle = ACE_OS::open (name,
                                        flags | ACE_NONBLOCK,
                                        perms);
      if (handle == ACE_INVALID_HANDLE
          && (errno == EWOULDBLOCK
              && (timeout->sec () > 0 || timeout->usec () > 0)))
        // This expression checks if we were polling.
        errno = ETIMEDOUT;

      return handle;
    }
  else
    return ACE_OS::open (name, flags, perms);
}

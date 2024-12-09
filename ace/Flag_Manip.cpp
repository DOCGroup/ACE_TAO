// $Id$

#include "ace/Flag_Manip.h"

#if defined (ACE_LACKS_FCNTL)
#  include "ace/OS_NS_stropts.h"
#  include "ace/OS_NS_errno.h"
#endif /* ACE_LACKS_FCNTL */

#if !defined (__ACE_INLINE__)
#include "ace/Flag_Manip.inl"
#endif /* __ACE_INLINE__ */

#if defined (CYGWIN32)
#  include "ace/os_include/os_termios.h"
#endif /* CYGWIN32 */

ACE_RCSID (ace,
           Flag_Manip,
           "$Id$")

// Flags are file status flags to turn on.

#ifndef ACE_LACKS_FCNTL
namespace {
  int set_flags_helper (ACE_HANDLE handle, int flags)
  {
    int val = ACE_OS::fcntl (handle, F_GETFL, 0);

    if (val == -1)
      return -1;

    // Turn on flags.
    ACE_SET_BITS (val, flags);

    if (ACE_OS::fcntl (handle, F_SETFL, val) == -1)
      return -1;
    else
      return 0;
  }

  int clr_flags_helper (ACE_HANDLE handle, int flags)
  {
    int val = ACE_OS::fcntl (handle, F_GETFL, 0);

    if (val == -1)
      return -1;

    // Turn flags off.
    ACE_CLR_BITS (val, flags);

    if (ACE_OS::fcntl (handle, F_SETFL, val) == -1)
      return -1;
    else
      return 0;
  }
}
#endif

// INTEGRITY 11.4.6 simulator does not support ACE_NONBLOCK flag.
// ACE_USES_GHS_ISIMPPC is defined when running on the simulator.
int
ACE::set_flags (ACE_HANDLE handle, int flags)
{
  ACE_TRACE ("ACE::set_flags");
#if defined (ACE_USES_GHS_ISIMPPC)
  switch (flags)
    {
    case ACE_NONBLOCK:
      ACE_UNUSED_ARG (handle);
      return 0;
    default:
# if defined (ACE_LACKS_FCNTL)
      ACE_NOTSUP_RETURN (-1);
# else
      return set_flags_helper (handle, flags);
# endif
    }
#else
# if defined (ACE_LACKS_FCNTL)
  switch (flags)
    {
    case ACE_NONBLOCK:
#   if defined (INTEGRITY) && defined (ACE_LACKS_IOCTL)
      ACE_UNUSED_ARG (handle);
      return 0;
#   else
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 1;
        return ACE_OS::ioctl (handle, FIONBIO, &nonblock);
      }
#   endif
    default:
      ACE_NOTSUP_RETURN (-1);
    }
# else
  return set_flags_helper (handle, flags);
# endif /* ACE_LACKS_FCNTL */
#endif /* ACE_USES_GHS_ISIMPPC */
}

// Flags are the file status flags to turn off.

int
ACE::clr_flags (ACE_HANDLE handle, int flags)
{
  ACE_TRACE ("ACE::clr_flags");
#if defined (ACE_USES_GHS_ISIMPPC)
  switch (flags)
    {
    case ACE_NONBLOCK:
      ACE_UNUSED_ARG (handle);
      return 0;
    default:
# if defined (ACE_LACKS_FCNTL)
      ACE_NOTSUP_RETURN (-1);
# else
      return clr_flags_helper (handle, flags);
# endif
    }
#else
# if defined (ACE_LACKS_FCNTL)
  switch (flags)
    {
    case ACE_NONBLOCK:
#   if defined (INTEGRITY) && defined (ACE_LACKS_IOCTL)
      ACE_UNUSED_ARG (handle);
      return 0;
#   else
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 0;
        return ACE_OS::ioctl (handle, FIONBIO, &nonblock);
      }
#   endif
    default:
      ACE_NOTSUP_RETURN (-1);
    }
# else
  return clr_flags_helper (handle, flags);
# endif /* ACE_LACKS_FCNTL */
#endif /* ACE_USES_GHS_ISIMPPC */
}

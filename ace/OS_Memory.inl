// $Id$

#if !defined (ACE_LACKS_SBRK)
#  include "ace/os_include/os_unistd.h"
#endif /* !ACE_LACKS_SBRK */

ACE_INLINE void *
ACE_OS_Memory::sbrk (int brk)
{
#if defined (ACE_LACKS_SBRK)
  ACE_UNUSED_ARG (brk);
  ACE_NOTSUP_RETURN (0);
#else
  ACE_OSCALL_RETURN (::sbrk (brk), void *, 0);
#endif /* VXWORKS */
}

// $Id$

#if ! defined (ACE_PSOS_DIAB_MIPS)
# include /**/ <stdlib.h>
#endif /* ! ACE_PSOS_DIAB_MIPS */

#if !defined (ACE_LACKS_UNISTD_H) && !defined (ACE_PSOS)
# include /**/ <unistd.h>
#endif /* !ACE_LACKS_UNISTD_H && !ACE_PSOS */

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

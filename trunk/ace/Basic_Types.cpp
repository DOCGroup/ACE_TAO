// $Id$

#define ACE_BUILD_DLL

#include "ace/OS.h"
#if !defined (__ACE_INLINE__)
# include "ace/Basic_Types.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, Basic_Types, "$Id$")

#if defined (ACE_LACKS_LONGLONG_T)

void
ACE_U_LongLong::output (FILE *file) const
{
  if (hi_ > 0)
    ACE_OS::fprintf (file, "0x%lx%0*lx", hi_, 2 * sizeof lo_, lo_);
  else
    ACE_OS::fprintf (file, "0x%lx", lo_);
}

#endif /* ACE_LACKS_LONGLONG_T */


// $Id$
//
// Build this file with g++.  It can be linked in to a ACE application
// that was compiled with GreenHills.  It wouldn't be necessary if I
// knew a way to correctly move values from registers to a 64-bit
// variable in GHS asm code.  That's easy with g++ asm.

#include "ace/OS.h"

ACE_RCSID(ace, gethrtime, "$Id$")

extern "C"
ACE_hrtime_t
ACE_gethrtime (void)
{
#if defined (ACE_HAS_PENTIUM)
  // ACE_TRACE ("ACE_gethrtime");

#if defined (ACE_LACKS_LONGLONG_T)
  double now;
#else  /* ! ACE_LACKS_LONGLONG_T */
  ACE_hrtime_t now;
#endif /* ! ACE_LACKS_LONGLONG_T */

  // See comments about the RDTSC Pentium instruction for the
  // ACE_WIN32 version of ACE_OS::gethrtime () in ace/OS.i.
  //
  // Read the high-res tick counter directly into memory variable
  // "now".  The A constraint signifies a 64-bit int.
#if defined (__GNUG__)
  asm volatile ("rdtsc" : "=A" (now) : : "memory");
#elif defined (ghs)
  asm ("rdtsc");
  asm ("movl -16(%ebp),%edx");
  asm ("movl -12(%ebp),%eax");
#else
# error unsupported compiler
#endif

#if defined (ACE_LACKS_LONGLONG_T)
  // ACE_U_LongLong doesn't have the same layout as now, so construct
  // it "properly".
  ACE_UINT32 least, most;
  ACE_OS::memcpy (&least, &now, sizeof (ACE_UINT32));
  ACE_OS::memcpy (&most, (unsigned char *) &now + sizeof (ACE_UINT32),
                  sizeof (ACE_UINT32));

  const ACE_hrtime_t ret (least, most);
  return ret;
#else  /* ! ACE_LACKS_LONGLONG_T */
  return now;
#endif /* ! ACE_LACKS_LONGLONG_T */

#else  /* ! ACE_HAS_PENTIUM */
# error This file can _only_ be compiled with ACE_HAS_PENTIUM.
#endif /* ! ACE_HAS_PENTIUM */
}

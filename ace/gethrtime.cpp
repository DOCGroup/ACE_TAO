// $Id$
//
// Build this file with g++.  It can be linked in to a ACE application
// that was compiled with GreenHills.  It wouldn't be necessary if I
// knew a way to move values from registers to variables in ghs asm
// code.  That's easy with g++ asm.

#include "ace/OS.h"

extern "C"
ACE_hrtime_t
ACE_gethrtime ()
{
#if defined (__GNUG__) && defined (ACE_HAS_PENTIUM)
  // ACE_TRACE ("ACE_gethrtime");

#if defined (ACE_LACKS_LONGLONG_T)
  double now;
#else  /* ! ACE_LACKS_LONGLONG_T */
  ACE_hrtime_t now;
#endif /* ! ACE_LACKS_LONGLONG_T */

  // See comments about the RDTSC Pentium instruction for the ACE_WIN32
  // version of ACE_OS::gethrtime () in ace/OS.i.
  //
  // Read the high-res tick counter directly into memory variable "now".
  // The A constraint signifies a 64-bit int.
  asm volatile ("rdtsc" : "=A" (now) : : "memory");

#if defined (ACE_LACKS_LONGLONG_T)
  // ACE_U_LongLong doesn't have the same layout as now, so construct
  // it "properly".
  unsigned int least, most;
  ACE_OS::memcpy (&least, &now, sizeof (unsigned int));
  ACE_OS::memcpy (&most, &now + sizeof (unsigned int), sizeof (unsigned int));

  ACE_hrtime_t ret (least, most);
  return ret;
#else  /* ! ACE_LACKS_LONGLONG_T */
  return now;
#endif /* ! ACE_LACKS_LONGLONG_T */

#else  /* ! __GNUG__  ||  ! ACE_HAS_PENTIUM */
# error This file can _only_ be compiled with g++, with ACE_HAS_PENTIUM.
#endif /* ! __GNUG__  ||  ! ACE_HAS_PENTIUM */
}

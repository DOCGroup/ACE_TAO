/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    (none)
//
// = FILENAME
//    Timer.h
//
// = DESCRIPTION
//    Adapter to platform-specific timer class.  The "default interface"
//    is that of ACE_High_Res_Timer.  Clients should use its member functions,
//    but declare instances of type Timer instead of ACE_High_Res_Timer.
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#if ! defined (TIMER_H)
#define TIMER_H

#if defined (__sun)
#include <ace/High_Res_Timer.h>

typedef ACE_High_Res_Timer Timer;


#if 0
#elif defined (VXWORKS)
// NOTE:  ppctimer.h isn't distributed with ACE.  It is based on
// an assembly language routing for reading the clock that is
// available in the Motorola documentation for the PowerPC.

#include <ppctimer.h>

class Timer : public PPCTimer
{
public:
  Timer () : PPCTimer () {}
  ~Timer () {}    // WARNING:  non-virtual, assumes that base class destructor
                  // doesn't do anything significant

  void print_ave (const char *message,
		  const int /* iterations */ = 1,
		  ACE_HANDLE /* handle */ = ACE_STDOUT)
    {
      printTimerStats (message);
    }

private:
  // the following functions are not implemented . . .
  Timer (const Timer &);
  Timer &operator= (const Timer &);
};
#endif // 0

#else
// dummy class so that the test will build, but it won't provide
// timings

class Timer
{
public:
  Timer () {}
  ~Timer () {}

  void print_ave (const char *message,
		  const int /* iterations */ = 1,
                  ACE_HANDLE /* handle */ = ACE_STDOUT) {}

  void elapsed_microseconds (hrtime_t &usecs) const {}

private:
  // the following functions are not implemented . . .
  Timer (const Timer &);
  Timer &operator= (const Timer &);
};
#endif

#endif /* TIMER_H */


/* EOF */

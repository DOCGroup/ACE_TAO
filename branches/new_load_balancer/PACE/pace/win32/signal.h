/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/signal.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#ifndef PACE_SIGNAL_H_WIN32
#define PACE_SIGNAL_H_WIN32

#include "pace/sys/types.h"
#include "pace/time.h"
#include <signal.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  typedef sig_atomic_t pace_sig_atomic_t;

#ifndef PACE_SIGSET_T
#define PACE_SIGSET_T
  typedef struct sigset_t
  {
    /* signal set type */
    unsigned int sigbits_[4];
  } pace_sigset_t;
#endif /* PACE_SIGSET_T */

  typedef struct sigaction pace_s_sigaction;

#ifndef PACE_SIGINFO_T
#define PACE_SIGINFO_T
  typedef struct siginfo_t
  {
    PACE_HANDLE si_handle_;
    // Win32 HANDLE that has become signaled.

    PACE_HANDLE *si_handles_;
    // Array of Win32 HANDLEs all of which have become signaled.
  } pace_siginfo_t;
# endif /* PACE_SIGINFO_T */

  typedef struct sigevent pace_sigevent;
  typedef union sigval pace_sigval;

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SIGNAL_H_WIN32 */

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

#ifndef PACE_SIGATOMIC_T
#define PACE_SIGATOMIC_T
  typedef sig_atomic_t pace_sig_atomic_t;
#endif /* PACE_SIGATOMIC_T */

#ifndef PACE_SIGSET_T
#define PACE_SIGSET_T
  enum { PACE_SIG_BITS = 4 };
  typedef struct /* sigset_t */ {
    /* signal set type */
    unsigned int sigbits_[PACE_SIG_BITS];
  } pace_sigset_t;
#endif /* PACE_SIGSET_T */

#ifndef PACE_SIGACTION_S
#define PACE_SIGACTION_S
  typedef void (__cdecl *pace_sig_pf)(int);
  typedef struct /* sigaction */ {
    int sa_flags;
    pace_sig_pf sa_handler;
    pace_sigset_t sa_mask;
  }  pace_sigaction_s;
#endif /* PACE_SIGACTION_S */

#ifndef PACE_SIGINFO_T
#define PACE_SIGINFO_T
  typedef struct /* siginfo_t */ {

    PACE_HANDLE si_handle_;
    // Win32 HANDLE that has become signaled.

    PACE_HANDLE *si_handles_;
    // Array of Win32 HANDLEs all of which have become signaled.
  } pace_siginfo_t;
# endif /* PACE_SIGINFO_T */

#ifndef PACE_SIGEVENT
  typedef struct sigevent pace_sigevent;
#endif /* PACE_SIGEVENT */

#ifndef PACE_SIGVAL
  typedef union sigval pace_sigval;
#endif /* PACE_SIGVAL */

#if (PACE_HAS_POSIX_SIG_UOF)
  int sigaction (int sig, const pace_sigaction_s * act,
                 pace_sigaction_s * oact);
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SIGNAL_H_WIN32 */

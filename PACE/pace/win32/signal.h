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
  typedef sigset_t pace_sigset_t;
  typedef struct sigaction pace_s_sigaction;
  typedef siginfo_t pace_siginfo_t;
  typedef struct sigevent pace_sigevent;
  typedef union sigval pace_sigval;

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SIGNAL_H_WIN32 */

/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/signal.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/signal.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/signal.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_SIG_UOF)
int
sigaction (int sig, const pace_sigaction_s * act,
           pace_sigaction_s * oact)
{
  pace_sigaction_s sa;

  if (oact == 0)
    {
      oact = &sa;
    }

  if (act == 0)
    {
      oact->sa_handler = signal (sig, SIG_IGN);
      signal (sig, oact->sa_handler);
    }
  else
    {
      oact->sa_handler = signal (sig, act->sa_handler);
    }
  return (oact->sa_handler == SIG_ERR ? -1 : 0);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

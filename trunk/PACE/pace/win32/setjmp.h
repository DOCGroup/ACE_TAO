/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/setjmp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SETJMP_H
#define PACE_SETJMP_H

#include <setjmp.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_setjmp (jmp_buf env);

  PACE_INLINE int pace_sigsetjmp (sigjmp_buf env, int savemask);

  PACE_INLINE void pace_longjmp (jmp_buf env, int val);

  PACE_INLINE void pace_siglongjmp (sigjmp_buf env, int val);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/setjmp.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SETJMP_H */

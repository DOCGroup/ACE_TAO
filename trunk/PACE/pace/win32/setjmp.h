/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/setjmp.h
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

  typedef jmp_buf pace_jmp_buf;
  typedef sigjmp_buf pace_sigjmp_buf;

  /**
     PACE's implementation of the POSIX function setjmp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.1.
   */
  PACE_INLINE int pace_setjmp (pace_jmp_buf env);

  /**
     PACE's implementation of the POSIX function sigsetjmp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.1.
   */
  PACE_INLINE int pace_sigsetjmp (pace_sigjmp_buf env, int savemask);

  /**
     PACE's implementation of the POSIX function longjmp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.1.
   */
  PACE_INLINE void pace_longjmp (pace_jmp_buf env, int val);

  /**
     PACE's implementation of the POSIX function siglongjmp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.1.
   */
  PACE_INLINE void pace_siglongjmp (pace_sigjmp_buf env, int val);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/setjmp.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SETJMP_H */

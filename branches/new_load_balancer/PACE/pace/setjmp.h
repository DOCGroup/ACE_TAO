/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/setjmp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SETJMP_H
#define PACE_SETJMP_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/setjmp.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/setjmp.h"
#elif (PACE_WIN32)
# include "pace/win32/setjmp.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */
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
#  if (PACE_HAS_POSIX)
#    include "pace/posix/setjmp.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/setjmp.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/setjmp.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SETJMP_H */

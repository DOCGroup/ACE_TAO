/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = Filename
 *    pace/math.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_MATH_H
#define PACE_MATH_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/math.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/math.h"
#elif (PACE_WIN32)
# include "pace/win32/math.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function acos.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_acos (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function asin.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_asin (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function atan.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_atan (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function atan2.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_atan2 (double x, double y);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function cos.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_cos (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function sin.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_sin (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function tan.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_tan (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function cosh.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_cosh (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function sinh.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_sinh (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function tanh.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_tanh (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function exp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_exp (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function frexp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_frexp (double x, int * exp);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function ldexp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_ldexp (double x, int exp);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function log.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_log (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function log10.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_log10 (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function modf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_modf (double x, double * iptr);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function pow.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_pow (double x, double y);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function sqrt.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_sqrt (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function ceil.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_ceil (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function fabs.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_fabs (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function floor.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_floor (double x);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function fmod.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_fmod (double x, double y);
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/math.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/math.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/math.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_MATH_H */

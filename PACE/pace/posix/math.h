/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/math.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_MATH_H
#define PACE_MATH_H

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE double pace_acos (double x);

  PACE_INLINE double pace_asin (double x);

  PACE_INLINE double pace_atan (double x);

  PACE_INLINE double pace_atan2 (double x, double y);

  PACE_INLINE double pace_cos (double x);

  PACE_INLINE double pace_sin (double x);

  PACE_INLINE double pace_tan (double x);

  PACE_INLINE double pace_cosh (double x);

  PACE_INLINE double pace_sinh (double x);

  PACE_INLINE double pace_tanh (double x);

  PACE_INLINE double pace_exp (double x);

  PACE_INLINE double pace_frexp (double x, int * exp);

  PACE_INLINE double pace_ldexp (double x, int exp);

  PACE_INLINE double pace_log (double x);

  PACE_INLINE double pace_log10 (double x);

  PACE_INLINE double pace_modf (double x, double * iptr);

  PACE_INLINE double pace_pow (double x);

  PACE_INLINE double pace_sqrt (double x);

  PACE_INLINE double pace_ceil (double x);

  PACE_INLINE double pace_fabs (double x);

  PACE_INLINE double pace_floor (double x);

  PACE_INLINE double pace_fmod (double x, double y);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/math.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_MATH_H */

/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/math.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

/* FUZZ: disable check_for_math_include */
#include <math.h>

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_acos (double x)
{
  return acos (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_asin (double x)
{
  return asin (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_atan (double x)
{
  return atan (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_atan2 (double x, double y)
{
  return atan2 (x, y);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_cos (double x)
{
  return cos (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_sin (double x)
{
  return sin (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_tan (double x)
{
  return tan (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_cosh (double x)
{
  return cosh (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_sinh (double x)
{
  return sinh (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_tanh (double x)
{
  return tanh (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_exp (double x)
{
  return exp (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_frexp (double x, int * exp)
{
  return frexp (x, exp);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_ldexp (double x, int exp)
{
  return ldexp (x, exp);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_log (double x)
{
  return log (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_log10 (double x)
{
  return log10 (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_modf (double x, double * iptr)
{
  return modf (x, iptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_pow (double x, double y)
{
  return pow (x, y);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_sqrt (double x)
{
  return sqrt (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_ceil (double x)
{
  return ceil (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_fabs (double x)
{
  return fabs (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_floor (double x)
{
  return floor (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_fmod (double x, double y)
{
  return fmod (x, y);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/math.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

/* FUZZ: disable check_for_math_include */
#include <math.h>

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_acos (double x)
{
  PACE_TRACE("pace_acos");

  return acos (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_asin (double x)
{
  PACE_TRACE("pace_asin");

  return asin (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_atan (double x)
{
  PACE_TRACE("pace_atan");

  return atan (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_atan2 (double x, double y)
{
  PACE_TRACE("pace_atan2");

  return atan2 (x, y);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_cos (double x)
{
  PACE_TRACE("pace_cos");

  return cos (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_sin (double x)
{
  PACE_TRACE("pace_sin");

  return sin (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_tan (double x)
{
  PACE_TRACE("pace_tan");

  return tan (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_cosh (double x)
{
  PACE_TRACE("pace_cosh");

  return cosh (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_sinh (double x)
{
  PACE_TRACE("pace_sinh");

  return sinh (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_tanh (double x)
{
  PACE_TRACE("pace_tanh");

  return tanh (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_exp (double x)
{
  PACE_TRACE("pace_exp");

  return exp (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_frexp (double x, int * exp)
{
  PACE_TRACE("pace_frexp");

  return frexp (x, exp);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_ldexp (double x, int exp)
{
  PACE_TRACE("pace_ldexp");

  return ldexp (x, exp);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_log (double x)
{
  PACE_TRACE("pace_log");

  return log (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_log10 (double x)
{
  PACE_TRACE("pace_log10");

  return log10 (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_modf (double x, double * iptr)
{
  PACE_TRACE("pace_modf");

  return modf (x, iptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_pow (double x, double y)
{
  PACE_TRACE("pace_pow");

  return pow (x, y);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_sqrt (double x)
{
  PACE_TRACE("pace_sqrt");

  return sqrt (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_ceil (double x)
{
  PACE_TRACE("pace_ceil");

  return ceil (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_fabs (double x)
{
  PACE_TRACE("pace_fabs");

  return fabs (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_floor (double x)
{
  PACE_TRACE("pace_floor");

  return floor (x);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_fmod (double x, double y)
{
  PACE_TRACE("pace_fmod");

  return fmod (x, y);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

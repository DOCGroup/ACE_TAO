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

#include <math.h>

PACE_INLINE
double
pace_acos (double x)
{
  return acos (x);
}

PACE_INLINE
double
pace_asin (double x)
{
  return asin (x);
}

PACE_INLINE
double
pace_atan (double x)
{
  return atan (x);
}

PACE_INLINE
double
pace_atan2 (double x, double y)
{
  return atan2 (x, y);
}

PACE_INLINE
double
pace_cos (double x)
{
  return cos (x);
}

PACE_INLINE
double
pace_sin (double x)
{
  return sin (x);
}

PACE_INLINE
double
pace_tan (double x)
{
  return tan (x);
}

PACE_INLINE
double
pace_cosh (double x)
{
  return cosh (x);
}

PACE_INLINE
double
pace_sinh (double x)
{
  return sinh (x);
}

PACE_INLINE
double
pace_tanh (double x)
{
  return tanh (x);
}

PACE_INLINE
double
pace_exp (double x)
{
  return exp (x);
}

PACE_INLINE
double
pace_frexp (double x, int * exp)
{
  return frexp (x, exp);
}

PACE_INLINE
double
pace_ldexp (double x, int exp)
{
  return ldexp (x, exp);
}

PACE_INLINE
double
pace_log (double x)
{
  return log (x);
}

PACE_INLINE
double
pace_log10 (double x)
{
  return log10 (x);
}

PACE_INLINE
double
pace_modf (double x, double * iptr)
{
  return modf (x, iptr);
}

PACE_INLINE
double
pace_pow (double x, double y)
{
  return pow (x, y);
}

PACE_INLINE
double
pace_sqrt (double x)
{
  return sqrt (x);
}

PACE_INLINE
double
pace_ceil (double x)
{
  return ceil (x);
}

PACE_INLINE
double
pace_fabs (double x)
{
  return fabs (x);
}

PACE_INLINE
double
pace_floor (double x)
{
  return floor (x);
}

PACE_INLINE
double
pace_fmod (double x, double y)
{
  return fmod (x, y);
}


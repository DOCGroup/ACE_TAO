/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdlib.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <stdlib.h>

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
void
pace_abort (void)
{
  abort ();
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_abs (int val)
{
  return abs (val);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_atexit (void (*func)(void))
{
  return atexit (func);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
double
pace_atof (const char * str)
{
  return atof (str);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_atoi (const char * str)
{
  return atoi (str);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
long
pace_atol (const char * str)
{
  return atol (str);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
# if defined (PACE_HAS_CPLUSPLUS)
PACE_INLINE
void *
pace_bsearch (const void *key, const void *base,
              size_t nel, size_t size,
              pace_bsearch_pf compar)
{
  return bsearch (key, base, nel, size, compar);
}
# else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
void *
pace_bsearch (const void *key, const void *base,
              size_t nel, size_t size,
              int (*compar)(const void *,const void *))
{
  return bsearch (key, base, nel, size, compar);
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_div_t
pace_div (int numer, int denom)
{
  return div (numer, denom);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
void
pace_exit (int status)
{
  exit (status);
  return;
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
char *
pace_getenv (const char * name)
{
  return getenv (name);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
long int
pace_labs (long int j)
{
  return labs (j);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_ldiv_t
pace_ldiv (long int numer, long int denom)
{
  return ldiv (numer, denom);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mblen (const char* s, pace_size_t n)
{
  return mblen (s, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_size_t
pace_mbstowcs (pace_wchar_t* pwcs,
               const char* s,
               pace_size_t n)
{
  return mbstowcs (pwcs, s, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mbtowc (pace_wchar_t* pwc,
             const char* s,
             pace_size_t n)
{
  return mbtowc (pwc, s, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
# if (PACE_HAS_CPLUSPLUS)
PACE_INLINE
void
pace_qsort (void * base, size_t nel, size_t width,
            pace_bsearch_pf compar)
{
  qsort (base, nel, width, compar);
}
# else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
void
pace_qsort (void * base, size_t nel, size_t width,
            int (*compar)(const void *,const void *))
{
  qsort (base, nel, width, compar);
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_rand ()
{
  return rand ();
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_srand (unsigned int seed)
{
  srand (seed);
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
int
pace_rand_r (unsigned int *seed)
{
#if defined (PACE_HAS_REENTRANT)
  return rand_r (seed);
#else  /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG (seed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_REENTRANT */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */


#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
double
pace_strtod (const char* nptr,
             char** endptr)
{
  return strtod (nptr, endptr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_strtol (const char* nptr,
             char** endptr,
             int base)
{
  return strtol (nptr, endptr, base);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
unsigned long int
pace_strtoul (const char* nptr,
              char** endptr,
              int base)
{
  return strtoul (nptr, endptr, base);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_system (const char* string)
{
  return system (string);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_size_t
pace_wcstombs (char* s,
               const pace_wchar_t* pwcs,
               pace_size_t n)
{
  return wcstombs (s, pwcs, n);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_wctomb (char* s, pace_wchar_t wchar)
{
  return wctomb (s, wchar);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

/* Memory Management. */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void *
pace_malloc (size_t size)
{
  return malloc (size);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void *
pace_calloc (size_t nelem, size_t elsize)
{
  return calloc (nelem, elsize);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_free (void * ptr)
{
  free (ptr);
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void *
pace_realloc (void * ptr, size_t size)
{
  return realloc (ptr, size);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stdlib.inl
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
PACE_INLINE
void *
pace_bsearch (const void *key, const void *base,
              size_t nel, size_t size,
              int (*compar)(const void *,const void *))
{
  return bsearch (key, base, nel, size, compar);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

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

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_qsort (void * base, size_t nel, size_t width,
            int (*compar)(const void *, const void *))
{
  qsort (base, nel, width, compar);
}
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
  PACE_UNUSED_ARG (seed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

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





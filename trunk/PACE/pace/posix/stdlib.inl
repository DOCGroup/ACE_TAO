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

PACE_INLINE
int
pace_atoi (const char * str)
{
  return atoi (str);
}

PACE_INLINE
long
pace_atol (const char * str)
{
  return atol (str);
}

PACE_INLINE
void
pace_exit (int status)
{
  exit (status);
  return;
}

PACE_INLINE
char *
pace_getenv (const char * name)
{
  return getenv (name);
}

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

/* Memory Management. */

PACE_INLINE
void *
pace_malloc (size_t size)
{
  return malloc (size);
}

PACE_INLINE
void *
pace_calloc (size_t nelem, size_t elsize)
{
  return calloc (nelem, elsize);
}

PACE_INLINE
void
pace_free (void * ptr)
{
  free (ptr);
  return;
}

PACE_INLINE
void *
pace_realloc (void * ptr, size_t size)
{
  return realloc (ptr, size);
}

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
#if defined (PACE_LINUX)
# ifdef __USE_POSIX
    return rand_r (seed);
# else  /* rand_r is not available */
    PACE_UNUSED_ARG (seed);
    PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* rand_r is not available */
#elif defined (PACE_LYNXOS)
  // Not available through LynxOS 3.1.0.
  PACE_UNUSED_ARG (seed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#elif defined (PACE_SUNOS)
# if defined (__EXTENSIONS__) || defined (_REENTRANT) || \
  (_POSIX_C_SOURCE - 0 >= 199506L)
    return rand_r (seed);
# else  /* rand_r is not available */
    PACE_UNUSED_ARG (seed);
    PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* rand_r is not available */
#else
  return rand_r (seed);
#endif
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

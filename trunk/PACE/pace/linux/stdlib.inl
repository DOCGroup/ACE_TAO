/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    stdlib.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


#include <stdlib.h>


PACE_INLINE
void
pace_abort ()
{
  abort ();
  return;
}

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
long long
pace_atoll (const char * str)
{
  return atoll (str);
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
char *
pace_lltostr (long longvalue, char * endptr)
{
  return lltostr (longvalue, endptr);
}

PACE_INLINE
int
pace_rand_r (unsigned int *seed)
{
  return rand_r (seed);
}

PACE_INLINE
long
pace_strtol (const char * str, char ** endptr, int base)
{
  return strtol (str, endptr, base);
}

PACE_INLINE
long long
pace_strtoll (const char * str, char ** endptr, int base)
{
  return strtoll (str, endptr, base);
}

PACE_INLINE
char *
pace_ulltostr (unsigned long longvalue, char * endptr)
{
  return ulltostr (longvalue, endptr);
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
pace_memalign (size_t alignment, size_t size)
{
  return memalign (alignment, size);
}

PACE_INLINE
void *
pace_realloc (void * ptr, size_t size)
{
  return realloc (ptr, size);
}

PACE_INLINE
void *
pace_valloc (size_t size)
{
  return valloc (size);
}


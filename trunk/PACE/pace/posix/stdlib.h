/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdlib.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDLIB_H
#define PACE_STDLIB_H

#include "pace/unistd.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_atoi (const char * str);

  PACE_INLINE long pace_atol (const char * str);

  PACE_INLINE void pace_exit (int status);

  PACE_INLINE char * pace_getenv (const char * name);

  PACE_INLINE int pace_rand_r (unsigned int * seed);

  /* Memory Management. */

  PACE_INLINE void * pace_malloc (size_t size);

  PACE_INLINE void * pace_calloc (size_t nelem, size_t elsize);

  PACE_INLINE void pace_free (void * ptr);

  PACE_INLINE void * pace_realloc (void * ptr, size_t size);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/stdlib.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STDLIB_H */

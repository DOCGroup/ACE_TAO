/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/semaphore.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SEMAPHORE_H
#define PACE_SEMAPHORE_H

#include <semaphore.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_sem_close (sem_t * sem);

  PACE_INLINE int pace_sem_destroy (sem_t * sem);

  PACE_INLINE int pace_sem_getvalue (sem_t * sem, int * sval);

  PACE_INLINE int pace_sem_init (sem_t * sem, int pshared, unsigned int value);

  # define pace_sem_open(X) sem_open X
  /* int fprintf (FILE *stream, const char *format, (*//* args *//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE int pace_sem_post (sem_t * sem);

  PACE_INLINE int pace_sem_trywait (sem_t * sem);

  PACE_INLINE int pace_sem_unlink (const char * name);

  PACE_INLINE int pace_sem_wait (sem_t * sem);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/semaphore.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SEMAPHORE_H */

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

typedef sem_t pace_sem_t;

  /**
     PACE's implementation of the POSIX function sem_close.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.4.
   */
  PACE_INLINE int pace_sem_close (pace_sem_t * sem);

  /**
     PACE's implementation of the POSIX function sem_destroy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.2.
   */
  PACE_INLINE int pace_sem_destroy (pace_sem_t * sem);

  /**
     PACE's implementation of the POSIX function sem_getvalue.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.8.
   */
  PACE_INLINE int pace_sem_getvalue (pace_sem_t * sem, int * sval);

  /**
     PACE's implementation of the POSIX function sem_init.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.1.
   */
  PACE_INLINE int pace_sem_init (pace_sem_t * sem, int pshared, unsigned int value);

  /**
     PACE's implementation of the POSIX function sem_open.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.3.
   */
  # define pace_sem_open(X) sem_open X
  /* int fprintf (FILE *stream, const char *format, (*//* args *//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  /**
     PACE's implementation of the POSIX function sem_post.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.7.
   */
  PACE_INLINE int pace_sem_post (pace_sem_t * sem);

  /**
     PACE's implementation of the POSIX function sem_trywait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.6.
   */
  PACE_INLINE int pace_sem_trywait (pace_sem_t * sem);

  /**
     PACE's implementation of the POSIX function sem_unlink.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.5.
   */
  PACE_INLINE int pace_sem_unlink (const char * name);

  /**
     PACE's implementation of the POSIX function sem_wait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.2.6.
   */
  PACE_INLINE int pace_sem_wait (pace_sem_t * sem);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/semaphore.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SEMAPHORE_H */

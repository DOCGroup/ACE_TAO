/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/semaphore.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <io.h>

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_close (pace_sem_t * sem)
{
  return sem_close (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_destroy (pace_sem_t * sem)
{
  return sem_destroy (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_getvalue (pace_sem_t * sem, int * sval)
{
  PACE_UNUSED_ARG (sem);
  PACE_UNUSED_ARG (sval);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_init (pace_sem_t * sem,
               int pshared,
               unsigned int value)
{
  return sem_init (sem, pshared, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_sem_t *
pace_sem_open (const char * name, int oflag, ...)
{
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (oflag);
  return (pace_sem_t)NULL;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_post (pace_sem_t * sem)
{
  return sem_post (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_trywait (pace_sem_t * sem)
{
  return sem_trywait (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_unlink (const char * name)
{
#if defined (__BORLANDC__)
  return unlink (name);
#else /* __BORLANDC__ */
  return _unlink (name);
#endif /* __BORLANDC__ */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_wait (pace_sem_t * sem)
{
  return sem_wait (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */


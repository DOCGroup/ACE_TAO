/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/semaphore.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_close (pace_sem_t * sem)
{
  PACE_TRACE("pace_sem_close");

  return sem_close (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_destroy (pace_sem_t * sem)
{
  PACE_TRACE("pace_sem_destroy");

  return sem_destroy (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_getvalue (pace_sem_t * sem, int * sval)
{
  PACE_TRACE("pace_sem_getvalue");

  return sem_getvalue (sem, sval);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_init (pace_sem_t * sem, int pshared, unsigned int value)
{
  PACE_TRACE("pace_sem_init");

  return sem_init (sem, pshared, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_post (pace_sem_t * sem)
{
  PACE_TRACE("pace_sem_post");

  return sem_post (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_trywait (pace_sem_t * sem)
{
  PACE_TRACE("pace_sem_trywait");

  return sem_trywait (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_unlink (const char * name)
{
  PACE_TRACE("pace_sem_unlink");

  return sem_unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_wait (pace_sem_t * sem)
{
  PACE_TRACE("pace_sem_wait");

  return sem_wait (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

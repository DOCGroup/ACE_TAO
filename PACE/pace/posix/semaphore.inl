/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/semaphore.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_sem_close (pace_sem_t * sem)
{
  return sem_close (sem);
}

PACE_INLINE
int
pace_sem_destroy (pace_sem_t * sem)
{
  return sem_destroy (sem);
}

PACE_INLINE
int
pace_sem_getvalue (pace_sem_t * sem, int * sval)
{
  return sem_getvalue (sem, sval);
}

PACE_INLINE
int
pace_sem_init (pace_sem_t * sem, int pshared, unsigned int value)
{
  return sem_init (sem, pshared, value);
}

PACE_INLINE
int
pace_sem_post (pace_sem_t * sem)
{
  return sem_post (sem);
}

PACE_INLINE
int
pace_sem_trywait (pace_sem_t * sem)
{
  return sem_trywait (sem);
}

PACE_INLINE
int
pace_sem_unlink (const char * name)
{
  return sem_unlink (name);
}

PACE_INLINE
int
pace_sem_wait (pace_sem_t * sem)
{
  return sem_wait (sem);
}

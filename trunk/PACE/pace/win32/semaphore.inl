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

PACE_INLINE
int
pace_sem_close (sem_t * sem)
{
  return sem_close (sem);
}

PACE_INLINE
int
pace_sem_destroy (sem_t * sem)
{
  return sem_destroy (sem);
}

PACE_INLINE
int
pace_sem_getvalue (sem_t * sem, int * sval)
{
  return sem_getvalue (sem, sval);
}

PACE_INLINE
int
pace_sem_init (sem_t * sem, int pshared, unsigned int value)
{
  return sem_init (sem, pshared, value);
}

PACE_INLINE
int
pace_sem_post (sem_t * sem)
{
  return sem_post (sem);
}

PACE_INLINE
int
pace_sem_trywait (sem_t * sem)
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
pace_sem_wait (sem_t * sem)
{
  return sem_wait (sem);
}

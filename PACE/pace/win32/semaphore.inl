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

/* None of these emulations should be active. Work in progress! */

#include <io.h>
#include <stdio.h>

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_close (pace_sem_t * sem)
{
  if (CloseHandle (*sem))
    {
      return 0;
    }
  else
    {
      errno = GetLastError ();
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_destroy (pace_sem_t * sem)
{
  if (CloseHandle (*sem))
    {
      return 0;
    }
  else
    {
      errno = GetLastError ();
      return -1;
    }

  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_getvalue (pace_sem_t * sem, int * sval)
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_init (pace_sem_t * sem,
                             int pshared,
                             unsigned int value)
{
  /* Create the semaphore with its value initialized to <count> and
     its maximum value initialized to <max>.

     How do we want to call CreateSemaphore?
   */
  *sem = CreateSemaphore (0, value, 2147483647, "noname");

  if (*sem == 0)
    {
      PACE_FAIL_RETURN (-1);
    }
  /* NOTREACHED */
  else
    {
      return 0;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_post (pace_sem_t * sem)
{
  if (ReleaseSemaphore (*sem, 1, 0))
    {
      return 0;
    }
  else
    {
      errno = GetLastError ();
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_trywait (pace_sem_t * sem)
{
  int result = WaitForSingleObject (*sem, 0);

  if (result == WAIT_OBJECT_0)
    {
      return 0;
    }
  else
    {
      if (result == WAIT_TIMEOUT)
        {
          errno = EBUSY;
        }
      else
        {
          errno = GetLastError ();
        }
      /* This is a hack, we need to find an appropriate mapping... */
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_unlink (const char * name)
{
  if (_unlink (name))
    {
      return 0;
    }
  else
    {
      errno = GetLastError ();
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_win32_emulate_sem_wait (pace_sem_t * sem)
{
  switch (WaitForSingleObject (*sem, INFINITE))
    {
    case WAIT_OBJECT_0:
      return 0;
    default:
      /* This is a hack, we need to find an appropriate mapping... */
      errno = GetLastError ();
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_close (pace_sem_t * sem)
{
  return pace_win32_emulate_sem_close (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_destroy (pace_sem_t * sem)
{
  return pace_win32_emulate_sem_destroy (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_getvalue (pace_sem_t * sem, int * sval)
{
  return pace_win32_emulate_sem_getvalue (sem, sval);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_init (pace_sem_t * sem,
                             int pshared,
                             unsigned int value)
{
  return pace_sem_init (sem, pshared, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_post (pace_sem_t * sem)
{
  return pace_win32_emulate_sem_post (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_trywait (pace_sem_t * sem)
{
  return pace_win32_emulate_sem_trywait (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_unlink (const char * name)
{
  return pace_win32_emulate_sem_unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_wait (pace_sem_t * sem)
{
  return pace_win32_emulate_sem_wait (sem);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */


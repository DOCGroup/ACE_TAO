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
#include <stdio.h>

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_close (pace_sem_t * sem)
{
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (CloseHandle (sem), pace_result_), int, -1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_destroy (pace_sem_t * sem)
{
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (CloseHandle (sem), pace_result_), int, -1);
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
  /* Create the semaphore with its value initialized to <count> and
     its maximum value initialized to <max>.

     How do we want to call CreateSemaphore? What about the char *?
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
pace_sem_post (pace_sem_t * sem)
{
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (ReleaseSemaphore (*sem, 1, 0), pace_result_), int, -1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_trywait (pace_sem_t * sem)
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
pace_sem_unlink (const char * name)
{
  return _unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sem_wait (pace_sem_t * sem)
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


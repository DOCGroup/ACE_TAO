/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/aio.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_cancel (PACE_HANDLE fildes, pace_aiocb * aiocbp)
{
  return aio_cancel (fildes, aiocbp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_error (const pace_aiocb * aiocbp)
{
  return aio_error (PACE_NONCONST_ARG_CAST (struct aiocb *) aiocbp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_fsync (int op, pace_aiocb * aiocbp)
{
  PACE_UNUSED_ARG (op);
  PACE_UNUSED_ARG (aiocbp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_read (pace_aiocb * aiocbp)
{
  return aio_read (aiocbp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_return (pace_aiocb * aiocbp)
{
  return aio_return (aiocbp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_suspend (const pace_aiocb * const list[],
                  int nent,
                  const pace_timespec * timeout)
{
  return aio_suspend (PACE_AIO_SUSPEND_LIST_ARG_CAST list,
                      nent,
                      PACE_NONCONST_ARG_CAST (struct timespec *) timeout);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_aio_write (pace_aiocb * aiocbp)
{
  return aio_write (aiocbp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_lio_listio (int mode,
                 pace_aiocb * const list[],
                 int nent,
                 pace_sigevent * sig)
{
  return lio_listio (mode, PACE_NONCONST_ARG_CAST (struct aiocb **) list,
                     nent, sig);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/aio.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_aio_cancel (int fildes, pace_aiocb * aiocbp)
{
  return aio_cancel (fildes, aiocbp);
}

PACE_INLINE
int
pace_aio_error (const pace_aiocb * aiocbp)
{
  return aio_error (PACE_NONCONST_ARG_CAST (struct aiocb *) aiocbp);
}

PACE_INLINE
int
pace_aio_fsync (int op, pace_aiocb * aiocbp)
{
  return aio_fsync (op, aiocbp);
}

PACE_INLINE
int
pace_aio_read (pace_aiocb * aiocbp)
{
  return aio_read (aiocbp);
}

PACE_INLINE
int
pace_aio_return (pace_aiocb * aiocbp)
{
  return aio_return (aiocbp);
}

PACE_INLINE
int
pace_aio_suspend (const pace_aiocb * const list[],
                  int nent,
                  const pace_timespec * timeout)
{
  return aio_suspend (PACE_NONCONST_ARG_CAST (struct aiocb **) list,
                      nent,
                      PACE_NONCONST_ARG_CAST (struct timespec *) timeout);
}

PACE_INLINE
int
pace_aio_write (pace_aiocb * aiocbp)
{
  return aio_write (aiocbp);
}

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

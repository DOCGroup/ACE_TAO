/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/aio.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_aio_cancel (int fildes, struct aiocb * aiocbp)
{
  return aio_cancel (fildes, aiocbp);
}

PACE_INLINE
int
pace_aio_error (const struct aiocb * aiocbp)
{
  return aio_error (aiocbp);
}

PACE_INLINE
int
pace_aio_fsync (int op, struct aiocb * aiocbp)
{
  return aio_fsync (op, aiocbp);
}

PACE_INLINE
int
pace_aio_read (struct aiocb * aiocbp)
{
  return aio_read (aiocbp);
}

PACE_INLINE
int
pace_aio_return (struct aiocb * aiocbp)
{
  return aio_return (aiocbp);
}

PACE_INLINE
int
pace_aio_suspend (const struct aiocb * const list[],
                 int nent,
                 const struct timespec * timeout)
{
  return aio_suspend (list, nent, timeout);
}

PACE_INLINE
int
pace_aio_write (struct aiocb * aiocbp)
{
  return aio_write (aiocbp);
}

PACE_INLINE
int
pace_lio_listio (int mode,
                 struct aiocb * const list[],
                 int nent,
                 struct sigevent * sig)
{
  return lio_listio (mode, list, nent, sig);
}

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
pace_aio_cancel (int fildes, struct aiocb * aiocbp)
{
  return aio_cancel (fildes, aiocbp);
}

PACE_INLINE
int
pace_aio_error (const struct aiocb * aiocbp)
{
#if PACE_HAS_POSIX == PACE_LYNXOS
  /* Cast away const since LynxOS' prototypes aren't const */
  return aio_error ((struct aiocb *) aiocbp);
#else
  return aio_error (aiocbp);
#endif /* ! PACE_HAS_POSIX == PACE_LYNXOS */
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
#if PACE_HAS_POSIX == PACE_LYNXOS
  /* Cast away const since LynxOS' prototypes aren't const */
  return aio_suspend ((struct aiocb **) list,
                      nent,
                      (struct timespec *) timeout);
#else
  return aio_suspend (list, nent, timeout);
#endif /* ! PACE_HAS_POSIX == PACE_LYNXOS */
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
#if PACE_HAS_POSIX == PACE_LYNXOS
  /* Cast away const since LynxOS' prototypes aren't const */
  return lio_listio (mode, (struct aiocb **) list, nent, sig);
#else
  return lio_listio (mode, list, nent, sig);
#endif /* ! PACE_HAS_POSIX == PACE_LYNXOS */
}

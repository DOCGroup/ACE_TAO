/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/aio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_AIO_H
#define PACE_AIO_H

#include <aio.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /** The aio_cancel() function attempts to  cancel  one  or  more
     asynchronous I/O requests currently outstanding against file
     descriptor fildes. The aiocbp argument points to  the  asynchronous
     I/O  control  block for a particular request to be
     canceled. If aiocbp is NULL, then all outstanding cancelable
     asynchronous I/O requests against fildes are canceled. */
  PACE_INLINE int pace_aio_cancel (int fildes, struct aiocb * aiocb);

  PACE_INLINE int pace_aio_error (const struct aiocb * aiocbp);

  PACE_INLINE int pace_aio_fsync (int op, struct aiocb * aiocbp);

  PACE_INLINE int pace_aio_read (struct aiocb * aiocbp);

  PACE_INLINE int pace_aio_return (struct aiocb * aiocbp);

  PACE_INLINE int pace_aio_suspend (const struct aiocb * const list[],
                                    int nent,
                                    const struct timespec * timeout);

  PACE_INLINE int pace_aio_write (struct aiocb * aiocbp);

  PACE_INLINE int pace_lio_listio (int mode,
                                   struct aiocb * const list[],
                                   int nent,
                                   struct sigevent * sig);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/aio.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_AIO_H */






/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/aio.h
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

  /** 
     NEED TO ADD APPROPRIATE POSIX REFERENCE TEXT HERE.
     */
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
# include "pace/posix/aio.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_AIO_H */






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
     PACE's implementation of the POSIX function aio_cancel.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.7.
   */
  PACE_INLINE int pace_aio_cancel (int fildes, struct aiocb * aiocb);

  /** 
     PACE's implementation of the POSIX function aio_error.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.5.
   */
  PACE_INLINE int pace_aio_error (const struct aiocb * aiocbp);

  /** 
     PACE's implementation of the POSIX function aio_fsync.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.9.
   */
  PACE_INLINE int pace_aio_fsync (int op, struct aiocb * aiocbp);

  /** 
     PACE's implementation of the POSIX function aio_read.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.2.
   */
  PACE_INLINE int pace_aio_read (struct aiocb * aiocbp);

  /** 
     PACE's implementation of the POSIX function aio_return.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.6.
   */
  PACE_INLINE int pace_aio_return (struct aiocb * aiocbp);

  /** 
     PACE's implementation of the POSIX function aio_suspend.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.8.
   */
  PACE_INLINE int pace_aio_suspend (const struct aiocb * const list[],
                                    int nent,
                                    const struct timespec * timeout);

  /** 
     PACE's implementation of the POSIX function aio_write.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.3.
   */
  PACE_INLINE int pace_aio_write (struct aiocb * aiocbp);

  /** 
     PACE's implementation of the POSIX function lio_listio.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.4.
   */
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






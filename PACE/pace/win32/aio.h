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

#include "pace/time.h"
#include "pace/signal.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_AIO_ALLDONE AIO_ALLDONE
#define PACE_AIO_CANCELED AIO_CANCELED
#define PACE_AIO_NOCANCELED AIO_NOTCANCELED
#define PACE_LIO_NOP LIO_NOP
#define PACE_LIO_NOWAIT LIO_NOWAIT
#define PACE_LIO_READ LIO_READ
#define PACE_LIO_WAIT LIO_WAIT
#define PACE_LIO_WRITE LIO_WRITE

#ifndef PACE_AIOCB
#define PACE_AIOCB
typedef struct aiocb pace_aiocb;
#endif /* PACE_AIOCB */

  /**
     PACE's implementation of the POSIX function aio_cancel.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.7.
   */
  PACE_INLINE int pace_aio_cancel (int fildes, pace_aiocb * aiocb);

  /**
     PACE's implementation of the POSIX function aio_error.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.5.
   */
  PACE_INLINE int pace_aio_error (const pace_aiocb * aiocbp);

  /**
     PACE's implementation of the POSIX function aio_fsync.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.9.
   */
  PACE_INLINE int pace_aio_fsync (int op, pace_aiocb * aiocbp);

  /**
     PACE's implementation of the POSIX function aio_read.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.2.
   */
  PACE_INLINE int pace_aio_read (pace_aiocb * aiocbp);

  /**
     PACE's implementation of the POSIX function aio_return.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.6.
   */
  PACE_INLINE int pace_aio_return (pace_aiocb * aiocbp);

  /**
     PACE's implementation of the POSIX function aio_suspend.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.8.
   */
  PACE_INLINE int pace_aio_suspend (const pace_aiocb * const list[],
                                    int nent,
                                    const pace_timespec * timeout);

  /**
     PACE's implementation of the POSIX function aio_write.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.3.
   */
  PACE_INLINE int pace_aio_write (pace_aiocb * aiocbp);

  /**
     PACE's implementation of the POSIX function lio_listio.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), section 6.7.4.
   */
  PACE_INLINE int pace_lio_listio (int mode,
                                   pace_aiocb * const list[],
                                   int nent,
                                   pace_sigevent * sig);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/aio.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_AIO_H */

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

#ifndef PACE_AIO_H_POSIX
#define PACE_AIO_H_POSIX

#include <aio.h>
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

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_AIO_H_POSIX */

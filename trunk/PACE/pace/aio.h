/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/aio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_AIO_H_INDIRECT
#define PACE_AIO_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/aio.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/aio.h"
#elif defined (PACE_WIN32)
# include "pace/win32/aio.h"
#endif

#endif /* PACE_AIO_H_INDIRECT */

/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/fnctl.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_FCNTL_H_INDIRECT
#define PACE_FCNTL_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/fcntl.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/fcntl.h"
#elif defined (PACE_WIN32)
# include "pace/win32/fcntl.h"
#endif

#endif /* PACE_FCNTL_H_INDIRECT */

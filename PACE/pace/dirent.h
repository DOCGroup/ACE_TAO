/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/dirent.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DIRENT_H_INDIRECT
#define PACE_DIRENT_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/dirent.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/dirent.h"
#elif defined (PACE_WIN32)
# include "pace/win32/dirent.h"
#endif

#endif /* PACE_DIRENT_H_INDIRECT */

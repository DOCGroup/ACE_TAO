/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/mman.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_MMAN_H_INDIRECT
#define PACE_SYS_MMAN_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/mman.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/mman.h"
#elif defined (PACE_WIN32)
# include "pace/win32/mman.h"
#endif

#endif /* PACE_SYS_MMAN_H_INDIRECT */

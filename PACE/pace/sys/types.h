/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/types.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TYPES_H_INDIRECT
#define PACE_SYS_TYPES_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/types.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/types.h"
#elif defined (PACE_WIN32)
# include "pace/win32/types.h"
#endif

#endif /* PACE_SYS_TYPES_H_INDIRECT */
